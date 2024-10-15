#include <../include/Automaton.h>

Automaton create_from_reg_exp(const std::string& reg_exp) {
  std::string left_part;
  std::string right_part;
  int parentheses_count = 0;
  int current_step = 1;
  enum class Operation { Merge, Concatenation };
  Operation operation;
  for (auto sym : reg_exp) {
    if (current_step == 1) {
      left_part += sym;
      if (sym == '(') {
        ++parentheses_count;
      } else if (sym == ')') {
        --parentheses_count;
      } else if (parentheses_count == 0 && sym == '|') {
        current_step = 2;
        left_part.pop_back();
        operation = Operation::Merge;
      }
    } else {
      right_part += sym;
    }
  }

  if (right_part.empty()) {
    left_part = "";
    for (auto sym : reg_exp) {
      if (current_step == 1) {
        left_part += sym;
        if (sym == '(') {
          ++parentheses_count;
        } else if (sym == ')') {
          --parentheses_count;
        }
        if (parentheses_count == 0) {
          current_step = 2;
        }
      } else if (current_step == 2) {
        if (sym == '*' or sym == '+') {
          left_part += sym;
        } else {
          operation = Operation::Concatenation;
          right_part += sym;
          current_step = 3;
        }
      } else {
        right_part += sym;
      }
    }
  }

  Automaton left_automaton;
  Automaton right_automaton;
  if (right_part.empty()) {
    if (left_part.back() == '*') {
      left_part.pop_back();
      left_automaton = create_from_reg_exp(left_part);
      left_automaton.enclose();
    } else if (left_part.back() == '+') {
      left_part.pop_back();
      left_automaton = create_from_reg_exp(left_part);
      left_automaton.enclose();
      left_automaton.remove_empty_word();
    } else if (left_part.back() == ')') {
      left_part = left_part.substr(1, left_part.size() - 2);
      left_automaton = create_from_reg_exp(left_part);
    } else {
      left_automaton = Automaton(left_part);
    }
  } else {
    left_automaton = create_from_reg_exp(left_part);
    right_automaton = create_from_reg_exp(right_part);
    if (operation == Operation::Merge) {
      left_automaton.merge(right_automaton);
    } else {
      left_automaton.concatenate(right_automaton);
    }
  }
  return left_automaton;
}
