cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(NeoOnnx LANGUAGES CXX)

set(PROJECT_VERSION ${FINE_VERSION_MAJOR}.${FINE_VERSION_MINOR}.${FINE_VERSION_PATCH})

# Build NeoOnnx as shared library.
option(NeoOnnx_BUILD_SHARED "Build NeoOnnx as shared library." ON)

add_subdirectory(src)

if(NOT USE_FINE_OBJECTS AND NeoML_INSTALL)
    install(DIRECTORY include/NeoOnnx DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

# Build Onnx2NeoML
if(NOT ANDROID AND NOT IOS)
    add_subdirectory(Onnx2NeoML)
endif()