// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "client_app.h"

#include <string>
#include <unistd.h>

#if defined(CEF_X11)
#include <X11/Xlib.h>
#endif

#include "include/base/cef_logging.h"
#include "include/cef_command_line.h"

#if defined(CEF_X11)
namespace 
{
int XErrorHandlerImpl(Display* display, XErrorEvent* event) 
{
    LOG(WARNING) << "X error received: "
                << "type " << event->type << ", "
                << "serial " << event->serial << ", "
                << "error_code " << static_cast<int>(event->error_code) << ", "
                << "request_code " << static_cast<int>(event->request_code)
                << ", "
                << "minor_code " << static_cast<int>(event->minor_code);
    return 0;
}

int XIOErrorHandlerImpl(Display* display) 
{
    return 0;
}
}  // namespace
#endif  // defined(CEF_X11)

// Entry point function for all processes.
int main(int argc, char* argv[]) 
{
    // Provide CEF with command-line arguments.
    CefMainArgs main_args(argc, argv);

    // CEF applications have multiple sub-processes (render, plugin, GPU, etc)
    // that share the same executable. This function checks the command-line and,
    // if this is a sub-process, executes the appropriate logic.
    int exit_code = CefExecuteProcess(main_args, nullptr, nullptr);
    if (exit_code >= 0) 
    {
        // The sub-process has completed so return here.
        return exit_code;
    }

    #if defined(CEF_X11)
        // Install xlib error handlers so that the application won't be terminated
        // on non-fatal errors.
        XSetErrorHandler(XErrorHandlerImpl);
        XSetIOErrorHandler(XIOErrorHandlerImpl);
    #endif

    // Parse command-line arguments for use in this method.
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
    command_line->InitFromArgv(argc, argv);

    // Specify CEF global settings here.
    CefSettings settings;

    if (command_line->HasSwitch("enable-chrome-runtime")) 
    {
        // Enable experimental Chrome runtime. See issue #2969 for details.
        settings.chrome_runtime = true;
    }
    
    // When generating projects with CMake the CEF_USE_SANDBOX value will be defined
    // automatically. Pass -DUSE_SANDBOX=OFF to the CMake command-line to disable
    // use of the sandbox.
    #if !defined(CEF_USE_SANDBOX)
        settings.no_sandbox = true;
    #endif

    // ClientApp implements application-level callbacks for the browser process.
    // It will create the first browser instance in OnContextInitialized() after
    // CEF has initialized.
    CefRefPtr<ClientApp> app(new ClientApp);

    // Initialize CEF for the browser process.
    CefInitialize(main_args, settings, app.get(), nullptr);

    // Run the CEF message loop. This will block until CefQuitMessageLoop() is
    // called.
    CefRunMessageLoop();

    // Shut down CEF.
    CefShutdown();

    return 0;
}
