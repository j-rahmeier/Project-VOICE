////////////////////////////////////////////////////////////////////////////////
// $$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A tool that filters C/C++ code for a set of registrations macros.
//   Input is from stdin, output is to stdout. Names of macros are taken from
//   the REGISTRY_MACROS macro defined in BCIRegistry.h.
//
//   Matches within comments are ignored, but no preprocessing is done, so #ifdef'd
//   macro calls are always copied to stdout.
//
////////////////////////////////////////////////////////////////////////////////
#include "BCIRegistry.h"

#include <cctype>
#include <iostream>
#include <set>
#include <string>

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        std::cerr << "The extract_registry build tool does not take any arguments." << std::endl;
        return -1;
    }

    // Do some parsing to remove comments from the input stream.
    std::string code;
    int prev = '\0';
    enum
    {
        readingCode,
        readingCComment,
        readingCppComment,
        readingString,
    } state = readingCode;

    do
    {
        switch (state)
        {
        case readingCode:
            switch (std::cin.peek())
            {
            case '*':
                if (prev == '/')
                    state = readingCComment;
                else
                    code += prev;
                break;
            case '"':
                state = readingString;
                code += prev;
                break;
            case '/':
                if (prev == '/')
                    state = readingCppComment;
                else
                    code += prev;
            default:
                code += prev;
            }
            break; /* readingCode */

        case readingCComment:
            if (std::cin.peek() == '/' && prev == '*')
                state = readingCode;
            break; /* readingCComment */

        case readingCppComment:
            if (std::cin.peek() == '\n')
                state = readingCode;
            break; /* readingCppComment */

        case readingString:
            if (std::cin.peek() == '"' && prev != '\\')
                state = readingCode;
            code += prev;
            break; /* readingString */

        default:
            std::cerr << "unexpected state when parsing" << std::endl;
            return -1;
        }
        prev = std::cin.get();
    } while (std::cin);

    // Read macro names into a set to remove duplicates.
    const std::string macros_[] = REGISTRY_MACROS;
    std::set<std::string> macros;
    macros.insert(macros_, macros_ + sizeof(macros_) / sizeof(*macros_));

    // Search code for macro calls.
    for (const auto &macro : macros)
    {
        std::string::size_type pos = code.find(macro);
        while (pos != std::string::npos)
        {
            bool foundName = (pos == 0 || ::isspace(code[pos - 1]));
            pos += macro.length();
            if (foundName)
            {
                while (pos < code.length() && ::isspace(code[pos]))
                    ++pos;
                if (pos < code.length() && code[pos] == '(')
                { // Here, we ignore the possibility that one of the function arguments might be
                    // a string literal containing a ')'
                    std::string args;
                    int bracketLevel = 0;
                    do
                    {
                        if (code[pos] == '(')
                            ++bracketLevel;
                        else if (code[pos] == ')')
                            --bracketLevel;
                        args += code[pos++];
                    } while (pos < code.length() && bracketLevel > 0);
                    std::cout << macro << args << ";\n";
                }
            }
            pos = code.find(macro, pos);
        }
    }
    return 0;
}
