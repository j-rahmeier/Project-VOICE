////////////////////////////////////////////////////////////////////////////////
// $Id: VideoFrame.cpp 8039 2024-04-17 18:12:01Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A container for video frames in ARGB32 format, with support for
//   shared memory "transmission" if connected locally.
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
#include "VideoFrame.h"

#include "LengthField.h"
#include "BinaryData.h"
#include "LazyArray.h"
#include "Debugging.h"
#include "SharedMemoryMap.h"

namespace {
    enum
    {
        BigEndian = 1, LittleEndian = 2,
        BOMMask = BigEndian | LittleEndian,
    };
    static const int cBOM = (Tiny::HostOrder == Tiny::BigEndian) ? BigEndian : LittleEndian;

    // This will be replaced with std::byteswap() once C++23 is available
    template<class T> void SwapBytes(T& t)
    {
        union { T* t; char* c; } data = { &t };
        for (int i = 0; i < sizeof(T) / 2; ++i)
          std::swap(data.c[i], data.c[sizeof(T) - i - 1]);
    }

    SharedMemoryMap& ShmContainer()
    {
        static SharedMemoryMap instance;
        return instance;
    }
}

struct VideoFrame::Private
{
    int mWidth = 0, mHeight = 0;
    LazyArray<uint32_t> mLazyArray;
    std::shared_ptr<SharedMemory> mSharedMemory;
};

VideoFrame::VideoFrame(int inWidth, int inHeight, const uint32_t* inpData)
: p(new Private)
{
  Init(inWidth, inHeight, inpData);
}

VideoFrame::VideoFrame(int inWidth, int inHeight, const void* inpData)
: p(new Private)
{
    Init(inWidth, inHeight, static_cast<const uint32_t*>(inpData));
}

void VideoFrame::Init(int inWidth, int inHeight, const uint32_t* inpData)
{
    p->mWidth = inWidth;
    p->mHeight = inHeight;
    p->mLazyArray = LazyArray<uint32_t>(inWidth * inHeight);
    auto pData = p->mLazyArray.Data();
    if (!inpData) {
        ::memset(pData, 0, inWidth * inHeight * sizeof(*pData));
    }
    else {
        ::memcpy(pData, inpData, inWidth * inHeight * sizeof(*pData));
    }
}

VideoFrame::VideoFrame(const VideoFrame &b)
: p(new Private)
{
    p->mWidth = b.Width();
    p->mHeight = b.Height();
    p->mSharedMemory = b.p->mSharedMemory;
    p->mLazyArray.ShallowAssignFrom(b.p->mLazyArray);
}

VideoFrame::~VideoFrame()
{
    delete p;
}

VideoFrame &VideoFrame::operator=(const VideoFrame &b)
{
    if (&b != this)
    {
        int prevSize = p->mWidth * p->mHeight;
        p->mWidth = b.Width();
        p->mHeight = b.Height();
        if (!p->mSharedMemory && !b.p->mSharedMemory)
            p->mLazyArray.ShallowAssignFrom(b.p->mLazyArray);
        else
            p->mLazyArray.DeepAssignFrom(b.p->mLazyArray);
    }
    return *this;
}

VideoFrame &VideoFrame::SetBlack()
{
    auto pData = p->mLazyArray.GetWritableData();
    std::memset(pData, 0, p->mWidth * p->mHeight * sizeof(*pData));
    return *this;
}

VideoFrame &VideoFrame::Fill(RGBColor c)
{
    uint32_t* pData = p->mLazyArray.GetWritableData();
    for (int i = 0; i < p->mWidth * p->mHeight; ++i)
        pData[i] = c;
    return *this;
}

VideoFrame &VideoFrame::AssignData(int inWidth, int inHeight, const uint32_t* inpData)
{
    int newCount = inHeight * inWidth;
    if (newCount != p->mWidth * p->mHeight) {
        LazyArray<uint32_t> newData;
        if (p->mSharedMemory) {
            if (p->mSharedMemory->Protocol() == "file://")
                throw std_runtime_error << "Cannot resize shared memory if tied to a file";
            p->mSharedMemory = ShmContainer().New(newCount * sizeof(uint32_t));
            std::shared_ptr<LazyArray<uint32_t>::Memory>
                pNewMemory(new LazyArray<uint32_t>::ShmMemory(p->mSharedMemory, newCount));
            newData = LazyArray<uint32_t>(pNewMemory);
        }
        else {
            newData = LazyArray<uint32_t>(newCount);
        }
        p->mLazyArray = newData;
    }
    p->mWidth = inWidth;
    p->mHeight = inHeight;
    uint32_t* pData = p->mLazyArray.GetWritableData();
    if (pData != inpData) {
        ::memcpy(pData, inpData, p->mWidth * p->mHeight * sizeof(uint32_t));
    }
    return *this;
}

