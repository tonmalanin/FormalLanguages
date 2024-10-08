#include <../include/Automaton.h>

Automaton create_from_reg_exp(const std::string& reg_exp) {
  std::string part1;
  std::string part2;
  int bracket_count = 0;
  int current_step = 1;
  enum class Operation { Union, Concatenation };
  Operation operation;
  for (auto sym : reg_exp) {
    if (current_step == 1) {
      part1 += sym;
      if (sym == '(') {
        ++bracket_count;
      } else if (sym == ')') {
        --bracket_count;
      }
      if (bracket_count == 0) {
        current_step = 2;
      }
    } else if (current_step == 2) {
      if (sym == '*' or sym == '+') {
        part1 += sym;
      } else if (sym == '|') {
        operation = Operation::Union;
        current_step = 3;
      } else {
        operation = Operation::Concatenation;
        part2 += sym;
        current_step = 3;
      }
    } else {
      part2 += sym;
    }
  }
  Automaton automaton1;
  Automaton automaton2;
  if (part2.empty()) {
    if (part1.back() == '*') {
      part1.pop_back();
      automaton1 = create_from_reg_exp(part1);
      automaton1.enclose();
    } else if (part1.back() == '+') {
      part1.pop_back();
      automaton1 = create_from_reg_exp(part1);
      automaton2 = create_from_reg_exp(part1);
      automaton2.enclose();
      automaton1.concatenate(automaton2);
    } else if (part1.back() == ')') {
      part1 = part1.substr(1, part1.size() - 2);
      automaton1 = create_from_reg_exp(part1);
    }else {
      automaton1 = Automaton(part1);
    }
  } else {
    automaton1 = create_from_reg_exp(part1);
    automaton2 = create_from_reg_exp(part2);
    if (operation == Operation::Union) {
      automaton1.unite(automaton2);
    } else {
      automaton1.concatenate(automaton2);
    }
  }
  return automaton1;
}
