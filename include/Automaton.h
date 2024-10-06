#pragma once

#include <algorithm>
#include <json.hpp>
#include <map>
#include <set>
#include <string>
#include <vector>

using json = nlohmann::json;

const std::string kEpsilon = "ε";

class Automaton {
 private:
  struct Edge {
    size_t dest;
    std::string symbol;

    auto operator<=>(const Edge& other) const = default;
  };

  size_t start;
  size_t state_num;
  std::vector<bool> is_final;
  std::vector<std::set<Edge>> delta;

  void add_transitions(size_t state, std::vector<bool>& used,
                       std::set<Edge>& new_transitions);

 public:
  explicit Automaton(const json& j);

  explicit operator json() const;

  void remove_epsilon_transitions();
};