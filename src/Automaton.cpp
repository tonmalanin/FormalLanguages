#include <../include/Automaton.h>

Automaton::Automaton(const json& j) : state_num(0) {
  std::map<std::string, size_t> states;
  for (std::string s : j["states"]) {
    states[s] = state_num;
    ++state_num;
  }
  start = {states[j["s0"]]};
  is_final.resize(state_num, false);
  for (std::string s : j["final"]) {
    is_final[states[s]] = true;
  }
  delta.resize(state_num);
  for (json edge : j["delta"]) {
    std::string sym = edge["sym"];
    delta[states[edge["from"]]].insert({states[edge["to"]], edge["sym"]});
    if (sym != kEpsilon) {
      alphabet.insert(sym);
    }
  }
}

std::string int_to_str(size_t n) {
  std::string result;
  if (n == 0) {
    return "0";
  }
  while (n != 0) {
    result += '0' + n % 10;
    n /= 10;
  }
  std::reverse(result.begin(), result.end());
  return result;
}

Automaton::operator json() const {
  json j;
  j["s0"] = int_to_str(start[0]);
  j["states"] = std::vector<std::string>();
  for (size_t i = 0; i < state_num; ++i) {
    j["states"].push_back(int_to_str(i));
  }
  j["final"] = std::vector<std::string>();
  for (size_t i = 0; i < state_num; ++i) {
    if (is_final[i]) {
      j["final"].push_back(int_to_str(i));
    }
  }
  j["delta"] = std::vector<json>();
  for (size_t i = 0; i < state_num; ++i) {
    json edge;
    edge["from"] = int_to_str(i);
    for (auto& elem : delta[i]) {
      edge["to"] = int_to_str(elem.dest);
      edge["sym"] = elem.symbol;
      j["delta"].push_back(edge);
    }
  }
  return j;
}

Automaton& Automaton::unite(const Automaton& other) {
  for (size_t i = 0; i < other.state_num; ++i) {
    delta.emplace_back();
    for (auto& edge : other.delta[i]) {
      delta[i + state_num].insert({edge.dest + state_num, edge.symbol});
    }
  }
  delta[start[0]].insert({state_num + other.start[0], kEpsilon});
  for (size_t i = 0; i < other.state_num; ++i) {
    is_final.push_back(other.is_final[i]);
  }
  state_num += other.state_num;
  for (auto& sym : other.alphabet) {
    alphabet.insert(sym);
  }
  return *this;
}

Automaton& Automaton::concatenate(const Automaton& other) {
  for (size_t i = 0; i < other.state_num; ++i) {
    delta.emplace_back();
    for (auto& edge : other.delta[i]) {
      delta[i + state_num].insert({edge.dest + state_num, edge.symbol});
    }
  }
  for (size_t i = 0; i < state_num; ++i) {
    if (is_final[i]) {
      delta[i].insert({other.start[0] + state_num, kEpsilon});
      is_final[i] = false;
    }
  }
  for (size_t i = 0; i < other.state_num; ++i) {
    is_final.push_back(other.is_final[i]);
  }
  state_num += other.state_num;
  for (auto& sym : other.alphabet) {
    alphabet.insert(sym);
  }
  return *this;
}

Automaton& Automaton::enclose() {
  for (size_t i = 0; i < state_num; ++i) {
    if (is_final[i]) {
      delta[i].insert({start[0], kEpsilon});
    }
  }
  is_final[start[0]] = true;
  return *this;
}

Automaton::Automaton(const std::string& sym)
    : start({0}), state_num(2), is_final({false, true}) {
  delta.resize(2);
  delta[0].insert({1, sym});
  if (sym != kEpsilon) {
    alphabet.insert(sym);
  }
}
