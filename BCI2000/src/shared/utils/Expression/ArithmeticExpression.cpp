//////////////////////////////////////////////////////////////////////////////////////
// $Id: ArithmeticExpression.cpp 8071 2024-04-24 18:34:36Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A simple expression parser for use within BCI2000.
//   ArithmeticExpression provides expression parsing; its Expression
//   descendant, in addition, allows access to State and Signal values.
//   For details about expression syntax, see Expression.h.
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
//////////////////////////////////////////////////////////////////////////////////////
#include "ArithmeticExpression.h"

#include "BCIException.h"
#include "BCIStream.h"
#include "ClassName.h"
#include "Numeric.h"

#include <cmath>
#include <iostream>
#include <limits>
#include <string>

using namespace ExpressionParser;

static const std::pair<std::string, double> sConstants[] = {
    std::make_pair("pi", 4.0 * ::atan(1.0)),
    std::make_pair("e", ::exp(1.0)),
    std::make_pair("inf", std::numeric_limits<double>::infinity()),
    std::make_pair("nan", std::numeric_limits<double>::quiet_NaN()),
    std::make_pair("true", 1),
    std::make_pair("false", 0),
};
const ArithmeticExpression::VariableContainer ArithmeticExpression::Constants(
    &sConstants[0], &sConstants[sizeof(sConstants) / sizeof(*sConstants)]);

ArithmeticExpression::ArithmeticExpression(const std::string &s)
    : mExpression(s), mThrowOnError(false), mCompilationState(none)
{
}

ArithmeticExpression::ArithmeticExpression(const ArithmeticExpression &e)
    : mExpression(e.mExpression), mThrowOnError(e.mThrowOnError), mCompilationState(none)
{
}

ArithmeticExpression::~ArithmeticExpression()
{
}

const ArithmeticExpression &ArithmeticExpression::operator=(const ArithmeticExpression &e)
{
    mExpression = e.mExpression;
    mInput.str("");
    mInput.clear();
    mErrors.str("");
    mErrors.clear();
    mContext = Context();
    mThrowOnError = e.mThrowOnError;
    mCompilationState = none;
    CollectGarbage();
    mStatements.Clear();
    return *this;
}

bool ArithmeticExpression::Compile(const Context &inContext)
{
    mContext = inContext;
    mCompilationState = Parse() ? success : attempted;
    ReportErrors();
    return mCompilationState == success;
}

bool ArithmeticExpression::IsValid(const Context &inContext)
{
    if (mCompilationState != none)
        throw std_logic_error << "IsValid() called on compiled expression";
    mErrors.clear();
    mErrors.str("");
    mContext = inContext;
    if (mContext.variables)
        mContext.variables = new VariableContainer(*mContext.variables);
    if (Parse())
        DoEvaluate();
    delete mContext.variables;
    mContext = Context();
    bool result = mErrors.str().empty();
    mErrors.clear();
    mErrors.str("");
    return result;
}

double ArithmeticExpression::Evaluate()
{
    double result = 0;
    if (mCompilationState == none)
        Compile();
    if (mCompilationState == success)
        result = DoEvaluate();
    ReportErrors();
    return result;
}

double ArithmeticExpression::DoEvaluate()
{
    double result = 0;
    try
    {
        for (int i = 0; i < mStatements.Size(); ++i)
            result = mStatements[i]->Evaluate();
    }
    catch (const Tiny::Exception &e)
    {
        Errors() << e.What() << std::endl;
    }
    return result;
}

void ArithmeticExpression::Add(Node *inpNode)
{
    mStatements.Add(inpNode);
}

bool ArithmeticExpression::IsAllowedInsideName(char c) const
{
    return ::isalnum(c) || c == '_';
}

Node *ArithmeticExpression::Variable(const std::string &inName)
{
    Node *result = NULL;
    if (mContext.constants)
    {
        VariableContainer::const_iterator i = mContext.constants->find(inName);
        if (i != mContext.constants->end())
            result = new ConstantNode(i->second);
    }
    if (!result && mContext.variables)
    {
        VariableContainer::iterator i = mContext.variables->find(inName);
        if (i != mContext.variables->end())
            result = new VariableNode(i->second);
    }
    if (!result)
        throw ParsingError() << "Variable \"" << inName << "\" does not exist";
    return result;
}

