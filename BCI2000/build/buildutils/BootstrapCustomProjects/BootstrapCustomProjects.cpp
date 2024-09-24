///////////////////////////////////////////////////////////////////////////////////
// $Id: BootstrapCustomProjects.cpp 7581 2023-09-07 17:16:43Z mellinger $
// Authors: Jeremy Hill, mellinger@neurotechcenter.org
// Description: A tool to create BCI2000 filters and modules from templates.
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
///////////////////////////////////////////////////////////////////////////////////

#include "FilePath.h"
#include "FileUtils.h"
#include "ProcessUtils.h"
#include "Resources.h"
#include "StringUtils.h"
#include "Exception.h"

#include "buildenv.h"

#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <ctime>

static const std::string sUser = ProcessUtils::UserName() + '@' + ProcessUtils::HostName();
time_t sNow = std::time(nullptr);
static const std::string sDate = std::ctime(&sNow);

struct SourcePath
{
    SourcePath(const std::string &rel)
    {
        init(rel);
    }
    SourcePath(const char* rel)
    {
        init(rel);
    }
    void init(const std::string &rel)
    {
        Rel = FileUtils::NormalizedPath(rel);
        Abs = FilePath(rel).ToAbsolute(Root).ToString();
        Abs = FileUtils::NormalizedPath(Abs);
        if (!Abs.empty() && Abs.find(Root) != 0)
            throw std_exception << "Source path " << Abs << " located outside source tree:\n" << Root;
    }
    SourcePath operator+(const std::string &name)
    {
        return SourcePath(Rel + "/" + name);
    }
    std::string Rel, Abs;
    static std::string Root;
};
std::string SourcePath::Root = FileUtils::NormalizedPath(CMAKE_SOURCE_DIR "/../");

static std::string PromptForInput(const std::string &s)
{
    std::cout << s << std::flush;
    std::string line;
    std::getline(std::cin, line);
    return StringUtils::Strip(line, StringUtils::WhiteSpace + "'\"");
}

static std::string LegalizeName(const std::string& s)
{
    std::string result;
    if (s.empty())
        return s;
    if (::isdigit(s.front()))
        result += '_';
    for (const auto c : s)
    {
        if (::isalnum(c))
            result += c;
        else
            result += '_';
    }
    result.front() = ::toupper(result.front());
    return result;
}

static std::string LegalizePath(const std::string& s)
{
    std::string result;
    for (const auto c : s)
    {
        if (::isalnum(c) || c == '\\' || c == '/')
            result += c;
        else
            result += '_';
    }
    return result;
}

static std::string FileGetContents(const SourcePath &name)
{
    std::string s;
    std::getline(std::ifstream(name.Abs), s, '\0');
    return s;
}

static bool FilePutContents(const SourcePath &name, const std::string &content)
{
    std::ofstream file(name.Abs, std::ios::trunc);
    if (!file.is_open())
        return false;
    if (!file.write(content.data(), content.length()).flush())
        return false;
    return true;
}

static std::string ResGetContents(const bci::Resource *res)
{
    std::string s;
    s.reserve(res->length);
    for (const char *p = res->data; p < res->data + res->length; ++p)
        if (*p && *p != '\r')
            s += *p;
    return s;
}

typedef std::map<std::string, std::string> Variables;

static Variables MakeVariables(const std::string& name)
{
    Variables vars;
    vars["name"] = name;
    vars["ucname"] = StringUtils::ToUpper(name);
    vars["author"] = sUser;
    vars["date"] = StringUtils::Strip(sDate);
    return vars;
}

static std::string Varexpand(const std::string &s, const Variables &vars)
{
    std::string r = s;
    size_t pos = 0;
    for (const auto &var : vars)
        while ((pos = r.find("$" + var.first + "$")) != std::string::npos)
            r = r.substr(0, pos) + var.second + r.substr(pos + var.first.length() + 2);
    return r;
}

static void CreateFileWithContents(const SourcePath &dst, const std::string &contents)
{
    if (FileUtils::Exists(dst.Abs))
        std::cout << "-- file already exists - leaving it unchanged: " << dst.Abs << std::endl;
    else if (FilePutContents(dst, contents))
        std::cout << "-- created " << dst.Rel << std::endl;
    else
        throw std_exception << "failed to open file for writing: " << dst.Abs;
}

