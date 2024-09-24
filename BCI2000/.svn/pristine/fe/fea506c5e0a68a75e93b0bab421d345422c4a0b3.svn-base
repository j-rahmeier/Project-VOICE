////////////////////////////////////////////////////////////////////////////////
// $id$
// Author: mellinger@neurotechcenter.org
// Description: A utility that creates a macOS alias (Finder shortcut).
////////////////////////////////////////////////////////////////////////////////
#include <Foundation/Foundation.h>
#include <iostream>

static const char* sUsage = "make_macOS_alias <existing file> <alias file>";

int main(int argc, char** argv)
{
    if (argc < 3) {
        std::cerr << "Too few arguments, usage:\n" << sUsage << std::endl;
        return -1;
    }
    if (argc > 3) {
        std::cerr << "Too many arguments, usage:\n" << sUsage << std::endl;
        return -1;
    }
    NSString *originalPath = [NSString stringWithCString:argv[1] encoding:NSUTF8StringEncoding];
    NSString *aliasPath = [NSString stringWithCString:argv[2] encoding:NSUTF8StringEncoding];
    NSURL *originalUrl = [NSURL fileURLWithPath:originalPath];
    NSURL *aliasUrl = [NSURL fileURLWithPath:aliasPath];
    NSData *bookmarkData = [originalUrl bookmarkDataWithOptions: NSURLBookmarkCreationSuitableForBookmarkFile includingResourceValuesForKeys:nil relativeToURL:nil error:NULL];

    if(bookmarkData == nil)
        return -1;
        
    BOOL success = [NSURL writeBookmarkData:bookmarkData toURL:aliasUrl    options:NSURLBookmarkCreationSuitableForBookmarkFile error:NULL];
    if (NO == success)
        return -1;
    
    return 0;
}
