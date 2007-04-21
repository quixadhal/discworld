/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: enchant.c,v 1.16 2002/06/05 21:19:11 ohdamn Exp $
 * $Log: enchant.c,v $
 * Revision 1.16  2002/06/05 21:19:11  ohdamn
 * Fixed typo
 *
 * Revision 1.15  2002/02/21 19:53:53  tannah
 * added some call_others so that query_max_enchant() and
 * query_degrade_enchant() could be shadowed... (and then didn't shadow
 * them after all).
 *
 * Revision 1.14  2001/04/13 00:04:07  pinkfish
 * Add in a degrade enchantment cutoff.
 *
 * Revision 1.13  2000/05/24 02:16:22  pinkfish
 * Make the rounds up even more generous.
 *
 * Revision 1.12  2000/05/24 02:12:46  pinkfish
 * It should round up the enchant values a little more generously.
 *
 * Revision 1.11  2000/05/19 19:36:06  pinkfish
 * Fix up a typo that caused add_enchant not to work.
 *
 * Revision 1.9  2000/05/19 06:46:41  pinkfish
 * Make enchantment degrade.
 *
 * Revision 1.8  2000/04/21 18:03:47  tannah
 * Fixed another bug.  Hat. Hat. Ernkgh.
 *
 * Revision 1.7  2000/04/20 21:14:17  tannah
 * Fixed a tiny leetle bugge in enchant_string(). Hat. Hat. Hat.
 *
 * Revision 1.6  2000/04/20 20:16:09  pinkfish
 * Add in talisman messages.
 *
 * Revision 1.5  1999/10/04 22:01:42  pinkfish
 * Fix up enchant to be more useful in how it handles enchantment.  Make the
 * variable private.
 *
 * Revision 1.4  1999/05/20 17:04:45  dragonkin
 * Added extra check in enchant_string() for enchants over max enchant
 *
 * Revision 1.3  1999/02/16 20:19:52  ceres
 * Minor tweaks
 *
 * Revision 1.2  1998/03/23 21:29:58  olorin
 * made query_max_enchant() maskable
 *
 * Revision 1.1  1998/01/06 03:59:10  ceres
 * Initial revision
 * 
*/
/**
 * Module to handle the amount of magical charge (enchantment) of an
 * object has.  The maximum enchantment of an item is determined by
 * by its weight - the material of composition is not yet taken into
 * account.
 * <p>
 * Enchanting an object will add to its value, and, if the object
 * is an item of armour, will improve its armour class.
 *
 * @change 20th May 1994 Deutha
 *    added a function to calculate the maximum
 *                 enchantment based on weight.
 * @author Pinkfish
 */

nosave private int _enchanted;
nosave private int _set_enchant_time;
nosave private int _degrade_enchant;

#define ENCHANT_DEGRADE_TIME (8 * 7 * 24 * 60 * 60)

/**
 * This method returns the maximum possible enchantment for the
 * object.  This is based on its weight.
 * @return the maximum enchantment
 */
int query_max_enchant()
{
   return 5 + (int) this_object()->query_weight() / 4;
}                               /* query_max_enchant() */

/**
 * This method returns the maximum enchantment before the enchantment
 * starts to degrade.
 * @return the enchantment level which enchantment degrades to
 */
int query_degrade_enchant() {
   if (!_degrade_enchant) {
      return this_object()->query_max_enchant() / 2;
   }
   return _degrade_enchant;
} /* query_degrade_enchant() */

/**
 * This method sets the enchantment at which things start to degrade.
 * @param enchant the enchantment level to set it at
 */
void set_degrade_enchant(int enchant) {
   if (enchant > this_object()->query_max_enchant()) {
      enchant = this_object()->query_max_enchant();
   }
   _degrade_enchant = enchant;
} /* set_degreade_enchant() */

/**
 * This method returns the current enchantment string for the object.  The 
 * string is created from the percentage of the max enchantment that it is 
 * enchanted to.  If the object is also a talisman, an identifying string is 
 * appended to the enchantment message.
 * <p>
 * These messages are only visible to creators and players who can see
 * octarine.
 * <p>
 * If the property "octarine_mess" is set, then this will be printed instead 
 * of the automatically generated enchantment message.
 * <p>
 * If the property "talisman_mess" is set, then this will be printed instead
 * of the automatically generated talisman message if appropriate.
 * @return the enchantment string and talisman string
 * @see set_enchant()
 * @see add_enchant()
 * @see query_max_enchant()
 * @see query_enchant()
 */
