#include <../include/Automaton.h>

void Automaton::add_transitions(size_t state, std::vector<bool>& used,
                                std::set<Edge>& new_transitions) {
  used[state] = true;
  for (auto& edge : delta[state]) {
    if (edge.symbol == kEpsilon) {
      if (!used[edge.dest]) {
        add_transitions(edge.dest, used, new_transitions);
      }
      if (is_final[edge.dest]) {
        is_final[state] = true;
      }
    } else {
      new_transitions.insert(edge);
    }
  }
}

void Automaton::remove_epsilon_transitions() {
  std::vector<bool> used;
  std::set<Edge> transitions;
  for (size_t i = 0; i < state_num; ++i) {
    used.clear();
    used.resize(state_num, false);
    transitions.clear();
    add_transitions(i, used, transitions);
    for (auto& edge : transitions) {
      delta[i].insert(edge);
    }
  }
  for (size_t i = 0; i < state_num; ++i) {
    transitions.clear();
    for (auto& edge : delta[i]) {
      if (edge.symbol == kEpsilon) {
        transitions.insert(edge);
      }
    }
    for (auto& edge : transitions) {
      delta[i].erase(edge);
    }
  }
}

void Automaton::make_determined() {
  std::vector<std::set<size_t>> states;
  states.emplace_back();
  for (auto state : start) {
    states[0].insert(state);
  }
  size_t h = 0;
  size_t t = 1;
  std::vector<std::set<Edge>> new_delta;
  while (h != t) {
    new_delta.emplace_back();
    std::map<std::string, std::set<size_t>> transitions;
    for (auto& sym : alphabet) {
      transitions[sym] = {};
    }
    for (auto elem : states[h]) {
      for (auto& edge : delta[elem]) {
        transitions[edge.symbol].insert(edge.dest);
      }
    }
    for (auto& elem : transitions) {
      auto key = elem.first;
      auto value = elem.second;
      size_t id = t;
      for (size_t i = 0; i < t; ++i) {
        if (states[i] == value) {
          id = i;
          break;
        }
      }
      if (id == t) {
        ++t;
        states.push_back(value);
      }
      new_delta[h].insert({id, key});
    }
    ++h;
  }
  std::vector<bool> new_final(t, false);
  for (size_t i = 0; i < t; ++i) {
    for (auto state : states[i]) {
      new_final[i] = (new_final[i] || is_final[state]);
    }
  }
  start = {0};
  state_num = t;
  is_final = new_final;
  delta = new_delta;
}
