////////////////////////////////////////////////////////////////////////////////
// $Id: WebcamPipeline.cpp 7882 2024-03-01 20:34:21Z mellinger $
// Author: mellinger@neurotechcenter.com
// Description: A Windows Media Foundation pipeline which reads video input
//   from a camera, and displays it in a window. Optionally, an intermediate
//   transform may be specified.
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2023: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#include "WebcamPipeline.h"
#include "Errors.h"
#include "ComStrings.h"
#include "Exception.h"
#include "Debugging.h"

#include <vector>
#include <algorithm>

#include <Propidl.h>
#include <propvarutil.h>
#include <wininet.h>
#include <nserror.h>

namespace {
  std::string GetMediaErrorMessage(HRESULT hr)
  {
    SysError err(hr);
    if (err.IsKnown())
        return err.Message();
    HMODULE wmerror_dll = ::LoadLibraryA("wmerror.dll");
    if (err.IsKnown(wmerror_dll))
        return err.Message(wmerror_dll);

    std::ostringstream oss;
    switch (hr & 0xffff)
    {
        case ERROR_INTERNET_CONNECTION_ABORTED:
            oss << "The connection with the server has been terminated";
            break;
        case ERROR_INTERNET_CONNECTION_RESET:
            oss << "The connection with the server has been reset";
            break;
        case ERROR_INTERNET_CANNOT_CONNECT:
            oss << "The attempt to connect to the server failed";
            break;
    }
    if (oss.str().empty())
        oss << "Unknown error";
    oss << " (" << std::hex << std::showbase << hr << ")";
    return oss.str();
  }

  class DeviceList
  {
  public:
    DeviceList(IMFAttributes* attr)
    {
      HRSucceedOrThrow hr = ::MFEnumDeviceSources(attr, &mpList, &mCount);
    }
    ~DeviceList()
    {
      for (int i = 0; i < mCount; ++i)
        mpList[i]->Release();
      ::CoTaskMemFree(mpList);
    }
    uint32_t size()
    {
      return mCount;
    }
    com::Ptr<IMFActivate> operator[](int i)
    {
      return mpList[i];
    }
  private:
    IMFActivate** mpList = nullptr;
    UINT32 mCount = 0;
  };
}

WebcamPipeline::WebcamPipeline()
  : mRefCount(0),
    mCameraIndex(-1),
    mWindowHandle(NULL),
    mDoneEvent(INVALID_HANDLE_VALUE),
    mSessionState(MESessionUnknown)
{
  mDoneEvent = ::CreateEventA(nullptr, FALSE, FALSE, nullptr);
  HRSucceedOrThrow hr = ::MFCreateMediaSession(nullptr, mpMediaSession.Assignee());
  hr = mpMediaSession->BeginGetEvent(this, nullptr);
}

WebcamPipeline::~WebcamPipeline()
{
  ::CloseHandle(mDoneEvent);
}

std::string WebcamPipeline::Error() const
{
  std::lock_guard<std::mutex> lock(mMutex);
  return mError;
}

void WebcamPipeline::SetError(const std::string& s)
{
  std::lock_guard<std::mutex> lock(mMutex);
  mError = s;
}

// IUnknown
HRESULT WebcamPipeline::QueryInterface(const IID& inIID, void** outInterface)
{
  *outInterface = nullptr;
  if (inIID == IID_IMFAsyncCallback)
    *outInterface = static_cast<IMFAsyncCallback*>(this);
  else if (inIID == IID_IUnknown)
    *outInterface = static_cast<IUnknown*>(this);
  if (*outInterface)
    static_cast<IUnknown*>(*outInterface)->AddRef();
  return *outInterface ? S_OK : E_NOINTERFACE;
}

ULONG WebcamPipeline::AddRef()
{
  return ::InterlockedIncrement(&mRefCount);
}

ULONG WebcamPipeline::Release()
{
  ULONG refCount = ::InterlockedDecrement(&mRefCount);
  if (refCount == 0)
    delete this;
  else if (refCount < 0)
    throw std_logic_error << "Release() results in less than 0 references";
  return refCount;
}

// IMFAsyncCallback
HRESULT WebcamPipeline::GetParameters(DWORD* pdwFlags, DWORD* pdwQueue)
{
  *pdwFlags |= MFASYNC_FAST_IO_PROCESSING_CALLBACK;
  *pdwQueue = MFASYNC_CALLBACK_QUEUE_STANDARD;
  return S_OK;
}

