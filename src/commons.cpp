#include "commons.h"

vector<string> SplitString(string input, string delimeter) {
  size_t found;
  vector<string> result;
  while ((found = input.find(delimeter)) != string::npos) {
    string sub = input.substr(0, found);
    if (!sub.empty())
      result.push_back(sub);
    input.replace(0, found + 1, "");
  }
  result.push_back(input);
  return result;
}