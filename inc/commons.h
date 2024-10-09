#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <vector>

using namespace std;

#define ull unsigned long long

vector<string> SplitString(string input, string delimeter);

template <typename T> bool Contains(const vector<T> &v, const T &f) {
  for (const auto &vf : v) {
    if (vf == f)
      return true;
  }
  return false;
}