#pragma once

#include <algorithm>
#include <iostream>
#include <json.hpp>
#include <map>
#include <set>
#include <string>
#include <vector>

using json = nlohmann::json;

const std::string kEpsilon = "#";

std::string int_to_str(size_t n);

class Automaton {
 private:
  struct Transition {
    size_t dest;
    std::string symbol;

    auto operator<=>(const Transition& other) const = default;
  };

  std::vector<size_t> start;
  size_t state_num;
  std::vector<bool> is_final;
  std::vector<std::set<Transition>> delta;
  std::set<std::string> alphabet;

  void add_transitions(size_t state, std::vector<bool>& used,
                       std::set<Transition>& transitions);

 public:
  Automaton() = default;

  Automaton(const Automaton& other) = default;

  Automaton& operator=(const Automaton& other) = default;

  ~Automaton() = default;

  Automaton(Automaton&& other) = default;

  Automaton& operator=(Automaton&& other) = default;

  explicit Automaton(const std::string& sym);

  explicit Automaton(const json& json_object);

  explicit operator json() const;

  void remove_epsilon_transitions();

  void make_deterministic();

  Automaton& merge(const Automaton& other);

  Automaton& concatenate(const Automaton& other);

  Automaton& enclose();

  void reverse();

  void make_minimal();

  void remove_empty_word();

  void set_alphabet(const std::string& compressed_alphabet);
};

Automaton create_from_reg_exp(const std::string& reg_exp);
