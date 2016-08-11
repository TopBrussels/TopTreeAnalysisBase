include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${TOPBRUSSELS_INCLUDE_DIRS})
link_directories(~/lib)
add_executable(testBTagWeightTools BTagWeightToolsTestSuite.cpp)
target_link_libraries(testBTagWeightTools ${GTEST_LIBRARIES} ${ROOT_LIBRARIES} "TopTreeAna80" "TopTreeAnaContent80")

#set(testBTagWeightToolsROOTHistArg ${PROJECT_SOURCE_DIR}/test/data/HistosPtEta_JECon_76X_v1.root)
#set(testBTagWeightToolsCSVArg ${PROJECT_SOURCE_DIR}/Calibrations/BTagging/CSVv2_76X_combToMujets.csv)
#add_test(AllTestsInBTagWeightToolsTestSuite testBTagWeightTools  ${testBTagWeightToolsROOTHistArg} ${testBTagWeightToolsCSVArg})

set(testBTagWeightToolsArgs ${PROJECT_SOURCE_DIR}/test/data/HistosPtEta_JECon_76X_v1.root ${PROJECT_SOURCE_DIR}/Calibrations/BTagging/CSVv2_76X_combToMujets.csv)
GTEST_ADD_TESTS(testBTagWeightTools  "${testBTagWeightToolsArgs}" BTagWeightToolsTestSuite.cpp)

