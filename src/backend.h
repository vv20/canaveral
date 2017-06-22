#pragma	once

#include "sample.h"

class BackEnd {
  public:
    BackEnd ();
    void playSample (Sample* sample);
    void stopSample (Sample* sample);
};

