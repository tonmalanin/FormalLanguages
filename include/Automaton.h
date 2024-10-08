#pragma once

#include <algorithm>
#include <iostream>
#include <json.hpp>
#include <map>
#include <set>
#include <string>
#include <vector>

using json = nlohmann::json;

const std::string kEpsilon = "z";

std::string int_to_str(size_t n);

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
                       std::set<Edge>& transitions);

 public:
  Automaton() = default;

  Automaton(const Automaton& other) = default;

  Automaton& operator=(const Automaton& other) = default;

  ~Automaton() = default;

  Automaton(Automaton&& other) = default;

  Automaton& operator=(Automaton&& other) = default;

  explicit Automaton(const std::string& sym);

  explicit Automaton(const json& j);

  explicit operator json() const;

  void remove_epsilon_transitions();

  void make_determined();

  Automaton& unite(const Automaton& other);

  Automaton& concatenate(const Automaton& other);

  Automaton& enclose();
};

Automaton create_from_reg_exp(const std::string& reg_exp);