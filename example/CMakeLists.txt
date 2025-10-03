cmake_minimum_required(VERSION 3.30)

project(Singleton-Example CXX)
enable_language(CXX)

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# `-std=gnu++23`
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSIONS true)

# Common compile options
add_compile_options(
  "-Wall" "-Wextra"
  "-fno-exceptions" "-fno-rtti"
)

# Use ld.lld instead of ld.bfd
set(CMAKE_LINKER_TYPE LLD)

# Detect PIE support; otherwise CMake will not add -fPIE automatically
include(CheckPIESupported)
check_pie_supported()
set(CMAKE_POSITION_INDEPENDENT_CODE true)

# Hide symbols by default to prove that singleton works across shared objects
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN true)

# Directories and files
set(header_include_dir
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
)
set(example_headers
  ${header_include_dir}/singleton.hxx
  ${CMAKE_CURRENT_SOURCE_DIR}/example.hxx
)

# Shared library linked at compile time
add_library(from_dynamic
  SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/dyn_lib_a.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/dyn_lib_b.cc
)
target_include_directories(from_dynamic
  PRIVATE
    ${header_include_dir}
)

# Shared library loaded at runtime with dlopen
add_library(from_dlopen
  SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/dyn_lib_dlopen.cc
)
target_include_directories(from_dlopen
  PRIVATE
    ${header_include_dir}
)

# Static library
add_library(from_static
  STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/static_lib.cc
)
target_include_directories(from_static
  PRIVATE
    ${header_include_dir}
)

# Example executable
add_executable(example
  ${CMAKE_CURRENT_SOURCE_DIR}/example.cc
  ${example_headers}
)
target_include_directories(example
  PRIVATE
    ${header_include_dir}
)
target_link_libraries(example
  PRIVATE
    from_dynamic
    from_static
    dl
)
# ensure dlopen lib is built for the example
add_dependencies(example from_dlopen)
