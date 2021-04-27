cmake_minimum_required(VERSION 3.10)
set(CMAKE_CONFIGURATION_TYPES Debug Release)

INCLUDE(cmake/version.cmake) # get the version number

project(CEF3App
        DESCRIPTION "Crossplatform CEF GUI project"
        VERSION "${VERSION_NO_COMMIT}"
        HOMEPAGE_URL "http://x.com"
        LANGUAGES CXX C) # C needed for CEF

set(CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: Debug Release" FORCE)
endif()
message("Build type: ${CMAKE_BUILD_TYPE}")

set(APP_NAME "CEF3 App")
set(APP_FOLDER_NAME "CEF3App")

# Use folders in the resulting project files.
set_property(GLOBAL PROPERTY OS_FOLDERS ON)

include(cmake/CEF/cef.cmake) #cmake build