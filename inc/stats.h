#pragma once

#include <chrono>

#include "commons.h"

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
  StatsAtom tStar;
  StatsAtom label;
};