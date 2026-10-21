cmake_minimum_required(VERSION 3.20)
project(axxist-usvfs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Statically link the MSVC runtime so the launcher has no VC++ redistributable dependency.
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(USVFS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../vendor/usvfs" CACHE PATH
    "Checkout of ModOrganizer2/usvfs built with scripts/vendor-usvfs.mjs")

# USVFS ships parallel x64/x86 proxy DLLs. A 32-bit game can only be hooked by the 32-bit
# launcher, so this project is built once per architecture (see scripts/vendor-usvfs.mjs).
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
  set(USVFS_IMPORT usvfs_x86)
  set(LAUNCHER_NAME axxist-usvfs-x86)
else()
  set(USVFS_IMPORT usvfs_x64)
  set(LAUNCHER_NAME axxist-usvfs)
endif()

add_executable(axxist-usvfs WIN32 main.cpp)
set_target_properties(axxist-usvfs PROPERTIES OUTPUT_NAME "${LAUNCHER_NAME}")
target_compile_definitions(axxist-usvfs PRIVATE UNICODE _UNICODE NOMINMAX WIN32_LEAN_AND_MEAN)
target_include_directories(axxist-usvfs PRIVATE "${USVFS_DIR}/include")
target_link_directories(axxist-usvfs PRIVATE "${USVFS_DIR}/lib")
target_link_libraries(axxist-usvfs PRIVATE ${USVFS_IMPORT} shell32)

install(TARGETS axxist-usvfs RUNTIME DESTINATION .)
