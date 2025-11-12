
cmake_minimum_required(VERSION 3.20)
project(game VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for LSP/clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find SFML 3.0
find_package(SFML 3.0 COMPONENTS Graphics Window System Audio Network REQUIRED)

# Find yaml-cpp (with hint for Homebrew location)
if(APPLE)
    # For Apple Silicon
    set(YAML_CPP_HINTS "/opt/homebrew/include" "/opt/homebrew/lib")
    # For Intel Macs, add: "/usr/local/include" "/usr/local/lib"
endif()

find_path(YAML_CPP_INCLUDE_DIR 
    NAMES yaml-cpp/yaml.h
    HINTS ${YAML_CPP_HINTS}
)

find_library(YAML_CPP_LIBRARY
    NAMES yaml-cpp
    HINTS ${YAML_CPP_HINTS}
)

if(NOT YAML_CPP_INCLUDE_DIR OR NOT YAML_CPP_LIBRARY)
    message(FATAL_ERROR "yaml-cpp not found! Install with: brew install yaml-cpp")
endif()

include_directories(
    include
    ${YAML_CPP_INCLUDE_DIR}
)
# Compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Automatically collect all .cpp files in src/
file(GLOB_RECURSE SOURCES 
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

set(SOURCES 
	src/main.cpp)
# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link SFML 3.0 (note the SFML:: prefix)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    SFML::Audio
    SFML::Network
    ${YAML_CPP_LIBRARY}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy assets to build directory

# macOS specific: Fix @rpath issues
if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        BUILD_RPATH "@executable_path/../lib;@loader_path/../lib"
        INSTALL_RPATH "@executable_path/../lib;@loader_path/../lib"
    )
endif()

# Optional: Add tests
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
