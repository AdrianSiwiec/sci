#include <iomanip>
#include <sstream>

#include "commons.h"
#include "label_set.h"
#include "label_solver.h"
#include "solver.h"
#include "stats.h"
#include "structs.h"

std::string beautify_duration(std::chrono::seconds input_seconds) {
  using namespace std::chrono;
  typedef duration<int, std::ratio<86400>> days;
  auto d = duration_cast<days>(input_seconds);
  input_seconds -= d;
  auto h = duration_cast<hours>(input_seconds);
  input_seconds -= h;
  auto m = duration_cast<minutes>(input_seconds);
  input_seconds -= m;
  auto s = duration_cast<seconds>(input_seconds);

  auto dc = d.count();
  auto hc = h.count();
  auto mc = m.count();
  auto sc = s.count();

  std::stringstream ss;
  ss.fill('0');
  if (dc) {
    ss << d.count() << "d";
  }
  if (dc || hc) {
    if (dc) {
      ss << std::setw(2);
    } // pad if second set of numbers
    ss << h.count() << "h";
  }
  if (dc || hc || mc) {
    if (dc || hc) {
      ss << std::setw(2);
    }
    ss << m.count() << "m";
  }
  if (dc || hc || mc || sc) {
    if (dc || hc || mc) {
      ss << std::setw(2);
    }
    ss << s.count() << 's';
  }

  return ss.str();
}

int main() {
  vector<string> lines;
  for (string line; getline(cin, line);) {
    lines.push_back(line);
  }
  cerr << "Read " << lines.size() << endl;

  PrintStatsHeader();

  TimePoint bStartTime = chrono::high_resolution_clock::now();
  for (int i = 0; i < lines.size(); i++) {

    auto formulas = DoParseFormulas(lines[i], false);
    // Benchmark supports only one formula lines
    assert(formulas.size() == 1);

    // Insert NOT -- because those algorithms check if it's closed not if it's
    // tautology
    // formulas[0] = FNot(formulas[0]);

    Stats stats(formulas[0]);

    auto node = DoSolve(formulas, stats.tStar, false);
    auto labelNode = DoSolveLabel(formulas, stats.label, false);
    assert(node.is_closed.has_value());
    assert(labelNode.is_closed.has_value());
    assert(node.is_closed.value() == labelNode.is_closed.value());
    stats.isTautology = node.is_closed.value();

    PrintStats(stats);

    if (i > 0 && ((i % 100) == 0)) {
      auto d = chrono::duration_cast<chrono::seconds>(
          (chrono::high_resolution_clock::now() - bStartTime));

      cerr << "Benchmarked " << i << "/" << lines.size() << " ("
           << (100.0 * i) / lines.size() << "%) lines in "
           << beautify_duration(d) << ". ";

      Duration dLast = chrono::high_resolution_clock::now() - bStartTime;

      Duration remainingTime = dLast * ((lines.size() - i) / (double)i);
      chrono::seconds remainingSeconds =
          chrono::duration_cast<chrono::seconds>(remainingTime);
      cerr << "Est. remaining time: " << beautify_duration(remainingSeconds)
           << endl;
    }
  }
}