#ifndef __SYS__CMR
#define __SYS__CMR

/**
 * This file defines useful things for the cmr_handler and cmr_library.
 * In particular it defines the MINERALS, MINERAL_ADJECTIVES and SKILLS arrays
 * used when adding materials to the handler.
 * @author Deutha
 * @see add_material
 */
#define MINERALS 0
#define GEMS 1
#define METALS 2
#define WOODS 3
#define MATERIALS ({ "mineral", "gem", "metal", "wood" })
#define MATERIAL_ADJECTIVES ({ "mineral", "gem", "metallic", "wooden" })

#define KNOWN 0
#define MAGIC 1
#define OS 2
#define OW 3
#define OVG 4
#define OVJ 5
#define LEARNT 99
#define SKILLS ({ \
      "none", \
      "magic", \
      "crafts.smithing", \
      "crafts.carpentry", \
      "other.valueing.gems", \
      "other.valueing.jewellery", \
   })

#define CMR_HANDLER "/obj/handlers/cmr_handler"
#define CMR_LIBRARY "/obj/handlers/cmr_library"

#endif /* __SYS__CMR */
