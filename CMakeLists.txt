cmake_minimum_required(VERSION 2.8)
add_compile_options(-std=c++11)
project(dxl_driver)

# set(CMAKE_CXX_CPPLINT "cpplint;--linelength=120")
find_package(GTest REQUIRED )

include_directories(
  BEFORE ${GTEST_INCLUDE_DIRS}
)
include_directories(
  include
  dxl_driver_core/include
)

# unit tests
enable_testing()

### CORE ###
add_library(
  ${PROJECT_NAME}_core
  "dxl_driver_core/src/DxlProtocolV1.cpp"
)

### UNIT TESTS ###
add_executable(
${PROJECT_NAME}_unit_tests
  "test/unit_test_main.cpp"
  "test/mock.cpp"
  "test/test_protocol_v1.cpp"
)

target_include_directories(
  ${PROJECT_NAME}_unit_tests 
  PRIVATE 
  ${INCLUDE_DIRECTORIES}
  test 
  ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(
  ${PROJECT_NAME}_unit_tests 
  ${PROJECT_NAME}_core
  ${GTEST_BOTH_LIBRARIES} 
  pthread 
)

add_dependencies(
  ${PROJECT_NAME}_unit_tests 
  ${PROJECT_NAME}_core
)

add_test(${PROJECT_NAME}_unit_tests ${PROJECT_NAME}_unit_tests)
