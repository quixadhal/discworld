/**
 * This is the options control code to setup and fiddle with player options
 * for various things.
 * @author Pinkfish
 * @started Mon Oct 27 12:46:36 PST 2003
 */

#define PLAYER_OPTIONS_CLASS
#include <player.h>

mixed query_property(string);
varargs void add_property(string,mixed,int);

/**
 * This method does all the mangeling needed to bring the class up to the
 * new sizes.
 */
private class player_options_control query_player_options() {
   class player_options_control options;

   options = query_property(PLAYER_OPTIONS_CONTROL_PROP);
   if (!options) {
      return new(class player_options_control);
   }

   if (sizeof(options) == 4) {
      class player_options_control newbit;

      // Upgrade the size.
      newbit = new(class player_options_control);
      newbit->follow_everyone = options->follow_everyone;
      newbit->follow_groups = options->follow_groups;
      newbit->follow_friends = options->follow_friends;
      newbit->lead_behind = options->lead_behind;
      options = newbit;
   }
   return options;
}

/**
 * Should the player auto follow in groups.
 */
int query_auto_follow_group() {
   class player_options_control options;

   options = query_property(PLAYER_OPTIONS_CONTROL_PROP);
   if (!options) {
      return 0;
   }

   return options->follow_groups;
}

/**
 * Should the player auto follow friends.
 */
int query_auto_follow_friends() {
   class player_options_control options;

   options = query_property(PLAYER_OPTIONS_CONTROL_PROP);
   if (!options) {
      return 0;
   }

   return options->follow_friends;
}

/**
 * Should the player auto follow everyone.
 */
int query_auto_follow_everyone() {
   class player_options_control options;

   options = query_property(PLAYER_OPTIONS_CONTROL_PROP);
   if (!options) {
      return 0;
   }

   return options->follow_everyone;
}

/**
 * Should the player auto follow everyone.
 */
int query_lead_from_behind() {
   class player_options_control options;

   options = query_property(PLAYER_OPTIONS_CONTROL_PROP);
   if (!options) {
      return 0;
   }

   return options->lead_behind;
}

/**
 * Set the follow in groups automatically flag
 * @param flag
 */
void set_auto_follow_group(int flag) {
   class player_options_control options;
   
   options = query_property(PLAYER_OPTIONS_CONTROL_PROP);

   if (!options) {
      options = new(class player_options_control);
   }
   options->follow_groups = flag;
   add_property(PLAYER_OPTIONS_CONTROL_PROP, options);
}

/**
 * Set the follow frienda automatically flag.
 * @param flag set the follow friends flag
 */
void set_auto_follow_friends(int flag) {
   class player_options_control options;

   options = query_property(PLAYER_OPTIONS_CONTROL_PROP);

   if (!options) {
      options = new(class player_options_control);
   }
   options->follow_friends = flag;
   add_property(PLAYER_OPTIONS_CONTROL_PROP, options);
}

/**
 * Sets the follow everyone flag.
 * @param flag follow everyone
 */
void set_auto_follow_everyone(int flag) {
   class player_options_control options;

   options = query_property(PLAYER_OPTIONS_CONTROL_PROP);

   if (!options) {
      options = new(class player_options_control);
   }
   options->follow_everyone = flag;
   add_property(PLAYER_OPTIONS_CONTROL_PROP, options);
}

/**
 * Sets the lead from behind flag.
 * @param flag the lead from behind flag
 */
void set_lead_from_behind(int flag) {
   class player_options_control options;

   options = query_property(PLAYER_OPTIONS_CONTROL_PROP);

   if (!options) {
      options = new(class player_options_control);
   }
   options->lead_behind = flag;
   add_property(PLAYER_OPTIONS_CONTROL_PROP, options);
}

/**
 * Sets the mxp toggle flag.  If this flag is set to 1 then mxp is
 * disabled.
 */
void set_mxp_disable(int flag) {
   class player_options_control options;

   options = query_player_options();

   options->mxp_disable = 1;
   add_property(PLAYER_OPTIONS_CONTROL_PROP, options);
}

/**
 * This queries the mxp diable flag.
 * @return the current state of the mxp disable flag
 */
int query_mxp_disable() {
   class player_options_control options;

   options = query_player_options();
   return options->mxp_disable;
}

/**
 * This method returns if we should use the mxp codes or not.
 * @return use mxp codes or not
 */
int is_mxp_enabled() {
#if efun_defined(has_mxp)
   if (has_mxp(this_object())) {
      return !query_mxp_disable();
   }
#endif
   return 0;
}
