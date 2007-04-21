/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: desc.c,v 1.11 2003/01/30 16:44:28 taffyd Exp $
 * 
*/
/**
 * The main description handling class.  Keeps track of the longs short
 * etc.
 * @author Pinkfish
 * @change sometime Deutha
 * Added all the a_short(), the_short() etc stuff.
 */
inherit "/std/basic/hide_invis";

nosave mixed short_d;
nosave mixed plural_d;
nosave mixed long_d;
private nosave string short_desc;

/**
 * This method sets the short description for the object.  Be careful
 * with using function pointers here, as they do not save when
 * players log out.
 * <p>
 * The short description is the description seen in peoples inventorys and
 * in brief mode when you enter rooms. This should be only a few words
 * long and should contain a fairly good description of the
 * object. Colour processing is done on basicly all of discworlds output,
 * so that will work in the short.
 * @param words the short description
 * @example
 * set_short("red rose");
 * @see short()
 * @see query_short()
 * @see a_short()
 * @see the_short()
 * @see poss_short()
 * @see one_short()
 */
void set_short(mixed words) {
  if (functionp(words)) {
    if (!short_d)
      plural_d = (: pluralize(evaluate(short_d)) :);
  } else {
     if ( !short_d && words && ( words != "" ) )
        plural_d = pluralize( words );
  }
   short_d = words;
} /* set_short() */

/**
 * This method sets the long description for the object.  The long description
 * is what the player sees with they 'look' at the object.  Please make sure
 * tht you use sentances in your long description.  Something like
 * 'The red box' is not a sentance.  You can use colour codes in your
 * long description to make it look cool.
 * @see help::look
 * @see long()
 * @see query_long()
 * @see help::colours
 * @param str the long description
 * @example
 * set_long("Picture perfect and glowing with an internal radience the "
 *          "Red Rose is simply wonderful.\n");
 */
void set_long(mixed str) { long_d = str; }
/**
 * This method sets the main plural for the object.  The main plural
 * is like the short description for the pluralised object.  If this
 * is not set then the short will attempt to be pluralised to
 * figure it out.
 * @see efun::pluralize()
 * @see query_plural()
 * @see query_main_plural()
 * @see set_short()
 * @see add_plural()
 * @param str the main plural
 * @example
 * set_main_plural("green wombles");
 */
void set_main_plural(mixed str) { plural_d = str; }
/**
 * This method returns the current value of the main plural.
 * @return the main plural
 */
mixed query_main_plural() { return plural_d; }

/**
 * This method returns the unadulterated short description.  The other
 * short calls might modify the output to display some status information.
 * @return the short description
 * @see short()
 * @see set_short()
 * @see a_short()
 * @see the_short()
 * @see poss_short()
 * @see one_short()
 */
mixed query_short() {
   if ( functionp( short_d ) )  {
      return evaluate(short_d);
   }
   else return short_d;
}

/** 
 * This method creates a simple short description for players based on their
 * stats, gender and guild.
 */
string query_short_desc() {
  string tmp;
  
  if(!short_desc) {
    short_desc = "";
    if(this_object()->query_str() > 16)
      short_desc += "strapping ";
    else if(this_object()->query_dex() > 16)
      short_desc += "nimble ";
    else if(this_object()->query_int() > 16)
      short_desc += "studious ";
    else if(this_object()->query_wis() > 16)
      short_desc += "pious ";

    short_desc += "young ";
    switch(this_object()->query_guild_ob()) {
    case "/std/guilds/warrior":
      short_desc += (this_object()->query_gender() == 2? "female " : "male ") +
        "warrior";
      break;
    case "/std/guilds/thief":
      tmp = "/std/guilds/thief"->query_guild_data();
      if(tmp)
        short_desc += (this_object()->query_gender() == 2? "female ": "male ")+
          tmp;
      else
        short_desc += (this_object()->query_gender() == 2? "female ": "male ")+
          "thief";
      break;
    case "/std/guilds/wizard":
      short_desc += (this_object()->query_gender() == 2? "female " : "male ") +
        "wizard";
      break;
    case "/std/guilds/witch":
      short_desc += "witch";
      break;
    case "/std/guilds/assassin":
      short_desc += (this_object()->query_gender() == 2? "female " : "male ") +
        "assassin";
      break;
    default:
      short_desc += (this_object()->query_gender() == 2? "lass" : "lad");
    }
  }

  return short_desc;
}

/**
 * This method returns the pluralized version of query_short_desc(). 
 * @see query_short_desc()
 */
string query_plural_desc() {
    // a strapping young thief -> two strapping young thieves. 
    // the standard pluralize() function will be fine for now.
    return pluralize( query_short_desc() ); 
} /* query_plural_desc() */ 

/**
 * This method returns the unadulterated long description.  The other
 * long calls might modify the output to display some status information.
 * @return the long description
 * @see long()
 * @see set_long()
 */