static std::vector<SourcePath> sCreatedFiles;

static void CreateFileFromTemplate(const SourcePath &dst, const bci::Resource *tmpl, const Variables &vars)
{
    std::string s = ResGetContents(tmpl);
    CreateFileWithContents(dst, Varexpand(s, vars));
    sCreatedFiles.push_back(dst);
}

static void EnsureDirectory(const SourcePath &name)
{
    FileUtils::MakeDirectory(name.Abs, true);
    if (!FileUtils::IsDirectory(name.Abs))
        throw std_exception << "Could not create directory " << name.Rel;
    if (!FileUtils::IsWritableDirectory(name.Abs))
        throw std_exception << name.Rel << " is not a writable directory";
}

typedef enum
{
    none = 0,
    adc = 1,
    filter = 2,
    task = 3,
    hyperscanning = 4,
} Kind;

static int NewFilter(int kind, std::string name, std::string parent)
{
    while (kind == none)
    {
        std::string s = PromptForInput("1 BufferedADC (for SignalSource modules)\n"
                                       "2 GenericFilter (for all modules, but usually SignalProcessing)\n"
                                       "3 ApplicationBase (for application modules)\n"
                                       "4 HyperscanningApplicationBase (for hyperscanning application modules)\n"
                                       "\nEnter filter type [default is 2]: ");
        if (s.empty())
            kind = filter;
        else
            kind = ::atoi(s.c_str());

        switch (kind)
        {
        case adc:
        case filter:
        case task:
        case hyperscanning:
            break;
        default:
            kind = none;
        }
    }

    bci::Resource *pCpp = nullptr, *pHdr = nullptr;
    switch (kind)
    {
    case adc:
        pCpp = &bci::Resources::TemplateBufferedADC_cpp_in;
        pHdr = &bci::Resources::TemplateBufferedADC_h_in;
        break;
    case filter:
        pCpp = &bci::Resources::TemplateFilter_cpp_in;
        pHdr = &bci::Resources::TemplateFilter_h_in;
        break;
    case task:
        pCpp = &bci::Resources::TemplateApplication_cpp_in;
        pHdr = &bci::Resources::TemplateApplication_h_in;
        break;
    case hyperscanning:
        pCpp = &bci::Resources::TemplateHyperscanningApplication_cpp_in;
        pHdr = &bci::Resources::TemplateHyperscanningApplication_h_in;
    }

    while (name.empty())
        name = PromptForInput("Enter filter name: ");

    if (::isdigit(name.front()))
        throw std_exception << "\"" << name << "\" is an illegal filter name (name may not start with a numeral)";
    for (const auto &c : name)
        if (!(::isalpha(c) || ::isdigit(c) || c == '_'))
            throw std_exception
                << "\"" << name
                << "\" is an illegal filter name (name may only contain alphanumeric characters and underscore)";

    while (parent.empty())
    {
        parent = PromptForInput("Enter parent module's project directory: ");
        if (!parent.empty())
        {
            SourcePath p(parent);
            if (!FileUtils::IsDirectory(p.Abs))
            {
                std::cout << "directory " << p.Abs << " does not exist" << std::endl;
                parent.clear();
            }
        }
    }

    SourcePath proj(parent);
    if (!FileUtils::IsDirectory(proj.Abs))
        throw std_exception << "directory does not exist: " << proj.Abs;

    std::cout << "Adding filter " << name << " to module " << proj.Rel << std::endl;

    Variables vars = MakeVariables(name);
    std::string srcname = name + ".cpp", hdrname = name + ".h";
    CreateFileFromTemplate(proj + srcname, pCpp, vars);
    CreateFileFromTemplate(proj + hdrname, pHdr, vars);

    SourcePath cmFile = proj + "CMakeLists.txt";
    std::string content = FileGetContents(cmFile);
    std::regex pattern("(bci2000_add_\\w+\\s*\\(.*\\n?(\\s*)\\S+[^)]*)", std::regex::icase);
    if (std::regex_search(content, pattern))
    {
        content = std::regex_replace(content, pattern, "$1$2" + srcname + "\n");
        if (FilePutContents(cmFile, content))
            std::cout << "-- added " << srcname << " to " << cmFile.Rel << std::endl;
    }

    SourcePath pdFile = proj + "PipeDefinition.cpp";
    if (FileUtils::IsFile(pdFile.Abs))
    {
        content = FileGetContents(pdFile);
        if (!std::regex_search(content, std::regex("^\\s*#include\\s+\"" + hdrname + "\"\\s*$")))
        {
            content += "\n#include \"" + hdrname + "\"\nFilter( " + name + ", 2.X );\n";
            content += "#error   Change the location token 2.X in the line above, to ensure that " + name +
                       " is sorted to the desired position in the filter sequence. Then, delete this error line\n";
            if (FilePutContents(pdFile, content))
                std::cout << "-- added " << name << " reference to " << pdFile.Rel << std::endl;
        }
    }
    return 0;
}

