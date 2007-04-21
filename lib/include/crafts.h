/**
 * The main crafts include file.  Contains everything you needed to know
 * about crafts and more.
 * @author Pinkfish
 * @started Fri Nov 12 17:53:57 PST 1999
 */
#ifndef __CRAFTS
#define __CRAFTS

/**
 * The value the craft functions return as an error.
 */
#define CRAFT_ERROR 0

/**
 * The file to inherit to make a craft process
 */
#define CRAFT_INHERIT "/obj/crafts/base"

/**
 * The location of the craft handler.
 */
#define CRAFT_HANDLER "/obj/crafts/craft_handler"

/**
 * This is the definition for a required input material.
 */
#define CRAFT_MATERIAL_REQUIRED 1
/**
 * This is the definition for a 'one of' input material.  The material
 * must be one of these 'one of' materials.
 */
#define CRAFT_MATERIAL_ONE_OF 2
/**
 * This is the definition for an optional input material
 */
#define CRAFT_MATERIAL_OPTIONAL 3
/**
 * This is the definition for a denied input material
 */
#define CRAFT_MATERIAL_DENIED 4

/**
 * The mask to get of the actual type of the material classication.
 */
#define CRAFT_MATERIAL_TYPE_MASK 127

/**
 * This means that every material must have this material.
 */
#define CRAFT_MATERIAL_ALL    128

/**
 * Worthless quality.
 */
#define CRAFT_QUALITY_WORTHLESS -4
/**
 * Awful quality.
 */
#define CRAFT_QUALITY_AWFUL -3
/**
 * Bad quality.
 */
#define CRAFT_QUALITY_BAD -2
/**
 * Poor quality.
 */
#define CRAFT_QUALITY_POOR -1
/**
 * Normal quality.
 */
#define CRAFT_QUALITY_NORMAL 0
/**
 * Fine quality.
 */
#define CRAFT_QUALITY_FINE 1
/**
 * Good quality.
 */
#define CRAFT_QUALITY_GOOD 2
/**
 * Excelent quality.
 */
#define CRAFT_QUALITY_EXCELLENT 3
/**
 * Perfect quality.
 */
#define CRAFT_QUALITY_PERFECT 4


/**
 * This is a class to handle the bonus stuff for the process.
 * @element result the result of the 'taskmaster' call
 * @element skill_bonus the actual bonus from this skill
 * @element quality a quality indicator
 * @element bonus the overall bonus over the skills
 */
class crafts_bonus {
   mapping result;
   mapping skill_bonus;
   int quality;
   int bonus;
}

#endif
