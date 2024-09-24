////////////////////////////////////////////////////////////////////////////////
// $Id: Param.cpp 7943 2024-03-15 08:05:13Z mellinger $
// Authors: gschalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A type that represents a single BCI2000 parameter.
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
#include "Param.h"

#include "BCIException.h"
#include "Brackets.h"
#include "Debugging.h"

#include <cstdio>
#include <sstream>

static const char *sDefaultValue = "";
static const std::string sCommentSeparator = "//";
static const std::string sReadonlyTag = "(readonly)";
static const std::string sAllowOverrideTag = "(allow_override)";
static const std::string sParentScopeTag = "(parent_scope)";
static const std::string cEmptyString = "";

const std::ctype<char> &Param::ct()
{
    static const std::ctype<char> &_ct = std::use_facet<std::ctype<char>>(std::locale::classic());
    return _ct;
}

void Param::tolower(std::string &s)
{
    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
        *i = ct().tolower(*i);
}

void Param::tolower(StringUtils::String& s)
{
    tolower(static_cast<std::string&>(s));
}

// **************************************************************************
// Function:   SetDimensions
// Purpose:    Sets the dimensions of a matrix parameter.
//             Converts the type if necessary.
// Parameters: inDimension1 - size in dimension 1
//             inDimension2 - size in dimension 2
// Returns:    N/A
// **************************************************************************
Param &Param::SetDimensions(size_t inRows, size_t inCols)
{
    mType = "matrix";
    return SetDimensionsInternal(inRows, inCols);
}

Param &Param::SetDimensionsInternal(size_t inRows, size_t inCols)
{
    // To preserve existing values' indices, insert/remove values as needed.
    size_t rows = NumRows(), cols = NumColumns();
    if (rows != inRows || cols != inCols)
        mChanged = true;
    if (inCols > cols)
        for (size_t i = 0; i < rows; ++i)
            mValues.insert(mValues.begin() + i * inCols + cols, inCols - cols, sDefaultValue);
    else
        for (size_t i = 0; i < rows; ++i)
            mValues.erase(mValues.begin() + (i + 1) * inCols, mValues.begin() + i * inCols + cols);

    mDim1Index.Resize(inRows);
    mDim2Index.Resize(inCols);
    mValues.resize(inRows * inCols, sDefaultValue);

    return *this;
}

// **************************************************************************
// Function:   Param
// Purpose:    The constructor for the Param object
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
Param::Param()
: mReadonly(false), mAllowOverride(false), mParentScope(false),
  mChanged(false), mParentPath("/"), mValues(1, sDefaultValue)
{
}

// **************************************************************************
// Function:   Param
// Purpose:    Constructs and initializes a parameter object
// Parameters: self-explanatory
// Returns:    N/A
// **************************************************************************
Param::Param(const std::string &inPath, const std::string &inSection, const std::string &inType,
             const std::string &inValue, const std::string &inDefaultValue, const std::string &inLowRange,
             const std::string &inHighRange, const std::string &inComment)
    : mDefaultValue(inDefaultValue), mLowRange(inLowRange), mHighRange(inHighRange), mChanged(false), mReadonly(false),
      mAllowOverride(false), mValues(1, sDefaultValue)
{
    auto pos = inPath.rfind("/");
    if (pos == std::string::npos)
    {
        SetName(inPath);
        SetParentPath("/");
    }
    else
    {
        SetName(inPath.substr(pos + 1));
        SetParentPath(inPath.substr(0, pos + 1));
    }
    SetSection(inSection);
    SetType(inType);
    Value() = inValue;
    SetComment(inComment);
}

// **************************************************************************
// Function:   Param
// Purpose:    Constructs and initializes a parameter object, based upon
//             a parameter string, as defined in the project outline
// Parameters: char *paramstring
// Returns:    N/A
// **************************************************************************
Param Param::fromDefinition(const std::string &line)
{
    Param param;
    std::istringstream iss(line);
    if (!(iss >> param))
        throw std::invalid_argument("Invalid parameter line");
    std::string remainder;
    std::getline(iss, remainder, '\0');
    if (!remainder.empty())
        SuggestDebugging << "Unread characters in parameter line: \"" << remainder << "\"";
    return param;
}

