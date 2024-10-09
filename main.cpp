#include "include/Automaton.h"

int main(int argc, char** argv) {
  Automaton automaton;
  if (argv[1][1] == 's') {
    automaton = create_from_reg_exp(argv[3]);
  } else {
    automaton = Automaton(static_cast<json>(argv[3]));
  }
  automaton.remove_epsilon_transitions();
  if (argv[2][1] == 'd') {
    automaton.make_determined();
  } else if (argv[2][1] == 'm') {
    automaton.make_minimal();
  }
  std::cout << static_cast<json>(automaton);
  return 0;
}