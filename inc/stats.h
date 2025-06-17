#pragma once

#include <chrono>

#include "commons.h"
#include "structs.h"

using namespace std::literals;

typedef chrono::duration<double, std::nano> Duration;
typedef chrono::time_point<std::chrono::high_resolution_clock> TimePoint;

struct StatsAtom {
  Duration duration;
  int treeSize;
  int treeDepth;
};
static StatsAtom dummyStatsAtom;

class DurationMeasurer {
  Duration &duration;
  TimePoint startTime;

public:
  DurationMeasurer(Duration &duration) : duration(duration) {
    startTime = chrono::high_resolution_clock::now();
  }

  ~DurationMeasurer() {
    duration = chrono::high_resolution_clock::now() - startTime;
  }
};

struct Stats {
  Stats(Formula formula) : formula(formula) {}
  Stats() : formula(0) {}

  const Formula formula;
  StatsAtom tStar;
  StatsAtom label;
};

void PrintStatsHeader();
void PrintStats(const Stats &stats);