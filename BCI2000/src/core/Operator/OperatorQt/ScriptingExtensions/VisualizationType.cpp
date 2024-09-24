//////////////////////////////////////////////////////////////////////
// $Id$
// Authors: mellinger@neurotechcenter.de
// Description: Scripting extension for visualization objects.
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
#include "VisualizationType.h"

#include "WindowType.h"
#include "VisRecorder.h"
#include "BCI_OperatorLib.h"
#include "ParserToken.h"

#include <sstream>

namespace {
    const int sMinSize = 50; // minimun height and width for visualizations

    template<class T> bool SetVisProperty(const std::string& visID, const std::string& property, const T& value)
    {
        std::ostringstream oss;
        oss << "SET LogLevel 0; SET AbortOnError false; ";
        oss << "SET VISPROPERTY " << visID << "." << property << " \"" << value << "\";";
        return BCI_ExecuteScript(oss.str().c_str());
    }
}

namespace {
    ScriptingExtensions::VisualizationType sInstance;
    const char* sHelp = "Record Visualization <Visualization ID> [on|off], "
                        "Move Visualization <Visualization ID> <x> <y>, "
                        "Resize Visualization <Visualization ID> <width> <height>, "
                        "Arrange Visualization <Visualization ID> <rows> <cols> <row> <col> [<rowspan> <colspan>]";
}

namespace ScriptingExtensions {
    const ObjectType::MethodEntry VisualizationType::sMethodTable[] = {
      {"Record", &VisualizationType::Record },
      {"Move", &VisualizationType::Move },
      {"Resize", &VisualizationType::Resize },
      {"Arrange", &VisualizationType::Arrange },
      { 0 }
    };

    const char* VisualizationType::Name() const
    {
        return "Visualization|Vis";
    }

    const char* VisualizationType::Help() const
    {
        return sHelp;
    }

    const ObjectType::MethodEntry* VisualizationType::MethodTable() const
    {
        return sMethodTable;
    }

    bool VisualizationType::Record(std::istream& is, std::ostream& os)
    {
        ParserToken visID, token;
        if (!(is >> visID))
            return false;
        bool on = true;
        if (is >> token)
        {
            if (!::stricmp(token.c_str(), "on"))
                ;
            else if (!::stricmp(token.c_str(), "off"))
                on = false;
            else
                return false;
        }
        if (on)
        {
            if (!VisRecorder::VisualizationRegistered(visID)
                && ::BCI_GetStateOfOperation() != BCI_StateIdle)
            {
                os << "System must be in idle state to register visualization recording";
            }
            else
            {
                VisRecorder::StartRecording(visID);
            }
        }
        else
        {
            VisRecorder::StopRecording(visID);
        }
        return true;
    }

    bool VisualizationType::Move(std::istream& is, std::ostream& os)
    {
        ParserToken visID;
        if (!(is >> visID))
        {
            os << "Expected a visualization ID";
            return false;
        }
        ParserToken x, y;
        if (!(is >> x >> y))
        {
            os << "Expected x and y coordinates";
            return false;
        }
        int ix = ::atoi(x.c_str()), iy = ::atoi(y.c_str());
        SetVisProperty(visID, "left", ix);
        SetVisProperty(visID, "top", iy);
        return true;
    }

    bool VisualizationType::Resize(std::istream& is, std::ostream& os)
    {
        ParserToken visID;
        if (!(is >> visID))
        {
            os << "Expected a visualization ID";
            return false;
        }
        ParserToken width, height;
        if (!(is >> width >> height))
        {
            os << "Expected width and height";
            return false;
        }
        QSize size = WindowType::AdjustSize(QSize(::atoi(width.c_str()), ::atoi(height.c_str())));
        SetVisProperty(visID, "width", size.width());
        SetVisProperty(visID, "height", size.height());
        return true;
    }

    bool VisualizationType::Arrange(std::istream& is, std::ostream& os)
    {
        ParserToken visID;
        if (!(is >> visID))
        {
            os << "Expected a visualization ID";
            return false;
        }
        QRect rect = WindowType::GetGridRect(is, os);
        if (rect.width() < 0)
        {
            return false;
        }
        if (rect.width() < sMinSize)
            rect.setWidth(sMinSize);
        if (rect.height() < sMinSize)
            rect.setHeight(sMinSize);
        QSize size = WindowType::AdjustSize(rect.size());
        SetVisProperty(visID, "left", rect.left());
        SetVisProperty(visID, "top", rect.top());
        SetVisProperty(visID, "width", size.width());
        SetVisProperty(visID, "height", size.height());
        return true;
    }
}
