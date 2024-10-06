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
    for (const Edge& elem : delta[i]) {
      edge["to"] = int_to_str(elem.dest);
      edge["sym"] = elem.symbol;
      j["delta"].push_back(edge);
    }
  }
  return j;
}