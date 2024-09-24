////////////////////////////////////////////////////////////////////////////////
// $Id: bci_datadiff.cpp 7544 2023-08-21 18:42:59Z mellinger $
// Author: griffin.milsap@gmail.com, juergen.mellinger@uni-tuebingen.de
// Description: Does a simple diff between two data streams, spits out problems
// Alternate name: GriffDiff (awesome, yes?)
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

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

const double cSignificantError = 1.5;

int main(int argc, char *argv[])
{
    // Welcome message
    std::cout << "bci_datadiff" << std::endl << "BCI2000 Project 2009-2023" << std::endl << std::endl;

    if (argc > 1 && !::stricmp(argv[1], "-v"))
        return 0;

    // User has no idea how to use the program.
    if (argc < 3)
    {
        std::cerr << "Usage: bci_datadiff testFile refFile [outputFile]" << std::endl;
        return (1);
    }

    // Try to open the test file
    std::ifstream in_test(argv[1]);
    if (!in_test.is_open())
    {
        std::cerr << "Could not open test file, " << argv[1] << ", for reading." << std::endl;
        return (1);
    }

    // Try to open the reference
    std::ifstream in_ref(argv[2]);
    if (!in_ref.is_open())
    {
        std::cerr << "Could not open reference file, " << argv[2] << ", for reading." << std::endl;
        return (1);
    }

    // If the user specified an output file, we should try to open that too.
    std::ofstream out_file;
    bool output = false;
    if (argc > 3)
    {
        out_file.open(argv[3]);
        if (!out_file.is_open())
        {
            std::cerr << "Could not open output file, " << argv[3] << ", for writing." << std::endl;
            return (1);
        }
        output = true;
    }

    // Some of the data at the top of the file may be different
    // but not important.  We'll skip past that.
    const std::string mark = "VisSignalProperties";
    bool skip = true;
    std::string s;
    while (skip)
    {
        skip = !!std::getline(in_test, s);
        std::istringstream iss(s);
        skip = skip && iss >> s;
        skip = skip && s != mark;
    }
    skip = true;
    while (skip)
    {
        skip = !!std::getline(in_ref, s);
        std::istringstream iss(s);
        skip = skip && iss >> s;
        skip = skip && s != mark;
    }
    if (in_ref.eof() || in_test.eof())
        return -1; // files should contain valid lines

    // We havn't found differences yet
    int numDifferences = 0;
    int numSignificantDifferences = 0;

    // Read the files
    std::cout << "Comparing test: " << argv[1] << " vs ref: " << argv[2] << " ..." << std::endl
              << "------------------------------------------" << std::endl;
    if (output)
    {
        out_file << "TortoiseMerge \"" << argv[2] << "\" \"" << argv[1] << "\"\n\n";
        out_file << "Comparing test: " << argv[1] << " vs ref: " << argv[2] << " ..." << std::endl
                 << "------------------------------------------" << std::endl;
    }
    static const char* ignoreStates[] =
    {
      "SourceTime:", "StimulusTime:", "PresentationTime:", "TestLoggerCounter:",
      "PresentationRequested:", "PresentationDisplayed:", "PresentationFrame:",
      "PreStimulusTime:", "AudioBufferTime:", "AudioPresentationTime:",
    };
    while (!in_test.eof() || !in_ref.eof())
    {
        // Get the input from the test and the ref
        std::string testLine;
        std::getline(in_test, testLine);
        std::string refLine;
        std::getline(in_ref, refLine);

        // Test to see if it's one of the states we should ignore
        std::string refState;
        std::istringstream issRef(refLine);
        issRef >> refState;
        bool skip = false;
        for (auto state : ignoreStates)
        {
            if (refState == state)
            {
                skip = true;
                break;
            }
        }
        if (skip)
            continue;
        // Skip states that are absent from the reference file
        skip = true;
        while (skip)
        {
            std::string testState;
            std::istringstream iss(testLine);
            skip = !!(iss >> testState);
            skip = skip && testState != refState;
            skip = skip && !testState.empty() && *testState.rbegin() == ':';
            skip = skip && getline(in_test, testLine);
        }
        // For signal data, compute a relative error
        if (refState == "VisSignal")
        {
            std::getline(in_test, testLine);
            std::getline(in_ref, refLine);
            std::getline(in_test, testLine);
            std::getline(in_ref, refLine);
            std::getline(in_test, testLine, '}');
            std::getline(in_ref, refLine, '}');
            std::stringstream refStream(refLine), testStream(testLine);
            double refVal = 0.0, testVal = 0.0;
            while (refStream >> refVal && testStream >> testVal)
            {
                double err = ::fabs(testVal - refVal);
                if (err > 0)
                {
                    std::ostringstream oss;
                    oss << "Difference: signal value \"" << testVal << "\" should be \"" << refVal << "\"."
                        << std::endl;
                    std::cout << oss.str();
                    if (output)
                        out_file << oss.str();
                    ++numDifferences;
                }
                if (err >= cSignificantError)
                    ++numSignificantDifferences;
            }
        }
        // Compare the two lines
        else if (testLine != refLine)
        {
            std::cout << "Difference: \"" << testLine << "\" should be \"" << refLine << "\"." << std::endl;
            if (output)
                out_file << "Difference: \"" << testLine << "\" should be \"" << refLine << "\"." << std::endl;
            numDifferences++;
            numSignificantDifferences++;
        }
    }

    std::cout << "------------------------------------------" << std::endl;
    if (output)
        out_file << "------------------------------------------" << std::endl;

    // Output the number of differences to the file
    if (output)
        out_file << "Number of Differences: " << numDifferences << std::endl
                 << "Number of Significant Differences: " << numSignificantDifferences << std::endl;

    // Tell the user what's up.
    if (numDifferences != 0)
    {
        std::cout << "Differences have been found." << std::endl
                  << "There is/are " << numDifferences << "/" << numSignificantDifferences
                  << " total/significant difference(s) between the ref and test file. " << std::endl;
    }
    else
        std::cout << "No differences found!" << std::endl;

    // Cleanup
    if (output)
        out_file.close();
    in_test.close();
    in_ref.close();
    return numSignificantDifferences;
}
