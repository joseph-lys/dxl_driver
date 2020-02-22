cmake_minimum_required(VERSION 2.8)
add_compile_options(-std=c++11)
project(dxl_driver)

# set(CMAKE_CXX_CPPLINT "cpplint;--linelength=120")

include_directories(
  include
  dxl_driver_core/include
)

# unit tests
enable_testing()
find_package(GTest REQUIRED )
include_directories( 
  ${GTEST_INCLUDE_DIRS} 
)

### CORE ###
add_library(
  ${PROJECT_NAME}_core
  "dxl_driver_core/src/DxlProtocolV1.cpp"
)

### MOCK ###
add_library(
${PROJECT_NAME}_mock
  "test/mock.cpp"
)
target_include_directories(${PROJECT_NAME}_mock PRIVATE test)


### UNIT TESTS ###
add_executable(
${PROJECT_NAME}_unit_tests
  # "test/test_v1_driver.cpp"
  # "test/test_v1_reader.cpp"
  "test/test_v1_writer.cpp"
)
target_include_directories(
  ${PROJECT_NAME}_unit_tests 
  PRIVATE 
  test 
  ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(
  ${PROJECT_NAME}_unit_tests 
  ${PROJECT_NAME}_mock
)

add_test(${PROJECT_NAME}_unit_tests ${PROJECT_NAME}_unit_tests)