Node *ArithmeticExpression::VariableAssignment(const std::string &inName, Node *inRHS)
{
    Node *result = NULL;
    if (mContext.constants && mContext.constants->find(inName) != mContext.constants->end())
        throw ParsingError() << inName << ": Not assignable";
    else if (!mContext.variables)
        throw ParsingError() << inName << ": Cannot create variables";
    else
        result = new AssignmentNode((*mContext.variables)[inName], inRHS);
    return result;
}

namespace
{
double Add(double a, double b)
{
    return a + b;
}
double Subtract(double a, double b)
{
    return a - b;
}
double Negate(double a)
{
    return -a;
}
double Multiply(double a, double b)
{
    return a * b;
}
double Divide(double a, double b)
{
    return a / b;
}

double Equal(double a, double b)
{
    return a == b;
}
double NotEqual(double a, double b)
{
    return a != b;
}
double Greater(double a, double b)
{
    return a > b;
}
double GreaterEqual(double a, double b)
{
    return a >= b;
}
double Less(double a, double b)
{
    return a < b;
}
double LessEqual(double a, double b)
{
    return a <= b;
}

double Not(double a)
{
    return !a;
}
double And(double a, double b)
{
    return a && b;
}
double Or(double a, double b)
{
    return a || b;
}
double Conditional(double a, double b, double c)
{
    return a ? b : c;
}

double Min(double a, double b)
{
    return std::min(a, b);
}
double Max(double a, double b)
{
    return std::max(a, b);
}

double bits(double value, double pos, double count)
{
    if (value != ::floor(value) || pos < 0)
        return NaN(value);
    uint64_t mask = ~0LL;
    mask >>= (64 - int(count));
    return (uint64_t(value) >> int(pos)) & mask;
}

double bit(double value, double pos)
{
    if (value != ::floor(value) || pos < 0)
        return NaN(value);
    return uint64_t(value) & (1 << int(pos)) ? 1 : 0;
}
} // namespace

