////////////////////////////////////////////////////////////////////////////////
// $Id: ParamList.cpp 8291 2024-07-30 16:17:34Z mellinger $
// Authors: gschalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: ParamList is a BCI2000 type that represents a collection of
//   parameters.
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
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
#include "ParamList.h"

#include "Debugging.h"
#include "ParamRef.h"

#include <algorithm>
#include <fstream>
#include <set>
#include <sstream>

namespace
{
std::string RevertPath(std::string path)
{
    std::vector<std::string> elements;
    size_t pos1 = 0, pos2 = path.find('/');
    while (pos2 != std::string::npos)
    {
        elements.push_back(path.substr(pos1, pos2 - pos1));
        pos1 = pos2 + 1;
        pos2 = path.find('/', pos2 + 1);
    }
    elements.push_back(path.substr(pos1));
    std::string rev;
    for (auto i = elements.rbegin(); i != elements.rend(); ++i)
        rev += "/" + *i;
    return rev.substr(1);
}
} // namespace

int PathLength(const std::string &s)
{
    int length = 0;
    for (const auto &c : s)
        if (c == '/')
            ++length;
    return length;
}

std::string TailPath(const std::string &s, int j)
{
    int count = 0;
    int i = s.length();
    while (count < j && i > 0)
        if (s[--i] == '/')
            ++count;
    while (i > 0 && s[i - 1] != '/')
        --i;
    return s.substr(i);
}

struct ParamList::Private
{
    struct ParamEntry
    {
        ParamEntry() : SortingHint(0.0)
        {
        }
        class Param Param;
        float SortingHint;
        static bool Compare(const ParamEntry *p, const ParamEntry *q)
        {
            return p->SortingHint < q->SortingHint;
        }
    };
    static bool PathCiLess(char a, char b)
    {
        if (b == '/')
            return false;
        if (a == '/')
            return true;
        return Param::ciless(a, b);
    }
    struct PathCmp
    {
        bool operator()(const std::string &a, const std::string &b) const
        {
            return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(), PathCiLess);
        }
    };
    typedef std::map<std::string, ParamEntry, PathCmp> ParamContainer;
    struct Range
    {
        ParamContainer::iterator begin, end;
        int length() const
        {
            int n = 0;
            for (auto i = begin; i != end; ++i)
                ++n;
            return n;
        }
    };
    ParamContainer mParams;
    typedef std::vector<ParamEntry *> Index;
    Index mIndex;

    void BuildIndex()
    {
        mIndex.clear();
        for (ParamContainer::iterator i = mParams.begin(); i != mParams.end(); ++i)
            mIndex.push_back(&i->second);
    }
    // Get the entire range of entries matching at the path's end.
    Range GetRange(const std::string &path) const
    {
        auto &params = const_cast<ParamContainer &>(mParams);
        Range r = {params.end(), params.end()};
        if (path.empty())
            return r;
        if (path.front() == '/') // absolute path, strict match
        {
            auto i = params.find(RevertPath(path));
            if (i != params.end())
            {
                r.begin = i;
                r.end = ++i;
            }
            return r;
        }
        // relative path, subpath match
        std::string revpath = RevertPath(path);
        r.begin = params.lower_bound(revpath);
        r.end = r.begin;
        while (r.end != params.end() && SubpathMatch(r.end->first, revpath))
            ++r.end;
        return r;
    }

    static bool SubpathMatch(const std::string &path, const std::string &subpath)
    {
        if (subpath.length() > path.length())
            return false;
        if (subpath.length() == path.length())
            return subpath == path;
        if (path[subpath.length()] == '/')
            return path.find(subpath) == 0;
        return false;
    }

    void AssignFrom(const Private *other)
    {
        mParams = other->mParams;
        BuildIndex();
    }
};

ParamList::ParamList() : p(new Private)
{
}

ParamList::~ParamList()
{
    delete p;
}

