/**
 * This handles the setup code for rooms and objects.  It simplifies the whole
 * system and makes it nice and generic.
 * @author Pinkfish
 * @started May 16th
 */
private int _do_setup;

/**
 * This method increments the setup variable.
 */
protected void inc_setup() {
   _do_setup++;
} /* inc_setup() */

/**
 * This method decrements the setup variable.
 */
protected void dec_setup() {
   _do_setup--;
} /* dec_setup() */

/**
 * This method checks to see if the setup is possible.
 */
protected void do_setup() {
   if (!_do_setup) {
      this_object()->setup();
   }
} /* do_setup() */

/**
 * This method checks to see if the reset is possible.
 */
protected void do_reset() {
   if (!_do_setup) {
      this_object()->reset();
   }
} /* do_reset() */
