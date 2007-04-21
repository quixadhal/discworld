/**
 * The include file for armour stuff.
 * @author Ceres
 */
#ifndef __SYS__ARMOUR
#define __SYS__ARMOUR

/**
 * Defines for armour callbacks.
 * ARM_INNER places the magical protection inside all physical armour but
 * just above the skin.
 * ARM_OUTER places the protection just outside the physical armour
 */
#define ARM_INNER 0
#define ARM_OUTER 1

/**
 * Class used to store callback data.
 * @entry id A unique ID number given to each entry.
 * @entry callback the object & function names for the callback
 * @entry data any additional data the callback function desires.
 */
class armour_callback {
  int id;
  mixed callback;
  mixed data;
}

#endif /* __SYS__ARMOUR */
