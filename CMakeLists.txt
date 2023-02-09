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

add_library(
  atm
  src/atm/atm_controller.cpp
)

target_link_libraries(
  atm
  bank
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

add_executable(
  atm_test
  test/atm_controller.cpp
)
target_link_libraries(
  atm_test
  bank
  atm
  GTest::gtest_main
)

gtest_discover_tests(bank_test)

gtest_discover_tests(atm_test)