ParamList::ParamList(const ParamList &other) : p(new Private)
{
    p->AssignFrom(other.p);
}

ParamList &ParamList::operator=(const ParamList &other)
{
    if (&other != this)
        p->AssignFrom(other.p);
    return *this;
}

ParamList::iterator ParamList::begin()
{
  return iterator(this, 0);
}

ParamList::iterator ParamList::end()
{
  return iterator(this, Size());
}

ParamList::const_iterator ParamList::begin() const
{
    return const_iterator(this, 0);
}

ParamList::const_iterator ParamList::end() const
{
    return const_iterator(this, Size());
}

int ParamList::Size() const
{
    return static_cast<int>(p->mParams.size());
}

bool ParamList::Empty() const
{
    return p->mParams.empty();
}

bool ParamList::Exists(const std::string &name) const
{
    auto r = p->GetRange(name);
    return r.begin != r.end;
}

const Param &ParamList::ByIndex(size_t index) const
{
    return p->mIndex.at(index)->Param;
}

Param &ParamList::ByIndex(size_t index)
{
    return p->mIndex.at(index)->Param;
}

// **************************************************************************
// Function:   ByName
// Purpose:    Access a parameter by its name.
// Parameters: Parameter name.
// Returns:    Returns a reference to a parameter with a given name.
// **************************************************************************
Param &ParamList::ByPath(const std::string &inPath)
{
    static Param emptyParam = Param().SetName("").SetSection("Default").SetType("int");
    if (inPath.empty())
        return emptyParam;

    auto r = p->GetRange(inPath);
    if (r.begin == r.end)
    {
        auto revpath = RevertPath(inPath);
        if (revpath.back() != '/')
            revpath.push_back('/');
        p->mIndex.push_back(&p->mParams[revpath]);
        std::string path = inPath;
        if (path.front() != '/')
            path = "/" + path;
        size_t pos = path.rfind('/');
        p->mIndex.back()->Param.SetName(path.substr(pos + 1));
        p->mIndex.back()->Param.SetParentPath(path.substr(0, pos + 1));
        return p->mIndex.back()->Param;
    }
    return r.begin->second.Param;
}

const Param &ParamList::ByPath(const std::string &inPath) const
{
    static Param defaultParam = Param().SetName("").SetSection("Default").SetType("int");
    const Param *result = &defaultParam;
    auto r = p->GetRange(inPath);
    if (r.begin != r.end)
        result = &r.begin->second.Param;
    return *result;
}

std::string ParamList::OriginalPath(const Param *inParam) const
{
    for (const auto &entry : p->mParams)
        if (&entry.second.Param == inParam)
            return RevertPath(entry.first);
    return inParam->Path();
}

MutableParamRef ParamList::operator()(const std::string &inPath)
{
    return MutableParamRef(&ByPath(inPath));
}

ParamRef ParamList::operator()(const std::string &inPath) const
{
    return ParamRef(&ByPath(inPath));
}

// **************************************************************************
// Function:   Clear
// Purpose:    deletes all parameters in the parameter list
//             as a result, the list still exists, but does not contain any parameter
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void ParamList::Clear()
{
    p->mIndex.clear();
    p->mParams.clear();
}

// **************************************************************************
// Function:   Add
// Purpose:    adds a new parameter to the list of parameters
//             if a parameter with the same name already exists,
//             it updates the currently stored parameter with the provided values
// Parameters: inParam       - reference to a Param object representing the
//                             parameter,
//             inSortingHint - float value used as an additional sorting
//                             criterion.
// Returns:    N/A
// **************************************************************************
void ParamList::Add(const Param &inParam, float inSortingHint)
{
    const Param &param = ByPath(inParam.Path());
    auto path = this->OriginalPath(&param);
    auto revpath = RevertPath(path);
    Private::ParamEntry &entry = p->mParams[revpath];
    entry.Param = inParam;
    entry.SortingHint = inSortingHint;
}