varargs mixed query_long(string str, int dark) {
  if (functionp(long_d))
    return evaluate(long_d);
  return long_d;
} /* query_long() */

/**
 * This method returns extra details about the object that should be
 * known above the basic long.  This is printed to people who do not
 * own the object in question (long() is used if you own it).
 * @param arg the argument the look was called with
 * @param dark how dark the place is
 * @param looker the person looking at the object
 */
string query_long_details(string arg, int dark, object looker) {
   return 0;
} /* query_long_details() */

/**
 * This method returns the current short description.  This is often
 * overridden to return status information about the object.
 * @return the short description string
 * @param dark if it is dark when viewing
 */
varargs string short(int dark) {
  if (functionp(short_d))
    return (string)evaluate(short_d);
  return short_d; 
} /* short() */

/**
 * This method returns the current long description.  This is
 * often overriden to display other information.
 * @return the current long descriptions
 * @param str the string which matched us
 * @param dark is it dark when viewing
 * @see set_long()
 */
string long(string str, int dark) {
  if (!long_d)
    return "You see nothing particularly interesting.\n";
  if (functionp(long_d))
    return (string)evaluate(long_d);
  return long_d;
} /* long() */

/**
 * This method returns the current plural string.  This is often overridden
 * to display extra information about the object, like the short and
 * long calls.  There is always an exception to a naming scheme :)
 * @see set_main_plural()
 * @see query_main_plural()
 * @return the plural string
 * @param dark if it was dark when viewing
 */
varargs string query_plural( int dark ) {
  if (!plural_d)
    if (!short( dark ))
      return 0;
    else
      return pluralize(short( dark ));
  if (functionp(plural_d))
    return (string)evaluate(plural_d);
  return plural_d;
} /* query_plural() */

/**
 * This returns the determinate for the object.  The determinate is
 * something like 'the'.  You set the derterminate by setting
 * the "determinate" property.  The viewer is check to see if
 * they can see the object as well, if it cannot be seen
 * "' is returned.
 * @example
 * // Set the determinate property
 * add_property("determinate", "the");
 * @see /std/basic/property->add_property()
 * @param thing the thing to check for visibility
 */
string query_determinate(object thing) {
  if(query_visible(thing)) {
#ifdef USE_ROLE_PLAYING_SHORTS
    if(interactive(this_object()) &&
      thing->query_role_playing() &&
       !thing->is_friend(this_object()->query_name()))
      return "a ";
#endif
    return (string)this_object()->query_property( "determinate" );
  }
  return "";
} /* query_determinate() */

/**
 * This method does weird stuff.  Pretty much it just calls the
 * short() function with the dark parameter...  This should not be
 * used to generate output.  Use a_short(), the_short(), or
 * one_short or poss_short() instead.
 * @see short()
 * @see /global/player->check_dark()
 * @return the string being the short
 * @param thing the viewer
 * @see a_short()
 * @see the_short()
 * @see poss_short()
 * @see one_short()
 */
varargs string pretty_short( object thing ) {
   int dark, verbose;

   if ( objectp( thing ) ) {
      dark = (int)thing->check_dark( (int)this_object()->query_light() ); 
      if(userp(thing))
        verbose = thing->query_verbose("names");
   }
     
   return (string)this_object()->short( dark, verbose );
} /* pretty_short() */

/**
 * The observer dependant plural.  It was specifical written to
 * handle money that needed to be displayed over several lines but
 * only be one object. The way this was achived was if pretty_plural
 * returns an array then each of the elements of the array is considered
 * to be a seperate object and displayed accordingly. 
 * @see set_main_plural()
 * @see /obj/money->pretty_plural()
 * @param thing the observer
 * @return the plural string
 */
varargs string pretty_plural( object thing ) {
   int dark;
   if ( objectp( thing ) )
      dark = (int)thing->check_dark( (int)this_object()->query_light() );
   return (string)this_object()->query_plural( dark );
} /* pretty_plural() */

/**
 * This method returns the "indeterminate" short.  This function returns a
 * string that the message system replaces, when messages are printed, by the
 * determinate of the object and its short as given by pretty_short. The
 * string should not be stored, since the object to which it refers may
 * not later exist; it can be processed with convert_message.
 * <p>
 * The return of this function needs more processing before the player
 * sees it.  In general you will not need to worry about this.
 * @param flag If this flag is set, do not bring the player out of hiding.
 * @see set_short()
 * @see short()
 * @see the_short()
 * @see poss_short()
 * @see one_short()
 * @see /global/events->convert_message()
 * @example
 * "a poor beggar"
 * "the Weasel"
 * "Detritus"
 */
