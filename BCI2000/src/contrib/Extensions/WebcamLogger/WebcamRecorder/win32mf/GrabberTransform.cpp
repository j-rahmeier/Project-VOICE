////////////////////////////////////////////////////////////////////////////////
// $Id: GrabberTransform.cpp 7882 2024-03-01 20:34:21Z mellinger $
// Author: mellinger@neurotechcenter.com
// Description: A base class for Windows Media Foundation transforms that allow 
//   grabbing content as it flows through the pipeline.
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
#include "GrabberTransform.h"
#include "VideoMediaType.h"
#include "AudioMediaType.h"
#include "Debugging.h"

#include <mfapi.h>
#include <Mferror.h>

#include "Errors.h"

namespace {
  IMFMediaType* CloneMediaType(IMFMediaType* pType)
  {
    IMFMediaType* pCopy;
    HRSucceedOrThrow hr = ::MFCreateMediaType(&pCopy);
    hr = pType->CopyAllItems(pCopy);
    return pCopy;
  }
}

GrabberTransform::GrabberTransform()
  : mRefCount(0)
{
}

GrabberTransform::~GrabberTransform()
{
}

void GrabberTransform::AddInputAvailableType(DWORD streamID, IMFMediaType* pType)
{
  std::lock_guard<std::mutex> lock(mMutex);
  if (streamID < 0)
    throw std_runtime_error << "Negative stream ID: " << streamID;
  mInputAvailableTypes.resize(std::max<int>(streamID + 1, mInputAvailableTypes.size()));
  mInputTypes.resize(mInputAvailableTypes.size());
  mOutputActive.resize(mInputAvailableTypes.size(), false);
  mSamples.resize(mInputAvailableTypes.size());
  mInputAvailableTypes[streamID].push_back(pType);
}

// IUnknown
HRESULT GrabberTransform::QueryInterface(const IID& inIID, void** outInterface)
{
  *outInterface = nullptr;
  if (inIID == IID_IMFTransform)
    *outInterface = static_cast<IMFTransform*>(this);
  else if (inIID == IID_IUnknown)
    *outInterface = static_cast<IUnknown*>(this);
  if (*outInterface)
    this->AddRef();
  return *outInterface ? S_OK : E_NOINTERFACE;
}

ULONG GrabberTransform::AddRef()
{
  return ::InterlockedIncrement(&mRefCount);
}

ULONG GrabberTransform::Release()
{
  ULONG refCount = ::InterlockedDecrement(&mRefCount);
  if (refCount == 0)
    delete this;
  return refCount;
}

// IMFTransform

HRESULT GrabberTransform::GetStreamLimits(
  DWORD* pdwInputMinimum,
  DWORD* pdwInputMaximum,
  DWORD* pdwOutputMinimum,
  DWORD* pdwOutputMaximum)
{
  std::lock_guard<std::mutex> lock(mMutex);
  *pdwInputMinimum = *pdwOutputMinimum = 1;
  *pdwInputMaximum = *pdwOutputMaximum = MFT_STREAMS_UNLIMITED;
  return S_OK;
}

HRESULT GrabberTransform::GetStreamCount(
  DWORD* pcInputStreams,
  DWORD* pcOutputStreams)
{
  std::lock_guard<std::mutex> lock(mMutex);
  *pcInputStreams = *pcOutputStreams = mInputAvailableTypes.size();
  return S_OK;
}

HRESULT GrabberTransform::GetStreamIDs(
  DWORD dwInputIDArraySize,
  DWORD* pdwInputIDs,
  DWORD dwOutputIDArraySize,
  DWORD* pdwOutputIDs)
{
  return E_NOTIMPL;
}

HRESULT GrabberTransform::GetInputStreamInfo(
  DWORD dwInputStreamID,
  MFT_INPUT_STREAM_INFO* pStreamInfo)
{
  std::lock_guard<std::mutex> lock(mMutex);
  if (!StreamExists(dwInputStreamID))
    return MF_E_INVALIDSTREAMNUMBER;
  ::memset(pStreamInfo, 0, sizeof(*pStreamInfo));
  pStreamInfo->dwFlags = MFT_INPUT_STREAM_PROCESSES_IN_PLACE;
  return S_OK;
}

