/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: philosophies.c,v 1.6 2000/03/21 02:43:56 ceres Exp $
 * $Log: philosophies.c,v $
 * Revision 1.6  2000/03/21 02:43:56  ceres
 * Fixed bug that meant you could only learn one argument!
 *
 * Revision 1.5  1999/09/18 14:56:31  ringo
 * Think I've fixed the duplication problem now.
 * Will not display dupes from query_all_known and will remove any dupes when add_known or remove_known are called.
 * remove_known() and reset_known() added for Lords
 *
 * Revision 1.4  1999/09/12 19:30:45  ringo
 * Added overwrite flag to write_file in add_known()
 * Also added basic dupe checking in query_all_known
 * Still needs major re-work to fix dupes
 *
 * Revision 1.3  1998/09/05 19:30:39  ceres
 * Moved the files into subdirectories.
 *
 * Revision 1.2  1998/05/11 08:04:46  pinkfish
 * Add in documentation.
 *
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
 */
/**
 * This handler contains all the information on the philosophies used
 * in philosphical debates.
 * @author Deutha
 */

#define SAVE_FILE "/save/philosophies"
#define SAVE_DIR "/save/philosophies/"

#define PHILOSOPHY_TYPE 0
#define PHILOSOPHY_BONUS 1
#define PHILOSOPHY_NEEDED 2
#define PHILOSOPHY_EFFECT 3
#define PHILOSOPHY_ARGUMENTS 4

nosave string current_person, current_philosophies;
mapping philosophies;

/*
 * Philosophies mapping has elements of the form:
 *    name : ({
 *       [string] type (i.e. type.points is used),
 *       [int] bonus (i.e. bonus in type.points that is needed),
 *       [string] what is needed/increased to start,
 *       [string] debate effect file name,
 *        [mapping] arguments
 *     })
 *
 *  All of these except for the arguments are set when the
 *  philosophy is added; the arguments are added once the
 *  philosophy exists.
 */
void create() {
  seteuid( (string)"/secure/master"->
           creator_file( file_name( this_object() ) ) );
   philosophies = ([ ]);
   if ( file_size( SAVE_FILE +".o" ) > 0 )
      unguarded( (: restore_object, SAVE_FILE :) );
} /* create() */

/**
 * This method saves the current state of the object.
 */
void save_me() { unguarded( (: save_object, SAVE_FILE :) ); }

/**
 * This method returns the current set of philosphies available.
 * The mapping consists of the value being the name of the philosphy
 * and the value being an array consisting of 5 elements.  You should
 * be using this call for anything except debugging purposes.  It
 * would create a bad dependancy otherwise.
 * <pre>
 * name : ({
 *       [string]  type (i.e. type.points is used),
 *       [int]     bonus (i.e. bonus in type.points that is needed),
 *       [string]  what is needed/increased to start,
 *       [string]  debate effect file name,
 *       [mapping] arguments
 *     })
 * </pre>
 * @return a mapping of values
 * @see query_philosophy_names()
 */
mapping query_philosophies() { return philosophies; }

/**
 * This method returns the names of all the current philosophies.
 * @return an array of strings of the names of philosophies
 * @see query_philosophy()
 * @see add_philosophy()
 * @see remove_philosophy()
 */
string *query_philosophy_names() { return keys( philosophies ); }

/**
 * This method determines if the named philosphy exists.
 * @return 1 if it exists, 0 if it does not
 * @param name the name of the philosphy to test
 * @see query_philosophy_names()
 * @see add_philosophy()
 * @see remove_philosophy()
 */
int query_philosophy( string name ) {
   if ( !philosophies[ name ] ) {
      return 0;
   }
   return 1;
} /* query_philosophy() */

/**
 * This method adds a philosphy into the current list of philosphies.
 * @param name the name of the philosophy
 * @param type the type of the philosophy
 * @param bonus the bonus of the philosophy
 * @param needed the string describing what you need
 * @param effect the effect of the philosphy
 * @example
 * add_philosophy("theology", "faith", 50, "the power of your faith",
 *                "/std/effects/religious/theological_debate");
 * @see query_philosophy()
 * @see query_philosophy_names()
 * @see remove_philosophy()
 */
void add_philosophy( string name, string type, int bonus, string needed,
      string effect ) {
   philosophies[ name ] = ({ type, bonus, needed, effect, ([ ]) });
   save_me();
} /* add_philosophy() */

/**
 * This method removes the specified philosophy from the current list.
 * @param name the name of the philosphy to remove
 * @see query_philosophy()
 * @see query_philosophy_names()
 * @see add_philosophy() 
 */
void remove_philosophy( string name ) {
   if ( !philosophies[ name ] ) {
      return;
   }
   map_delete( philosophies, name );
   save_me();
} /* remove_philosophy() */