// **************************************************************************
// Function:   Add
// Purpose:    adds a new parameter to the list of parameters
//             if a parameter with the same name already exists,
//             it updates the currently stored parameter with the provided values
// Parameters: inLine - ASCII string, as defined in project description,
//                           defining this parameter
// Returns:    true if inLine is a correct parameter line, false otherwise
// **************************************************************************
bool ParamList::Add(const std::string &inLine)
{
    std::istringstream linestream(inLine);
    Param param;
    if (linestream >> param)
        Add(param);
    return !!linestream;
}

// **************************************************************************
// Function:   Delete
// Purpose:    deletes a parameter of a given name in the list of parameters
//             it also frees the memory for this particular parameter
//             it does not do anything, if the parameter does not exist
// Parameters: name - name of the parameter
// Returns:    N/A
// **************************************************************************
bool ParamList::Delete(const std::string &inPath)
{
    auto r = p->GetRange(inPath);
    if (r.begin == r.end)
        return false;

    Private::ParamContainer::iterator i = r.begin;
    if (i != p->mParams.end())
    {
        Private::Index::iterator j = p->mIndex.begin();
        while (j != p->mIndex.end() && *j != &i->second)
            ++j;
        Assert(j != p->mIndex.end());
        p->mIndex.erase(j);
        p->mParams.erase(i);
    }
    return true;
}

// **************************************************************************
// Function:   InsertInto
// Purpose:    Member function for formatted stream output of the entire
//             parameter list.
//             For partial output, use another instance of type ParamList
//             to hold the desired subset as in ParamList::SaveParameterList().
//             All formatted output functions are, for consistency's sake,
//             supposed to use this function.
// Parameters: Output stream to write into.
// Returns:    Output stream.
// **************************************************************************
std::ostream &ParamList::InsertInto(std::ostream &os) const
{
    for (int i = 0; i < Size(); ++i)
        os << ByIndex(i) << '\n';
    return os;
}

// **************************************************************************
// Function:   ExtractFrom
// Purpose:    Member function for formatted stream input of the entire
//             parameter list. The list is cleared before reading.
//             For partial input, use another instance of type ParamList
//             to hold the desired subset as in ParamList::LoadParameterList().
//             All formatted input functions are, for consistency's sake,
//             supposed to use this function.
// Parameters: Input stream to read from.
// Returns:    Input stream.
// **************************************************************************
std::istream &ParamList::ExtractFrom(std::istream &is)
{
    Clear();
    Param param;
    is >> std::ws;
    while (!is.eof() && is >> param >> std::ws)
        ByPath(param.Path()) = param;
    return is;
}

// **************************************************************************
// Function:   Serialize
// Purpose:    Member function for binary stream output of the entire
//             parameter list.
//             For partial output, use another instance of type ParamList
//             to hold the desired subset as in ParamList::SaveParameterList().
// Parameters: Output stream to write into.
// Returns:    Output stream written into.
// **************************************************************************
std::ostream &ParamList::Serialize(std::ostream &os) const
{
    for (int i = 0; i < Size(); ++i)
        ByIndex(i).Serialize(os);
    return os;
}

// **************************************************************************
// Function:   Unserialize
// Purpose:    Member function for binary stream input of the entire
//             parameter list. The list is cleared before reading.
//             For partial input, use another instance of type ParamList
//             to hold the desired subset as in ParamList::LoadParameterList().
// Parameters: Input stream to read from.
// Returns:    N/A
// **************************************************************************
std::istream &ParamList::Unserialize(std::istream &is)
{
    return ExtractFrom(is);
}

// **************************************************************************
// Function:   Save
// Purpose:    Saves the current list of paramters in a parameter file
// Parameters: char *filename - filename to save the list to
//             usetags - if usetags is true, then the "tag" value in each parameter
//                       determines whether the parameter should be saved
//                       if the tag value in the parameter is true, then the
//                       parameter will NOT be saved
//                       if usetags is false, then all parameters are saved
// Returns:    true - successful
//             false - error (disk full, etc.)
// **************************************************************************
bool ParamList::Save(const std::string &inFileName) const
{
    std::ofstream file(inFileName);
    if (!file.is_open())
        return false;
    file << *this;
    return !file.fail();
}