// Deprecated
Param::Param(const std::string &parameterDefinition)
{
    *this = fromDefinition(parameterDefinition);
}

// **************************************************************************
// Function:   SetNumValues
// Purpose:    Sets the number of values in this parameter
//             Converts the type to "list" if necessary
// Parameters: new number of values in this parameter
// Returns:    N/A
// **************************************************************************
Param &Param::SetNumValues(size_t inN)
{
    if (inN > 1 && mType.Find("list") == std::string::npos)
        mType = "list";
    return SetDimensionsInternal(inN, 1);
}

// **************************************************************************
// Function:   SetSection
// Purpose:    sets the section name of the parameter
// Parameters: char pointer to the section name
// Returns:    N/A
// **************************************************************************
Param &Param::SetSection(const std::string &s)
{
    if (mSections.empty())
        mSections.push_back(EncodedString(s));
    else
        mSections[0] = s;
    mChanged = true;
    SetComment(mComment);
    return *this;
}

Param &Param::SetComment(const std::string &s)
{
    mChanged = true;
    mComment = s;
    std::string t = mComment;
    tolower(t);
    mReadonly = (t.find(sReadonlyTag) != std::string::npos);
    mAllowOverride = (t.find(sAllowOverrideTag) != std::string::npos);
    mParentScope = (t.find(sParentScopeTag) != std::string::npos);
#if 0
#ifdef TODO
#error Remove "System" check for V4
#endif
  if( !mReadonly )
    if( !mSections.empty() && Param::strciequal( mSections[0], "System" ) )
      if( mSections.size() < 2 || !Param::strciequal( mSections[1], "Command Line Arguments" ) )
      {
        mReadonly = true;
        mComment += sReadonlyTag;
      }
#endif
    return *this;
}

Param &Param::SetParentPath(const std::string &s)
{
    mChanged = true;
    mParentPath = s;
    if (!mParentPath.Empty() && mParentPath.Back() != '/')
        mParentPath.PushBack('/');
    return *this;
}

std::string Param::Path() const
{
    std::string s = mParentPath;
    if (!s.empty() && s.back() != '/')
        s += "/";
    s += mName;
    return s;
}

// **************************************************************************
// Function:   Value
// Purpose:    bounds-checked access to a parameter's value
// Parameters: value index/indices
// Returns:    value reference
// **************************************************************************
const Param::ParamValue &Param::Value(size_t idx) const
{
    if (idx >= mValues.size())
        throw std::range_error("Index " + std::to_string(idx) + " out of range when accessing parameter " + Name());
    return mValues[idx];
}

Param::ParamValue &Param::Value(size_t idx)
{
    mChanged = true;
    return const_cast<Param::ParamValue &>(const_cast<const Param *>(this)->Value(idx));
}

const Param::ParamValue &Param::Value(size_t row, size_t col) const
{
    BoundsCheck(row, col);
    return Value(row * NumColumns() + col);
}

Param::ParamValue &Param::Value(size_t row, size_t col)
{
    BoundsCheck(row, col);
    return Value(row * NumColumns() + col);
}

void Param::BoundsCheck(size_t row, size_t col) const
{
    if (static_cast<int>(row) >= NumRows())
        throw std::range_error("Row index " + std::to_string(row) + " out of range when accessing parameter " + Name());
    if (static_cast<int>(col) >= NumColumns())
        throw std::range_error("Column index " + std::to_string(col) + " out of range when accessing parameter " + Name());
}

bool Param::IsTypeCompatible(const std::string& other) const
{
    if (mType.Find("matrix") != std::string::npos && other.find("matrix") == std::string::npos)
        return false;
    if (mType.Find("list") != std::string::npos && other.find("list") == std::string::npos)
        return false;
    return true;
}

Param &Param::Unchanged()
{
    mChanged = false;
    for (size_t i = 0; i < mValues.size(); ++i)
        if (mValues[i].mpParam)
            mValues[i].mpParam->Unchanged();
    return *this;
}

bool Param::Changed() const
{
    for (size_t i = 0; !mChanged && i < mValues.size(); ++i)
        if (mValues[i].mpParam)
            mChanged = mChanged || mValues[i].mpParam->Changed();
    return mChanged;
}

