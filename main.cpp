#include "include/logic.h"
#include <chrono>
#include <thread>

int main() {

  while (true) {
    App ps(GetUpdates());
    if (IsThereUpdates) {
      ps.ParseUpdates();
      ps.Process_Message();
    }
    std::this_thread::sleep_for(std::chrono::seconds(3));
  }
  return 0;
}