HRESULT WebcamPipeline::Invoke(IMFAsyncResult* pAsyncResult)
{
  com::Ptr<IMFMediaEvent> pEvent;
  if (!mpMediaSession)
    return S_OK;
  HRESULT hr = mpMediaSession->EndGetEvent(pAsyncResult, pEvent.Assignee());
  MediaEventType eventType;
  hr = pEvent->GetType(&eventType);
  try {
    OnEvent(pEvent, eventType);
  }
  catch(const std::exception& e) {
    SetError(e.what());
    std::string s = e.what();
    s += "\n";
    ::OutputDebugStringA(s.c_str());
    ::PostMessageA(mWindowHandle, WM_USER, 0, 0);
    ::SetEvent(mDoneEvent);
  }
  if (eventType != MESessionClosed && eventType != MEEndOfPresentation)
    hr = mpMediaSession->BeginGetEvent(this, nullptr);
  return hr;
}


void WebcamPipeline::OnEvent(IMFMediaEvent* pEvent, MediaEventType eventType)
{
  HRESULT eventStatus = S_OK;
  HRSucceedOrThrow hr = pEvent->GetStatus(&eventStatus);
  if (FAILED(eventStatus))
    OnMediaError(eventStatus);
  else
    OnMediaEvent(pEvent, eventType);
}

void WebcamPipeline::OnMediaEvent(IMFMediaEvent* pEvent, MediaEventType type)
{
  HRSucceedOrThrow hr;
  UINT32 status = 0;
  switch (type)
  {
  case MESessionTopologyStatus:
    // Raised by the Media Session when the status of a topology changes. 
    // Get the topology changed status code. This is an enum in the event
    hr = pEvent->GetUINT32(MF_EVENT_TOPOLOGY_STATUS, &status);
    // the one we are most interested in is MF_TOPOSTATUS_READY
    // which we get then the Topology is built and ready to run.
    OnTopologyStatusChanged(MF_TOPOSTATUS(status));
    ::SetEvent(mDoneEvent);
    break;

  case MESessionStarted:
    // Raised when the IMFMediaSession::Start method completes asynchronously. 
    mSessionState = MESessionStarted;
    ::SetEvent(mDoneEvent);
    break;

  case MESessionPaused:
    // Raised when the IMFMediaSession::Pause method completes asynchronously. 
    mSessionState = MESessionPaused;
    ::SetEvent(mDoneEvent);
    break;

  case MESessionStopped:
    // Raised when the IMFMediaSession::Stop method completes asynchronously.
      mSessionState = MESessionStopped;
    ::SetEvent(mDoneEvent);
    break;

  case MESessionClosed:
    // Raised when the IMFMediaSession::Close method completes asynchronously. 
      mSessionState = MESessionClosed;
    ::SetEvent(mDoneEvent);
    break;

  case MESessionCapabilitiesChanged:
    // Raised by the Media Session when the session capabilities change.
    // You can use IMFMediaEvent::GetValue to figure out what they are
    break;

  case MESessionTopologySet:
    // Raised after the IMFMediaSession::SetTopology method completes asynchronously. 
    // The Media Session raises this event after it resolves the topology into a full topology and queues the topology for playback. 
    break;

  case MESessionNotifyPresentationTime:
    // Raised by the Media Session when a new presentation starts. 
    // This event indicates when the presentation will start and the offset between the presentation time and the source time.      
    break;

  case MEEndOfPresentation:
    // Raised by a media source when a presentation ends. This event signals that all streams 
    // in the presentation are complete. The Media Session forwards this event to the application.
    break;

  case MESessionRateChanged:
    // Raised by the Media Session when the playback rate changes. This event is sent after the 
    // IMFRateControl::SetRate method completes asynchronously. 
    break;

  default:
    break;
  }
}

void WebcamPipeline::OnMediaError(HRESULT hr)
{
  switch (hr)
  {
  // list recoverable errors here
  case NS_E_FILE_EXISTS:
    return;
  }

  if (mError.empty())
    mError = GetMediaErrorMessage(hr);
  if (mWindowHandle)
    ::PostMessageA(mWindowHandle, WM_DESTROY, 0, 0);
}

