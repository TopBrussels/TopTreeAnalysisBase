include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${TOPBRUSSELS_INCLUDE_DIRS})
add_executable(testKinFitter TestKinFitterSuite.cpp)
target_link_libraries(testKinFitter ${GTEST_LIBRARIES} ${ROOT_LIBRARIES})

#add_test(AllTestsInTestKinFitter testKinFitter)
GTEST_ADD_TESTS(testKinFitter "" TestKinFitterSuite.cpp)

