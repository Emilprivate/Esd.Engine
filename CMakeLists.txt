cmake_minimum_required(VERSION 3.16)

# Set project name, version, and languages
project(EsdEngine VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(
        ${CMAKE_SOURCE_DIR}/Libs/SDL/include
        ${CMAKE_SOURCE_DIR}/Libs/imgui
        ${CMAKE_SOURCE_DIR}/Src
)

# Add the executable
file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/Src/*.cpp
        ${CMAKE_SOURCE_DIR}/Src/*.hpp
        )

# Add necessary ImGui files
list(APPEND SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/Libs/imgui/imgui.cpp
        ${CMAKE_SOURCE_DIR}/Libs/imgui/imgui_demo.cpp
        ${CMAKE_SOURCE_DIR}/Libs/imgui/imgui_draw.cpp
        ${CMAKE_SOURCE_DIR}/Libs/imgui/imgui_tables.cpp
        ${CMAKE_SOURCE_DIR}/Libs/imgui/imgui_widgets.cpp
        ${CMAKE_SOURCE_DIR}/Libs/imgui/backends/imgui_impl_sdl2.cpp
        ${CMAKE_SOURCE_DIR}/Libs/imgui/backends/imgui_impl_opengl2.cpp
        )
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Linking libraries
if (WIN32)
    target_link_libraries(${PROJECT_NAME}
            ${CMAKE_SOURCE_DIR}/Libs/SDL/lib/x64/SDL2.lib
            ${CMAKE_SOURCE_DIR}/Libs/SDL/lib/x64/SDL2main.lib
            gdi32 opengl32 imm32)
elseif(UNIX)
    find_package(SDL2 REQUIRED)
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} SDL2::SDL2 SDL2::SDL2main ${OPENGL_gl_LIBRARY})
    if(APPLE)
        target_link_libraries(${PROJECT_NAME} "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
    else()
        target_link_libraries(${PROJECT_NAME} GL dl)
    endif()
endif()