VideoFrame &VideoFrame::AssignData(int width, int height, const void* pData)
{
    return AssignData(width, height, static_cast<const uint32_t*>(pData));
}

bool VideoFrame::Empty() const
{
    return p->mWidth * p->mHeight == 0;
}

int VideoFrame::Width() const
{
    return p->mWidth;
}

int VideoFrame::Height() const
{
    return p->mHeight;
}

const uint32_t* VideoFrame::RawData() const
{
    return p->mLazyArray.Data();
}

uint32_t* VideoFrame::MutableRawData()
{
    return p->mLazyArray.GetWritableData();
}

bool VideoFrame::ShareAcrossModules()
{
    if (!p->mSharedMemory && p->mLazyArray.Count() != 0)
    {
        p->mSharedMemory = ShmContainer().New(p->mLazyArray.Count() * sizeof(uint32_t));
        std::shared_ptr<LazyArray<uint32_t>::Memory>
            pNewMemory(new LazyArray<uint32_t>::ShmMemory(p->mSharedMemory, p->mLazyArray.Count()));
        LazyArray<uint32_t> newValues(pNewMemory);
        newValues.DeepAssignFrom(p->mLazyArray);
        p->mLazyArray.ShallowAssignFrom(newValues);
    }
    return !!p->mSharedMemory;
}

bool VideoFrame::IsSharedAcrossModules() const
{
    return !!p->mSharedMemory;
}

void VideoFrame::AttachToSharedMemory(const std::string& inName)
{
    if (!p->mSharedMemory || p->mSharedMemory->Name() != inName)
    {
        p->mSharedMemory = ShmContainer().Get(inName);
        std::shared_ptr<LazyArray<uint32_t>::Memory>
            pNewMemory(new LazyArray<uint32_t>::ShmMemory(p->mSharedMemory, p->mLazyArray.Count()));
        p->mLazyArray = LazyArray<uint32_t>(pNewMemory);
    }
}

std::ostream &VideoFrame::Serialize(std::ostream &os) const
{
    int flags = cBOM;
    if (!!p->mSharedMemory)
        flags |= SharedFlag;
    os.put(flags);

    LengthField<2> width(p->mWidth), height(p->mHeight);
    width.Serialize(os);
    height.Serialize(os);
    if (!!p->mSharedMemory)
    {
        std::atomic_thread_fence(std::memory_order_seq_cst);
        os.write(p->mSharedMemory->Name().c_str(), p->mSharedMemory->Name().length() + 1);
    }
    else
    {
        auto pData = p->mLazyArray.Data();
        os.write(reinterpret_cast<const char*>(pData), p->mWidth * p->mHeight * sizeof(*pData));
    }
    return os;
}

std::istream &VideoFrame::Unserialize(std::istream &is)
{
    int flags = is.get();
    bool shm = flags & SharedFlag;
    bool needSwap = (flags & BOMMask) != cBOM;
    if (needSwap)
        Assert(!shm);

    int prevSize = p->mWidth * p->mHeight;
    LengthField<2> width, height;
    width.Unserialize(is);
    height.Unserialize(is);
    p->mWidth = width;
    p->mHeight = height;

    if (shm)
    {
        std::string name;
        std::getline(is, name, '\0');
        AttachToSharedMemory(name);
        std::atomic_thread_fence(std::memory_order_seq_cst);
    }
    else
    {
        if (p->mWidth * p->mHeight != prevSize) {
            p->mLazyArray = LazyArray<uint32_t>(p->mWidth * p->mHeight);
        }
        auto pData = p->mLazyArray.GetWritableData();
        is.read(reinterpret_cast<char*>(pData), p->mWidth * p->mHeight * sizeof(*pData));

        if (needSwap) {
            for (int i = 0; i < p->mWidth * p->mHeight; ++i)
                SwapBytes(pData[i]);
        }
    }

    return is;
}

VideoFrame::ConstPixelRef VideoFrame::operator()(int x, int y) const
{
    return ConstPixelRef(*this, x, y);
}

VideoFrame::PixelRef VideoFrame::operator()(int x, int y)
{
    return PixelRef(*this, x, y);
}

// PixelRef
VideoFrame::PixelRef::PixelRef(VideoFrame& f, int x, int y)
: mpData(f.MutableRawData() + y * f.Width() + x)
{
}

VideoFrame::PixelRef::operator RGBColor() const
{
    return *mpData;
}

VideoFrame::PixelRef& VideoFrame::PixelRef::operator=(const RGBColor& c)
{
    *mpData = c;
    return *this;
}

// ConstPixelRef
VideoFrame::ConstPixelRef::ConstPixelRef(const VideoFrame& f, int x, int y)
    : mpData(f.RawData() + y * f.Width() + x)
{
}

VideoFrame::ConstPixelRef::operator RGBColor() const
{
    return *mpData;
}

