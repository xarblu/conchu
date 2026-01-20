cmake_minimum_required(VERSION 3.20)

project(conchu
    DESCRIPTION "A simple container update checker"
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)

# dependencies
find_package(httplib REQUIRED)
find_package(nlohmann_json REQUIRED)

# sources
include(src/CMakeLists.txt)
