////////////////////////////////////////////////////////////////////////////////
// $Id: DllFilterNode.cpp 8204 2024-06-19 18:22:03Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A node for filters loaded from a shared library.
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
#include "DllFilterNode.h"
#include "DylibImports.h"
#include "FileUtils.h"
#include "FilterGraphNode.h"
#include "FilterWrapperLibrary.h"
#include "VersionInfo.h"

namespace
{
struct FilterLibrary : Dylib::Library
{
    FilterLibrary(const std::string &path) : Dylib::Library(path)
    {
        Dylib::Import sImports[] = {
#define _(x) {#x, (void **)&x, Dylib::Import::cMangled},
            _(InterfaceVersion) _(Compiler) _(BuildType) _(BuildInfo) _(FilterName) _(ReleaseString) 
            _(SetBcierr) _(SetBciwarn) _(SetBciout) _(SetBcidbg) _(SetVisualizationChannel)
            _(SetMeasurementUnits) _(SetEnvironmentContext)
            _(Instantiate)
#undef _
        };
        Library::Resolve(sImports, sizeof(sImports) / sizeof(*sImports));
    }

    const char *(STDCALL *InterfaceVersion)() = nullptr;
    const char *(STDCALL *Compiler)() = nullptr;
    const char *(STDCALL *BuildType)() = nullptr;
    const char *(STDCALL *BuildInfo)() = nullptr;
    const char *(STDCALL *FilterName)() = nullptr;
    void(STDCALL *ReleaseString)(const char *) = nullptr;

    void(STDCALL *SetBcierr)(BCIStream::OutStream *) = nullptr;
    void(STDCALL *SetBciout)(BCIStream::OutStream *) = nullptr;
    void(STDCALL *SetBciwarn)(BCIStream::OutStream *) = nullptr;
    void(STDCALL *SetBcidbg)(BCIStream::OutStream *) = nullptr;

    void(STDCALL *SetVisualizationChannel)(bci::MessageChannel *) = nullptr;
    void(STDCALL *SetMeasurementUnits)(MeasurementUnits *) = nullptr;
    void(STDCALL *SetEnvironmentContext)(Environment::Context *) = nullptr;
    GenericFilter *(STDCALL *Instantiate)(Directory::Node *) = nullptr;
};

struct Registrar : GenericFilter::Registrar
{
    const FilterGraph::Node *mpNode;
    GenericFilter *&mpFilter;
    Directory::Node *mpDirNode;
    FilterLibrary *mpLibrary;
    Registrar(const FilterGraph::Node *pNode, GenericFilter *&pFilter, Directory::Node *pDirNode,
              FilterLibrary *pLibrary)
        : mpNode(pNode), mpFilter(pFilter), mpDirNode(pDirNode), mpLibrary(pLibrary)
    {
    }
    GenericFilter *NewInstance() const override
    {
        mpLibrary->SetBcierr(&bcierr__);
        mpLibrary->SetBciwarn(&bciwarn__);
        mpLibrary->SetBciout(&bciout__);
        mpLibrary->SetBcidbg(&bcidbg__);
        mpLibrary->SetVisualizationChannel(nullptr);
        mpLibrary->SetMeasurementUnits(MeasurementUnits::Instance());
        mpLibrary->SetEnvironmentContext(Environment::Context::CurrentInstance());
        GenericFilter *pFilter = mpLibrary->Instantiate(mpDirNode);
        pFilter->SetName(mpNode->name());
        mpFilter = pFilter;
        return pFilter;
    }
    const std::type_info &Typeid() const override
    {
        return typeid(GenericFilter);
    }
};
} // namespace

