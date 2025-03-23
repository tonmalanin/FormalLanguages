find_package(GTest)

add_executable(Tests EXCLUDE_FROM_ALL MainTests.cpp AutomatonTestCase.cpp)

target_link_libraries(Tests PUBLIC ${GTEST_LIBRARIES} Automaton MakeDetermined AutomatonFromRegularExpression MakeMinimal)
