cmake_minimum_required(VERSION 3.5)
project(fman)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(submodules/SDL EXCLUDE_FROM_ALL)

set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/submodules/imgui")
set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp
)
set(IMGUI_BACKEND_SOURCES
        ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
        ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)
set(IMGUI_ALL_SOURCES
        ${IMGUI_SOURCES}
        ${IMGUI_BACKEND_SOURCES}
)

find_package(CURL REQUIRED)
find_package(Threads REQUIRED)

add_library(fman_lib
    lib/fman_http.cpp
    lib/fman_window_manager.cpp
)
target_include_directories(fman_lib PRIVATE ${IMGUI_DIR})

add_executable(fman main.cpp ${IMGUI_ALL_SOURCES})
target_include_directories(fman PRIVATE lib)
target_include_directories(fman PRIVATE ${IMGUI_DIR} ${IMGUI_DIR}/backends)
target_link_libraries(fman PRIVATE fman_lib Threads::Threads SDL3::SDL3 CURL::libcurl)

file(COPY resources DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

option(BUILD_TESTS "Build unit tests." OFF)
if(BUILD_TESTS)

    set(FMAN_SOURCES_TESTS
            tests/test_curl.cpp
    )

    enable_testing()
    find_package(GTest REQUIRED)
    include(GoogleTest)
    message(STATUS "Building tests...")

    add_executable(fman_tests ${FMAN_SOURCES_TESTS})
    target_include_directories(fman_tests PRIVATE lib)
    target_link_libraries(fman_tests fman_lib GTest::gmock GTest::gtest Threads::Threads CURL::libcurl)
    gtest_discover_tests(fman_tests)
endif()