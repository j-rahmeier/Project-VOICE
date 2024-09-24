////////////////////////////////////////////////////////////////////
// $Id $
// Author:      mellinger@neurotechcenter.org
// Description: A utility that fixes the Headerlen field in a
//   BCI2000 .dat file, e.g. after editing in a hex editor.
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
////////////////////////////////////////////////////////////////////
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#if WIN32
#include <fcntl.h>
#include <io.h>
#endif

#include "Debugging.h"

static const char *sUsage = "Usage: fix_dat_headerlen ...\n"
                            "  This utility reads a BCI2000 dat file from stdin and writes it to stdout,\n"
                            "  updating the headerlen field to match the header's actual size.\n";
;

int main(int argc, char *argv[])
{
#if WIN32
    setmode(fileno(stdin), O_BINARY);
    setmode(fileno(stdout), O_BINARY);
#endif
    std::string line;
    std::vector<std::string> header;
    do
    {
        std::getline(std::cin, line, '\r');
        if (std::cin.peek() != '\n')
        {
            std::cerr << "Unexpected input" << std::endl;
            return -1;
        }
        std::cin.ignore();
        header.push_back(line);
    } while (!line.empty());

    std::vector<std::pair<std::string, std::string>> firstLine;
    std::istringstream iss(header.front());
    std::string name, value;
    do
    {
        iss >> std::ws;
        std::getline(iss, name, '=');
        iss >> value;
        if (iss)
            firstLine.push_back(std::make_pair(name, value));
    } while (iss);
    auto i = firstLine.begin();
    for (; i != firstLine.end(); ++i)
        if (i->first == "HeaderLen")
            break;
    if (i == firstLine.end())
    {
        std::cerr << "HeaderLen field missing from header" << std::endl;
        return -1;
    }
    int headerlen = 0;
    for (size_t j = 0; j < firstLine.size(); ++j)
        headerlen += firstLine[j].first.length() + firstLine[j].second.length() + 2;
    headerlen += firstLine.size() - 1 + 2;
    for (size_t j = 1; j < header.size(); ++j)
        headerlen += header[j].length() + 2;

    bool done = false;
    do
    {
        value = std::to_string(headerlen);
        done = value.length() == i->second.length();
        headerlen -= i->second.length();
        headerlen += value.length();
        i->second = value;
    } while (!done);

    header.front().clear();
    for (size_t j = 0; j < firstLine.size(); ++j)
    {
        if (j != 0)
            header.front() += " ";
        header.front() += firstLine[j].first + "= " + firstLine[j].second;
    }

    std::ostringstream oss;
    for (const auto &line : header)
        oss << line << "\r\n";
    Assert(oss.tellp() == headerlen);
    std::cout.write(oss.str().c_str(), headerlen);

    do
    {
        char buf[4096];
        std::cin.read(buf, sizeof(buf));
        std::cout.write(buf, std::cin.gcount());
    } while (std::cin);

    return 0;
}
