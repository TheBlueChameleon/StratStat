cmake_minimum_required(VERSION 4.1.1)

include(FetchContent)
include(CMakePrintHelpers)

# ============================================================================ #
# language definition

project(StratStat LANGUAGES
    CXX
    VERSION 0.1
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================================ #
# external dependencies

FetchContent_Declare(
  argparse
  GIT_REPOSITORY git@github.com:p-ranav/argparse.git
  GIT_TAG "v3.2"
  GIT_SHALLOW TRUE
  GIT_PROGRESS ON
  SYSTEM
)
FetchContent_Declare(
    lua
    GIT_REPOSITORY "https://github.com/marovira/lua"
    GIT_TAG "5.4.8"
)
FetchContent_Declare(
    jsonxx
    GIT_REPOSITORY "https://github.com/hjiang/jsonxx"
    GIT_TAG "v1.0.1"
)
FetchContent_Declare(
    loguru
    GIT_REPOSITORY "https://github.com/emilk/loguru"
    GIT_TAG "v2.1.0"
)
# more `FetchContent_Declare`s (if any). They should all be declared
# before any calls to FetchContent_Make_available (see docs for why).

FetchContent_MakeAvailable(argparse)
FetchContent_MakeAvailable(lua)
FetchContent_MakeAvailable(jsonxx)
FetchContent_MakeAvailable(loguru)
# more `FetchContent_MakeAvailable`s (if any).

# cmake_print_variables(argparse_SOURCE_DIR)
# cmake_print_variables(lua_SOURCE_DIR)

# ============================================================================ #
# source file definitions

include_directories(
    src/
    "${argparse_SOURCE_DIR}/include"
    "${lua_SOURCE_DIR}/src"
    "${jsonxx_SOURCE_DIR}"
    "${loguru_SOURCE_DIR}"
)

# ............................................................................ #
# JSONXX explicit

add_library(jsonxx-lib STATIC
    ${jsonxx_SOURCE_DIR}/jsonxx.cc
)

# ............................................................................ #
# loguru explicit

add_library(loguru-lib STATIC
    ${loguru_SOURCE_DIR}/loguru.cpp
)

# ............................................................................ #
# engine

add_library(Base-Engine STATIC
    src/engine/foo.cpp
    src/app/clihandler.hpp src/app/clihandler.cpp
)

add_library(Gen1-Engine SHARED
    src/engine/gen1/gen1.cpp
)
target_link_libraries(Gen1-Engine PRIVATE
    Base-Engine
)

add_library(Gen2-Engine SHARED
    src/engine/foo.cpp
)

# ............................................................................ #
# main

add_executable(StratStat
    src/app/main.cpp
    src/app/config.hpp
    src/app/clihandler.cpp src/app/clihandler.hpp
)

target_link_libraries(StratStat PRIVATE
    lua::lua
    argparse
    jsonxx-lib
    loguru-lib
)


# ============================================================================ #

include(GNUInstallDirs)
install(TARGETS StratStat
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