static int NewLogger(std::string name, int parent, bool havePrivate)
{
    while (name.empty())
        name = PromptForInput("Enter logger name: ");

    if (!std::regex_match(name, std::regex(".*logger", std::regex::icase)))
        name += "Logger";
    name = LegalizeName(name);
    std::string shortName = name.substr(0, name.length() - 6); // remove "Logger" from short name

    std::string prompt = "Choose a parent directory:\n"
        "1 src/custom/Extensions [recommended default]\n"
        "2 src/contrib/Extensions\n";
    if (havePrivate)
        prompt += "3 src/private/Extensions\n";

    int max = havePrivate ? 3 : 2;
    while (parent < 1 || parent > max)
    {
        std::string sel = PromptForInput(prompt);
        if (sel.empty())
            parent = 1;
        else
            parent = ::atoi(sel.c_str());
    }

    std::string extensionsDir;
    std::string extensionsFile;
    switch (parent)
    {
        case 1:
            extensionsDir = "src/custom/Extensions";
            extensionsFile = "../CustomConfig.cmake";
            break;
        case 2:
            extensionsDir = "src/contrib/Extensions";
            extensionsFile = ""; // "Extensions.cmake"; adapt to r7498
            break;
        case 3:
            extensionsDir = "src/private/Extensions";
            extensionsFile = "../PrivateConfig.cmake";
            break;
        default:
            return -1;
    }
    SourcePath newExtDir(extensionsDir + "/" + name);
    if (FileUtils::IsDirectory(newExtDir.Abs))
        throw std_exception << "directory exists, will not be touched: " << newExtDir.Abs;
    EnsureDirectory(newExtDir.Abs);
    std::cout << "\nA new logger extension has been created at " << newExtDir.Rel << std::endl;

    Variables vars = MakeVariables(name);
    vars["sname"] = shortName;

    CreateFileFromTemplate(newExtDir + (name + ".h"), &bci::Resources::TemplateLogger_h_in, vars);
    CreateFileFromTemplate(newExtDir + (name + ".cpp"), &bci::Resources::TemplateLogger_cpp_in, vars);
    CreateFileFromTemplate(newExtDir + "IncludeExtension.cmake", &bci::Resources::TemplateIncludeExtension_cmake_in, vars);

    bool extFileOK = true;
    if (!extensionsFile.empty())
    {
        SourcePath extFile = extensionsDir + "/" + extensionsFile;
        if (!FileUtils::IsFile(extFile.Abs))
            extFileOK = FilePutContents(extFile, "");
        if (extFileOK)
        {
            std::string ef = FileGetContents(extFile);
            auto lines = StringUtils::Split("\n", ef);
            int lastMatch = -1;
            std::regex extensionLine("\\s*include_extension\\s*\\(.*\\)\\s*", std::regex::icase);
            for (int i = 0; i < lines.size(); ++i)
                if (std::regex_match(lines[i], extensionLine))
                    lastMatch = i;
            if (lastMatch < 0) // append at end if no match
                lastMatch = int(lines.size()) - 1;

            SourcePath srcPath = std::string("src");
            std::string relPath = FileUtils::RelativePath(newExtDir.Abs, srcPath.Abs);
            relPath = FileUtils::NormalizedPath(relPath);
            std::string extLine = "include_extension(" + name + " \"${BCI2000_SRC_DIR}/" + relPath + "\")";
            lines.insert(lines.begin() + (lastMatch + 1), extLine);

            ef = StringUtils::Join("\n", lines);
            extFileOK = FilePutContents(extFile, ef);
        }
        if (!extFileOK)
        {
            std::cerr << "\nCould not write to " << extFile.Abs << std::endl;
            return -1;
        }
    }
    if (extFileOK)
    {
        std::cout << "\nNOTE: Run CMake again to ensure that this extension is included in the build.\n"
                  << "You will also need to enable EXTENSION_" << vars["ucname"] << " after the first\n"
                  << "CMake run, then run CMake a second time.\n"
                  << "After recompiling, add --Log" << vars["sname"] << "=1 to the source module's\n"
                  << "command line arguments before starting BCI2000.\n"
                  << std::endl;

    }
    std::cout << "To expand the new logger extension, edit the following files:\n";
    for (const auto& file : sCreatedFiles)
        std::cout << "* " << file.Rel << "\n";
    std::cout << std::endl;

    return 0;
}