// **************************************************************************
// Function:   ExtractFrom
// Purpose:    Member function for formatted stream input of a single
//             parameter.
//             All formatted input functions are, for consistency's sake,
//             supposed to use this function.
// Parameters: Input stream to read from.
// Returns:    Input stream read from.
// **************************************************************************
std::istream &Param::ExtractFrom(std::istream &is)
{
    mChanged = true;
    mSections.clear();
    mType.Clear();
    mName.Clear();
    mValues.clear();
    mComment.clear();

    is >> std::ws;
    std::string delimiters = "\n\r";
    EncodedString value;
    // Unnamed parameters are enclosed in bracket pairs, and they omit section and name.
    bool unnamedParam = Brackets::IsOpening(is.peek());
    if (unnamedParam)
    {
        delimiters += Brackets::ClosingMatch(is.get());
        is >> value;
        SetType(value);
    }
    else
    {
        is >> mSections >> mType;
        if (is >> value && value.Length() > 0 && value.Back() == '=')
        {
            std::string path = value.Substr(0, value.Length() - 1);
            size_t pos = path.rfind('/');
            if (pos == std::string::npos)
            {
                SetParentPath("");
                SetName(path);
            }
            else
            {
                SetParentPath(path.substr(0, pos + 1));
                SetName(path.substr(pos + 1));
            }
        }
        else
            is.setstate(std::ios::failbit);
    }

    if (mType.Find("matrix") != std::string::npos)
    {
        is >> mDim1Index >> mDim2Index;
#if 0
        if (mDim2Index.Size() < 1)
            mDim2Index.Resize(1);
#endif
    }
    else if (mType.Find("list") != std::string::npos)
    {
        is >> mDim1Index;
        mDim2Index.Resize(1);
    }
    else
    {
        mDim1Index.Resize(1);
        mDim2Index.Resize(1);
    }

    bool syntaxError = !is;
    // Not all matrix/list entries are required for a parameter definition.
    mValues.resize(mDim1Index.Size() * mDim2Index.Size(), sDefaultValue);
    ValueContainer::iterator i = mValues.begin();
    while (i != mValues.end() && is.peek() != EOF && delimiters.find(is.peek()) == std::string::npos)
        is >> std::ws >> *i++;

    // Remaining elements are optional.
    std::string remainder;
    while (is && is.peek() != EOF && delimiters.find(is.peek()) == std::string::npos)
        remainder += is.get();

    size_t commentSepPos = remainder.rfind(sCommentSeparator);
    if (commentSepPos != std::string::npos)
    {
        size_t commentPos = commentSepPos + sCommentSeparator.length();
        while (commentPos < remainder.size() && ct().is(ct().space, remainder[commentPos]))
            ++commentPos;
        mComment = remainder.substr(commentPos);
        remainder = remainder.substr(0, commentSepPos) + " ";
    }
    std::istringstream iss(remainder);
    EncodedString *finalEntries[] = {&mDefaultValue, &mLowRange, &mHighRange};
    size_t numFinalEntries = sizeof(finalEntries) / sizeof(*finalEntries), entry = 0;
    while (entry < numFinalEntries && iss >> std::ws >> value)
        *finalEntries[entry++] = value;
    while (entry < numFinalEntries)
        *finalEntries[entry++] = EncodedString("");

    if (unnamedParam)
        is.get();
    SetComment(mComment);
    // Use the stream's failbit to report syntax errors.
    is.clear();
    if (syntaxError)
        is.setstate(std::ios::failbit);
    return is;
}

