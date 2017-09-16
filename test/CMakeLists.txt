file(GLOB simcc_unittest_SRCS *.cc
							 ${PROJECT_SOURCE_DIR}/3rdparty/gtest/src/gtest-all.cc
							 ${PROJECT_SOURCE_DIR}/3rdparty/gtest/src/gtest_main.cc)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty ${PROJECT_SOURCE_DIR}/3rdparty/gtest)

if (MSVC)
link_directories(${LIBRARY_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}/)
endif (MSVC)

add_executable(simcc_unittest ${simcc_unittest_SRCS})
target_link_libraries(simcc_unittest simcc_static ${DEPENDENT_LIBRARIES})

include (CTest)
add_test(NAME simcc_unittest COMMAND simcc_unittest)
