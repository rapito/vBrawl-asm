set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ppc)
set(CMAKE_CROSSCOMPILING 1)

set(SOURCE_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

if(DEFINED ENV{DEVKITPPC})
  set(DEVKIT_DIR $ENV{DEVKITPPC})
else()
  set(DEVKIT_DIR ${SOURCE_ROOT_DIR}/BuildSystem/Compiler)
endif()

set(PPC_BIN_DIRECTORY ${DEVKIT_DIR}/bin)

set(CMAKE_C_COMPILER ${PPC_BIN_DIRECTORY}/powerpc-eabi-gcc)
set(CMAKE_CXX_COMPILER ${PPC_BIN_DIRECTORY}/powerpc-eabi-g++)

if(WIN32)
  set(CMAKE_C_COMPILER ${CMAKE_C_COMPILER}.exe)
  set(CMAKE_CXX_COMPILER ${CMAKE_CXX_COMPILER}.exe)
endif()
