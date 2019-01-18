.DT
cwc.h
Disk World autodoc help
cwc.h

.SH Description
.SP 5 5

The base #define for Cwc.  All defines must ultimately be built on
this define.

.EP
.SP 10 5

.EP

.SH Defines
.SI 3
* %^BOLD%^BES_PELARGIC%^RESET%^
.EI
.SP 7 5

The location for Bes Pelargic.  All #define's in /d/cwc/Bes_Pelargic/path.h
are built on this #define.

.EP

.SI 3
* %^BOLD%^CITY_INHERIT%^RESET%^
.EI
.SP 7 5

The location of the base city inherit.  This file should be inherited
into all rooms in Cwc that link into the hospital, inside and outside.

.EP

.SI 3
* %^BOLD%^CUSTOM_NPCS%^RESET%^
.EI

.SI 3
* %^BOLD%^CWC%^RESET%^
.EI

.SI 3
* %^BOLD%^CWC_CHARS%^RESET%^
.EI
.SP 7 5

The location of the generic NPC characters directory.  Most NPCs in here
are generic NPCs which can be used with the hospital.  They must be then
configured to move and walk in the city properly.

.EP

.SI 3
* %^BOLD%^CWC_HANDLERS%^RESET%^
.EI
.SP 7 5

The location for Cwc wide handlers.

.EP

.SI 3
* %^BOLD%^CWC_ITEMS%^RESET%^
.EI
.SP 7 5

The location of Cwc wide items.  The Cwc armoury accesses files in this
directory.

.EP

.SI 3
* %^BOLD%^CWC_SAVE%^RESET%^
.EI
.SP 7 5

The location of the Cwc save directory.

.EP

.SI 3
* %^BOLD%^CWC_START_LOCATION%^RESET%^
.EI
.SP 7 5

The default start location for people in Cwc.  This is an absolute path
and should be revised at a later date.

.EP

.SI 3
* %^BOLD%^CWC_TELEPORT_INHERIT%^RESET%^
.EI
.SP 7 5

The location of the Cwc teleport inheritable.  This inherit should be 
added to rooms to make them follow the restricted teleport rules 
for Cwc.

.EP

.SI 3
* %^BOLD%^HOSPITAL_INHERIT%^RESET%^
.EI
.SP 7 5

The location of the Cwc hospital inheritable.

.EP

.SI 3
* %^BOLD%^HOSPITAL_PATH%^RESET%^
.EI
.SP 7 5

The location of hospital data files.

.EP

.SI 3
* %^BOLD%^HUNGHUNG%^RESET%^
.EI
.SP 7 5

The location for the city of Hunghung.  All #defines in /d/cwc/Hunghung/path.h
are built on this #define.

.EP

.SI 3
* %^BOLD%^INHERIT%^RESET%^
.EI
.SP 7 5

The location of Cwc wide inheritables.

.EP

.SI 3
* %^BOLD%^ORIGAMI_HANDLER%^RESET%^
.EI
.SP 7 5

The location of the Cwc origami handler.  This handler determines 
how you can fold a piece of paper, and transforms a piece of paper
into a folded item.

.EP

.SI 3
* %^BOLD%^SAVE%^RESET%^
.EI

.SI 3
* %^BOLD%^SECURE%^RESET%^
.EI
.SP 7 5

This #define points to the secure Cwc directory.  This directory is 
only really used for domain administrative purposes.

.EP

.SI 3
* %^BOLD%^SHOO_LI%^RESET%^
.EI

