#include <stdio.h>
#include <string.h>

#include "config.h"
#include "lint.h"
#include "interpret.h"
#include "object.h"
#include "exec.h"

/* These are both permanent refs */
/* Don't release this pointer ever. It is used elsewhere. */
char *simul_efun_file_name;
static struct program *simul_efunp = 0;

void set_simul_efun (name)
  char *name;
{
  struct object *ob;

  if (!name) {
    fprintf (stderr, "No simul_efun\n");
    return;
  }
  simul_efun_file_name = make_shared_string (name);

  ob = find_object2(simul_efun_file_name);

  if (ob == 0) {
    ob = load_object(simul_efun_file_name, 0);
    if (ob == 0) {
      fprintf (stderr, "The simul_efun file %s was not loaded.\n",
         simul_efun_file_name);
      exit(1);
    }
  }

  simul_efunp = ob->prog;
  reference_prog (simul_efunp, "Simul-efun");
} /* set_simul_efun() */
