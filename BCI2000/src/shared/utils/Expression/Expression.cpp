//////////////////////////////////////////////////////////////////////////////////////
// $Id: Expression.cpp 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A simple expression parser for use within BCI2000.
//              See Expression.h for details about expressions.
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
#include "Expression.h"
#include "BCIException.h"
#include <sstream>

using namespace ExpressionParser;

const Expression::VariableContainer &Expression::Constants = ArithmeticExpression::Constants;

Expression &Expression::SetOptionalAccess(State::ValueType inDefaultValue)
{
    mOptionalAccess = true;
    mDefaultValue = inDefaultValue;
    return *this;
}

Expression &Expression::ClearOptionalAccess()
{
    mOptionalAccess = false;
    return *this;
}

bool Expression::IsValid(const GenericSignal *inpSignal, int inSample, const ArithmeticExpression::Context &inContext)
{
    mAllowStateAssignment = false;
    mpSignal = inpSignal;
    mSample = inSample;
    return ArithmeticExpression::IsValid(inContext);
}

bool Expression::IsValid(const SignalProperties &inProperties, int inSample,
                         const ArithmeticExpression::Context &inContext)
{
    GenericSignal signal(inProperties);
    return IsValid(&signal, inSample, inContext);
}

bool Expression::RefersStates() const
{
    Assert(CompilationState() != ArithmeticExpression::none);
    if (mRefersStates < 0)
        mRefersStates = ContainsNode<StateNode>() || ContainsNode<StateAssignmentNode>();
    return mRefersStates;
}

bool Expression::RefersSignal() const
{
    Assert(CompilationState() != ArithmeticExpression::none);
    if (mRefersSignal < 0)
        mRefersSignal = ContainsNode<SignalNode>();
    return mRefersSignal;
}

double Expression::Evaluate(const GenericSignal *inpSignal, int inSample) const
{
    mAllowStateAssignment = (Environment::Phase() != Environment::preflight);
    mpSignal = inpSignal;
    mSample = inSample;
    Expression *pThis = const_cast<Expression *>(this);
    return pThis->ArithmeticExpression::Evaluate();
}

Node *Expression::Variable(const std::string &inName)
{
    Node *result = NewStateNode(inName);
    if (!result)
        result = ArithmeticExpression::Variable(inName);
    return result;
}

Node *Expression::Signal(AddressNode *inChannelAddress, AddressNode *inElementAddress)
{
    Node* pNode = nullptr;
    if (inElementAddress)
        pNode = new SignalNode(mpSignal, inChannelAddress, inElementAddress);
    else
        pNode = new SignalNode(mpSignal, inChannelAddress, &mSample);
    return pNode;
}

Node *Expression::State(const std::string &inName)
{
    Node *result = NewStateNode(inName);
    if (!result)
        Environment::State(inName); // This will display an error message.
    return result;
}

Node *Expression::NewStateNode(const std::string &inName)
{
    Node *result = NULL;
    if (States->Exists(inName))
        result = new StateNode(Environment::State(inName), mSample);
    else if (mOptionalAccess)
        result = new ConstantNode(mDefaultValue);
    return result;
}

Node *Expression::StateAssignment(const std::string &inName, Node *inRHS)
{
    return new StateAssignmentNode(Environment::State(inName), inRHS, mSample, mAllowStateAssignment);
}

Node* Expression::MemberFunction(const std::string& object, const std::string& function, const NodeList& args)
{
    bool found = false;
    const char* functions[] = {"AsFloat", "AsSigned", "AsUnsigned",};
    for (const auto name : functions)
    {
        if (function == name)
        {
            found = true;
            if (args.Size() > 0)
                throw bciexception << name << "() takes no arguments but was called with "
                                   << args.Size() << " arguments";
        }
    }
    if (!found)
        throw bciexception << "Unknown function: " << function;
    Node* pState = State(object);
    if (pState)
    {
        if (function == "AsFloat")
            return new StateAsFloatNode(pState);
        if (function == "AsSigned")
            return new StateAsSignedNode(pState);
        if (function == "AsUnsigned")
            return new StateAsUnsignedNode(pState);
    }
    return nullptr;
}


