cmake_minimum_required(VERSION 3.5)
project(ATM)

set (CMAKE_CXX_STANDARD 14)

find_package(GTest)

include_directories(
  include
)

add_library(
  bank
  src/bank/bank_api.cpp
)

enable_testing()

add_executable(
  bank_test
  test/bank_api.cpp
)

target_link_libraries(
  bank_test
  bank
  GTest::gtest_main
)

gtest_discover_tests(bank_test)
