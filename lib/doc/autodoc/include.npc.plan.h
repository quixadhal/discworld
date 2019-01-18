.DT
plan.h
Disk World autodoc help
plan.h

.SH Description
.SP 5 5

This file contains all the needed defines for the plans.  This is part
of the npc inteligence system.
.EP
.SP 10 5


Written by Pinkfish

Started Thu Aug 13 07:35:51 EDT 1998

.EP

.SH Defines
.SI 3
* %^BOLD%^PLAN_BASE_INHERIT%^RESET%^
.EI
.SP 7 5

This is the file to use as a base class for all plans.  It defines
all the default plan behaviour and makes sure you do not declare your
control functions with the wrong arguments.

.EP

.SI 3
* %^BOLD%^PLAN_DIR%^RESET%^
.EI
.SP 7 5

The directory all the default plans are stored in.

.EP

.SI 3
* %^BOLD%^PLAN_HIGH_SUTABILTIY%^RESET%^
.EI
.SP 7 5

This is the high sutability for a plan.

.EP

.SI 3
* %^BOLD%^PLAN_INVALID_SUTABILITY%^RESET%^
.EI
.SP 7 5

This should be returned if the plan is not at all sutabile for the
specified situation.

.EP

.SI 3
* %^BOLD%^PLAN_LOW_SUTABILITY%^RESET%^
.EI
.SP 7 5

This is the low sutability for a plan.

.EP

.SI 3
* %^BOLD%^PLAN_LOWEST_SUTABILITY%^RESET%^
.EI
.SP 7 5

This is the lowest sutability used for a plan.

.EP

.SI 3
* %^BOLD%^PLAN_MEDIUM_SUTABILITY%^RESET%^
.EI
.SP 7 5

This is the medium sutability for a plan.

.EP