HRESULT GrabberTransform::GetOutputStreamInfo(
  DWORD dwOutputStreamID,
  MFT_OUTPUT_STREAM_INFO* pStreamInfo)
{
  std::lock_guard<std::mutex> lock(mMutex);
  if (!StreamExists(dwOutputStreamID))
    return MF_E_INVALIDSTREAMNUMBER;
  ::memset(pStreamInfo, 0, sizeof(*pStreamInfo));
  pStreamInfo->dwFlags = MFT_OUTPUT_STREAM_OPTIONAL | MFT_OUTPUT_STREAM_PROVIDES_SAMPLES;
  return S_OK;
}

HRESULT GrabberTransform::GetAttributes(
  IMFAttributes** pAttributes)
{
  return E_NOTIMPL;
}

HRESULT GrabberTransform::GetInputStreamAttributes(
  DWORD dwInputStreamID,
  IMFAttributes** pAttributes)
{
  return E_NOTIMPL;
}

HRESULT GrabberTransform::GetOutputStreamAttributes(
  DWORD dwOutputStreamID,
  IMFAttributes** pAttributes)
{
  return E_NOTIMPL;
}

HRESULT GrabberTransform::DeleteInputStream(
  DWORD dwStreamID)
{
  return E_NOTIMPL;
}

HRESULT GrabberTransform::AddInputStreams(
  DWORD cStreams,
  DWORD* adwStreamIDs)
{
  return E_NOTIMPL;
}

HRESULT GrabberTransform::GetInputAvailableType(
  DWORD dwInputStreamID,
  DWORD dwTypeIndex,
  IMFMediaType** ppType)
{
  std::lock_guard<std::mutex> lock(mMutex);
  if (!StreamExists(dwInputStreamID))
    return MF_E_INVALIDSTREAMNUMBER;
  if (dwTypeIndex < 0)
    return MF_E_INVALIDINDEX;
  auto& types = mInputAvailableTypes[dwInputStreamID];
  if (dwTypeIndex < types.size()) {
    *ppType = CloneMediaType(types[dwTypeIndex]);
    return S_OK;
  }
  return MF_E_NO_MORE_TYPES;
}

HRESULT GrabberTransform::GetOutputAvailableType(
  DWORD dwOutputStreamID,
  DWORD dwTypeIndex,
  IMFMediaType** ppType)
{
  return GetInputAvailableType(dwOutputStreamID, dwTypeIndex, ppType);
}

HRESULT GrabberTransform::SetInputType(
  DWORD dwInputStreamID,
  IMFMediaType* pType,
  DWORD dwFlags)
{
  std::lock_guard<std::mutex> lock(mMutex);
  if (!StreamExists(dwInputStreamID))
    return MF_E_INVALIDSTREAMNUMBER;
  if (mSamples[dwInputStreamID])
    return MF_E_TRANSFORM_CANNOT_CHANGE_MEDIATYPE_WHILE_PROCESSING;
  if (!pType) {
    if (!(dwFlags & MFT_SET_TYPE_TEST_ONLY))
      mInputTypes[dwInputStreamID] = nullptr;
    return S_OK;
  }

  bool isCompatible = false;
  for (auto& pAvailableType : mInputAvailableTypes[dwInputStreamID]) {
    if (VideoMediaType::IsVideo(pType) && VideoMediaType::IsVideo(pAvailableType))
      isCompatible = isCompatible || VideoMediaType(pType).IsCompatible(pAvailableType);
    else if(AudioMediaType::IsAudio(pType) && AudioMediaType::IsAudio(pAvailableType))
      isCompatible = isCompatible || AudioMediaType(pType).IsCompatible(pAvailableType);
  }
  if (!isCompatible)
    return MF_E_INVALIDMEDIATYPE;

  if (!(dwFlags & MFT_SET_TYPE_TEST_ONLY))
    mInputTypes[dwInputStreamID] = CloneMediaType(pType);
  return S_OK;
}