// **************************************************************************
// Function:   InsertInto
// Purpose:    Member function for formatted stream output of a single
//             parameter.
//             All formatted output functions are, for consistency's sake,
//             supposed to use this function.
// Parameters: Output stream to write into.
// Returns:    Output stream written into.
// **************************************************************************
std::ostream &Param::InsertInto(std::ostream &os) const
{
    bool isUnnamed = mName.Empty();
    if (isUnnamed) // Un-named parameters are enclosed in brackets.
        os << Brackets::OpeningDefault << ' ' << mType << ' ';
    else
    {
        EncodedString path = mParentPath + mName;
        os << mSections << ' ' << mType << ' ' << path << "= ";
    }

    if (mType.Find("matrix") != std::string::npos)
        os << RowLabels() << ' ' << ColumnLabels() << ' ';
    else if (mType.Find("list") != std::string::npos)
        os << Labels() << ' ';
    for (int i = 0; i < NumValues(); ++i)
        os << Value(i) << ' ';
    if (!(mDefaultValue.Empty() && mLowRange.Empty() && mHighRange.Empty()))
        os << mDefaultValue << ' ' << mLowRange << ' ' << mHighRange << ' ';
    if (!mComment.empty())
    {
        os << sCommentSeparator << ' ' << mComment;
        if (isUnnamed)
            os << ' ';
    }
    if (isUnnamed)
        os << Brackets::ClosingDefault;

    return os;
}

// **************************************************************************
// Function:   Unserialize
// Purpose:    Member function for input of a single
//             parameter from a binary stream, as in a parameter message.
// Parameters: Input stream to read from.
// Returns:    Input stream read from.
// **************************************************************************
std::istream &Param::Unserialize(std::istream &is)
{
    ExtractFrom(is);
    // Some old modules out there don't send CRLF after binary Param messages.
    if (!is.eof() && (is.get() != '\r'))
        is.setstate(std::ios::failbit);
    if (!is.eof() && (is.get() != '\n'))
        is.setstate(std::ios::failbit);
    return is;
}

// **************************************************************************
// Function:   Serialize
// Purpose:    Member function for output of a single
//             parameter into a binary stream, as in a parameter message.
// Parameters: Output stream to write into.
// Returns:    Output stream written into.
// **************************************************************************
std::ostream &Param::Serialize(std::ostream &os) const
{
    return InsertInto(os).write("\r\n", 2);
}

// **************************************************************************
// Function:   operator=
// Purpose:    Assignment from one parameter instance to another.
// Parameters: Param instance to be assigned.
// Returns:    *this.
// **************************************************************************
Param &Param::operator=(const Param &p)
{
    if (this != &p)
    {
        mSections = p.mSections;
        mName = p.mName;
        mParentPath = p.mParentPath;
        mType = p.mType;
        mDefaultValue = p.mDefaultValue;
        mLowRange = p.mLowRange;
        mHighRange = p.mHighRange;
        SetComment(p.mComment);

        mDim1Index = p.mDim1Index;
        mDim2Index = p.mDim2Index;
        mValues = p.mValues;

        mChanged = p.mChanged;
    }
    return *this;
}

// **************************************************************************
// Function:   AssignValues
// Purpose:    Assignment of parameter values
// Parameters: Param instance to be assigned.
// Returns:    *this.
// **************************************************************************
Param &Param::AssignValues(const Param &p, bool inSetChanged)
{
    if (this != &p)
    {
        if (Readonly())
            throw std::runtime_error("Trying to assign to readonly parameter " + Name());

        if (!IsTypeCompatible(p.mType))
            throw std::runtime_error("Trying to assign " 
                                    + p.mType + " parameter \"" + p.mName 
                                    + "\" to " + mType + " parameter \"" + mName + "\"");

        mDim1Index = p.mDim1Index;
        mDim2Index = p.mDim2Index;
        mValues = p.mValues;

        mChanged = p.mChanged || inSetChanged;
    }
    return *this;
}

/////////////////////////////////////////////////////////////////////////////
// paramValue definitions                                                  //
/////////////////////////////////////////////////////////////////////////////

// **************************************************************************
// Function:   Kind
// Purpose:    Returns the kind of parameter value this instance represents.
// Parameters: kind of ParamValue.
// Returns:    N/A
// **************************************************************************
int Param::ParamValue::Kind() const
{
    int result = Null;
    if (mNative == parameter)
    {
        std::string type = mpParam->Type();
        if (type.find("list") != std::string::npos)
            result = List;
        else if (type.find("matrix") != std::string::npos)
            result = Matrix;
        else
            result = Single;
    }
    else if (mNative == string)
        result = Single;
    return result;
}

