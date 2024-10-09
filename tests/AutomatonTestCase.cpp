#include "Automaton.h"
#include "gtest/gtest.h"

class AutomatonTestCase : public ::testing::Test {
 public:
  static std::string complicated_reg_exp;
  static std::string odd_reg_exp;
};

std::string AutomatonTestCase::complicated_reg_exp = "a(a(ab)*a(ab)*|b)*";
std::string AutomatonTestCase::odd_reg_exp = "(((z|z)*)+|((z*)*(z+)+)*)*";

TEST_F(AutomatonTestCase, ParsingComplicatedRegularExpression) {
  EXPECT_NO_THROW(create_from_reg_exp(complicated_reg_exp));
}

TEST_F(AutomatonTestCase, ParsingOddRegularExpression) {
  EXPECT_NO_THROW(create_from_reg_exp(odd_reg_exp));
}

TEST_F(AutomatonTestCase, RemoveEpsilonFromComplicated) {
  Automaton automaton = create_from_reg_exp(complicated_reg_exp);
  EXPECT_NO_THROW(automaton.remove_epsilon_transitions());
}

TEST_F(AutomatonTestCase, RemoveEpsilonFromOdd) {
  Automaton automaton = create_from_reg_exp(odd_reg_exp);
  EXPECT_NO_THROW(automaton.remove_epsilon_transitions());
}

TEST_F(AutomatonTestCase, MakeMinimalFromComplicated) {
  Automaton automaton = create_from_reg_exp(complicated_reg_exp);
  automaton.remove_epsilon_transitions();
  EXPECT_NO_THROW(automaton.make_minimal());
}

TEST_F(AutomatonTestCase, MakeMinimalFromOdd) {
  Automaton automaton = create_from_reg_exp(odd_reg_exp);
  automaton.remove_epsilon_transitions();
  EXPECT_NO_THROW(automaton.make_minimal());
}


