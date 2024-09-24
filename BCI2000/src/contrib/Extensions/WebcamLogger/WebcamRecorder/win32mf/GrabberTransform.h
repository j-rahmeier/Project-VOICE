////////////////////////////////////////////////////////////////////////////////
// $Id: GrabberTransform.h 7882 2024-03-01 20:34:21Z mellinger $
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
#ifndef GRABBER_TRANSFORM_H
#define GRABBER_TRANSFORM_H

#include <mfidl.h>
#include <mutex>
#include "ComPtr.h"

#include <vector>
#include <map>

class GrabberTransform : public IMFTransform
{
public:
  GrabberTransform();
  ~GrabberTransform();

  void AddInputAvailableType(DWORD StreamID, IMFMediaType*);

  HRESULT __stdcall QueryInterface(const IID& inIID, void** outInterface) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;

  HRESULT __stdcall GetStreamLimits(
    DWORD* pdwInputMinimum,
    DWORD* pdwInputMaximum,
    DWORD* pdwOutputMinimum,
    DWORD* pdwOutputMaximum) override;

  HRESULT __stdcall GetStreamCount(
    DWORD* pcInputStreams,
    DWORD* pcOutputStreams) override;

  HRESULT __stdcall GetStreamIDs(
    DWORD dwInputIDArraySize,
    DWORD* pdwInputIDs,
    DWORD dwOutputIDArraySize,
    DWORD* pdwOutputIDs) override;

  HRESULT __stdcall GetInputStreamInfo(
    DWORD dwInputStreamID,
    MFT_INPUT_STREAM_INFO* pStreamInfo) override;

  HRESULT __stdcall GetOutputStreamInfo(
    DWORD dwOutputStreamID,
    MFT_OUTPUT_STREAM_INFO* pStreamInfo) override;

  HRESULT __stdcall GetAttributes(
    IMFAttributes** pAttributes) override;

  HRESULT __stdcall GetInputStreamAttributes(
    DWORD dwInputStreamID,
    IMFAttributes** pAttributes) override;

  HRESULT __stdcall GetOutputStreamAttributes(
    DWORD dwOutputStreamID,
    IMFAttributes** pAttributes) override;

  HRESULT __stdcall DeleteInputStream(
    DWORD dwStreamID) override;

  HRESULT __stdcall AddInputStreams(
    DWORD cStreams,
    DWORD* adwStreamIDs) override;

  HRESULT __stdcall GetInputAvailableType(
    DWORD dwInputStreamID,
    DWORD dwTypeIndex,
    IMFMediaType** ppType) override;

  HRESULT __stdcall GetOutputAvailableType(
    DWORD dwOutputStreamID,
    DWORD dwTypeIndex,
    IMFMediaType** ppType) override;

  HRESULT __stdcall SetInputType(
    DWORD dwInputStreamID,
    IMFMediaType* pType,
    DWORD dwFlags) override;

  HRESULT __stdcall SetOutputType(
    DWORD dwOutputStreamID,
    IMFMediaType* pType,
    DWORD dwFlags) override;

  HRESULT __stdcall GetInputCurrentType(
    DWORD dwInputStreamID,
    IMFMediaType** ppType) override;

  HRESULT __stdcall GetOutputCurrentType(
    DWORD dwOutputStreamID,
    IMFMediaType** ppType) override;

  HRESULT __stdcall GetInputStatus(
    DWORD dwInputStreamID,
    DWORD* pdwFlags) override;

  HRESULT __stdcall GetOutputStatus(
    DWORD* pdwFlags) override;

  HRESULT __stdcall SetOutputBounds(
    LONGLONG hnsLowerBound,
    LONGLONG hnsUpperBound) override;

  HRESULT __stdcall ProcessEvent(
    DWORD dwInputStreamID,
    IMFMediaEvent* pEvent) override;

  HRESULT __stdcall ProcessMessage(
    MFT_MESSAGE_TYPE eMessage,
    ULONG_PTR ulParam) override;

  HRESULT __stdcall ProcessInput(
    DWORD dwInputStreamID,
    IMFSample* pSample,
    DWORD dwFlags) override;

  HRESULT __stdcall ProcessOutput(
    DWORD dwFlags,
    DWORD cOutputBufferCount,
    MFT_OUTPUT_DATA_BUFFER* pOutputSamples,
    DWORD* pdwStatus) override;

protected:
  virtual HRESULT OnProcess(DWORD, com::Ptr<IMFSample> pSample)
  {
    return S_OK;
  }

private:
  bool StreamExists(DWORD) const;

private:
  LONG mRefCount;
  std::vector<std::vector<com::Ptr<IMFMediaType>>> mInputAvailableTypes; // per stream
  std::vector<com::Ptr<IMFMediaType>> mInputTypes; // per stream
  std::vector<bool> mOutputActive; // per stream
  std::vector<com::Ptr<IMFSample>> mSamples; // per stream
  std::mutex mMutex;
};

#endif // GRABBER_TRANSFORM_H
