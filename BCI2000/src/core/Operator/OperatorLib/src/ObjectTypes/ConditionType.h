////////////////////////////////////////////////////////////////////////////////
// $Id: ConditionType.h 7195 2023-02-07 18:31:03Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A CommandInterpreter type for logical conditions.
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
#ifndef CONDITION_TYPE_H
#define CONDITION_TYPE_H

#include "ObjectType.h"

namespace Interpreter
{

class ConditionType : public ObjectType
{
  protected:
    const char *Name() const override
    {
        return "Condition";
    }
    const MethodEntry *MethodTable() const override
    {
        return sMethodTable;
    }

  public:
    static bool Evaluate(CommandInterpreter &);
    static bool EvaluateExpression(CommandInterpreter &);

  private:
    static const MethodEntry sMethodTable[];
    static ConditionType sInstance;
};

} // namespace Interpreter

#endif // CONDITION_TYPE_H