HRESULT GrabberTransform::SetOutputType(
  DWORD dwOutputStreamID,
  IMFMediaType* pType,
  DWORD dwFlags)
{
  std::lock_guard<std::mutex> lock(mMutex);
  if (!StreamExists(dwOutputStreamID))
    return MF_E_INVALIDSTREAMNUMBER;
  if (mSamples[dwOutputStreamID])
    return MF_E_TRANSFORM_CANNOT_CHANGE_MEDIATYPE_WHILE_PROCESSING;
  if (!mInputTypes[dwOutputStreamID])
    return MF_E_TRANSFORM_TYPE_NOT_SET;
  if (!pType)
  {
    mOutputActive[dwOutputStreamID] = false;
    return S_OK;
  }
  if (VideoMediaType::IsVideo(pType) && VideoMediaType(pType).IsCompatible(mInputTypes[dwOutputStreamID]))
  {
    mOutputActive[dwOutputStreamID] = true;
    return S_OK;
  }
  if (AudioMediaType::IsAudio(pType) && AudioMediaType(pType).IsCompatible(mInputTypes[dwOutputStreamID]))
  {
    mOutputActive[dwOutputStreamID] = true;
    return S_OK;
  }
  return MF_E_INVALIDMEDIATYPE;
}

HRESULT GrabberTransform::GetInputCurrentType(
  DWORD dwInputStreamID,
  IMFMediaType** ppType)
{
  std::lock_guard<std::mutex> lock(mMutex);
  if (!StreamExists(dwInputStreamID))
    return MF_E_INVALIDSTREAMNUMBER;
  if (!mInputTypes[dwInputStreamID])
    return MF_E_TRANSFORM_TYPE_NOT_SET;
  *ppType = CloneMediaType(mInputTypes[dwInputStreamID]);
  return S_OK;
}

HRESULT GrabberTransform::GetOutputCurrentType(
  DWORD dwOutputStreamID,
  IMFMediaType** ppType)
{
  std::lock_guard<std::mutex> lock(mMutex);
  if (!StreamExists(dwOutputStreamID))
    return MF_E_INVALIDSTREAMNUMBER;
  if (!mInputTypes[dwOutputStreamID] || !mOutputActive[dwOutputStreamID])
    return MF_E_TRANSFORM_TYPE_NOT_SET;
  *ppType = CloneMediaType(mInputTypes[dwOutputStreamID]);
  return S_OK;
}

HRESULT GrabberTransform::GetInputStatus(
  DWORD dwInputStreamID,
  DWORD* pdwFlags)
{
  std::lock_guard<std::mutex> lock(mMutex);
  if (!StreamExists(dwInputStreamID))
    return MF_E_INVALIDSTREAMNUMBER;
  DWORD flags = 0;
  if (!mSamples[dwInputStreamID])
    flags |= MFT_INPUT_STATUS_ACCEPT_DATA;
  *pdwFlags = flags;
  return S_OK;
}

HRESULT GrabberTransform::GetOutputStatus(
  DWORD* pdwFlags)
{
  std::lock_guard<std::mutex> lock(mMutex);
  DWORD flags = 0;
  for (int i = 0; i < mSamples.size(); ++i)
    if (mSamples[i] && mOutputActive[i])
      flags |= MFT_OUTPUT_STATUS_SAMPLE_READY;
  *pdwFlags = flags;
  return S_OK;
}

HRESULT GrabberTransform::SetOutputBounds(
  LONGLONG hnsLowerBound,
  LONGLONG hnsUpperBound)
{
  return E_NOTIMPL;
}

HRESULT GrabberTransform::ProcessEvent(
  DWORD dwInputStreamID,
  IMFMediaEvent* pEvent)
{
  return E_NOTIMPL;
}

