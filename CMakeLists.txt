cmake_minimum_required(VERSION 4.1.1)

include(FetchContent)
include(CMakePrintHelpers)
include(GNUInstallDirs)

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
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG        "v1.17.0"
)
# more `FetchContent_Declare`s (if any). They should all be declared
# before any calls to FetchContent_Make_available (see docs for why).

FetchContent_MakeAvailable(argparse)
FetchContent_MakeAvailable(lua)
FetchContent_MakeAvailable(jsonxx)
FetchContent_MakeAvailable(csv2)
FetchContent_MakeAvailable(spdlog)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
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

target_compile_options(jsonxx-lib PRIVATE -fPIC)

# ............................................................................ #

add_library(jsonxx-validation-lib STATIC
    src/jsonxx-validation/constants.hpp
    src/jsonxx-validation/jsonvalidationtypes.hpp       src/jsonxx-validation/jsonvalidationtypes.cpp
    src/jsonxx-validation/specification.hpp             src/jsonxx-validation/specification.cpp
    src/jsonxx-validation/jsonvalidation.hpp            src/jsonxx-validation/jsonvalidation.cpp
    src/jsonxx-validation/jsonvalidationtionresult.hpp  src/jsonxx-validation/jsonvalidationtionresult.cpp
    src/jsonxx-validation/jsonsetoperations.hpp         src/jsonxx-validation/jsonsetoperations.cpp
    
)

target_compile_options(jsonxx-validation-lib PRIVATE -fPIC)

# ............................................................................ #
# engine shared routines

add_library(Base-Engine STATIC
    src/app/errors.hpp
    src/engine/validationinterface.hpp
    src/engine/interface.hpp src/engine/interface.cpp
    src/engine/sharedtypes.hpp src/engine/sharedtypes.cpp
    src/engine/shared.hpp src/engine/shared.cpp
    src/engine/engine.hpp src/engine/engine.cpp
    src/engine/commonvaluecollection.hpp src/engine/commonvaluecollection.cpp
    src/engine/commondatabase.hpp src/engine/commondatabase.cpp
    src/engine/commonvaluemapvalidationresult.hpp src/engine/commonvaluemapvalidationresult.cpp
)

target_compile_options(Base-Engine PRIVATE -fPIC)

target_link_libraries(Base-Engine PRIVATE
    jsonxx-lib
    csv2
)

# ............................................................................ #
# gen1 engine

add_library(Gen1-Engine SHARED
    src/engine/gen1/constants.hpp src/engine/gen1/constants.cpp
    src/engine/interface.hpp src/engine/gen1/interface.cpp
    src/engine/gen1/inputvalidation.cpp
)

target_link_libraries(Gen1-Engine PUBLIC
    Base-Engine
    jsonxx-validation-lib
)

# ............................................................................ #
# gen2 engine

# add_library(Gen2-Engine SHARED
#     src/engine/interface.hpp
# )

# target_link_libraries(Gen2-Engine PUBLIC
#     Base-Engine
# )

# ............................................................................ #
# main

add_executable(StratStat
    src/app/main.cpp
    src/app/errors.hpp
    src/app/config.hpp src/app/config.cpp
    src/app/cliparser.hpp src/app/cliparser.cpp
    src/app/logging.hpp src/app/logging.cpp
    src/app/enginewrapper.hpp src/app/enginewrapper.cpp
    src/app/luawrapper.hpp src/app/luawrapper.cpp
)

target_link_libraries(StratStat PRIVATE
    lua::lua
    argparse
    csv2
    jsonxx-lib
    jsonxx-validation-lib
    spdlog
)

# ............................................................................ #
# tests

include(GoogleTest)

enable_testing()

add_executable(StratStat-Test
    src/test/main.cpp
    src/test/testjsonxxvalidation.hpp src/test/testjsonxxvalidation.cpp
)

if (TARGET GTest::gtest_main)
    message("found it!")
else()
    message("no gtest!")
endif()

target_link_libraries(StratStat-Test
    GTest::gtest_main
    jsonxx-lib
    jsonxx-validation-lib
)


gtest_discover_tests(StratStat-Test)

# ============================================================================ #

install(TARGETS StratStat
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
