cmake_minimum_required(VERSION 3.15)

project(ProjectMCodes)

include(CMake/CompilerSettings.cmake)
include(CMake/Paths.cmake)
include(CMake/Functions.cmake)

create_folder(${PROJECT_BINARY_DIR}/TempCodes)

add_custom_target(resetIncludedCodes ALL
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_BINARY_DIR}/IncludedCodes && ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/IncludedCodes
        BYPRODUCTS ${PROJECT_BINARY_DIR}/IncludedCodes/*
        OUTPUT ${PROJECT_BINARY_DIR}/IncludedCodes/*)


#Used to wait until everything is built
add_custom_target(codeBarrier DEPENDS resetIncludedCodes)

include_directories(${librariesDirectory})
include_directories(${globalsDirectory})


#code subdirectories
add_subdirectory(Codes)
add_subdirectory(Libraries)
add_subdirectory(Globals)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

add_custom_target(build ALL
        COMMAND ${Python3_EXECUTABLE} ${builder} ${PROJECT_BINARY_DIR} ${PROJECT_BINARY_DIR}/IncludedCodes ${PPC_BIN_DIRECTORY} ${functionMapDirectory} ${settingsPath}
        DEPENDS codeBarrier)