varargs string a_short( int flag ) {
/*
   if ( objectp( thing ) ) {
      log_file( "SHORT", "a_short from "+ implode( map_array( previous_object( -1 ) + ({ this_object() }), (: file_name( $1 ) :) ), ", " ) +"\n" );
      return (string)thing->a_short();
   }
*/
   if ( this_object() == this_player() && !flag )
      this_object()->remove_hide_invis( "hiding" );
   return "$a_short:"+ file_name( this_object() ) +"$";
} /* a_short() */

/**
 * This method returns an "out-of-a-group" short.
 * This function returns a string that the message system replaces, when
 * messages are printed, by a string dependent on the determinate of
 * the object and its short as given by pretty_short. If the viewer
 * is not in the same environment as the object or if the determinate is
 * defined and is not "a " or "an ", that string is the determinate. If
 * there is more than one object with the same plural as that object in its
 * environment, then that string is "one of the "; otherwise the
 * string is "the ". The output from from one_short should not be
 * stored, since the object to which it refers may not later exist; it can be
 * processed with convert_message. One_short is mostly used for
 * referring to a living object when it performs an action independent
 * of any previous actions. 
 * <p>
 * The return of this function needs more processing before the player
 * sees it.  In general you will not need to worry about this.
 * @param flag If this flag is set, do not bring the player out of hiding.
 * @see set_short()
 * @see short()
 * @see a_short()
 * @see the_short()
 * @see poss_short()
 * @see /global/events->convert_message()
 * @example
 * "one of the poor beggars"
 * "the Weasel"
 * "Detritus"
 * "One of the sailors falls over"
 */
varargs string one_short( int flag ) {
   if ( this_object() == this_player() && !flag )
      this_object()->remove_hide_invis( "hiding" );
   return "$one_short:"+ file_name( this_object() ) +"$";
} /* one_short() */

/**
 * This method returns the "possessed" short.  This function returns a
 * string that the message system replaces, when messages are printed, by a
 * string dependent on the possessor of the object and its short as
 * given by pretty_short. If the object has no environment or is not in a
 * living object or corpse, that string is the determinate. If the
 * environment of the object is the viewer of the message, that string
 * is "your". If the determinate is defined and is not "a " or "an ",
 * or, if it is not defined or is "a " or "an " but it is the only thing in its
 * environment with its plural, that string is the possessive of the owner, if
 * the owner has already been mentioned in constructing a sentence, or the
 * owner's the_short plus "'s "; otherwise, the string is "one
 * of the " plus the possessive bit as described in the previous part of
 * this sentence. Got all that? Good :) The string should not be stored,
 * since the object to which it refers may not later exist; it can be
 * processed with convert_message. Poss_short is mostly used for referring
 * to an object when a person has just used it to do something.
 * <p>
 * The return of this function needs more processing before the player
 * sees it.  In general you will not need to worry about this.
 * @param flag If this flag is set, do not bring the player out of hiding.
 * @see set_short()
 * @see short()
 * @see a_short()
 * @see the_short()
 * @see one_short()
 * @see /global/events->convert_message()
 * @example
 * "one of Wombat's knives".
 * "one of his wands".
 * "her Wyrm Sword".
 * "Frenkel twists one of his rings."
 */
varargs string poss_short( int flag ) {
   if ( this_object() == this_player() && !flag )
      this_object()->remove_hide_invis( "hiding" );
   return "$poss_short:"+ file_name( this_object() ) +"$";
} /* poss_short() */

/**
 * Just like poss_short() but returning a definite article.
 * @see poss_short()
 */
varargs string the_poss_short( int flag ) {
   if ( this_object() == this_player() && !flag )
      this_object()->remove_hide_invis( "hiding" );
   return "$the_poss_short:"+ file_name( this_object() ) +"$";
} /* poss_short() */

/**
 * This method returns the "determinate" short.  This function returns a
 * string that the message system replaces, when messages are printed, by the
 * determinate of the object (or "the" if the determinate is "a", "an" or
 * undefined) and its short as given by pretty_short. The string
 * should not be stored, since the object to which it refers may not later
 * exist; it can be processed with convert_message.
 * <p>
 * The return of this function needs more processing before the player
 * sees it.  In general you will not need to worry about this.
 * @param flag If this flag is set, do not bring the player out of hiding.
 * @see set_short()
 * @see short()
 * @see a_short()
 * @see poss_short()
 * @see one_short()
 * @see /global/events->convert_message()
 * @example
 * "the poor beggar"
 * "the Weasel"
 * "Detritus"
 */
varargs string the_short( int flag ) {
/*
   if ( objectp( thing ) ) {
      log_file( "SHORT", "the_short from "+ implode( map_array( previous_object( -1 ) + ({ this_object() }), (: file_name( $1 ) :) ), ", " ) +"\n" );
      return (string)thing->the_short();
   }
*/
   if ( this_object() == this_player() && !flag )
      this_object()->remove_hide_invis( "hiding" );
   return "$the_short:"+ file_name( this_object() ) +"$";
} /* the_short() */
