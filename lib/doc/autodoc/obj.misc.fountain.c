.DT
fountain.c
Disk World autodoc help
fountain.c

.SH Description
.SP 5 5

Cool fountain with a variable refill rate!

.EP
.SP 10 5


Written by Terano.

To use:
  1) Clone!
  2) Call initialise with appropriate args!
  3) Something!
  4) Big profits!


.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/vessel.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/volumes.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^initialise%^RESET%^
.EI
.SI 5
varargs void initialise(string path, int amount, int time)
.EI
.SP 7 5

Initialise the fountain, the path is the path of the reagent to fill
the fountain with, the amount to refill per cycle, and the frequency of cycles.
rate to.
.EP

.SI 3
* %^BOLD%^reinitialise%^RESET%^
.EI
.SI 5
varargs void reinitialise(string path, int amount, int time)
.EI
.SP 7 5

Initialise the fountain, the path is the path of the reagent to fill
the fountain with, the amount to refill per cycle, and the frequency of cycles.
rate to.
.EP


