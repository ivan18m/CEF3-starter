#import <Cocoa/Cocoa.h>

#include "include/wrapper/cef_library_loader.h"

int main(int argc, char *argv[])
{
    CefScopedLibraryLoader library_loader;
    if (!library_loader.LoadInMain()){
        return 1;
    }

    return NSApplicationMain(argc, (const char **) argv);
}
