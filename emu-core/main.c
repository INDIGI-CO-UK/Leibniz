#include "newton.h"
#include "monitor.h"
#include "runt.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

void FlushDisplay(const char *display, int width, int height) {
}

void print_usage(const char *name) {
  fprintf(stderr, "usage: %s [-b bootmode] [-m mapfile] romfile\n", name);
  exit(1);
}

int main(int argc, char **argv) {
  extern char *optarg;
  int c, err = 0;
  
  char *bootmode = NULL;
  char *mapname = NULL;
  
  while ((c = getopt(argc, argv, "b:m:")) != -1) {
    switch (c) {
      case 'b':
        bootmode = optarg;
        break;
      case 'm':
        mapname = optarg;
        break;
      case '?':
        err = 1;
        break;
    }
  }
  
  if (err || optind == argc) {
    print_usage(argv[0]);
  }
  
  char *romFile = argv[optind];
  
  newton_t *newton = newton_new();
  if (newton_load_rom(newton, romFile) == -1) {
    return -1;
  }
  
  if (bootmode != NULL) {
    newton_set_bootmode(newton, atoi(bootmode));
  }
  
  if (mapname != NULL) {
    newton_load_mapfile(newton, mapname);
  }
  
  runt_t *runt = newton_get_runt(newton);
  runt_set_log_flags(runt, RuntLogAll, 1);
  
  newton_set_break_on_unknown_memory(newton, true);
  
  monitor_t *monitor = monitor_new();
  monitor_set_newton(monitor, newton);
  monitor_run(monitor);
  
  return 0;
}
