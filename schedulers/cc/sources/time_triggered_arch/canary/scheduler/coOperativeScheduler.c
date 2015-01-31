#include "canary/config.h"  // in every *.c or *.cc file
#include "canary/scheduler/coOperativeScheduler.h"

// App
#include "canary/tasks/onChain.h"

//static
void schDispatch_void() {
  onSlot();
  //lockSlot();
  //ulockSlot();
  //offSlot();
  //otherSlot();
}

void schRunLoop_void() {
  while(1) {
    schDispatch_void();
    break;  // TODO: remove it
  }
}

static
void schStop_void() {
  // TODO: some action
}