/**
 * This method returns the type of the philosophy.  An example of the
 * return value from this is "faith".
 * @param name the name of the philosophy to get the type of
 * @return the type of the philosophy
 * @see query_philosphy_bonus()
 * @see query_philosphy_effect()
 * @see query_philosphy_needed()
 * @see query_philosphy_argument()
 */
string query_philosophy_type( string name ) {
   if ( !philosophies[ name ] ) {
      return 0;
   }
   return philosophies[ name ][ PHILOSOPHY_TYPE ];
} /* query_philosophy_type() */

/**
 * This method returns the bonus of the philosophy.  An example of the
 * return value from this is 50.
 * @param name the name of the philosophy to get the bonus of
 * @return the bonus of the philosophy
 * @see query_philosphy_type()
 * @see query_philosphy_effect()
 * @see query_philosphy_needed()
 * @see query_philosphy_argument()
 */
int query_philosophy_bonus( string name ) {
   if ( !philosophies[ name ] ) {
      return 0;
   }
   return philosophies[ name ][ PHILOSOPHY_BONUS ];
} /* query_philosophy_bonus() */

/**
 * This method returns the needed string of the philosophy.  An example of the
 * return value from this is "the power of your faith".
 * @param name the name of the philosophy to get the needes string for
 * @return the needed string for the philosophy
 * @see query_philosphy_bonus()
 * @see query_philosphy_effect()
 * @see query_philosphy_type()
 * @see query_philosphy_argument()
 */
string query_philosophy_needed( string name ) {
   if ( !philosophies[ name ] ) {
      return 0;
   }
   return philosophies[ name ][ PHILOSOPHY_NEEDED ];
} /* query_philosophy_needed() */

/**
 * This method returns the effect of the philosophy.  An example of the
 * return value from this is "/std/effects/religious/theological_debate".
 * @param name the name of the philosophy to get the effect of
 * @return the effect of the philosophy
 * @see query_philosphy_bonus()
 * @see query_philosphy_type()
 * @see query_philosphy_needed()
 * @see query_philosphy_argument()
 */
string query_philosophy_effect( string name ) {
   if ( !philosophies[ name ] ) {
      return 0;
   }
   return philosophies[ name ][ PHILOSOPHY_EFFECT ];
} /* query_philosophy_effect() */

/**
 * This method returns the arguments of the philosophy.  This returns
 * a mapping containing the argument values.  This should not be
 * used except for debuggin purposes.
 * @param name the name of the philosophy to get the arguments for
 * @return the arguments for the philosophy
 * @see query_philosphy_bonus()
 * @see query_philosphy_type()
 * @see query_philosphy_needed()
 * @see query_philosphy_effect()
 * @see query_philosphy_argument()
 */
mapping query_philosophy_all_arguments( string name ) {
   if ( !philosophies[ name ] ) {
      return 0;
   }
   return copy( philosophies[ name ][ PHILOSOPHY_ARGUMENTS ] );
} /* query_philosophy_arguments() */

/**
 * This method returns the values for the specific argument in
 * philosophy.   The returned array consists of three values, they
 * are ({ skill, something, something }).
 * @param name the name of the philosphy to get the arguments for
 * @param argument the name of the argument to return
 * @return an array of the arguments values
 * @see query_philosphy_bonus()
 * @see query_philosphy_type()
 * @see query_philosphy_needed()
 * @see query_philosphy_effect()
 * @see add_philosphy_argument()
 * @see remove_philosphy_argument()
 */
mixed query_philosophy_argument( string name, string argument ) {
   if ( !philosophies[ name ] ) {
      return 0;
   }
   return philosophies[ name ][ PHILOSOPHY_ARGUMENTS ][ argument ];
} /* query_philosophy_argument() */

/**
 * This method adds in a philosophy argument to the specified philosophy.
 * @param name the name of the philosophy to add
 * @param argument the name of the argument to add
 * @param args the parameters for the argument
 * @see query_philosophy_argument()
 * @see remove_philosophy_argument()
 */
void add_philosophy_argument( string name, string argument, mixed args ) {
   if ( !philosophies[ name ] ) {
      return;
   }
   if ( !undefinedp( philosophies[ name ][ PHILOSOPHY_ARGUMENTS ][ argument ] ) ) {
      return;
   }
   philosophies[ name ][ PHILOSOPHY_ARGUMENTS ][ argument ] = args;
   save_me();
} /* add_philosophy_argument() */

/**
 * This method removes a specified argument from the philosophy.
 * @param name the name of the philosphy to remove the argument form
 * @param argument the argument name to remove
 * @see add_philosophy_argument()
 * @see query_philosoph_argument()
 */
