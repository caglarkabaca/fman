cmake_minimum_required(VERSION 3.5)
project(fman)

set(CMAKE_CXX_STANDARD 20)

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

set(FMAN_SOURCES
        main.cpp
        fman_http.cpp
)

add_executable(fman ${FMAN_SOURCES} ${IMGUI_ALL_SOURCES})

target_include_directories(fman PRIVATE ${IMGUI_DIR} ${IMGUI_DIR}/backends)
target_link_libraries(fman PRIVATE Threads::Threads SDL3::SDL3 CURL::libcurl)

file(COPY resources DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")