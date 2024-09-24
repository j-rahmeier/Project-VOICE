//////////////////////////////////////////////////////////////////////
// $Id$
// Authors: mellinger@neurotechcenter.de
// Description: Scripting extension dealing with websocket servers
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
///////////////////////////////////////////////////////////////////////
#ifndef WEBSOCKET_TYPE_H
#define WEBSOCKET_TYPE_H

#include "ScriptingExtensions.h"

namespace ScriptingExtensions {

    class WebsocketType : public ObjectType
    {
    public:
        const char* Name() const override;
        const char* Help() const override;

    protected:
        const MethodEntry* MethodTable() const override;

    public:
        static bool Start(std::istream&, std::ostream&);
        static bool Stop(std::istream&, std::ostream&);

    private:
        static const MethodEntry sMethodTable[];

    };
}

#endif // WEBSOCKET_TYPE_H

