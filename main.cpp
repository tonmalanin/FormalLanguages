#include <getopt.h>

#include "include/Automaton.h"

int main(int argc, char** argv) {
  Automaton automaton;
  std::string input_type;
  std::string required_result;
  const char* short_options = "i:r:";
  const struct option long_options[] = {
      {"input", required_argument, nullptr, 'i'},
      {"result", required_argument, nullptr, 'r'},
      {nullptr, 0, nullptr, 0}};
  int rez;
  int option_index;
  while ((rez = getopt_long(argc, argv, short_options, long_options,
                            &option_index)) != -1) {
    switch (rez) {
      case 'i': {
        input_type = optarg;
        break;
      }
      case 'r': {
        required_result = optarg;
        break;
      }
      default: {
        std::cout << "Unknown option";
        return 0;
      }
    }
  }
  if (input_type == "regexp") {
    automaton = create_from_reg_exp(argv[5]);
  } else {
    automaton = Automaton(static_cast<json>(argv[5]));
  }
  automaton.remove_epsilon_transitions();
  if (required_result == "dfa") {
    automaton.make_determined();
  } else if (required_result == "min-cdfa") {
    automaton.make_minimal();
  }
  std::cout << static_cast<json>(automaton);
  return 0;
}