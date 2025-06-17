#include "stats.h"

void PrintStatsHeader() {
  cout << "formula,"
          "formulaSize,"
          "formulaDepth,"
          "timeTStarMs,"
          "timeLabelMs,"
          "sizeTreeTStar,"
          "sizeTreeLabel,"
          "depthTreeTStar,"
          "depthTreeLabel,"
       << endl;
}

void PrintStats(const Stats &stats) {
  cout << stats.formula << ","              //
       << GetSize(stats.formula) << ","     //
       << GetDepth(stats.formula) << ","    //
       << stats.tStar.duration / 1ms << "," //
       << stats.label.duration / 1ms << "," //
       << stats.tStar.treeSize << ","       //
       << stats.label.treeSize << ","       //
       << stats.tStar.treeDepth << ","      //
       << stats.label.treeDepth << ","      //
       << endl;
}