#include "Automaton.h"
#include "gtest/gtest.h"

class AutomatonTestCase : public ::testing::Test {};

TEST(AutomatonTestCase, ComplicatedRegularExpression) {
  std::string reg_exp = "a(a(ab)*a(ab)*|b)*";
  EXPECT_NO_THROW(create_from_reg_exp(reg_exp));
}
