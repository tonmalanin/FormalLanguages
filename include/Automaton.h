#include <vector>

class Automaton {
private :
  struct Edge {
    size_t dest;
    char symbol;
  };

  size_t state_num;
  std::vector<bool> is_final;
  std::vector<std::vector<Edge>> delta;
};