%output "ExpressionParser.cpp"
%defines
%{
////////////////////////////////////////////////////////////////////////
// $Id: ExpressionParser.y 7677 2023-10-25 16:38:30Z mellinger $
// Author:      juergen.mellinger@uni-tuebingen.de
// Description: Bison grammar file for a simple expression parser.
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
#include "ArithmeticExpression.h"
#include "StringUtils.h"

#include <limits>

%}

%define api.pure
%define parse.error verbose
%parse-param { ::ArithmeticExpression* p }
%lex-param   { ::ArithmeticExpression* p }
%union
{
  ExpressionParser::StringNode*  str;
  ExpressionParser::Node*        node;
  ExpressionParser::NodeList*    list;
}

%{
namespace ExpressionParser
{
%}

/* Bison declarations.  */
%token <node>    NUMBER
%token <str>     NAME STRING STATE SIGNAL_ /* avoid interference with Qt's SIGNAL macro */
%left '?' ':'
%left '&' '|'
%left '=' '~' '!' '>' '<'
%left '-' '+'
%left '*' '/'
%left NEG     /* negation--unary minus */
%right '^'    /* exponentiation */
%left '.'     /* element operator */
%left ';'

%type <str> statename
%type <node> input statements statement exp addr assignment
%type <list> args list

%% /* The grammar follows.  */
input: /* empty */                   { $$ = NULL; }
     | statements                    { $$ = $1; }
;

statements:
       statement                     { $$ = $1; p->Add( $1 ); }
     | statements ';' statement      { $$ = $3; p->Add( $3 ); }
     | statements ';'                { $$ = $1; }
     | ';'                           { $$ = 0; }
;

statement:     
       exp                           { $$ = $1; }
     | assignment                    { $$ = $1; }
;

exp:   NUMBER                        { $$ = $1; }
     | STRING                        { $$ = $1; }
     | exp '+' exp                   { $$ = p->MakeFunction( @$, "+", $1, $3 ); }
     | exp '-' exp                   { $$ = p->MakeFunction( @$, "-", $1, $3 ); }
     | exp '*' exp                   { $$ = p->MakeFunction( @$, "*", $1, $3 ); }
     | exp '/' exp                   { $$ = p->MakeFunction( @$, "/", $1, $3 ); }
     | '-' exp %prec NEG             { $$ = p->MakeFunction( @$, "-1", $2 ); }
     | exp '^' exp                   { $$ = p->MakeFunction( @$, "^", $1, $3 ); }
     | exp '&' '&' exp               { $$ = p->MakeFunction( @$, "&&", $1, $4 ); }
     | exp '|' '|' exp               { $$ = p->MakeFunction( @$, "||", $1, $4 ); }
     | exp '=' '=' exp               { $$ = p->MakeFunction( @$, "==", $1, $4 ); }
     | exp '!' '=' exp               { $$ = p->MakeFunction( @$, "!=", $1, $4 ); }
     | exp '~' '=' exp               { $$ = p->MakeFunction( @$, "!=", $1, $4 ); }
     | exp '>' exp                   { $$ = p->MakeFunction( @$, ">", $1, $3 ); }
     | exp '<' exp                   { $$ = p->MakeFunction( @$, "<", $1, $3 ); }
     | exp '>' '=' exp               { $$ = p->MakeFunction( @$, ">=", $1, $4 ); }
     | exp '<' '=' exp               { $$ = p->MakeFunction( @$, "<=", $1, $4 ); }
     | '~' exp %prec NEG             { $$ = p->MakeFunction( @$, "!", $2 ); }
     | '!' exp %prec NEG             { $$ = p->MakeFunction( @$, "!", $2 ); }
     | '(' exp ')'                   { $$ = $2; }
     | exp '?' exp ':' exp           { $$ = p->MakeFunction( @$, "?:", $1, $3, $5 ); }
     
     | NAME                          { $$ = p->MakeVariable( @$, $1 ); }
     | NAME '(' args ')'             { $$ = p->MakeFunction( @$, $1, *$3 ); }
     | NAME '.' NAME '(' args ')'    { $$ = p->MakeMemberFunction( @$, $1, $3, *$5 ); }

     | STATE '(' statename ')'       { $$ = p->MakeState( @$, $3 ); }
     | SIGNAL_ '(' addr ',' addr ')' { $$ = p->MakeSignal( @$, $3, $5 ); }
     | SIGNAL_ '(' addr ')'          { $$ = p->MakeSignal( @$, $3, nullptr ); }
;

assignment: /* assignment uses the := operator to prevent unwanted assignment */
       NAME ':' '=' exp              { $$ = p->MakeVariableAssignment( @$, $1, $4 ); }
     | STATE '(' statename ')' ':' '=' exp
                                     { $$ = p->MakeStateAssignment( @$, $3, $7 ); }
;

args: /* empty */                    { $$ = p->MakeList(); }
     | list                          { $$ = $1; }
;

list:  exp                           { $$ = p->MakeList( $1 ); }
     | list ',' exp                  { $$ = $1; $$->Add( $3 ); }
;

addr:  exp                           { $$ = p->MakeAddress( @$, $1 ) ; }
     | exp NAME                      { $$ = p->MakeAddress( @$, $1, $2 ); }
;

statename:
       NAME                          { $$ = $1; }
     | STRING                        { $$ = $1; }
;

%%

  int
  yylex( YYSTYPE* pLval, YYLTYPE* pLoc, ArithmeticExpression* pInstance )
  {
    int token = -1;

    Node* pNode = nullptr;
    pInstance->mInput >> std::ws;
    pLoc->first_column = pInstance->mInput.tellg();
    int c = pInstance->mInput.peek();
    if( c == EOF )
      token = 0;
    else if( ::isdigit( c ) )
    {
      double value;
      if( pInstance->mInput >> value )
      {
        token = NUMBER;
        pLval->node = new ConstantNode(value);
        pNode = pLval->node;
      }
    }
    else if( pInstance->IsAllowedInsideName(c) )
    {
      std::string s;
      while( pInstance->IsAllowedInsideName(c) )
      {
        s += c;
        pInstance->mInput.ignore();
        c = pInstance->mInput.peek();
      }
      if( StringUtils::Ci(s) == "state" )
        token = STATE;
      else if( StringUtils::Ci(s) == "signal" )
        token = SIGNAL_;
      else
      {
        token = NAME;
        pLval->str = new StringNode( s );
        pNode = pLval->str;
      }
    }
    else if( c == '"' || c == '\'' ) // string literals
    {
      token = STRING;
      pInstance->mInput.ignore();
      std::string s;
      int d = pInstance->mInput.get();
      while (d != EOF && d != c)
      {
        s.push_back(d);
        d = pInstance->mInput.get();
      }
      if (d != c)
      {
         pInstance->MakeError(*pLoc, "unmatched quotes");
      }
      else
      {
        pLval->str = new StringNode( s );
        pNode = pLval->str;
      }
    }
    else if( c == '/' ) // handle comments
    {
      pInstance->mInput.ignore();
      token = c;
      if( pInstance->mInput.peek() == '/' )
      {
        pInstance->mInput.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
        token = yylex( pLval, pLoc, pInstance );
      }
    }
    else
    {
      pInstance->mInput.ignore();
      token = c;
    }
    pLoc->last_column = pInstance->mInput.tellg();
    if(pNode)
    {
      pNode->SetLocation(pLoc->first_column, pLoc->last_column);
      pInstance->Track(pNode);
    }
    return token;
  }

  void
  yyerror( YYLTYPE* pLloc, ArithmeticExpression* pInstance, const char* pError )
  {
    pInstance->MakeError(*pLloc, pError);
  }

} // namespace ExpressionParser