Node *ArithmeticExpression::Function(const std::string &inName, const NodeList &inArguments)
{
    Node *result = NULL;

#define CONSTFUNC0(x) {#x, true, 0, (void *)static_cast<FunctionNode<0>::Pointer>(&::x)},
#define CONSTFUNC1(x) {#x, true, 1, (void *)static_cast<FunctionNode<1>::Pointer>(&::x)},
#define CONSTFUNC2(x) {#x, true, 2, (void *)static_cast<FunctionNode<2>::Pointer>(&::x)},
#define CONSTFUNC3(x) {#x, true, 3, (void *)static_cast<FunctionNode<3>::Pointer>(&::x)},

    static const struct
    {
        const char *name;
        bool isConst; // same arguments give always the same result (e.g., for rand() this would be false)
        int numArgs;
        void *function;
    } functions[] = {{"+", true, 2, (void *)static_cast<FunctionNode<2>::Pointer>(&::Add)},
                     {"-", true, 2, (void *)static_cast<FunctionNode<2>::Pointer>(&Subtract)},
                     {"-1", true, 1, (void *)static_cast<FunctionNode<1>::Pointer>(&Negate)},
                     {"*", true, 2, (void *)static_cast<FunctionNode<2>::Pointer>(&Multiply)},
                     {"/", true, 2, (void *)static_cast<FunctionNode<2>::Pointer>(&Divide)},

                     {"==", true, 2, (void *)static_cast<FunctionNode<2>::Pointer>(&Equal)},
                     {"!=", true, 2, (void *)static_cast<FunctionNode<2>::Pointer>(&NotEqual)},
                     {">", true, 2, (void *)static_cast<FunctionNode<2>::Pointer>(&Greater)},
                     {">=", true, 2, (void *)static_cast<FunctionNode<2>::Pointer>(&GreaterEqual)},
                     {"<", true, 2, (void *)static_cast<FunctionNode<2>::Pointer>(&Less)},
                     {"<=", true, 2, (void *)static_cast<FunctionNode<2>::Pointer>(&LessEqual)},

                     {"!", true, 1, (void *)static_cast<FunctionNode<1>::Pointer>(&Not)},
                     // These will NOT evaluate their arguments conditionally as in C, but will always
                     // evaluate all arguments.
                     {"&&", true, 2, (void *)static_cast<FunctionNode<2>::Pointer>(&And)},
                     {"||", true, 2, (void *)static_cast<FunctionNode<2>::Pointer>(&Or)},
                     {"?:", true, 3, (void *)static_cast<FunctionNode<3>::Pointer>(&Conditional)},

                     {"min", true, 2, (void *)static_cast<FunctionNode<2>::Pointer>(&Min)},
                     {"max", true, 2, (void *)static_cast<FunctionNode<2>::Pointer>(&Max)},

                     CONSTFUNC1(sqrt)

                     CONSTFUNC1(fabs)
                     {"abs", true, 1, (void *)static_cast<FunctionNode<1>::Pointer>(&::fabs)},

                     CONSTFUNC2(fmod)
                     {"mod", true, 2, (void *)static_cast<FunctionNode<2>::Pointer>(&::fmod)},

                     CONSTFUNC1(floor) CONSTFUNC1(ceil)

                     CONSTFUNC1(exp) CONSTFUNC1(log) CONSTFUNC1(log10)
                     CONSTFUNC2(pow)
                     {"^", true, 2, (void *)static_cast<FunctionNode<2>::Pointer>(&::pow)},

                     CONSTFUNC1(sin) CONSTFUNC1(cos) CONSTFUNC1(tan)
                     CONSTFUNC1(asin) CONSTFUNC1(acos) CONSTFUNC1(atan) CONSTFUNC2(atan2)
                     CONSTFUNC1(sinh) CONSTFUNC1(cosh) CONSTFUNC1(tanh)

                     CONSTFUNC2(bit) CONSTFUNC3(bits)

    };
    static const size_t numFunctions = sizeof(functions) / sizeof(*functions);
    size_t i = 0;
    while (i < numFunctions && inName != functions[i].name)
        ++i;
    if (i >= numFunctions)
    {
        throw ParsingError() << inName << ": Unknown function";
    }
    else if (functions[i].numArgs != inArguments.Size())
    {
        throw ParsingError() << inName << ": Wrong number of arguments, expected " << functions[i].numArgs << ", got "
                             << inArguments.Size();
    }
    else
    {
        switch (functions[i].numArgs)
        {
        case 0:
            result = new FunctionNode<0>(functions[i].isConst, FunctionNode<0>::Pointer(functions[i].function));
            break;
        case 1:
            result = new FunctionNode<1>(functions[i].isConst, FunctionNode<1>::Pointer(functions[i].function),
                                         inArguments[0]);
            break;
        case 2:
            result = new FunctionNode<2>(functions[i].isConst, FunctionNode<2>::Pointer(functions[i].function),
                                         inArguments[0], inArguments[1]);
            break;
        case 3:
            result = new FunctionNode<3>(functions[i].isConst, FunctionNode<3>::Pointer(functions[i].function),
                                         inArguments[0], inArguments[1], inArguments[2]);
            break;
        default:
            throw ParsingError() << inName << "(): Unsupported number of function arguments";
        }
    }
    return result;
}

Node *ArithmeticExpression::MemberFunction(const std::string &inObject, const std::string &, const NodeList &)
{
    throw ParsingError() << inObject << ": Unknown object";
    return 0;
}

Node *ArithmeticExpression::Signal(AddressNode *, AddressNode *)
{
    throw ParsingError() << "Expressions of type " << ClassName(typeid(*this)) << " do not allow access to signals";
    return 0;
}

Node *ArithmeticExpression::State(const std::string &)
{
    throw ParsingError() << "Expressions of type " << ClassName(typeid(*this)) << " do not allow access to states";
    return 0;
}

Node *ArithmeticExpression::StateAssignment(const std::string &inName, Node *)
{
    return ArithmeticExpression::State(inName);
}

bool ArithmeticExpression::Parse()
{
    mStatements.Clear();
    mInput.clear();
    mInput.str(mExpression);
    try
    {
        ExpressionParser::yyparse(this);
    }
    catch (const Error &e)
    {
        Errors() << e.Message() << std::endl;
    }
    catch (const Tiny::Exception &e)
    {
        Errors() << e.What() << std::endl;
    }
    catch (...)
    {
        Errors() << "Unknown exception caught" << std::endl;
    }
    CollectGarbage();
    bool success = mErrors.str().empty();
    if (success)
        for (int i = 0; i < mStatements.Size(); ++i)
            mStatements[i] = mStatements[i]->Simplify();
    else
        mStatements.Clear();
    return success;
}

