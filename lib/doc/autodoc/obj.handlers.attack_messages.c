.DT
attack_messages.c
Disk World autodoc help
attack_messages.c

.SH Description
.SP 5 5

This handles the standard messages for attacks.  I cannot be bothered
rewriting the docs for this.
This handles the standard messages for the given damage types:
   blunt, sharp, pierce, fire.

 There are also subtypes:
   blunt - hands, feet, tail, hoof, wrestle, pommel.
   sharp - claws, chew, slice, chop.
   pierce - bite, beak, stab, horns, tusks.
.EP
.SP 10 5


Written by pinkfish

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^query_message%^RESET%^
.EI
.SI 5
mixed query_message(int dam, string type, object him, object me, string name, object wep, string zone)
.EI


