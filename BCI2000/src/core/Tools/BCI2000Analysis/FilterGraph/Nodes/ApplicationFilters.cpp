////////////////////////////////////////////////////////////////////////////////
// $Id: ApplicationFilters.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: Filters representing application modules.
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
#include "ApplicationFilters.h"

#include <vector>
#include <map>

struct ERPApplication::Private
{
    int mNumberOfSequences = 0;
    struct DataPoint { bool type; double value; };
    typedef std::map<int, std::vector<DataPoint>> Selections;
    Selections mSelections;

    bool mCanReport = true;
    int mNumMatrixRows = 0, mNumMatrixCols = 0;
    std::vector<std::string> mTargetDefinitions;
};

ERPApplication::ERPApplication()
: p(new Private)
{
}

ERPApplication::~ERPApplication()
{
    delete p;
}

void ERPApplication::Publish()
{
    BEGIN_PARAMETER_DEFINITIONS
        "Application int NumberOfSequences= auto",
        "Application int NumberOfSeq= auto",
        "Application intlist NumMatrixRows= 0",
        "Application intlist NumMatrixColumns= 0",
        "Application matrix TargetDefinitions= 0 0",
    END_PARAMETER_DEFINITIONS
}

void ERPApplication::AutoConfig(const SignalProperties&)
{
    if (ActualParameter("NumberOfSequences") == "auto")
        Parameter("NumberOfSequences") = Parameter("NumberOfSeq").ToString();
    if (ActualParameter("NumberOfSequences") == "auto")
        Parameter("NumberOfSequences") = OptionalParameter("EpochsToAverage", 1).ToString();
}

void ERPApplication::Preflight(const SignalProperties&, SignalProperties&) const
{
    if (enabled())
    {
        State("StimulusCodeRes");
        State("StimulusTypeRes");
        if (OptionalParameter("SingleEpochMode", -1) == 0)
            bcierr << "Parameter \"SingleEpochMode\" must be set to 1";

        Parameter("NumMatrixRows");
        Parameter("NumMatrixColumns");
        Parameter("TargetDefinitions");
    }
}

void ERPApplication::Initialize(const SignalProperties&, const SignalProperties&)
{
    if (enabled())
    {
        p->mNumberOfSequences = Parameter("NumberOfSequences");

        p->mCanReport = true;

        p->mNumMatrixRows = 0;
        ParamRef NumMatrixRows = Parameter("NumMatrixRows");
        if (NumMatrixRows->NumValues() > 1)
            p->mCanReport = false;
        else if (NumMatrixRows->NumValues() == 1)
            p->mNumMatrixRows = NumMatrixRows;

        p->mNumMatrixCols = 0;
        ParamRef NumMatrixCols = Parameter("NumMatrixColumns");
        if (NumMatrixCols->NumValues() > 1)
            p->mCanReport = false;
        else if(NumMatrixCols->NumValues() == 1)
            p->mNumMatrixCols = NumMatrixCols;

        p->mTargetDefinitions.clear();
        ParamRef Targets = Parameter("TargetDefinitions");
        for (int row = 0; row < Targets->NumRows(); ++row)
            p->mTargetDefinitions.push_back(Targets(row, 0));
    }
}

void ERPApplication::Process(const GenericSignal& input, GenericSignal&)
{
    if (enabled())
    {
        bool type = State("StimulusTypeRes");
        int code = State("StimulusCodeRes");
        double value = input(0, 0);
        if (code > 0)
        {
            Private::DataPoint point = { type, value };
            p->mSelections[code].push_back(point);
        }
    }
}

void ERPApplication::Report(std::ostream& os) const
{
    if (!p->mCanReport)
        return;

    if (p->mSelections.empty())
        return;

    if (p->mNumberOfSequences <= 0)
        return;

    int count = p->mSelections.begin()->second.size();
    for (const auto& selection : p->mSelections)
    {
        if (selection.second.size() != count)
        {
            os << "Trial count (number of presentations) differs across stimuli, cannot evaluate classifier for target selection"
               << std::endl;
            return;
        }
    }
    if (count % p->mNumberOfSequences)
    {
        os << "Trial count is not a multiple of NumberOfSequences, cannot evaluate classifier for target selection"
           << std::endl;;
        return;
    }

    int numberOfSymbols = count / p->mNumberOfSequences;
    std::vector<int> correctClassifications(p->mNumberOfSequences, 0);
    std::vector<std::vector<int>> recognizedSymbols(p->mNumberOfSequences);

    for (int n = 0; n < p->mNumberOfSequences; ++n)
    {
        // transfer all symbols into a code->value table
        for (int s = 0; s < numberOfSymbols; ++s)
        {
            std::map<int, double> classValues;
            std::vector<int> targets;
            for (const auto& selection : p->mSelections)
            {
                int idx = s * p->mNumberOfSequences;
                int code = selection.first;
                bool type = selection.second[idx].type;
                double value = 0;
                for (int i = 0; i <= n; ++i)
                {
                    Assert(selection.second[idx + i].type == type);
                    value += selection.second[idx + i].value;
                }
                classValues[code] = value;
                if (type)
                    targets.push_back(code);
            }
            if (targets.empty())
            {
                os << "No targets given, cannot evaluate classifier" << std::endl;
                return;
            }
            else if (targets.size() > 2)
            {
                os << "More than two targets given, don't know how handle this situation" << std::endl;
                return;
            }
            // test whether targets are classified correctly
            else if (targets.size() == 1)
            {
                int code = 0;
                double value = -Inf<double>();
                for (const auto& classValue : classValues)
                    if (classValue.second > value)
                        value = classValue.second, code = classValue.first;

                if (code == targets.front())
                    ++correctClassifications[n];
                int targetIdx = code - 1;
                recognizedSymbols[n].push_back(targetIdx);
            }
            else if (targets.size() == 2)
            {
                if (targets[1] < targets[0])
                    std::swap(targets[0], targets[1]);

                // Separate classification for rows and columns of speller matrix,
                // but count correct only if both row and column match.
                int code1 = 0, code2 = 0;
                double value1 = -Inf<double>(), value2 = -Inf<double>();
                for (const auto& classValue : classValues)
                {
                    if (classValue.first <= p->mNumMatrixRows)
                    {
                        if (classValue.second > value1)
                            value1 = classValue.second, code1 = classValue.first;
                    }
                    else
                    {
                        if (classValue.second > value2)
                            value2 = classValue.second, code2 = classValue.first;
                    }
                }
                if (code1 == targets.front() && code2 == targets.back())
                    ++correctClassifications[n];
                int targetRow = code1 - 1, targetCol = code2 - p->mNumMatrixRows - 1,
                    targetIdx = targetRow * p->mNumMatrixCols + targetCol;
                recognizedSymbols[n].push_back(targetIdx);
            }
        }
    }
    os << "Number of symbol selections: " << numberOfSymbols << "\n\n";
    os << "Epochs\tCorrect classifications\tSymbols\n";
    for (int n = 0; n < correctClassifications.size(); ++n)
    {
        int percent = correctClassifications[n] * 100.0 / numberOfSymbols + 0.5;
        os << n + 1 << "\t" << correctClassifications[n] << "\t" << percent << "%\t";
        for (auto targetIdx : recognizedSymbols[n])
        {
            if (targetIdx < p->mTargetDefinitions.size())
                os << p->mTargetDefinitions[targetIdx];
            else
                os << '?';
        }
        os << "\n";
    }
}
