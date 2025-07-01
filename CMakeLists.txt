cmake_minimum_required(VERSION 3.5)
project(ObsidianSync)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nlohmann_json 3.11.2 REQUIRED)

# Настройка CPR через FetchContent
include(FetchContent)
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.10.5
)
FetchContent_MakeAvailable(cpr)

add_executable(run
    main.cpp
    src/logic.cpp
)

target_include_directories(run PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(run PRIVATE
    cpr::cpr
    nlohmann_json::nlohmann_json
)