void ArithmeticExpression::ReportErrors()
{
    std::string errors = mErrors.str();
    std::ostringstream errorReport;
    if (!errors.empty())
    {
        int numErrors = 0;
        for (size_t pos = errors.find('\n'); pos != std::string::npos; pos = errors.find('\n', pos + 1))
            ++numErrors;
        errors = errors.substr(0, errors.length() - 1) + '.';
        if (numErrors > 1)
            for (size_t pos = errors.find('\n'); pos != std::string::npos; pos = errors.find('\n', pos + 3))
                errors = errors.substr(0, pos) + ".\n * " + errors.substr(pos + 1);
        errorReport << "When processing \"" << mExpression << "\""
                    << (numErrors == 1 ? ": " : ", multiple errors occurred.\n * ") << errors;
    }
    mErrors.clear();
    mErrors.str("");
    if (!errorReport.str().empty())
    {
        if (mThrowOnError)
            throw bciexception << errorReport.str();
        else
            bcierr_ << errorReport.str() << std::flush;
    }
}

void ArithmeticExpression::CollectGarbage()
{
    mParserObjects.clear();
}

Node *ArithmeticExpression::Track(Node *p)
{
    mParserObjects.push_back(p);
    return p;
}

NodeList *ArithmeticExpression::MakeList(Node *node)
{
    NodeList *p = new NodeList(node);
    mParserObjects.push_back(p);
    return p;
}

Node *ArithmeticExpression::MakeVariable(const YYLTYPE &loc, StringNode *pName)
{
    auto p = Variable(pName->Evaluate());
    p->SetLocation(loc.first_column, loc.last_column);
    return Track(p);
}

Node *ArithmeticExpression::MakeVariableAssignment(const YYLTYPE &loc, StringNode *pName, Node *pNode)
{
    auto p = VariableAssignment(pName->Evaluate(), pNode);
    p->SetLocation(loc.first_column, loc.last_column);
    return Track(p);
}

Node *ArithmeticExpression::MakeFunction(const YYLTYPE &loc, StringNode *pName, const NodeList &args)
{
    auto p = Function(pName->Evaluate(), args);
    p->SetLocation(loc.first_column, loc.last_column);
    return Track(p);
}

Node *ArithmeticExpression::MakeFunction(const YYLTYPE &loc, const char *pName, Node *n1, Node *n2, Node *n3)
{
    auto p = Function(pName, NodeList(n1).Add(n2).Add(n3));
    p->SetLocation(loc.first_column, loc.last_column);
    return Track(p);
}

Node *ArithmeticExpression::MakeMemberFunction(const YYLTYPE &loc, StringNode *pObject, StringNode *pName,
                                               const NodeList &args)
{
    auto p = MemberFunction(pObject->Evaluate(), pName->Evaluate(), args);
    p->SetLocation(loc.first_column, loc.last_column);
    return Track(p);
}

Node *ArithmeticExpression::MakeAddress(const YYLTYPE &loc, Node *n1, Node *n2)
{
    auto p = new AddressNode(n1, n2);
    p->SetLocation(loc.first_column, loc.last_column);
    return Track(p);
}

Node *ArithmeticExpression::MakeSignal(const YYLTYPE &loc, Node *n1, Node *n2)
{
    AddressNode *p1 = dynamic_cast<AddressNode *>(n1);
    AddressNode *p2 = dynamic_cast<AddressNode *>(n2);
    if (!p1)
        return 0;
    auto p = Signal(p1, p2);
    p->SetLocation(loc.first_column, loc.last_column);
    return Track(p);
}

Node *ArithmeticExpression::MakeState(const YYLTYPE &loc, StringNode *pName)
{
    auto p = State(pName->Evaluate());
    p->SetLocation(loc.first_column, loc.last_column);
    return Track(p);
}

Node *ArithmeticExpression::MakeStateAssignment(const YYLTYPE &loc, StringNode *pName, Node *pValue)
{
    auto p = StateAssignment(pName->Evaluate(), pValue);
    p->SetLocation(loc.first_column, loc.last_column);
    return Track(p);
}

// Error messages
void ArithmeticExpression::MakeError(const YYLTYPE &, const char* pMsg)
{
    std::string msg = pMsg;
    const std::string eof = "end of file";
    size_t pos = msg.find(eof);
    if (pos < msg.length())
        msg = msg.substr(0, pos) + "end of input" + msg.substr(pos + eof.length());
    mErrors << msg << std::endl;
}

// VariableContainer
std::ostream &operator<<(std::ostream &os, const ArithmeticExpression::VariableContainer &inVars)
{
    for (ArithmeticExpression::VariableContainer::const_iterator i = inVars.begin(); i != inVars.end(); ++i)
        os << i->first << ": " << i->second << '\n';
    return os;
}
