#include "daemon.h"


int main(int argc, char** argv) {

  auto dm = std::make_unique<core>(argv[0], argc, argv);
  dm->set_log_level(LOG_UPTO(LOG_INFO));
  dm->start_daemon();

  return 0;
}
