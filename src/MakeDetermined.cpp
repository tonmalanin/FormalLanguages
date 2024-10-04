#include <../include/Automaton.h>

void Automaton::add_transitions(int state, std::vector<bool>& used,
                                std::set<Edge>& new_transitions) {
  if (used[state]) {
    for (auto edge : delta[state]) {
      if (edge.symbol != epsilon) {
        new_transitions.insert(edge);
      }
    }
    return;
  }
  used[state] = true;
  for (auto edge : delta[state]) {
    if (edge.symbol == epsilon) {
      if (is_final[edge.dest]) {
        is_final[state] = true;
      }
      add_transitions(edge.dest, used, new_transitions);
    } else {
      new_transitions.insert(edge);
    }
  }
  for (auto edge : new_transitions) {
    delta[state].insert(edge);
  }
}

void Automaton::remove_epsilon_transitions() {
  std::vector<bool> used(state_num, false);
  std::set<Edge> transitions;
  for (int i = 0; i < state_num; ++i) {
    if (!used[i]) {
      new_transitions.clear();
      add_transitions(i, used, transitions);
    }
  }
  for (int i = 0; i < state_num; ++i) {
    transitions.clear();
    for (auto edge : delta[i]) {
      if (edge.symbol == epsilon) {
        transitions.insert(edge);
      }
    }
    for (auto edge : transitions) {
      delta[i].erase(edge);
    }
  }
}