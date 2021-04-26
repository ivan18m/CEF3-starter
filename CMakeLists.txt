cmake_minimum_required(VERSION 3.10)
set(CMAKE_CONFIGURATION_TYPES Debug Release)

project(CEF3App
        DESCRIPTION "Crossplatform CEF GUI project"
        LANGUAGES CXX C) # C needed for CEF

set(CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE )
endif()
message("Build type: ${CMAKE_BUILD_TYPE}")

INCLUDE(cmake/version.cmake) # get the version number

set(APP_NAME "CEF3 App")
set(APP_FOLDER_NAME "CEF3App")

# Use folders in the resulting project files.
set_property(GLOBAL PROPERTY OS_FOLDERS ON)

# CEF ROOT
if(APPLE)
    message("macOS detected")
    set(CEF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Mac")
elseif(WIN32)
    message("Windows detected")
    set(CEF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Win")
else()
    message("Linux detected")
    set(CEF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Linux")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/CEF")
find_package(CEF REQUIRED)

# Determine the target output directory.
SET_CEF_TARGET_OUT_DIR()

# Add libcef wrapper
add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} libcef_dll_wrapper)

# Mac OS X configuration (TODO)
if(APPLE)
    set(CEF_HELPER_TARGET "cefclient_Helper")
    set(CEF_HELPER_OUTPUT_NAME "cefclient Helper")

    set(CEFCLIENT_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Independent/client_app.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Independent/client_app.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Independent/client_handler.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Independent/client_handler.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Mac/main.mm"
        "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Mac/client_handler_mac.mm")
    
    ### Build main executable
    find_program(IBTOOL ibtool HINTS "/usr/bin" "${OSX_DEVELOPER_ROOT}/usr/bin")
    find_library(COCOA_LIBRARY Cocoa)
    file(GLOB SRC_FILES CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/Mac/*.mm"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/Independent/*.cpp"
    )
    add_executable(MainExecutable MACOSX_BUNDLE ${SRC_FILES})
    target_compile_features(MainExecutable PRIVATE cxx_range_for)
    target_link_libraries(MainExecutable
        ${COCOA_LIBRARY}
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Mac/lib/cef_sandbox.a"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Mac/lib/libcef_dll_wrapper.a"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Mac/lib/Chromium Embedded Framework.framework"
    )
    target_include_directories(MainExecutable PUBLIC
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Mac"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Mac/include"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/Independent"
    )
    set(EXECUTABLE_NAME ${APP_NAME})
    set(PRODUCT_NAME ${APP_NAME})
    set_target_properties(MainExecutable PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/Mac/CEF3SimpleSample-Info.plist"
        OUTPUT_NAME "${APP_NAME}"
    )
    add_custom_command (TARGET MainExecutable POST_BUILD
        COMMAND ${IBTOOL} --errors --warnings --notices
                --output-format human-readable-text --compile
                "${PROJECT_BINARY_DIR}/${APP_NAME}.app/Contents/Resources/Base.lproj/MainMenu.nib"
                "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/Mac/Resources/Base.lproj/MainMenu.xib"
    )
    add_custom_command(
        TARGET MainExecutable
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Mac/lib/Chromium Embedded Framework.framework"
                "${PROJECT_BINARY_DIR}/${APP_NAME}.app/Contents/Frameworks/Chromium Embedded Framework.framework"
        VERBATIM
    )
    
    ### Build helpers
    file(GLOB SRC_FILES CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/Mac/AppDelegate.mm"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/Mac/Helper Process Main.cpp"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/Independent/*.cpp"
    )
    set(HELPER_NAME "HelperApp")
    add_executable(${HELPER_NAME} MACOSX_BUNDLE ${SRC_FILES})
    target_compile_features(${HELPER_NAME} PRIVATE cxx_range_for)
    target_link_libraries(${HELPER_NAME}
        ${COCOA_LIBRARY}
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Mac/lib/cef_sandbox.a"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Mac/lib/libcef_dll_wrapper.a"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Mac/lib/Chromium Embedded Framework.framework"
    )
    target_include_directories(${HELPER_NAME} PUBLIC
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Mac"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Mac/include"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/Independent"
    )
    set_target_properties(${HELPER_NAME} PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Mac/CEF3SimpleSampleHelper-Info.plist"
        OUTPUT_NAME "${APP_NAME} Helper"
    )
    add_custom_command(
        TARGET ${HELPER_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Mac/lib/Chromium Embedded Framework.framework"
                "${PROJECT_BINARY_DIR}/${APP_NAME} Helper.app/Contents/Frameworks/Chromium Embedded Framework.framework"
        VERBATIM
    )
    add_custom_command(
        TARGET MainExecutable
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${PROJECT_BINARY_DIR}/${APP_NAME} Helper.app"
                "${PROJECT_BINARY_DIR}/${APP_NAME}.app/Contents/Frameworks/${APP_NAME} Helper.app"
        VERBATIM
    )
    add_dependencies(MainExecutable ${HELPER_NAME})
    set(HELPER_SUFFIXES "GPU" "Plugin" "Renderer")
    foreach(suffix ${HELPER_SUFFIXES})
        file(GLOB SRC_FILES CONFIGURE_DEPENDS
            "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/Mac/Helper Process Main.cpp"
            "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/Independent/*.cpp"
        )
        set(HELPER_NAME "${suffix}HelperApp")
        add_executable(${HELPER_NAME} MACOSX_BUNDLE ${SRC_FILES})
        target_compile_features(${HELPER_NAME} PRIVATE cxx_range_for)
        target_link_libraries(${HELPER_NAME}
            ${COCOA_LIBRARY}
            "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Mac/lib/cef_sandbox.a"
            "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Mac/lib/libcef_dll_wrapper.a"
            "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Mac/lib/Chromium Embedded Framework.framework"
        )
        target_include_directories(${HELPER_NAME} PUBLIC
            "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Mac"
            "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Mac/include"
            "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/Independent"
        )
        set_target_properties(${HELPER_NAME} PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Mac/CEF3SimpleSampleHelper-Info.plist"
            OUTPUT_NAME "${APP_NAME} Helper (${suffix})"
        )
        add_custom_command(
            TARGET ${HELPER_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Mac/lib/Chromium Embedded Framework.framework"
                    "${PROJECT_BINARY_DIR}/${APP_NAME} Helper (${suffix}).app/Contents/Frameworks/Chromium Embedded Framework.framework"
            VERBATIM
        )
        add_custom_command(
            TARGET MainExecutable
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${PROJECT_BINARY_DIR}/${APP_NAME} Helper (${suffix}).app"
                    "${PROJECT_BINARY_DIR}/${APP_NAME}.app/Contents/Frameworks/${APP_NAME} Helper (${suffix}).app"
            VERBATIM
        )
        add_dependencies(MainExecutable ${HELPER_NAME})
    endforeach()

# Windows configuration
elseif(WIN32)
    ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")

    # sources
    set(CEFCLIENT_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Independent/client_app.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Independent/client_app.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Independent/client_handler.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Independent/client_handler.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Win/cefclient.rc"
        "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Win/main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Win/resource.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Win/client_handler_win.cpp")
    
    # Executable target.
    add_executable(MainExecutable WIN32 ${CEFCLIENT_SRCS}) #target is called MainExecutable
    # target_include_directories(MainExecutable PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Independent") # can #include .h from independant
    add_dependencies(MainExecutable libcef_dll_wrapper)
    SET_EXECUTABLE_TARGET_PROPERTIES(MainExecutable)
    target_link_libraries(MainExecutable 
        libcef_lib 
        libcef_dll_wrapper 
        ${CEF_STANDARD_LIBS})

    if(USE_SANDBOX)
        # Logical target used to link the cef_sandbox library.
        ADD_LOGICAL_TARGET("cef_sandbox_lib" "${CEF_SANDBOX_LIB_DEBUG}" "${CEF_SANDBOX_LIB_RELEASE}")
        target_link_libraries(MainExecutable cef_sandbox_lib ${CEF_SANDBOX_STANDARD_LIBS})
    endif()

    # Add the custom manifest files to the executable.
    # ADD_WINDOWS_MANIFEST("${CMAKE_CURRENT_SOURCE_DIR}" "${APP_FOLDER_NAME}" "exe")

    if(MSVC)
        message("MSVC compiler detected")
        target_compile_features(MainExecutable PRIVATE cxx_range_for)
        target_compile_options(MainExecutable PUBLIC "/EHsc" "/MT")
    endif()
    
    target_include_directories(MainExecutable PUBLIC
        #${GTK3_INCLUDE_DIRS}
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Win"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Win/include"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/Independent"
    )
    set(EXECUTABLE_NAME ${APP_NAME})
    set(PRODUCT_NAME ${APP_NAME})
    set_target_properties(MainExecutable PROPERTIES OUTPUT_NAME "${APP_NAME}")

    # Copy CEF binary and resource files to the target output directory.
    COPY_FILES(MainExecutable "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${PROJECT_BINARY_DIR}/bin")
    COPY_FILES(MainExecutable "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${PROJECT_BINARY_DIR}/bin")

    add_custom_command(
        TARGET MainExecutable
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                "${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${APP_NAME}.exe"
                "${PROJECT_BINARY_DIR}/bin/${APP_NAME}.exe"
        VERBATIM
    )

# Linux configuration
else()
    ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")

    set(CEFCLIENT_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Independent/client_app.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Independent/client_app.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Independent/client_handler.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Independent/client_handler.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Linux/main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/${APP_FOLDER_NAME}/Linux/client_handler_linux.cpp")

    # Executable target.
    add_executable(MainExecutable ${CEFCLIENT_SRCS})
    SET_EXECUTABLE_TARGET_PROPERTIES(MainExecutable)
    add_dependencies(MainExecutable libcef_dll_wrapper)
    target_link_libraries(MainExecutable 
        libcef_lib 
        libcef_dll_wrapper 
        ${CEF_STANDARD_LIBS})

    # Set rpath so that libraries can be placed next to the executable.
    set_target_properties(MainExecutable PROPERTIES INSTALL_RPATH "$ORIGIN")
    set_target_properties(MainExecutable PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
    set_target_properties(MainExecutable PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CEF_TARGET_OUT_DIR})

    # We don't call deprecated GTK functions, and they can cause build failures, so disable them.
    add_definitions("-DGTK_DISABLE_DEPRECATED")

    target_include_directories(MainExecutable PUBLIC
        ${GTK3_INCLUDE_DIRS}
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Linux"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/CEF/x86/Linux/include"
        "${PROJECT_SOURCE_DIR}/${APP_FOLDER_NAME}/Independent"
    )
    set(EXECUTABLE_NAME ${APP_NAME})
    set(PRODUCT_NAME ${APP_NAME})
    set_target_properties(MainExecutable PROPERTIES OUTPUT_NAME "${APP_NAME}")

    # Copy CEF binary and resource files to the target output directory.
    COPY_FILES(MainExecutable "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${PROJECT_BINARY_DIR}/bin")
    COPY_FILES(MainExecutable "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${PROJECT_BINARY_DIR}/bin")
    if (EXISTS "${CEF_BINARY_DIR}/libminigbm.so")
        COPY_FILES(MainExecutable "libminigbm.so" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
    endif()

    # Set SUID permissions on the chrome-sandbox target.
    SET_LINUX_SUID_PERMISSIONS(MainExecutable "${CEF_TARGET_OUT_DIR}/chrome-sandbox")

    # Copy binary from bin
    add_custom_command(
        TARGET MainExecutable
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                "${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${APP_NAME}"
                "${PROJECT_BINARY_DIR}/bin/${APP_NAME}"
        VERBATIM
    )
endif()


# Copy web app
add_custom_command(
        TARGET MainExecutable
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${PROJECT_SOURCE_DIR}/assets/web"
                "${PROJECT_BINARY_DIR}/bin/assets/web"
        VERBATIM
    )