// **************************************************************************
// Function:   Assign
// Purpose:    Assigns the content of the ParamValue from another
//             instance of ParamValue.
// Parameters: ParamValue reference.
// Returns:    N/A
// **************************************************************************
void Param::ParamValue::Assign(const ParamValue &p)
{
    if (&p != this)
    {
        Param *pParamToDelete = 0;
        if (p.mNative == parameter)
        {
            delete mpString;
            mpString = 0;
            pParamToDelete = mpParam;
            mpParam = new Param(*p.mpParam);
        }
        else if (p.mNative == string)
        {
            if (!mpString)
                mpString = new EncodedString(*p.mpString);
            else
                *mpString = *p.mpString;
            pParamToDelete = mpParam;
            mpParam = 0;
        }
        mNative = p.mNative;
        delete pParamToDelete; // defer deletion in case assignment is from a sub-parameter
    }
}

// **************************************************************************
// Function:   Assign
// Purpose:    Assigns the content of the ParamValue from a string value.
// Parameters: string reference.
// Returns:    N/A
// **************************************************************************
void Param::ParamValue::Assign(const std::string &s)
{
    mNative = string;
    if (!mpString)
        mpString = new EncodedString(s);
    else
        *mpString = s;
    delete mpParam;
    mpParam = nullptr;
}

// **************************************************************************
// Function:   Assign
// Purpose:    Assigns the content of the ParamValue from a Param instance.
// Parameters: string reference.
// Returns:    N/A
// **************************************************************************
void Param::ParamValue::Assign(const Param &p)
{
    mNative = parameter;
    if (&p != mpParam)
    {
        delete mpParam;
        mpParam = new Param(p);
        delete mpString;
        mpString = NULL;
    }
}

// **************************************************************************
// Function:   AsString
// Purpose:    Returns a ParamValue in string form.
//             If the ParamValue is a Param, the string will be the Param
//             definition line, enclosed in brackets.
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
const StringUtils::String &Param::ParamValue::AsString() const
{
    if (!mpString)
        mpString = new EncodedString;
    if (mNative == parameter)
        *mpString << *this;
    return *mpString;
}

// **************************************************************************
// Function:   AsParam
// Purpose:    Returns a ParamValue as a Param.
//             If the ParamValue is not a Param, the result will be a single
//             valued Param.
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
const Param *Param::ParamValue::AsParam() const
{
    if (!mpParam)
        mpParam = new Param;
    if (mNative == string)
    {
        mpParam->SetNumValues(1);
        mpParam->Value(0) = *mpString;
    }
    return mpParam;
}

// **************************************************************************
// Function:   AsParam
// Purpose:    Returns a ParamValue as a Param.
//             If the ParamValue is not a Param, the result will be a single
//             valued Param.
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
Param *Param::ParamValue::AsParam()
{
    if (!mpParam)
        mpParam = new Param;
    if (mNative == string)
    {
        mpParam->SetNumValues(1);
        mpParam->Value(0) = *mpString;
        mNative = parameter;
    }
    return mpParam;
}

// **************************************************************************
// Function:   InsertInto
// Purpose:    Member function for formatted stream output of a single
//             parameter value.
//             All formatted output functions are, for consistency's sake,
//             supposed to use this function.
// Parameters: Output stream to write into.
// Returns:    Output stream written into.
// **************************************************************************
std::ostream &Param::ParamValue::InsertInto(std::ostream &os) const
{
    switch (mNative)
    {
    case parameter:
        os << *mpParam;
        break;
    case string:
        mpString->InsertInto(os, Brackets::BracketPairs());
        break;
    default:
        os << EncodedString("");
    }
    return os;
}

// **************************************************************************
// Function:   ExtractFrom
// Purpose:    Member function for formatted stream input of a single
//             parameter value.
//             All formatted input functions are, for consistency's sake,
//             supposed to use this function.
// Parameters: Input stream to read from.
// Returns:    Input stream read from.
// **************************************************************************
std::istream &Param::ParamValue::ExtractFrom(std::istream &is)
{
    delete mpString;
    mpString = NULL;
    delete mpParam;
    mpParam = NULL;
    mNative = none;
    if (is >> std::ws)
    {
        if (Brackets::IsOpening(is.peek()))
        {
            mpParam = new Param;
            is >> *mpParam;
            mNative = parameter;
        }
        else
        {
            mpString = new EncodedString;
            is >> *mpString;
            mNative = string;
        }
    }
    return is;
}