HRESULT GrabberTransform::ProcessMessage(
  MFT_MESSAGE_TYPE eMessage,
  ULONG_PTR ulParam)
{
  std::lock_guard<std::mutex> lock(mMutex);
  HRESULT hr = S_OK;
  switch (eMessage) {
  case MFT_MESSAGE_NOTIFY_START_OF_STREAM:
  case MFT_MESSAGE_COMMAND_FLUSH:
    for (auto& pSample : mSamples)
      pSample = nullptr;
    /* fall through */
  case MFT_MESSAGE_COMMAND_DRAIN:
  case MFT_MESSAGE_NOTIFY_END_OF_STREAM:
  case MFT_MESSAGE_NOTIFY_BEGIN_STREAMING:
  case MFT_MESSAGE_NOTIFY_END_STREAMING:
    hr = S_OK;
    break;

  case MFT_MESSAGE_COMMAND_MARKER:
  case MFT_MESSAGE_SET_D3D_MANAGER:
  default:
    hr = E_NOTIMPL;
    break;
  }
  return hr;
}

HRESULT GrabberTransform::ProcessInput(
  DWORD dwInputStreamID,
  IMFSample* pSample,
  DWORD dwFlags)
{
  std::lock_guard<std::mutex> lock(mMutex);
  if (!StreamExists(dwInputStreamID))
    return MF_E_INVALIDSTREAMNUMBER;
  if (!pSample)
    return E_POINTER;
  if (dwFlags != 0)
    return E_INVALIDARG;
  if (!mInputTypes[dwInputStreamID])
    return MF_E_TRANSFORM_TYPE_NOT_SET;
  if (mSamples[dwInputStreamID])
    return MF_E_NOTACCEPTING;
  mSamples[dwInputStreamID] = pSample;
  return S_OK;
}

HRESULT GrabberTransform::ProcessOutput(
  DWORD dwFlags,
  DWORD cOutputBufferCount,
  MFT_OUTPUT_DATA_BUFFER* pOutputSamples,
  DWORD* pdwStatus)
{
  std::lock_guard<std::mutex> lock(mMutex);
  if (dwFlags != 0)
    return E_INVALIDARG;
  int activeOutputs = 0;
  for (bool active : mOutputActive)
    if (active)
      ++activeOutputs;
  if (cOutputBufferCount < activeOutputs)
    return E_INVALIDARG;
  if (!pOutputSamples)
    return E_POINTER;
  for (int i = 0; i < cOutputBufferCount; ++i)
    if (!StreamExists(pOutputSamples[i].dwStreamID))
      return MF_E_INVALIDSTREAMNUMBER;
  bool haveSample = false;
  for (const auto& pSample : mSamples)
    if (pSample)
      haveSample = true;
  if (!haveSample)
    return MF_E_TRANSFORM_NEED_MORE_INPUT;

  for (int i = 0; i < mSamples.size(); ++i)
  {
    if (mSamples[i])
    {
      HRESULT hr = OnProcess(i, mSamples[i]);
      if (FAILED(hr))
        return hr;
    }
  }

  for (int i = 0; i < cOutputBufferCount; ++i)
  {
    DWORD streamID = pOutputSamples[i].dwStreamID;
    pOutputSamples[i].pEvents = nullptr;

    auto& pSample = mSamples[streamID];
    if (pSample && mOutputActive[streamID])
    {
      pOutputSamples[i].dwStatus = 0;
      pOutputSamples[i].pSample = pSample;
      pSample->AddRef();
    }
    else
    {
      pOutputSamples[i].dwStatus = MFT_OUTPUT_DATA_BUFFER_NO_SAMPLE;
      pOutputSamples[i].pSample = nullptr;
    }
    pSample = nullptr;
  }
  return S_OK;
}

bool GrabberTransform::StreamExists(DWORD streamID) const
{
    return streamID >= 0 && streamID < mInputTypes.size();
}