void remove_philosophy_argument( string name, string argument ) {
   if ( !philosophies[ name ] ) {
      return;
   }
   if ( undefinedp( philosophies[ name ][ PHILOSOPHY_ARGUMENTS ][ argument ] ) ) {
      return;
   }
   map_delete( philosophies[ name ][ PHILOSOPHY_ARGUMENTS ], argument );
   save_me();
} /* remove_philosophy_argument() */

/*
 * A person will not forget an argument (unless they refresh, in which
 *    case all of them are forgotten, i.e. the file is wiped), so let
 *    a person learn to use an argument by adding a new line to their
 *     file consisting of its name and to see if they know an argument,
 *     check in their file for its name. This method doesn't require
 *     a library-type handler.
 */

/**
 * This method returns all the arguments known to the person.
 * @param person the person to get the arguments for
 * @return the list of all the known arguments
 * @see query_known()
 * @see add_known()
 */
string *query_all_known( string person ) {
   string *oldstuff, *newstuff = ({}), key;

  if ( person != current_person ) {  
     if(unguarded((: file_length, SAVE_DIR + person[0..0]+"/"+person :)) < 1) {
       current_philosophies = "";
     } else {
       current_philosophies = unguarded((:read_file,
         SAVE_DIR+person[0..0]+"/"+person:));
     }
  }

  /* Get rid of duplications */
  oldstuff = explode(current_philosophies, "\n");
  foreach ( key in oldstuff ) {
     if ( member_array( key, newstuff ) == -1 ) {
          newstuff += ({ key });
      }
  }
  current_person = person;
  return newstuff;
} /* query_all_known() */
 
/**
 * This method checks to see if the person knows the argument.
 * @param person the person to check the argument for
 * @param argument the argument to check to see if it is known
 * @return 1 if it is known, 0 if not.
 * @see add_known()
 * @see query_all_known()
 */
int query_known( string person, string argument ) {
  if(person != current_person) {
    current_philosophies = "";
    if( unguarded( (: file_length,
		    SAVE_DIR + person[0..0]+"/"+person :) ) < 1 ) {
      current_person = person;
      return 0;
    }
    current_person = person;
    current_philosophies =
      unguarded( (: read_file, SAVE_DIR +  person[0..0]+"/"+person :) );
  }

  return (member_array(argument,explode(current_philosophies ,"\n")) != -1);
} /* query_known() */


/**
 * This method adds an argument to the specified person.
 * @param person the person to add the argument to
 * @param argument the argument to add to the person
 * @see query_known()
 * @see query_all_known()
 */
void add_known( string person, string argument ) {
   string *oldstuff, *newstuff = ({}), key;

   if ( !query_known( person, argument ) ) {
      if( !current_philosophies ) {
         current_philosophies = "";
      }
      current_philosophies += argument +"\n";
   }

   current_person = person;

   /* Get rid of duplications */
   oldstuff = explode(current_philosophies, "\n");
   foreach ( key in oldstuff ) {
     if ( member_array( key, newstuff ) == -1 ) {
       newstuff += ({ key });
     }
   }

   unguarded( (: write_file, SAVE_DIR +  person[0..0]+"/"+person,
      current_philosophies, 1 :));
} /* add_known() */


/**
 * This method removes an argument from the specified person.
 * @param person the person to remove the argument from
 * @param argument the argument to remove from the person
 * @see query_known()
 * @see query_all_known()
 */
int remove_known( string person, string argument ) {
   string *oldstuff, *newstuff = ({}), key;

   if ( !this_player()->query_lord() &&
      this_player()->query_name() != "ringo" ) {
      /* And me, coz I need to test it */
      tell_object(this_player(), "Lords only...\n");
      return 1;
   }
   oldstuff = query_all_known( person );
   current_person = person;
   current_philosophies = "";

   /* Get rid of duplications */
   foreach ( key in oldstuff ) {
      if ( member_array( key, newstuff ) == -1 &&
        key != argument ) {
          newstuff += ({ key });
          current_philosophies += sprintf( "%s\n", key );
       }
   }
   unguarded( (: write_file, SAVE_DIR +  person[0..0]+"/"+person,
      current_philosophies, 1 :));
} /* remove_known() */


/**
 * This method resets the argument list for the specified person.
 * @param person the person to remove the argument from
 * @see query_known()
 * @see query_all_known()
 */
int reset_known( string person ) {
   if ( !this_player()->query_lord() &&
      this_player()->query_name() != "ringo" ) {
      /* And me, coz I need to test it */
      tell_object(this_player(), "Lords only...\n");
      return 1;
   }
   current_person = person;
   current_philosophies = "";
   unguarded( (: write_file, SAVE_DIR +  person[0..0]+"/"+person,
      current_philosophies, 1 :));
} /* remove_known() */