namespace FilterGraph
{

struct DllFilterNode::Private
{
    std::string mFilterPath, mError;
    FilterLibrary *mpLibrary;
    Private() : mpLibrary(nullptr)
    {
    }
    ~Private()
    {
        delete mpLibrary;
    }
};

static const char *sRegisteredName = "DllFilterNode";
std::string DllFilterNode::sType = Node::registerType(sRegisteredName, &DllFilterNode::create);

DllFilterNode::DllFilterNode() : Node(sRegisteredName), p(new Private)
{
}

DllFilterNode::~DllFilterNode()
{
    delete p;
}

bool DllFilterNode::setFilterPath(const std::string &s)
{
    std::string refdir = FileUtils::ParentDirectory(FileUtils::ExecutablePath());
    std::string relpath = s, abspath = s;
    if (FileUtils::IsRelativePath(s))
        abspath = refdir + s;
    else
        relpath = FileUtils::RelativePath(s, refdir);
    delete p->mpLibrary;
    p->mFilterPath = relpath.empty() ? abspath : relpath;
    std::string libname = FileUtils::ExtractFile(abspath);
    p->mpLibrary = new FilterLibrary(abspath);
    if (p->mpLibrary->State() != Dylib::Library::resolvedAll)
    {
        p->mError = p->mpLibrary->Error();
        if (p->mError.empty())
            p->mError = "\"" + s + "\" is not a BCI2000 filter library.";
    }
    if (p->mError.empty())
    {
        const char *pVersion = p->mpLibrary->InterfaceVersion();
        std::string version = pVersion;
        p->mpLibrary->ReleaseString(pVersion);
        if (version != FILTER_WRAPPER_INTERFACE_VERSION)
        {
            p->mError = "Filter interface version mismatch between BCI2000Analysis, and "
                        + libname + ":\nneed \"" FILTER_WRAPPER_INTERFACE_VERSION "\", got \""
                        + version + "\".";
        }
    }
    if (p->mError.empty())
    {
        const char *pCompiler = p->mpLibrary->Compiler();
        std::string compiler = pCompiler;
        p->mpLibrary->ReleaseString(pCompiler);
        if (compiler != VersionInfo::Current[VersionInfo::Compiler])
        {
            p->mError = "Compiler mismatch between BCI2000Analysis, and "
                        + libname + ":\nneed \"" + VersionInfo::Current[VersionInfo::Compiler]
                        + "\", got \"" + compiler + "\".";
        }
    }
#if _MSC_VER
    if (p->mError.empty())
    {
        const char *pBuildType = p->mpLibrary->BuildType();
        std::string buildType = pBuildType;
        p->mpLibrary->ReleaseString(pBuildType);
        if (buildType != VersionInfo::Current[VersionInfo::BuildType])
        {
            p->mError =
                "Build type mismatch between BCI2000Analysis, and "
                + libname + ":\nneed \"" + VersionInfo::Current[VersionInfo::BuildType]
                + "\", got \"" + buildType + "\".";
        }
    }
#endif // _MSC_VER
    if (p->mError.empty() && name() == sRegisteredName)
    {
        const char *pName = p->mpLibrary->FilterName();
        Node::setName(pName);
        p->mpLibrary->ReleaseString(pName);
    }
    return p->mError.empty();
}

const std::string &DllFilterNode::filterPath() const
{
    return p->mFilterPath;
}

const std::string &DllFilterNode::error() const
{
    return p->mError;
}

Node *DllFilterNode::clone() const
{
    DllFilterNode *pNode = new DllFilterNode;
    pNode->copyFrom(this);
    pNode->setFilterPath(p->mFilterPath);
    return pNode;
}

Node *DllFilterNode::create()
{
    return new DllFilterNode;
}

const std::string &DllFilterNode::type() const
{
    return sType;
}

GenericFilter::Registrar *DllFilterNode::getRegistrar(Directory::Node *pNode) const
{
    if (!p->mpLibrary || !p->mError.empty())
        return nullptr;
    return new Registrar(this, this->mpFilter, pNode, p->mpLibrary);
}

void DllFilterNode::onSerialize(std::ostream &os) const
{
    Node::onSerialize(os);
    os << "{\n\"" << p->mFilterPath << "\"\n}\n";
}

void DllFilterNode::onUnserialize(std::istream &is)
{
    is >> std::ws;
    if (is.peek() != '{')
        is.setstate(std::ios::failbit);
    is.ignore();
    is >> std::ws;
    std::string path;
    if (is.peek() == '"')
    {
        is.ignore();
        std::getline(is, path, '"');
    }
    else
    {
        std::getline(is, path);
    }
    is >> std::ws;
    if (is.peek() != '}')
        is.setstate(std::ios::failbit);
    is.ignore();
    if (is.good())
        setFilterPath(path);
}

} // namespace FilterGraph