static int NewModule(int kind, std::string name, std::string parent)
{
    while (kind == none)
    {
        std::string s = PromptForInput("1 SignalSource\n"
                                       "2 SignalProcessing\n"
                                       "3 Application\n"
                                       "4 Hyperscanning Application\n"
                                       "\nEnter module type [default is 2]: ");
        if (s.empty())
            kind = filter;
        else
            kind = ::atoi(s.c_str());

        switch (kind)
        {
        case adc:
        case filter:
        case task:
        case hyperscanning:
            break;
        default:
            kind = none;
        }
    }
    while (name.empty())
        name = PromptForInput("Enter module name: ");

    std::string defaultParent = "src/custom";
    if (parent.empty())
        parent = PromptForInput("Enter parent directory [recommended default is \"" + defaultParent + "\" ]: ");
    if (parent.empty())
        parent = defaultParent;

    SourcePath proj = parent + "/" + name;

    bci::Resource *pCMakeLists = nullptr;
    std::string filtername = name, modtype;
    switch (kind)
    {
    case adc:
        modtype = "SignalSource";
        pCMakeLists = &bci::Resources::CMakeLists_SignalSource_txt_in;
        filtername = std::regex_replace(filtername, std::regex("(source$|signalsource$|adc$)", std::regex::icase), "");
        filtername += "ADC";
        break;
    case filter:
        modtype = "SignalProcessing";
        pCMakeLists = &bci::Resources::CMakeLists_SignalProcessing_txt_in;
        filtername =
            std::regex_replace(filtername, std::regex("(filter$|sigproc$|signalprocessing$)", std::regex::icase), "");
        filtername += "Filter";

        break;
    case task:
        modtype = "Application";
        pCMakeLists = &bci::Resources::CMakeLists_Application_txt_in;
        filtername = std::regex_replace(filtername, std::regex("(app(lication)?$|task$)", std::regex::icase), "");
        filtername += "Task";
        break;
    case hyperscanning:
        modtype = "Hyperscanning Application";
        pCMakeLists = &bci::Resources::CMakeLists_HyperscanningApplication_txt_in;
        filtername = std::regex_replace(filtername, std::regex("(app(lication)?$|task$)", std::regex::icase), "");
        filtername += "Task";
        break;
    }

    if (FileUtils::IsDirectory(proj.Abs))
        throw std_exception << "directory exists, will not be touched: " << proj.Abs;
    EnsureDirectory(proj.Abs);
    std::cout << "\nA new " << modtype << " project has been created at " << proj.Rel << std::endl;

    Variables vars = MakeVariables(name);

    SourcePath cmFile = proj + "CMakeLists.txt";
    CreateFileFromTemplate(cmFile, pCMakeLists, vars);
    if (kind == filter)
        CreateFileFromTemplate(proj + "PipeDefinition.cpp", &bci::Resources::PipeDefinition_cpp_in, vars);
    NewFilter(kind, filtername, proj.Rel);

    if (kind == adc)
    {
        EnsureDirectory(proj.Abs + "/batch");
        CreateFileFromTemplate(proj + ("/batch/CursorTask_" + name + ".bat"), &bci::Resources::CursorTask_ADC_bat_in, vars);
        CreateFileFromTemplate(proj + ("/batch/P3Speller_" + name + ".bat"), &bci::Resources::P3Speller_ADC_bat_in, vars);
        CreateFileFromTemplate(proj + ("/batch/StimulusPresentation_" + name + ".bat"), &bci::Resources::StimulusPresentation_ADC_bat_in, vars);
        EnsureDirectory(proj.Abs + "/parms");
        CreateFileFromTemplate(proj + ("/parms/" + name + ".prm"), &bci::Resources::ADC_prm_in, vars);
    }

    bool cmOK = true;
    SourcePath parentCm = proj + "/../CMakeLists.txt";
    if (!FileUtils::IsFile(parentCm.Abs))
        cmOK = FilePutContents(parentCm, "");
    if (cmOK)
    {
        std::string cm = FileGetContents(parentCm);
        cm += "add_subdirectory( " + FileUtils::ExtractName(proj.Abs) + " )\n";
        cmOK = FilePutContents(parentCm, cm);
    }
    if (cmOK)
        std::cout << "\nNOTE: Run CMake again to ensure that this module is included in the build.\n" << std::endl;
    else
    {
        std::cerr << "\nCould not write to " << parentCm.Abs << std::endl;
        return -1;
    }

    std::cout << "To expand the new module, edit the following files:\n";
    for (const auto &file : sCreatedFiles)
        std::cout << "* " << file.Rel << "\n";
    std::cout << std::endl;
    return 0;
}

