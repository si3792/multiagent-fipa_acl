# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.

cmake_minimum_required(VERSION 2.6)
find_package(Rock)
rock_init(fipa_acl 1.4)
set(PROJECT_DESCRIPTION "Library for FIPA Agent Communication Language and handling bit-efficient FIPA Messages")
add_definitions(-DNUMERIC_DEPRECATE=1)
add_definitions(-g0)
add_definitions(-DTIXML_USE_STL=1)
add_definitions(-DBOOST_FILESYSTEM_VERSION=3)

find_package(Boost 1.44.0 REQUIRED filesystem unit_test_framework regex thread system)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

set(CMAKE_BUILD_TYPE RelWithDebInfo)
rock_standard_layout()