void WebcamPipeline::OnTopologyStatusChanged(MF_TOPOSTATUS status)
{
  std::lock_guard<std::mutex> lock(mMutex);
  HRSucceedOrThrow hr;
  switch (status) {
  case MF_TOPOSTATUS_READY:
    hr = ::MFGetService(mpMediaSession, MR_VIDEO_RENDER_SERVICE,
      IID_IMFVideoDisplayControl, mpVideoDisplayControl.Assignee());
    break;
  }
}

void WebcamPipeline::SetDesiredVideoMediaType(const VideoMediaType& type)
{
  std::lock_guard<std::mutex> lock(mMutex);
  mDesiredVideoMediaType = type;
}

const VideoMediaType& WebcamPipeline::ActualVideoMediaType() const
{
  std::lock_guard<std::mutex> lock(mMutex);
  return mActualVideoMediaType;
}

void WebcamPipeline::SetDesiredAudioMediaType(const AudioMediaType& type)
{
  std::lock_guard<std::mutex> lock(mMutex);
  mDesiredAudioMediaType = type;
}

const AudioMediaType& WebcamPipeline::ActualAudioMediaType() const
{
  std::lock_guard<std::mutex> lock(mMutex);
  return mActualAudioMediaType;
}

void WebcamPipeline::ActivateCameraFromWebcamIndex(int inCameraIndex)
{
  std::lock_guard<std::mutex> lock(mMutex);
  com::Ptr<IMFAttributes> pConfig;
  // Create an attribute store to hold the search criteria.
  HRSucceedOrThrow hr = ::MFCreateAttributes(pConfig.Assignee(), 1);
  // Request video capture devices.
  hr = pConfig->SetGUID(
    MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
    MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID
  );

  // Enumerate devices.
  DeviceList devices(pConfig);

  // Create a media source for the desired device in the list.
  if (inCameraIndex >= devices.size())
    throw std::runtime_error("No camera available with index " + std::to_string(inCameraIndex));

  wchar_t buf[1024];
  uint32_t length = 0;
  hr = devices[inCameraIndex]->GetString(MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, buf, sizeof(buf)/2 - 1, &length);
  buf[length] = 0;
  mCameraName = com::DualString(buf);

  mCameraIndex = inCameraIndex;
  hr = devices[inCameraIndex]->ActivateObject(IID_IMFMediaSource, mpVideoSource.Assignee());
}

void WebcamPipeline::ActivateCameraFromURL(
  int inCameraIndex, const std::string& inCameraURL,
  int inBufferingTime, int inMaxBufferTimeMs
)
{
  std::lock_guard<std::mutex> lock(mMutex);
  mCameraName = inCameraURL;
  mCameraIndex = inCameraIndex;
  mpVideoSource = ActivateSourceFromURL(inCameraURL, inBufferingTime, inMaxBufferTimeMs);
}

void WebcamPipeline::ActivateAudioFromURL(const std::string& inURL, int inBufferingTime, int inMaxBufferTimeMs)
{
  std::lock_guard<std::mutex> lock(mMutex);
  mpAudioSource = ActivateSourceFromURL(inURL, inBufferingTime, inMaxBufferTimeMs);
}

com::Ptr<IMFMediaSource> WebcamPipeline::ActivateSourceFromURL(const std::string& inURL, int inBufferingTime, int inMaxBufferTimeMs)
{
  com::Ptr<IMFSourceResolver> pSourceResolver;
  HRSucceedOrThrow hr = ::MFCreateSourceResolver(pSourceResolver.Assignee());

  com::Ptr<IPropertyStore> pProperties;
  hr = ::PSCreateMemoryPropertyStore(IID_IPropertyStore, pProperties.Assignee());

  PROPERTYKEY keyBufferingTime = { MFNETSOURCE_BUFFERINGTIME, 0 };
  PROPVARIANT value;
  hr = ::InitPropVariantFromInt32(inBufferingTime, &value);
  hr = pProperties->SetValue(keyBufferingTime, value);
  hr = ::PropVariantClear(&value);

  PROPERTYKEY keyMaxBufferTimeMs = { MFNETSOURCE_MAXBUFFERTIMEMS, 0 };
  hr = ::InitPropVariantFromInt32(inMaxBufferTimeMs, &value);
  hr = pProperties->SetValue(keyMaxBufferTimeMs, value);
  hr = ::PropVariantClear(&value);

  PROPERTYKEY keyPreviewModeEnabled = { MFNETSOURCE_PREVIEWMODEENABLED, 0 };
  hr = ::InitPropVariantFromBoolean(TRUE, &value);
  hr = pProperties->SetValue(keyPreviewModeEnabled, value);
  hr = ::PropVariantClear(&value);

  MF_OBJECT_TYPE objectType = MF_OBJECT_INVALID;
  com::Ptr<IUnknown> pSourceUnknown;
  HRESULT res = pSourceResolver->CreateObjectFromURL(
      com::DualString(inURL),
      MF_RESOLUTION_MEDIASOURCE,
      pProperties,
      &objectType,
      pSourceUnknown.Assignee()
  );
  if (FAILED(res))
      throw std_runtime_error << "Could not create media source object from URL (" << GetMediaErrorMessage(res) << ")";
  else if (objectType != MF_OBJECT_MEDIASOURCE)
      throw std_runtime_error << "Could not create media source object from URL (unknown error)";

  com::Ptr<IMFMediaSource> pSource;
  hr = pSourceUnknown->QueryInterface(IID_IMFMediaSource, pSource.Assignee());
  return pSource;
}