int SetupFilterTool(std::string cppfile)
{
    while (cppfile.empty())
    {
        cppfile = PromptForInput("Enter path to an existing C++ filter file: ");
        if (!FileUtils::IsFile(cppfile))
        {
            std::cout << "file " << cppfile << " not found.\n";
            cppfile = "";
        }
    }
    if (!FileUtils::IsFile(cppfile))
    {
        std::cerr << "file " << cppfile << " not found.\n";
        return 1;
    }

    cppfile = FileUtils::AbsolutePath(cppfile);
    std::string parentPath = FileUtils::NormalizedPath(FileUtils::ExtractDirectory(cppfile)),
                stem = FileUtils::ExtractBase(cppfile), extension = FileUtils::ExtractExtension(cppfile),
                childDirName = "cmdline", childDirPath = parentPath + childDirName + "/",
                parentCMakeLists = parentPath + "CMakeLists.txt", childCMakeLists = childDirPath + "CMakeLists.txt";
    bool rerunCMake = false;

    std::cout << "\nSetting up " << stem << " as a command-line filter-tool target:" << std::endl;
    if (!FileUtils::Exists(childDirPath))
    {
        if (!FileUtils::MakeDirectory(childDirPath))
        {
            std::cerr << "could not create directory " << childDirPath << ".\n";
            return 1;
        }
        std::cout << "-- created directory " << FilePath(childDirPath).Truncated(50) << std::endl;
    }
    std::string parentCm = FileGetContents(parentCMakeLists);
    std::regex pattern("^\\s*(add_subdirectory\\s*\\(\\s*" + childDirName + "\\s*\\))", std::regex::icase);
    std::smatch match;
    std::regex_search(parentCm, match, pattern);
    if (!match.empty())
    {
        std::cout << "-- " << FilePath(parentCMakeLists).Truncated(70) << "\n   exists and contains\n     "
                  << match[match.size() - 1].str() << ",\n   will not be touched." << std::endl;
    }
    else
    {
        size_t pos = 0;
        while (!parentCm.empty() && parentCm.find("##", pos) == pos)
        {
            pos = parentCm.find('\n', pos);
            if (pos != std::string::npos)
                pos += 1;
        }
        std::string line = "add_subdirectory( " + childDirName + " )";
        parentCm = parentCm.substr(0, pos) + line + "\n" + parentCm.substr(pos);
        if (!FilePutContents(parentCMakeLists, parentCm))
        {
            std::cerr << "could not write file " << parentCMakeLists << ".\n";
            return 1;
        }
        else
        {
            std::cout << "-- prepended \"" << line << "\"\n   to " << FilePath(parentCMakeLists).Truncated(65)
                      << std::endl;
            rerunCMake = true;
        }
    }

    std::string childCm = FileGetContents(childCMakeLists);
    pattern =
        std::regex("^\\s*(bci2000_add_cmdline_filter\\(\\s*" + stem + "\\s+FROM \\.\\.\\s+.*\\)).*", std::regex::icase);
    std::regex_search(childCm, match, pattern);
    if (!match.empty())
    {
        std::cout << "-- " << FilePath(childCMakeLists).Truncated(70) << "\n   already contains\n     "
                  << match[match.size() - 1].str() << ",\n   will not be touched." << std::endl;
    }
    else
    {
        std::string line = "bci2000_add_cmdline_filter( " + stem + " FROM .. )";
        if (!childCm.empty() && childCm.back() != '\n')
            childCm += '\n';
        childCm += line + "\n";
        if (!FilePutContents(childCMakeLists, childCm))
        {
            std::cerr << "could not write file " << childCMakeLists << ".\n";
            return 1;
        }
        std::cout << "-- appended \"" << line << "\"\n   to " << FilePath(childCMakeLists).Truncated(65) << std::endl;
        rerunCMake = true;
    }
    if (rerunCMake)
        std::cout << "\nNOTE:\nRun CMake again to ensure that the new filter-tool is included in the build.\n"
                  << std::endl;
    return 0;
}

