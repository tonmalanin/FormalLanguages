#include <../include/Automaton.h>

void Automaton::reverse() {
  std::vector<std::set<Edge>> new_delta(state_num);
  std::vector<size_t> new_start;
  for (size_t i = 0; i < state_num; ++i) {
    for (auto& edge : delta[i]) {
      new_delta[edge.dest].insert({i, edge.symbol});
    }
    if (is_final[i]) {
      new_start.push_back(i);
    }
  }
  is_final.clear();
  is_final.resize(state_num, false);
  for (auto state : start) {
    is_final[state] = true;
  }
  delta = new_delta;
  start = new_start;
}

void Automaton::make_minimal() {
  this->reverse();
  this->make_determined();
  this->reverse();
  this->make_determined();
}
