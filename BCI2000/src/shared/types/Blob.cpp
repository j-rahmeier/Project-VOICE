////////////////////////////////////////////////////////////////////////////////
// $Id: Blob.cpp 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A type for binary large objects (BLOBs) which may be embedded
//   into BCI2000 data streams.
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
#include "Blob.h"

#include "Exception.h"
#include "FileUtils.h"
#include "StringUtils.h"

#include <fstream>
#include <iomanip>
#include <sstream>

static void NormalizeType(std::string &type)
{
    if (type.find(".") == 0)
        type = type.substr(1);
}

Blob::Blob() : mpData(0), mLength(0), mpTempFile(0)
{
}

Blob::Blob(const std::string &inFileName)
    : mFileName(FileUtils::AbsolutePath(inFileName)), mpData(0), mLength(0),
      mType(FileUtils::ExtractExtension(inFileName)), mpTempFile(0)
{
    NormalizeType(mType);
    std::ifstream in(inFileName.c_str(), std::ios::binary | std::ios::in);
    if (!in.is_open())
        throw std_runtime_error << "Could not open file for reading: " << inFileName;
    in.seekg(0, std::ios::end);
    mLength = static_cast<size_t>(in.tellg());
}

Blob::Blob(const bci::Resource &inResource)
    : mpData(inResource.data), mLength(inResource.length), mType(inResource.type), mpTempFile(0)
{
    NormalizeType(mType);
}

Blob::Blob(const char *data, size_t length, const std::string &type)
    : mpData(data), mLength(length), mType(type), mpTempFile(0)
{
    NormalizeType(mType);
}

Blob::~Blob()
{
    delete mpTempFile;
}

const std::string &Blob::Type() const
{
    return mType;
}

void Blob::GetStringData() const
{
    if (mpData)
    {
        if (mpData != mStringData.data())
        {
            mStringData = std::string(mpData, mLength);
            mpData = mStringData.data();
        }
    }
    else if (mStringData.empty())
    {
        mStringData.reserve(mLength);
        std::ifstream is(mFileName, std::ios::binary | std::ios::in);
        mStringData.assign(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>());
    }
}

bool Blob::SaveAs(const std::string &inFileName) const
{
    std::string newFile = FileUtils::AbsolutePath(inFileName);
    if (FileUtils::ExtractExtension(newFile) != "." + mType)
        newFile += "." + mType;

    std::ofstream out(newFile, std::ios::binary | std::ios::out);
    if (!out.is_open())
        return false;
    if (Write(out))
        mFileName = newFile;
    return !!out;
}

bool Blob::SaveAsTemp() const
{
    bool result = true;
    if (!mpTempFile)
    {
        mpTempFile = new FileUtils::TemporaryFile("." + mType);
        result = !!Write(*mpTempFile);
        mpTempFile->Close();
    }
    if (result)
        mFileName = mpTempFile->Name();
    return result;
}

const std::string &Blob::FileName() const
{
    return mFileName;
}

std::ostream &Blob::WriteAsResource(std::ostream &os) const
{
    std::istream is(0);
    if (mpData)
        is.rdbuf(new std::stringbuf(std::string(mpData, mLength), std::ios::in));
    else
    {
        std::filebuf *p = new std::filebuf;
        p->open(mFileName, std::ios::in | std::ios::binary);
        is.rdbuf(p);
    }
    std::string indent(static_cast<size_t>(os.width()), os.fill());
    std::ios::fmtflags format = os.flags();
    os << indent << std::dec;
    int n = 0;
    int c = 0;
    while ((c = is.get()) != EOF)
    {
        if (!(++n %= 40))
            os << "\n" << indent;
        os << c << ",";
    }
    os.flags(format);
    delete is.rdbuf(0);
    return os;
}

std::ostream &Blob::Write(std::ostream &os) const
{
    if (mpData)
        os.write(mpData, mLength);
    else if (mStringData.length() == mLength)
        os.write(mStringData.data(), mLength);
    else if (!mFileName.empty())
    {
        std::ifstream in(mFileName.c_str(), std::ios::binary | std::ios::in);
        if (!in.is_open())
            os.setstate(std::ios::failbit);
        os << in.rdbuf();
    }
    else
        os.setstate(std::ios::failbit);
    return os;
}

std::ostream &Blob::InsertInto(std::ostream &os) const
{
    os << mType << ":";
    GetStringData();
    return StringUtils::WriteAsBase64(os, mStringData);
}

std::istream &Blob::ExtractFrom(std::istream &is)
{
    std::getline(is, mType, ':');
    mpData = 0;
    delete mpTempFile;
    mpTempFile = 0;
    mFileName.clear();
    if (StringUtils::ReadAsBase64(is, mStringData, &::isspace))
    {
        mpData = mStringData.data();
        mLength = mStringData.size();
    }
    return is;
}