int SetupFilterDll(std::string cppfile)
{
    while (cppfile.empty())
    {
        cppfile = PromptForInput("Enter path to an existing C++ filter file: ");
        if (!FileUtils::IsFile(cppfile))
        {
            std::cout << "file " << cppfile << " not found.\n";
            cppfile = "";
        }
    }
    if (!FileUtils::IsFile(cppfile))
    {
        std::cerr << "file " << cppfile << " not found.\n";
        return 1;
    }

    cppfile = FileUtils::AbsolutePath(cppfile);
    std::string parentPath = FileUtils::NormalizedPath(FileUtils::ExtractDirectory(cppfile)),
                stem = FileUtils::ExtractBase(cppfile), extension = FileUtils::ExtractExtension(cppfile),
                childDirName = "filterdll", childDirPath = parentPath + childDirName + "/",
                parentCMakeLists = parentPath + "CMakeLists.txt", childCMakeLists = childDirPath + "CMakeLists.txt";
    bool rerunCMake = false;

    std::cout << "\nSetting up " << stem << " as a filter dll target:" << std::endl;
    if (!FileUtils::Exists(childDirPath))
    {
        if (!FileUtils::MakeDirectory(childDirPath))
        {
            std::cerr << "could not create directory " << childDirPath << ".\n";
            return 1;
        }
        std::cout << "-- created directory " << FilePath(childDirPath).Truncated(50) << std::endl;
    }
    std::string parentCm = FileGetContents(parentCMakeLists);
    std::regex pattern("^\\s*(add_subdirectory\\s*\\(\\s*" + childDirName + "\\s*\\))", std::regex::icase);
    std::smatch match;
    std::regex_search(parentCm, match, pattern);
    if (!match.empty())
    {
        std::cout << "-- " << FilePath(parentCMakeLists).Truncated(70) << "\n   exists and contains\n     "
                  << match[match.size() - 1].str() << ",\n   will not be touched." << std::endl;
    }
    else
    {
        size_t pos = 0;
        while (!parentCm.empty() && parentCm.find("##", pos) == pos)
        {
            pos = parentCm.find('\n', pos);
            if (pos != std::string::npos)
                pos += 1;
        }
        std::string line = "add_subdirectory( " + childDirName + " )";
        parentCm = parentCm.substr(0, pos) + line + "\n" + parentCm.substr(pos);
        if (!FilePutContents(parentCMakeLists, parentCm))
        {
            std::cerr << "could not write file " << parentCMakeLists << ".\n";
            return 1;
        }
        else
        {
            std::cout << "-- prepended \"" << line << "\"\n   to " << FilePath(parentCMakeLists).Truncated(65)
                      << std::endl;
            rerunCMake = true;
        }
    }

    std::string childCm = FileGetContents(childCMakeLists);
    pattern = std::regex("^\\s*(bci2000_add_filter_dll\\(\\s*" + stem + "Dll)\\s", std::regex::icase);
    std::regex_search(childCm, match, pattern);
    if (!match.empty())
    {
        std::cout << "-- " << FilePath(childCMakeLists).Truncated(70) << "\n   already contains\n\""
                  << match[match.size() - 1].str() << "\",\n   will not be touched." << std::endl;
    }
    else
    {
        std::string lines = "bci2000_add_filter_dll(\n  " + stem + "Dll\n  ../" + stem + ".cpp\n)";
        if (!childCm.empty() && childCm.back() != '\n')
            childCm += '\n';
        childCm += lines + "\n";
        if (!FilePutContents(childCMakeLists, childCm))
        {
            std::cerr << "could not write file " << childCMakeLists << ".\n";
            return 1;
        }
        std::cout << "-- appended \"" << lines << "\"\n   to " << FilePath(childCMakeLists).Truncated(65) << std::endl;
        rerunCMake = true;
    }
    if (rerunCMake)
        std::cout << "\nNOTE:\nRun CMake again to ensure that the new filter dll is included in the build.\n"
                  << std::endl;
    return 0;
}

