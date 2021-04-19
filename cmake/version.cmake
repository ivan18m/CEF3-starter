# (C) Ivan Mercep <ivan.merchep@gmail.com>

file(READ version.txt VERSION_TXT)
string(STRIP "${VERSION_TXT}" VERSION_TXT)
string(REGEX MATCHALL "^([0-9]+)\\.([0-9]+)\\.([0-9]+)-([0-9]+)$" OUTPUT ${VERSION_TXT})

set(VER_MAJOR  ${CMAKE_MATCH_1})
set(VER_MINOR  ${CMAKE_MATCH_2})
set(VER_PATCH  ${CMAKE_MATCH_3})
set(VER_COMMIT ${CMAKE_MATCH_4})
set(VERSION ${VER_MAJOR}.${VER_MINOR}.${VER_PATCH}-${VER_COMMIT})
message("Building version: ${VERSION}")

add_definitions(-DVERSION="${VERSION}")