// **************************************************************************
// Function:   Load
// Purpose:    Load parameters from a parameter file.
// Parameters: const std::string& inFilename - filename of the parameter file.
//             inImportNonexisting - if true, load parameters, even if they 
//                  currently do not exist in the list
// Returns:    true - successful
//             false - error
// **************************************************************************
bool ParamList::Load(const std::string &inFileName, bool inImportNonexisting)
{
    std::ifstream file(inFileName);
    ParamList paramsFromFile;
    file >> paramsFromFile;
    if (file.fail())
        return false;

    return CopyFrom(paramsFromFile);
}

// **************************************************************************
// Function:   CopyFrom
// Purpose:    Copy parameters from another ParamList, optionally leaving
//             out parameters that do not currently exist.
// Parameters: const ParamList& inOther - parameter list to copy from
//             nonexisting - if true, copy parameters even if they currently
//                       do not exist in the list
// Returns:    true - successful
//             false - error
// **************************************************************************
bool ParamList::CopyFrom(const ParamList& inOther, bool inImportNonexisting)
{
    ParamList paramsFromOther = inOther;

    // If desired, exclude parameters missing from the main parameter list.
    typedef std::set<std::string> NameSet;
    NameSet unwantedParams;
    if (!inImportNonexisting)
        for (int i = 0; i < paramsFromOther.Size(); ++i)
            if (!Exists(paramsFromOther.ByIndex(i).Path()))
                unwantedParams.insert(paramsFromOther.ByIndex(i).Path());

    for (NameSet::const_iterator i = unwantedParams.begin(); i != unwantedParams.end(); ++i)
        paramsFromOther.Delete(*i);

    for (int i = 0; i < paramsFromOther.Size(); ++i)
    {
        const Param& p = paramsFromOther.ByIndex(i);
        if (Exists(p.Path()))
        {
            Param& q = ByPath(p.Path());
            if (!q.Readonly())
                q.AssignValues(p, true);
        }
        else
        {
            Add(p);
        }
    }
    return true;
}

// **************************************************************************
// Function:   Join
// Purpose:    Join with parameters from another parameter list.
// Parameters: Parameter list to join with.
// Returns:    N/A
// **************************************************************************
void ParamList::Join(const ParamList& other)
{
    for (const auto& param : other)
    {
        if (Exists(param.Path()))
            ByPath(param.Path()).AssignValues(param);
        else
            Add(param);
    }
}

// **************************************************************************
// Function:   Sort
// Purpose:    Sorts parameters by their section fields, and sorting hints.
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void ParamList::Sort()
{
    // stable_sort will retain the relative order of parameters with identical
    // sections.
    stable_sort(p->mIndex.begin(), p->mIndex.end(), Private::ParamEntry::Compare);
}

// **************************************************************************
// Function:   Prune
// Purpose:    Reduces parameter paths to a minimum, maintaining distinguishability.
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void ParamList::Prune()
{
    for (auto i = p->mIndex.begin(); i != p->mIndex.end(); ++i)
    {
        auto parentPath = (*i)->Param.ParentPath();
        auto pathLength = PathLength(parentPath);
        for (int j = 0; j <= pathLength; ++j)
        {
            auto s = TailPath(parentPath, j);
            auto r = p->GetRange(s + (*i)->Param.Name());
            if (r.length() < 2)
            {
                if (parentPath != s) // don't accidentally mark Param as changed
                    (*i)->Param.SetParentPath(s);
                break;
            }
        }
    }
}

void ParamList::Unchanged()
{
    for (Private::Index::const_iterator i = p->mIndex.begin(); i != p->mIndex.end(); ++i)
        (*i)->Param.Unchanged();
}
