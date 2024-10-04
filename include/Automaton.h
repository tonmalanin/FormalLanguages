#include <set>
#include <vector>

using epsilon = ' ';

class Automaton {
 private:
  struct Edge {
    int dest;
    char symbol;
  };

  size_t state_num;
  std::vector<bool> is_final;
  std::vector<std::set<Edge>> delta;

  void add_transitions(int state, std::vector<bool>& used,
                       std::set<Edge>& new_transitions);

 public:
  void remove_epsilon_transitions();
};