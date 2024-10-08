#include <../include/Automaton.h>

Automaton::Automaton(const json& j) : state_num(0) {
  std::map<std::string, size_t> states;
  for (std::string s : j["states"]) {
    states[s] = state_num;
    ++state_num;
  }
  start = states[j["s0"]];
  is_final.resize(state_num, false);
  for (std::string s : j["final"]) {
    is_final[states[s]] = true;
  }
  delta.resize(state_num);
  for (json edge : j["delta"]) {
    size_t from = states[edge["from"]];
    size_t to = states[edge["to"]];
    std::string sym = edge["sym"];
    delta[from].insert({to, sym});
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
  j["s0"] = int_to_str(start);
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
  std::vector<std::set<Edge>> new_delta(2 + state_num + other.state_num);
  new_delta[0] = {{start + 1, kEpsilon}, {other.start + 1 + state_num, kEpsilon}};
  for (size_t i = 0; i < state_num; ++i) {
    for (auto& edge : delta[i]) {
      new_delta[i + 1].insert({edge.dest + 1, edge.symbol});
    }
    if (is_final[i]) {
      new_delta[i + 1].insert({state_num + other.state_num + 1, kEpsilon});
    }
  }
  for (size_t i = 0; i < other.state_num; ++i) {
    for (auto& edge : other.delta[i]) {
      new_delta[i + 1 + state_num].insert({edge.dest + state_num + 1, edge.symbol});
    }
    if (other.is_final[i]) {
      new_delta[i + 1 + state_num].insert({state_num + other.state_num + 1, kEpsilon});
    }
  }
  start = 0;
  state_num += 2 + other.state_num;
  is_final.clear();
  is_final.resize(state_num, false);
  is_final.back() = true;
  delta = new_delta;
  return *this;
}

Automaton& Automaton::concatenate(const Automaton& other) {
  for (size_t i = 0; i < other.state_num; ++i) {
    delta.emplace_back();
    for (auto& edge : other.delta[i]) {
      delta.back().insert({edge.dest + state_num, edge.symbol});
    }
    if (other.is_final[i]) {
      delta.back().insert({state_num + other.state_num, kEpsilon});
    }
  }
  for (size_t i = 0; i < state_num; ++i) {
    if (is_final[i]) {
      delta[i].insert({other.start + state_num, kEpsilon});
    }
  }
  delta.emplace_back();
  state_num += 1 + other.state_num;
  is_final.clear();
  is_final.resize(state_num, false);
  is_final.back() = true;
  return *this;
}

Automaton& Automaton::enclose() {
  delta[start].insert({state_num, kEpsilon});
  for (size_t i = 0; i < state_num; ++i) {
    if (is_final[i]) {
      delta[i].insert({start, kEpsilon});
      delta[i].insert({state_num, kEpsilon});
    }
  }
  delta.emplace_back();
  ++state_num;
  is_final.clear();
  is_final.resize(state_num, false);
  is_final.back() = true;
  return *this;
}

Automaton::Automaton(const std::string& sym)
    : start(0), state_num(2), is_final({false, true}) {
  delta.resize(2);
  delta[0].insert({1, sym});
}