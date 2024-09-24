////////////////////////////////////////////////////////////////////////
// $Id: Script.h 7331 2023-04-24 15:41:59Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that represents a parsed script.
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
////////////////////////////////////////////////////////////////////////
#ifndef SCRIPT_H
#define SCRIPT_H

#include "ScriptParser.h"
#include <sstream>
#include <vector>

class Script
{
  public:
    Script(const std::string &script, const std::string &name = "");
    ~Script();

    const std::string& Name() const;
    Script &Compile();
    Script& Execute(class CommandInterpreter&);
    Script &ExecuteAsSubroutine(class CommandInterpreter &, const std::vector<std::string> &args);

  private:
    // Interface to ParserNode
    void Track(ScriptParser::ParserNode *p)
    {
        mNodes.push_back(p);
    }
    int Line() const
    {
        return mLine;
    }
    void Line(int line)
    {
        mLine = line;
    }

  private:
    void ReportError(const std::string &) const;
    void Cleanup();

    std::istringstream mInputStream;
    std::string mName;
    int mLine;
    std::string mError;
    ScriptParser::ParserNode *mpRootNode;
    std::vector<ScriptParser::ParserNode *> mNodes;

    friend class ScriptParser::ParserNode;
    friend int ScriptParser::yyparse(Script *);
    friend int ScriptParser::yylex(ScriptParser::YYSTYPE *, Script *);
    friend void ScriptParser::yyerror(Script *, const char *);
};

#endif // SCRIPT_H
