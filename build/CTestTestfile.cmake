# CMake generated Testfile for 
# Source directory: /home/cperoni/laboratori/progetto
# Build directory: /home/cperoni/laboratori/progetto/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test([=[biliardo_test]=] "/home/cperoni/laboratori/progetto/build/Debug/biliardo_test")
  set_tests_properties([=[biliardo_test]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/cperoni/laboratori/progetto/CMakeLists.txt;50;add_test;/home/cperoni/laboratori/progetto/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test([=[biliardo_test]=] "/home/cperoni/laboratori/progetto/build/Release/biliardo_test")
  set_tests_properties([=[biliardo_test]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/cperoni/laboratori/progetto/CMakeLists.txt;50;add_test;/home/cperoni/laboratori/progetto/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test([=[biliardo_test]=] "/home/cperoni/laboratori/progetto/build/RelWithDebInfo/biliardo_test")
  set_tests_properties([=[biliardo_test]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/cperoni/laboratori/progetto/CMakeLists.txt;50;add_test;/home/cperoni/laboratori/progetto/CMakeLists.txt;0;")
else()
  add_test([=[biliardo_test]=] NOT_AVAILABLE)
endif()
