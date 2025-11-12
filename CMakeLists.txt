cmake_minimum_required(VERSION 3.22)
project(progettopf VERSION 0.1.0)

include(CTest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

string(APPEND CMAKE_CXX_FLAGS
      " -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion"
      " -Wshadow -Wimplicit-fallthrough -Wextra-semi -Wold-style-cast"
      " -fno-omit-frame-pointer")

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  string(APPEND CMAKE_CXX_FLAGS " -D_GLIBCXX_ASSERTIONS")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  string(APPEND CMAKE_CXX_FLAGS " -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_EXTENSIVE")
endif()

string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address,undefined")
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  string(APPEND CMAKE_CXX_FLAGS " -D_GLIBCXX_SANITIZE_STD_ALLOCATOR")
endif()
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address,undefined")

# Trova SFML (versione 2.6)
set(SFML_DIR "~/progetto/SFML-2.6.0/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# --- Programma principale ---
add_executable(programma
    main.cpp
    biliardo.cpp
    biliardo.hpp
    biliardo_statistica.cpp
)
target_link_libraries(programma PRIVATE sfml-graphics sfml-window sfml-system)

# --- Test ---
if (BUILD_TESTING)
  add_executable(biliardo_test
      biliardo_test.cpp
      biliardo.cpp
      biliardo.hpp
  )
  target_link_libraries(biliardo_test PRIVATE sfml-graphics sfml-window sfml-system)
  add_test(NAME biliardo_test COMMAND biliardo_test)
endif()