int NewFilter(int argc, char **argv, bool &interactive)
{
    const char *usage =
        "NewBCI2000Filter CLASSTYPE NAME PROJECTDIR\n"
        "\n"
        "e.g. NewBCI2000Filter    2   MyCustomFilter  ../src/custom/VeryNiceSignalProcessing\n"
        "\n"
        "CLASSTYPE: 1 creates a subclass of BufferedADC (for signal acquisition in SignalSource modules using newer "
        "buffered API)\n"
        "           2 creates a subclass of GenericFilter (for all modules, especially SignalProcessing)\n"
        "           3 creates a subclass of ApplicationBase (for Application modules)\n"
        "			4 creates a subclass of HyperscanningApplicationBase (for Hyperscanning Application modules)\n"
        "\n"
        "NAME:      The name of the filter class, and of the .cpp and .h files in which it is implemented.\n"
        "\n"
        "PROJECT:   The directory for the project to which the filter should be added.\n";
    if (argc > 4)
    {
        std::cerr << "Too many inputs. Usage is as follows:\n\n" << usage << std::endl;
        return 1;
    }
    if (argc > 1 && !::stricmp(argv[1], "--help"))
    {
        std::cout << "\n" << usage << std::endl;
        return 0;
    }

    int kind = none;
    if (argc > 1)
        kind = ::atoi(argv[1]);

    std::string name;
    if (argc > 2)
        name = argv[2];

    std::string proj;
    if (argc > 3)
        proj = argv[3];

    interactive = (argc < 4);

    return NewFilter(kind, name, proj);
}


int NewModule(int argc, char **argv, bool &interactive)
{
    const char *usage = "NewBCI2000Module MODTYPE NAME PARENT\n"
                        "\n"
                        "e.g. NewBCI2000Module    2  VeryNiceSignalProcessing   ../src/custom\n"
                        "\n"
                        "MODTYPE:   1 denotes SignalSource modules\n"
                        "           2 denotes SignalProcessing modules\n"
                        "           3 denotes Application modules\n"
                        "           4 denotes Hyperscanning Application modules\n"
                        "\n"
                        "NAME:      Used for the name of the project directory and resulting executable.\n"
                        "\n"
                        "PARENT:    The parent directory inside which the project directory is created.\n"
                        "           ../src/custom (relative to the \"build\" directory) is a good choice,\n"
                        "           since the svn version-control system ignores it, but cmake will include it.\n";
    if (argc > 4)
    {
        std::cerr << "Too many inputs. Usage is as follows:\n\n" << usage << std::endl;
        return 1;
    }
    if (argc > 1 && !::stricmp(argv[1], "--help"))
    {
        std::cout << "\n" << usage << std::endl;
        return 0;
    }

    int kind = none;
    if (argc > 1)
        kind = ::atoi(argv[1]);

    std::string name;
    if (argc > 2)
        name = argv[2];

    std::string proj;
    if (argc > 3)
        proj = argv[3];

    interactive = (argc < 4);

    return NewModule(kind, name, proj);
}

