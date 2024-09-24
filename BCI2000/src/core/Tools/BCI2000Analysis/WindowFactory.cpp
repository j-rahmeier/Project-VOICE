////////////////////////////////////////////////////////////////////////////////
// $Id: WindowFactory.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A function that creates new windows according to AnalysisData
//     content.
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
#include "WindowFactory.h"

#include "AnalysisWindow.h"
#include "CSPDataWindow.h"
#include "ClassifierDataWindow.h"
#include "ApplicationDataWindow.h"
#include "AnalysisData.h"

namespace WindowFactory {
    QWidget* create(AnalysisWindow* pParent, const AnalysisData& analysisData, const QString& file)
    {
        if (analysisData.kind.find("SpatialFilter") != std::string::npos)
            return new CSPDataWindow(pParent, analysisData, file);
        if (analysisData.kind.find("LinearClassifier") != std::string::npos)
            return new ClassifierDataWindow(pParent, analysisData, file);
        if (analysisData.kind.find("Application") != std::string::npos)
            return new ApplicationDataWindow(pParent, analysisData, file);
        return nullptr;
    }
}