string enchant_string()
{
   string tal_msg;
   string ench_msg;

   if (this_object()->query_property("talisman")) {
      tal_msg = (string) this_object()->query_property("talisman_mess");
      if (!tal_msg)
         tal_msg = "It has the hazy octarine sparkle of a magical talisman.";
      tal_msg += "\n";
   } else {
      tal_msg = "";
   }

   if (this_object()->query_property("octarine_mess")) {
      ench_msg = (string) this_object()->query_property("octarine_mess")
         + "\n";
   } else {
      switch ((this_object()->query_enchant() * 100) /
              this_object()->query_max_enchant()) {
      case 1..10:
         ench_msg = "It occasionally pulses with octarine light.\n";
         break;
      case 11..20:
         ench_msg = "It emits a slight octarine glow.\n";
         break;
      case 21..30:
         ench_msg = "It softly pulses in dull octarine shades.\n";
         break;
      case 31..40:
         ench_msg = "It gives off a steady but dull octarine glow.\n";
         break;
      case 41..50:
         ench_msg = "It gives off a steady octarine glow.\n";
         break;
      case 51..60:
         ench_msg = "It glows an intense octarine.\n";
         break;
      case 61..70:
         ench_msg = "It emits a bright octarine colour.\n";
         break;
      case 71..80:
         ench_msg = "It brightly pulses octarine.\n";
         break;
      case 81..90:
         ench_msg = "It glows brilliant octarine shades.\n";
         break;
      case 91..100:
         ench_msg = "It radiates pure octarine brilliance.\n";
         break;
      default:
         ench_msg = "";
      }
   }
   return ench_msg + tal_msg;
}                               /* enchant_string() */

/**
 * This method sets the current enchantment level.   If it is greator than
 * the max enchantment level then it will be set to the maximum
 * enchantment level.
 * @param number the new enchantment level
 * @see enchant_string()
 * @see query_enchant()
 * @see query_max_enchant()
 * @see add_enchant()
 */
void set_enchant(int number)
{
   if (number > this_object()->query_max_enchant()) {
      number = this_object()->query_max_enchant();
   }
   _set_enchant_time = time();
   _enchanted = number;
}                               /* set_enchant() */

/**
 * This method changes the current enchanment level.
 * @param numnber the amount to chant the enchantment level by
 * @see enchant_string()
 * @see query_enchant()
 * @see set_enchant()
 * @see query_max_enchant()
 */
int add_enchant(int number)
{
   set_enchant(this_object()->query_enchant() + number);
   return _enchanted;
}                               /* add_enchant() */

/**
 * This method queries the current enchantment level. 
 * @param number the new enchantment level
 * @see enchant_string()
 * @see set_enchant()
 * @see query_max_enchant()
 * @see add_enchant()
 */
int query_enchant()
{
   int max;
   int tmp;
   int tmp2;
   int degrade;

   max = this_object()->query_max_enchant();
   if (_enchanted > max) {
      _enchanted = max;
   }

   degrade = this_object()->query_degrade_enchant();
   if (_enchanted > degrade) {
      if (!_set_enchant_time) {
         _set_enchant_time = time();
      }
      if (time() - _set_enchant_time >= ENCHANT_DEGRADE_TIME) {
         _enchanted = degrade;
      } else {
         tmp2 = tmp = (_enchanted - degrade) * 100 + 99;
         tmp *= 100 - (((time() - _set_enchant_time) * 100) /
                       ENCHANT_DEGRADE_TIME);
/*
if (this_player() && this_player()->query_name() == "pinkfish") {
tell_creator("pinkfish", "Max %d start %d tmp %d enchant %d\n", max, tmp2, tmp, _enchanted);
}
 */
         return degrade + (tmp / 10000);
      }
   }
   return _enchanted;
}                               /* query_enchant() */

/**
 * This method returns the actual enchant of the object.
 * @return the actual enchant value
 */
int query_real_enchant()
{
   return _enchanted;
}                               /* query_real_enchant() */

/**
 * This method returns the time the time the enchantment was set.
 * @return the time the enchantment was set
 */
int query_enchant_set_time()
{
   return _set_enchant_time;
}                               /* query_enchant_set_time() */

/**
 * This method sets the time at which the enchant level was set.
 * @param tim the time to set it to
 */
protected void set_enchant_set_time(int tim)
{
   _set_enchant_time = tim;
}                               /* set_enchant_time() */
