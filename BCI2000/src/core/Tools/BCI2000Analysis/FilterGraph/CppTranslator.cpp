////////////////////////////////////////////////////////////////////////////////
// $Id: CppTranslator.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: Translate a filter graph into a C++ PipeDefinition file.
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
#include "CppTranslator.h"

#include "FileUtils.h"
#include "FilterGraph.h"
#include "FilterGraphNode.h"
#include "SplitterNode.h"
#include "StaticNodes.h"
#include "ApplicationNodes.h"

#include <ctime>
#include <iomanip>
#include <set>
#include <sstream>

namespace
{
struct UniqueList : std::vector<std::string>
{
    void add(const std::string &s)
    {
        if (std::find(begin(), end(), s) == end())
            push_back(s);
    }
};
} // namespace

namespace FilterGraph
{

struct CppTranslator::Private
{
    std::ostream *pOs;
    UniqueList mIncludes;

    std::string defineSplitter(const SplitterNode *);
    std::string defineSubgraph(const Graph &);
    static bool ignored(const std::string &);
};

CppTranslator::CppTranslator(std::ostream &os) : p(new Private)
{
    p->pOs = &os;
}

CppTranslator::~CppTranslator()
{
    delete p;
}

void CppTranslator::translate(const Graph &graph)
{
    std::time_t now = std::time(nullptr);
    *p->pOs << "// BCI2000 PipeDefinition file\n"
            << "// created by " << FileUtils::ApplicationTitle() << "\n"
            << "// " << std::put_time(std::localtime(&now), "%F %T %z") << "\n\n";
    std::string defs, body;
    char pos = '@';
    for (const auto &pNode : graph)
    {
        if (Private::ignored(pNode->name()))
            continue;
        auto pStaticNode = dynamic_cast<const StaticNode *>(pNode);
        if (pStaticNode)
            continue;
        auto pApplicationNode = dynamic_cast<const BasicApplicationNode *>(pNode);
        if (pApplicationNode)
            continue;
        auto pSplitterNode = dynamic_cast<const SplitterNode *>(pNode);
        if (pSplitterNode)
            defs += p->defineSplitter(pSplitterNode);
        else
            p->mIncludes.add(pNode->name());
        body += "Filter( " + pNode->name() + ", 2." + ++pos + " );\n";
    }
    for (const auto &h : p->mIncludes)
        *p->pOs << "#include \"" << h << ".h\"\n";
    *p->pOs << "\n" << defs << "\n" << body << "\n";
    p->mIncludes.clear();
}

std::string CppTranslator::Private::defineSplitter(const SplitterNode *pSplitterNode)
{
    std::string def = defineSubgraph(pSplitterNode->graph1());
    def += defineSubgraph(pSplitterNode->graph2());
    mIncludes.add("FilterCombination");
    std::string sdef = "struct " + pSplitterNode->name() + " : ParallelCombination<" + pSplitterNode->graph1().name() +
                       ", " + pSplitterNode->graph2().name() + "> {};\n\n";
    def += sdef;
    return def;
}

std::string CppTranslator::Private::defineSubgraph(const Graph &graph)
{
    mIncludes.add("SubchainFilter");
    std::string subdefs;
    std::string def = "struct " + graph.name() +
                      " : SubchainFilter\n"
                      "{\n" +
                      "  " + graph.name() + "()\n  {\n";
    for (auto pNode : graph)
    {
        auto pSplitterNode = dynamic_cast<const SplitterNode *>(pNode);
        if (pSplitterNode)
            subdefs += defineSplitter(pSplitterNode);
        else
            mIncludes.add(pNode->name());
        def += "    Add<" + pNode->name() + ">();\n";
    }
    def += "  }\n};\n\n";
    return subdefs + def;
}

bool CppTranslator::Private::ignored(const std::string &name)
{
    if (name == "AlignmentFilter")
        return true;
    if (name == "TransmissionFilter")
        return true;
    return false;
}

} // namespace FilterGraph
