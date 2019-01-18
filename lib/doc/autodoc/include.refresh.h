.DT
refresh.h
Disk World autodoc help
refresh.h

.SH Description
.SP 5 5

Definition so we can reach the refresh handler, and some
other definitions for the arguments passed to the callback
function.
.EP
.SP 10 5


Written by Ceres

.EP

.SH Defines
.SI 3
* %^BOLD%^PARTIAL_REFRESH%^RESET%^
.EI
.SP 7 5

The flag to the method that tells us that the refresh is a partial
one.

.EP

.SI 3
* %^BOLD%^PLAYER_DELETED%^RESET%^
.EI
.SP 7 5

The flag to the method that tells us that the refresh is actually
the player being deleted.

.EP

.SI 3
* %^BOLD%^REFRESH_HANDLER%^RESET%^
.EI
.SP 7 5

The path to the refresh handler itself.

.EP

.SI 3
* %^BOLD%^TOTAL_REFRESH%^RESET%^
.EI
.SP 7 5

The flag to the method that tells us that the refresh is a total
one.

.EP

