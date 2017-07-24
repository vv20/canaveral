#pragma	once

#include "sample.h"

class BackEnd {
  public:
    BackEnd ();
    void playSample (SampleInstance* sample);
    void silence();
};

