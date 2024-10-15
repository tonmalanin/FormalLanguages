#include <../include/Automaton.h>

Automaton::Automaton(const json& json_object) : state_num(0) {
  std::map<std::string, size_t> states;
  for (std::string s : json_object["states"]) {
    states[s] = state_num;
    ++state_num;
  }
  start = {states[json_object["s0"]]};
  is_final.resize(state_num, false);
  for (std::string s : json_object["final"]) {
    is_final[states[s]] = true;
  }

  delta.resize(state_num);
  for (json transition : json_object["delta"]) {
    std::string sym = transition["sym"];
    delta[states[transition["from"]]].insert(
        {states[transition["to"]], transition["sym"]});
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
  json json_object;
  json_object["s0"] = int_to_str(start[0]);
  json_object["states"] = std::vector<std::string>();
  for (size_t i = 0; i < state_num; ++i) {
    json_object["states"].push_back(int_to_str(i));
  }

  json_object["final"] = std::vector<std::string>();
  for (size_t i = 0; i < state_num; ++i) {
    if (is_final[i]) {
      json_object["final"].push_back(int_to_str(i));
    }
  }

  json_object["delta"] = std::vector<json>();
  for (size_t i = 0; i < state_num; ++i) {
    json edge;
    edge["from"] = int_to_str(i);
    for (auto& transition : delta[i]) {
      edge["to"] = int_to_str(transition.dest);
      edge["sym"] = transition.symbol;
      json_object["delta"].push_back(edge);
    }
  }
  return json_object;
}

Automaton& Automaton::merge(const Automaton& other) {
  for (size_t i = 0; i < other.state_num; ++i) {
    delta.emplace_back();
    for (auto& transition : other.delta[i]) {
      delta[i + state_num].insert(
          {transition.dest + state_num, transition.symbol});
    }
  }
  delta[start[0]].insert({state_num + other.start[0], kEpsilon});

  for (size_t i = 0; i < other.state_num; ++i) {
    is_final.push_back(other.is_final[i]);
  }
  state_num += other.state_num;
  return *this;
}

Automaton& Automaton::concatenate(const Automaton& other) {
  for (size_t i = 0; i < other.state_num; ++i) {
    delta.emplace_back();
    for (auto& transition : other.delta[i]) {
      delta[i + state_num].insert(
          {transition.dest + state_num, transition.symbol});
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
}

void Automaton::remove_empty_word() { is_final[start[0]] = false; }

void Automaton::set_alphabet(const std::string& compressed_alphabet) {
  size_t pos = 0;
  for (size_t i = 0; i < compressed_alphabet.size(); ++i) {
    if (compressed_alphabet[i] == kEpsilon[0]) {
      alphabet.insert(compressed_alphabet.substr(pos, i - pos));
      pos = i + 1;
    }
  }
  alphabet.insert(compressed_alphabet.substr(pos));
}