const std::string& WebcamPipeline::CameraName() const
{
  std::lock_guard<std::mutex> lock(mMutex);
  return mCameraName;
}

void WebcamPipeline::SetTransform(com::Ptr<IMFTransform> pTransform)
{
  std::lock_guard<std::mutex> lock(mMutex);
  mpTransform = pTransform;
}

com::Ptr<IMFTransform> WebcamPipeline::Transform() const
{
  std::lock_guard<std::mutex> lock(mMutex);
  return mpTransform;
}

void WebcamPipeline::SetWindowHandle(HWND h)
{
  std::lock_guard<std::mutex> lock(mMutex);
  HRSucceedOrThrow hr;
  if (mWindowHandle != h) {
    mWindowHandle = h;
    if (h == NULL)
      mpRendererActivate = nullptr;
    else
      hr = ::MFCreateVideoRendererActivate(h, mpRendererActivate.Assignee());
  }
}

HWND WebcamPipeline::WindowHandle() const
{
  std::lock_guard<std::mutex> lock(mMutex);
  return mWindowHandle;
}

void WebcamPipeline::CreateTopology()
{
  std::lock_guard<std::mutex> lock(mMutex);
  if (!mpVideoSource)
    throw std_runtime_error << "No source given";
  if (!mpRendererActivate)
    throw std_runtime_error << "No renderer defined";

  // Set up video and audio source nodes
  com::Ptr<IMFStreamDescriptor> pVideoStreamDescriptor;
  com::Ptr<IMFPresentationDescriptor> pVideoPresentationDescriptor;
  com::Ptr<IMFStreamDescriptor> pAudioStreamDescriptor;
  com::Ptr<IMFPresentationDescriptor> pAudioPresentationDescriptor;
  GetInputDescriptors(
    pVideoStreamDescriptor, pVideoPresentationDescriptor,
    pAudioStreamDescriptor, pAudioPresentationDescriptor
  );
  if (!pVideoStreamDescriptor)
    throw std_runtime_error << "Could not find selected video stream on camera " << mCameraIndex;

  HRSucceedOrThrow hr;
  com::Ptr<IMFMediaTypeHandler> pMediaTypeHandler;
  hr = pVideoStreamDescriptor->GetMediaTypeHandler(pMediaTypeHandler.Assignee());
  ChooseVideoType(pMediaTypeHandler);

  if (pAudioStreamDescriptor)
  {
      hr = pAudioStreamDescriptor->GetMediaTypeHandler(pMediaTypeHandler.Assignee());
      ChooseAudioType(pMediaTypeHandler);
  }

  com::Ptr<IMFTopologyNode> pNode;
  hr = ::MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, pNode.Assignee());
  hr = pNode->SetUnknown(MF_TOPONODE_SOURCE, mpVideoSource);
  hr = pNode->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, pVideoPresentationDescriptor);
  hr = pNode->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, pVideoStreamDescriptor);
  auto pVideoSourceNode = pNode;

  com::Ptr<IMFTopologyNode> pAudioSourceNode;
  if (pAudioStreamDescriptor)
  {
    hr = ::MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, pNode.Assignee());
    hr = pNode->SetUnknown(MF_TOPONODE_SOURCE, mpVideoSource);
    hr = pNode->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, pAudioPresentationDescriptor);
    hr = pNode->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, pAudioStreamDescriptor);
    pAudioSourceNode = pNode;
  }

  com::Ptr<IMFTopologyNode> pTransformNode;
  if (mpTransform) {
    hr = ::MFCreateTopologyNode(MF_TOPOLOGY_TRANSFORM_NODE, pNode.Assignee());
    hr = pNode->SetObject(mpTransform);
    pTransformNode = pNode;
  }

  hr = ::MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, pNode.Assignee());
  hr = pNode->SetObject(mpRendererActivate);
  auto pRendererNode = pNode;

  hr = ::MFCreateTopology(mpTopology.Assignee());
  if (pVideoSourceNode)
    hr = mpTopology->AddNode(pVideoSourceNode);
  if (pAudioSourceNode)
    hr = mpTopology->AddNode(pAudioSourceNode);
  hr = mpTopology->AddNode(pTransformNode);
  hr = mpTopology->AddNode(pRendererNode);

  // Connect Video
  if (pVideoSourceNode)
    hr = pVideoSourceNode->ConnectOutput(0, pTransformNode ? pTransformNode : pRendererNode, 0);
  if (pTransformNode)
    hr = pTransformNode->ConnectOutput(0, pRendererNode, 0);
  // Connect Audio
  if (pAudioSourceNode && pTransformNode)
    hr = pAudioSourceNode->ConnectOutput(0, pTransformNode, 1);
}

