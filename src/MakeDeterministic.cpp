#include <../include/Automaton.h>

void Automaton::add_transitions(size_t state, std::vector<bool>& used,
                                std::set<Transition>& new_transitions) {
  used[state] = true;
  for (auto& transition : delta[state]) {
    if (transition.symbol == kEpsilon) {
      if (!used[transition.dest]) {
        add_transitions(transition.dest, used, new_transitions);
      }
      if (is_final[transition.dest]) {
        is_final[state] = true;
      }
    } else {
      new_transitions.insert(transition);
    }
  }
}

void Automaton::remove_epsilon_transitions() {
  std::vector<bool> used;
  std::set<Transition> transitions;
  for (size_t i = 0; i < state_num; ++i) {
    used.clear();
    used.resize(state_num, false);
    transitions.clear();
    add_transitions(i, used, transitions);
    for (auto& transition : transitions) {
      delta[i].insert(transition);
    }
  }
  for (size_t i = 0; i < state_num; ++i) {
    transitions.clear();
    for (auto& transition : delta[i]) {
      if (transition.symbol == kEpsilon) {
        transitions.insert(transition);
      }
    }
    for (auto& transition : transitions) {
      delta[i].erase(transition);
    }
  }
}

void Automaton::make_deterministic() {
  std::vector<std::set<size_t>> states;
  states.emplace_back();
  for (auto state : start) {
    states[0].insert(state);
  }
  size_t queue_head = 0;
  size_t queue_tail = 1;
  std::vector<std::set<Transition>> new_delta;
  while (queue_head != queue_tail) {
    new_delta.emplace_back();
    std::map<std::string, std::set<size_t>> transitions;
    for (auto& sym : alphabet) {
      transitions[sym] = {};
    }
    for (auto state : states[queue_head]) {
      for (auto& transition : delta[state]) {
        transitions[transition.symbol].insert(transition.dest);
      }
    }
    for (auto& transition : transitions) {
      auto key = transition.first;
      auto value = transition.second;
      size_t id = queue_tail;
      for (size_t i = 0; i < queue_tail; ++i) {
        if (states[i] == value) {
          id = i;
          break;
        }
      }
      if (id == queue_tail) {
        ++queue_tail;
        states.push_back(value);
      }
      new_delta[queue_head].insert({id, key});
    }
    ++queue_head;
  }
  std::vector<bool> new_final(queue_tail, false);
  for (size_t i = 0; i < queue_tail; ++i) {
    for (auto state : states[i]) {
      new_final[i] = (new_final[i] || is_final[state]);
    }
  }
  start = {0};
  state_num = queue_tail;
  is_final = new_final;
  delta = new_delta;
}
