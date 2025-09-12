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
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

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
    csv2
    GIT_REPOSITORY "https://github.com/p-ranav/csv2"
    GIT_TAG "v0.1"
)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG        "v1.15.3"
    GIT_SHALLOW    true
)
# more `FetchContent_Declare`s (if any). They should all be declared
# before any calls to FetchContent_Make_available (see docs for why).

FetchContent_MakeAvailable(argparse)
FetchContent_MakeAvailable(lua)
FetchContent_MakeAvailable(jsonxx)
FetchContent_MakeAvailable(csv2)
FetchContent_MakeAvailable(spdlog)
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
    "${csv2_SOURCE_DIR}/include"
    "${spdlog_SOURCE_DIR}/include"
)

# ............................................................................ #
# JSONXX explicit

add_library(jsonxx-lib STATIC
    ${jsonxx_SOURCE_DIR}/jsonxx.cc
)

# ............................................................................ #
# engine shared routines

add_library(Base-Engine STATIC
    src/engine/interface.hpp
    src/engine/shared.cpp
    src/engine/pkmndatabase.hpp src/engine/pkmndatabase.cpp
    src/engine/pkmndef.hpp src/engine/pkmndef.cpp
    src/engine/movedatabase.hpp src/engine/movedatabase.cpp
    src/engine/movedef.hpp src/engine/movedef.cpp
)

if(WIN32)
    target_compile_definitions(Base-Engine PRIVATE PLOG_IMPORT)
    target_link_libraries(Base-Engine PRIVATE StratStat)
else()
    target_compile_definitions(Base-Engine PRIVATE PLOG_GLOBAL)
endif()

target_link_libraries(Base-Engine PRIVATE
    jsonxx-lib
    csv2
)

# ............................................................................ #
# gen1 engine

add_library(Gen1-Engine SHARED
    src/engine/interface.hpp
    src/engine/gen1/gen1.cpp
    src/engine/gen1/engine.hpp src/engine/gen1/engine.cpp
)

if(WIN32)
    target_compile_definitions(Gen1-Engine PRIVATE PLOG_IMPORT)
    target_link_libraries(Gen1-Engine PRIVATE StratStat)
else()
    target_compile_definitions(Gen1-Engine PRIVATE PLOG_GLOBAL)
endif()

target_link_libraries(Gen1-Engine PUBLIC
    Base-Engine
)

# ............................................................................ #
# gen2 engine

add_library(Gen2-Engine SHARED
    src/engine/interface.hpp
)

if(WIN32)
    target_compile_definitions(Gen2-Engine PRIVATE PLOG_IMPORT)
    target_link_libraries(Gen2-Engine PRIVATE StratStat)
else()
    target_compile_definitions(Gen2-Engine PRIVATE PLOG_GLOBAL)
endif()

target_link_libraries(Gen2-Engine PUBLIC
    Base-Engine
)

# ............................................................................ #
# main

add_executable(StratStat
    src/app/main.cpp

    src/app/config.hpp src/app/config.cpp
    src/app/cliparser.hpp src/app/cliparser.cpp
    src/engine/interface.hpp
    src/app/enginewrapper.hpp src/app/enginewrapper.cpp
)

if(WIN32)
    target_compile_definitions(StratStat PRIVATE PLOG_EXPORT)
    set_target_properties(StratStat PROPERTIES ENABLE_EXPORTS 1)
else()
    target_compile_definitions(StratStat PRIVATE PLOG_GLOBAL)
endif()

target_link_libraries(StratStat PRIVATE
    lua::lua
    argparse
    csv2
    jsonxx-lib
    spdlog
)


# ============================================================================ #

include(GNUInstallDirs)
install(TARGETS StratStat
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