com::Ptr<IMFVideoDisplayControl> WebcamPipeline::ResolveTopology()
{
  if (!mpTopology)
    throw std_runtime_error << "No topology given";
  // Setting the topology will result in a MESessionTopologySet event.
  // There, we get a pointer to the EVR control object.
  HRSucceedOrThrow hr = mpMediaSession->SetTopology(0, mpTopology);
  WaitForDone();
  mpTopology = nullptr;
  std::lock_guard<std::mutex> lock(mMutex);
  Assert(mpVideoDisplayControl);
  return mpVideoDisplayControl;
}

void WebcamPipeline::Start()
{
  if (mSessionState == MESessionUnknown || mSessionState == MESessionPaused)
  {
  PROPVARIANT pos = { 0 };
  HRSucceedOrThrow hr = mpMediaSession->Start(nullptr, &pos);
  WaitForDone();
}
}

void WebcamPipeline::Pause()
{
  if (mSessionState == MESessionStarted)
  {
  HRSucceedOrThrow hr = mpMediaSession->Pause();
  WaitForDone();
}
}

void WebcamPipeline::Stop()
{
  if (mSessionState != MESessionStopped)
  {
  HRSucceedOrThrow hr = mpMediaSession->Stop();
  WaitForDone();
}
}

void WebcamPipeline::Close()
{
  if (mSessionState != MESessionClosed)
  {
  HRSucceedOrThrow hr = mpMediaSession->Close();
    WaitForDone();
  }
}

void WebcamPipeline::Shutdown()
{
  HRSucceedOrThrow hr = mpMediaSession->Shutdown();
}

void WebcamPipeline::WaitForDone(int timeoutMs)
{
  switch (::WaitForSingleObject(mDoneEvent, timeoutMs))
  {
    case WAIT_OBJECT_0:
    case WAIT_TIMEOUT:
      break;
    default:
      throw std_runtime_error << "WaitForSingleObject: " << SysError().Message();
  }
}

