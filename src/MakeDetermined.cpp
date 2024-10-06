#include <../include/Automaton.h>

void Automaton::add_transitions(size_t state, std::vector<bool>& used,
                                std::set<Edge>& new_transitions) {
  if (used[state]) {
    for (const Edge& edge : delta[state]) {
      if (edge.symbol != kEpsilon) {
        new_transitions.insert(edge);
      }
    }
    return;
  }
  used[state] = true;
  for (const Edge& edge : delta[state]) {
    if (edge.symbol == kEpsilon) {
      if (is_final[edge.dest]) {
        is_final[state] = true;
      }
      add_transitions(edge.dest, used, new_transitions);
    } else {
      new_transitions.insert(edge);
    }
  }
  for (const Edge& edge : new_transitions) {
    delta[state].insert(edge);
  }
}

void Automaton::remove_epsilon_transitions() {
  std::vector<bool> used(state_num, false);
  std::set<Edge> transitions;
  for (size_t i = 0; i < state_num; ++i) {
    if (!used[i]) {
      transitions.clear();
      add_transitions(i, used, transitions);
    }
  }
  for (size_t i = 0; i < state_num; ++i) {
    transitions.clear();
    for (const Edge& edge : delta[i]) {
      if (edge.symbol == kEpsilon) {
        transitions.insert(edge);
      }
    }
    for (const Edge& edge : transitions) {
      delta[i].erase(edge);
    }
  }
}