// Additional Node classes
//  SignalNode
Expression::SignalNode::SignalNode(const SignalPointer &rpSignal, AddressNode *pCh, AddressNode *pEl)
    : mrpSignal(rpSignal), mpSample(nullptr),
      mpChannelAddress(pCh), mpElementAddress(pEl), mChannelIdx(-1), mElementIdx(-1)
{
    AddChild(pCh);
    AddChild(pEl);
}

Expression::SignalNode::SignalNode(const SignalPointer& rpSignal, AddressNode* pCh, const int* pSample)
  : mrpSignal(rpSignal), mpSample(pSample), 
    mpChannelAddress(pCh), mpElementAddress(nullptr), mChannelIdx(-1), mElementIdx(-1)
{
  AddChild(pCh);
}

double Expression::SignalNode::OnEvaluate()
{
    if (mrpSignal == NULL)
        throw bciexception << "No signal specified for expression evaluation";

    int channel = mChannelIdx;
    if (channel < 0)
    {
        std::string address = mpChannelAddress->Evaluate();
        channel = static_cast<int>(mrpSignal->Properties().ChannelIndex(address));
        if (mpChannelAddress->IsConst())
            mChannelIdx = channel;
    }
    if (channel < 0 || channel >= mrpSignal->Channels())
        throw bciexception << "Channel index or address (" << mpChannelAddress->Evaluate() << ") out of range";

    int element = -1;
    if (mpElementAddress)
    {
      element = mElementIdx;
      if (element < 0)
      {
        std::string address = mpElementAddress->Evaluate();
        element = static_cast<int>(mrpSignal->Properties().ElementIndex(address));
        if (mpElementAddress->IsConst())
          mElementIdx = element;
      }
      if (element < 0 || element >= mrpSignal->Elements())
        throw bciexception << "Element index or address (" << mpElementAddress->Evaluate() << ") out of range";
    }
    else
    {
      Assert(mpSample);
      element = *mpSample;
      if (element < 0 || element >= mrpSignal->Elements())
        return NaN<double>();
    }
    return (*mrpSignal)(channel, element);
}

// StateNode
Expression::StateNode::StateNode(const StateRef &state, const int &sample) : mStateRef(state), mrSample(sample)
{
}

double Expression::StateNode::OnEvaluate()
{
    return mStateRef(mrSample);
}

// StateAsFloatNode
Expression::StateAsFloatNode::StateAsFloatNode(Node* pStateNode) : mpStateNode(pStateNode)
{
}

double Expression::StateAsFloatNode::OnEvaluate()
{
    union { uint32_t i; float f; } value = { mpStateNode->Evaluate() };
    return value.f;
}

// StateAsSignedNode
Expression::StateAsSignedNode::StateAsSignedNode(Node* pStateNode) : mpStateNode(pStateNode)
{
}

double Expression::StateAsSignedNode::OnEvaluate()
{
    union { uint32_t u; int32_t s; } value = { mpStateNode->Evaluate() };
    return value.s;
}

// StateAsUnsignedNode
Expression::StateAsUnsignedNode::StateAsUnsignedNode(Node* pStateNode) : mpStateNode(pStateNode)
{
}

double Expression::StateAsUnsignedNode::OnEvaluate()
{
    uint32_t value = mpStateNode->Evaluate();
    return value;
}

// StateAssignmentNode
Expression::StateAssignmentNode::StateAssignmentNode(const StateRef &state, Node *inRHS, const int &sample,
                                                     const bool &allowed)
    : mStateRef(state), mrSample(sample), mrAllowed(allowed)
{
    AddChild(inRHS);
}

double Expression::StateAssignmentNode::OnEvaluate()
{
    double rhs = mChildren[0]->Evaluate();
    if (mrAllowed)
        mStateRef(mrSample) = static_cast<State::ValueType>(rhs);
    return rhs;
}