void WebcamPipeline::GetInputDescriptors(
    com::Ptr<IMFStreamDescriptor>& outVideoStreamDescriptor,
    com::Ptr<IMFPresentationDescriptor>& outVideoPresentationDescriptor,
    com::Ptr<IMFStreamDescriptor>& outAudioStreamDescriptor,
    com::Ptr<IMFPresentationDescriptor>& outAudioPresentationDescriptor
)
{
  // Get the first enabled video stream from the input device.
  // If audio streams are available, get the first enabled audio stream as well.
  HRSucceedOrThrow hr;
  hr = mpVideoSource->CreatePresentationDescriptor(outVideoPresentationDescriptor.Assignee());
  DWORD streamDescriptorCount = 0;
  hr = outVideoPresentationDescriptor->GetStreamDescriptorCount(&streamDescriptorCount);
  for (int i = 0; i < streamDescriptorCount; ++i) {
    com::Ptr<IMFStreamDescriptor> pStreamDescriptor;
    BOOL selected = FALSE;
    hr = outVideoPresentationDescriptor->GetStreamDescriptorByIndex(i, &selected, pStreamDescriptor.Assignee());
    com::Ptr<IMFMediaTypeHandler> pMediaTypeHandler;
    hr = pStreamDescriptor->GetMediaTypeHandler(pMediaTypeHandler.Assignee());
    GUID majorType;
    pMediaTypeHandler->GetMajorType(&majorType);
    if (majorType == MFMediaType_Video && selected) {
      outVideoStreamDescriptor = pStreamDescriptor;
      break;
    }
  }
  if (!mpAudioSource)
  {
    // see if there is an audio stream muxed with the video stream
    outAudioStreamDescriptor = nullptr;
    for (int i = 0; i < streamDescriptorCount; ++i) {
      com::Ptr<IMFStreamDescriptor> pStreamDescriptor;
      BOOL selected = FALSE;
      hr = outVideoPresentationDescriptor->GetStreamDescriptorByIndex(i, &selected, pStreamDescriptor.Assignee());
      com::Ptr<IMFMediaTypeHandler> pMediaTypeHandler;
      hr = pStreamDescriptor->GetMediaTypeHandler(pMediaTypeHandler.Assignee());
      GUID majorType;
      pMediaTypeHandler->GetMajorType(&majorType);
      if (majorType == MFMediaType_Audio && selected) {
        outAudioStreamDescriptor = pStreamDescriptor;
        outAudioPresentationDescriptor = outVideoPresentationDescriptor;
        break;
      }
    }
  }
  else // an audio source specified through AudioURL takes precedence
  {
    hr = mpAudioSource->CreatePresentationDescriptor(outAudioPresentationDescriptor.Assignee());
    DWORD streamDescriptorCount = 0;
    hr = outAudioPresentationDescriptor->GetStreamDescriptorCount(&streamDescriptorCount);
    for (int i = 0; i < streamDescriptorCount; ++i) {
      com::Ptr<IMFStreamDescriptor> pStreamDescriptor;
      BOOL selected = FALSE;
      hr = outAudioPresentationDescriptor->GetStreamDescriptorByIndex(i, &selected, pStreamDescriptor.Assignee());
      com::Ptr<IMFMediaTypeHandler> pMediaTypeHandler;
      hr = pStreamDescriptor->GetMediaTypeHandler(pMediaTypeHandler.Assignee());
      GUID majorType;
      pMediaTypeHandler->GetMajorType(&majorType);
      if (majorType == MFMediaType_Audio && selected) {
        outAudioStreamDescriptor = pStreamDescriptor;
        break;
      }
    }
  }
}

void WebcamPipeline::ChooseVideoType(IMFMediaTypeHandler* pHandler)
{
  HRSucceedOrThrow hr;
  DWORD count = 0;
  hr = pHandler->GetMediaTypeCount(&count);
  std::vector<VideoMediaType> formats;
  for (int i = 0; i < count; ++i) {
    com::Ptr<IMFMediaType> pMediaType;
    hr = pHandler->GetMediaTypeByIndex(i, pMediaType.Assignee());
    formats.push_back(pMediaType);
  }
  auto j = std::min_element(formats.begin(), formats.end(),
    [this](const VideoMediaType& a, const VideoMediaType& b)
    {
      return a.sqDist(mDesiredVideoMediaType) < b.sqDist(mDesiredVideoMediaType);
    }
  );
  if (j == formats.end())
    throw std_runtime_error << "No suitable MediaType found";
  hr = pHandler->SetCurrentMediaType(j->mpType);
  mActualVideoMediaType = *j;
}

void WebcamPipeline::ChooseAudioType(IMFMediaTypeHandler* pHandler)
{
    HRSucceedOrThrow hr;
    DWORD count = 0;
    hr = pHandler->GetMediaTypeCount(&count);
    std::vector<AudioMediaType> formats;
    for (int i = 0; i < count; ++i) {
        com::Ptr<IMFMediaType> pMediaType;
        hr = pHandler->GetMediaTypeByIndex(i, pMediaType.Assignee());
        formats.push_back(pMediaType);
    }
    auto j = std::min_element(formats.begin(), formats.end(),
        [this](const AudioMediaType& a, const AudioMediaType& b)
        {
            return a.sqDist(mDesiredAudioMediaType) < b.sqDist(mDesiredAudioMediaType);
        }
    );
    if (j == formats.end())
        throw std_runtime_error << "No suitable MediaType found";
    hr = pHandler->SetCurrentMediaType(j->mpType);
    mActualAudioMediaType = *j;
}