int NewLogger(int argc, char** argv, bool& interactive)
{
    SourcePath priv = std::string( "src/private/Extensions" );
    bool havePrivate = FileUtils::IsDirectory(priv.Abs);

    std::string usage = "NewBCI2000Logger NAME PARENT\n"
        "\n"
        "e.g. NewBCI2000Logger    VeryNiceLogger   1\n"
        "\n"
        "NAME:      Used for the name of the logger extension directory and logger class.\n"
        "\n"
        "PARENT:    The parent directory inside which the logger extension directory is created.\n"
        "           src/custom/Extensions is a good choice,\n"
        "           since the svn version-control system ignores it, but cmake will include it.\n"
        "           The following parent directories are available:\n"
        "           1 src/custom/Extensions\n"
        "           2 src/contrib/Extensions\n";
    if (havePrivate)
        usage += "           3 src/private/Extensions\n";

    if (argc > 3)
    {
        std::cerr << "Too many inputs. Usage is as follows:\n\n" << usage << std::endl;
        return 1;
    }
    if (argc > 1 && !::stricmp(argv[1], "--help"))
    {
        std::cout << "\n" << usage << std::endl;
        return 0;
    }

    std::string name;
    if (argc > 1)
        name = argv[1];

    int parent = 0;
    if (argc > 2)
        parent = ::atoi(argv[2]);
    else if (argc > 1)
        parent = 1;

    interactive = (argc < 2);

    return NewLogger(name, parent, havePrivate);
}

int SetupFilterTool(int argc, char **argv, bool &interactive)
{
    const char *usage = "NewBCI2000CommandlineTarget CPPFILE\n"
                        "\n"
                        "e.g. NewBCI2000CommandlineTarget    ../src/custom/MyCustomModule/MyCustomFilter.cpp\n"
                        "\n"
                        "CPPFILE:   Name of an existing filter cpp file\n";
    if (argc > 2)
    {
        std::cerr << "Too many inputs. Usage is as follows:\n\n" << usage << std::endl;
        return 1;
    }
    if (argc > 1 && !::stricmp(argv[1], "--help"))
    {
        std::cout << "\n" << usage << std::endl;
        return 0;
    }

    std::string cppfile;
    if (argc > 1)
        cppfile = argv[1];

    interactive = (argc < 2);
    return SetupFilterTool(cppfile);
}

int SetupFilterDll(int argc, char **argv, bool &interactive)
{
    const char *usage = "NewBCI2000FilterDll CPPFILE\n"
                        "\n"
                        "e.g. NewBCI2000FilterDll    ../src/custom/MyCustomModule/MyCustomFilter.cpp\n"
                        "\n"
                        "CPPFILE:   Name of an existing filter cpp file\n";
    if (argc > 2)
    {
        std::cerr << "Too many inputs. Usage is as follows:\n\n" << usage << std::endl;
        return 1;
    }
    if (argc > 1 && !::stricmp(argv[1], "--help"))
    {
        std::cout << "\n" << usage << std::endl;
        return 0;
    }

    std::string cppfile;
    if (argc > 1)
        cppfile = argv[1];

    interactive = (argc < 2);
    return SetupFilterDll(cppfile);
}

int main(int argc, char *argv[])
{
    int result = -1;
    bool interactive = false;
    try
    {
        typedef int (*CreateFunc)(int, char**, bool&);
        const struct { const char* expr; CreateFunc func; } functions[] =
        {
            {"new[[:alnum:]]*filterdll", &SetupFilterDll},
            {"new[[:alnum:]]*filtertool", &SetupFilterTool},
            {"new[[:alnum:]]*filter", &NewFilter},
            {"new[[:alnum:]]*module", &NewModule},
            {"new[[:alnum:]]*logger", &NewLogger},
        };
        std::string name = FileUtils::ExtractBase(argv[0]);
        CreateFunc f = nullptr;
        for (const auto& function : functions)
            if (std::regex_match(name, std::regex(function.expr, std::regex::icase)))
                f = function.func;
        if (f)
            result = f(argc, argv, interactive);
        else
            result = -1;
    }
    catch (const std::exception &exc)
    {
        std::cerr << exc.what() << std::endl;
        result = -1;
    }
    if (interactive)
        PromptForInput(result ? "\nErrors occurred -- press <enter> to quit" : "\nDone, press <enter> to quit");
    return result;
}
