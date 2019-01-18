.DT
crafts.h
Disk World autodoc help
crafts.h

.SH Description
.SP 5 5

The main crafts include file.  Contains everything you needed to know
about crafts and more.
.EP
.SP 10 5


Written by Pinkfish

Started Fri Nov 12 17:53:57 PST 1999

.EP

.SH Classes

.SI 3
* %^BOLD%^crafts_bonus%^RESET%^
class crafts_bonus {
mapping result;
mapping skill_bonus;
int quality;
int bonus;
}

.EI
.SP 7 5

This is a class to handle the bonus stuff for the process.
.EP


.SH Defines
.SI 3
* %^BOLD%^CRAFT_ERROR%^RESET%^
.EI
.SP 7 5

The value the craft functions return as an error.

.EP

.SI 3
* %^BOLD%^CRAFT_HANDLER%^RESET%^
.EI
.SP 7 5

The location of the craft handler.

.EP

.SI 3
* %^BOLD%^CRAFT_INHERIT%^RESET%^
.EI
.SP 7 5

The file to inherit to make a craft process

.EP

.SI 3
* %^BOLD%^CRAFT_MATERIAL_ALL%^RESET%^
.EI
.SP 7 5

This means that every material must have this material.

.EP

.SI 3
* %^BOLD%^CRAFT_MATERIAL_DENIED%^RESET%^
.EI
.SP 7 5

This is the definition for a denied input material

.EP

.SI 3
* %^BOLD%^CRAFT_MATERIAL_ONE_OF%^RESET%^
.EI
.SP 7 5

This is the definition for a 'one of' input material.  The material
must be one of these 'one of' materials.

.EP

.SI 3
* %^BOLD%^CRAFT_MATERIAL_OPTIONAL%^RESET%^
.EI
.SP 7 5

This is the definition for an optional input material

.EP

.SI 3
* %^BOLD%^CRAFT_MATERIAL_REQUIRED%^RESET%^
.EI
.SP 7 5

This is the definition for a required input material.

.EP

.SI 3
* %^BOLD%^CRAFT_MATERIAL_TYPE_MASK%^RESET%^
.EI
.SP 7 5

The mask to get of the actual type of the material classication.

.EP

.SI 3
* %^BOLD%^CRAFT_QUALITY_AWFUL%^RESET%^
.EI
.SP 7 5

Awful quality.

.EP

.SI 3
* %^BOLD%^CRAFT_QUALITY_BAD%^RESET%^
.EI
.SP 7 5

Bad quality.

.EP

.SI 3
* %^BOLD%^CRAFT_QUALITY_EXCELLENT%^RESET%^
.EI
.SP 7 5

Excelent quality.

.EP

.SI 3
* %^BOLD%^CRAFT_QUALITY_FINE%^RESET%^
.EI
.SP 7 5

Fine quality.

.EP

.SI 3
* %^BOLD%^CRAFT_QUALITY_GOOD%^RESET%^
.EI
.SP 7 5

Good quality.

.EP

.SI 3
* %^BOLD%^CRAFT_QUALITY_NORMAL%^RESET%^
.EI
.SP 7 5

Normal quality.

.EP

.SI 3
* %^BOLD%^CRAFT_QUALITY_PERFECT%^RESET%^
.EI
.SP 7 5

Perfect quality.

.EP

.SI 3
* %^BOLD%^CRAFT_QUALITY_POOR%^RESET%^
.EI
.SP 7 5

Poor quality.

.EP

.SI 3
* %^BOLD%^CRAFT_QUALITY_WORTHLESS%^RESET%^
.EI
.SP 7 5

Worthless quality.

.EP

