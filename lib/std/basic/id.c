

/*   -*- LPC -*-   */
/*
 * $Locker:  $
 * $Id: id.c,v 1.18 2002/03/15 21:59:19 tannah Exp $
*/
/**
 * This class keeps track of all the aliases, names and adjectives
 * which are used to match the object.
 * @see /global/player->add_command()
 * @see /std/basic/desc.c
 */
#include <parse_command.h>

inherit "/std/basic/id_match";

class Id {
  string name;
  string *alias;
  string *faux_alias;
  string *unique_faux_alias;
  string *adjectives;
  string *faux_adjectives;
  string *unique_faux_adjectives;
  string *plural_adjectives;
  string *plurals;
}

private nosave class Id _id;

int id_adjective(string);
int id_plural(string str);
int faux_id_allowed();

private void setup_id_class() {
   if (_id) {
      return ;
   }
   _id = new( class Id );
   _id->alias = ({ });
   _id->faux_alias = ({ });
   _id->unique_faux_alias = ({ });
   _id->plurals = ({ });
   _id->adjectives = ({ });
   _id->faux_adjectives = ({ });
   _id->unique_faux_adjectives = ({ });
   _id->plural_adjectives = ({ });
   _id->name = "object";
} /* setup_id_class() */

/* name handling stuff */
/**
 * This method sets the name of the object.   The name is the basic handle
 * used to reference the object.   It should not have any spaces
 * in it.
 * @example
 * set_name("rabbit");
 * @param str the name
 * @see query_name()
 * @see add_alias()
 * @see add_adjective()
 */
void set_name(string str) {
   _id->name = str;
}

/**
 * This method returns the name of the object.
 * @return the name of the object
 * @see set_name()
 * @see add_alias()
 * @see add_adjective()
 */
string query_name() {
   if (!_id) {
      return 0;
   }
   return _id->name;
}

/**
 * This method returns the capitalized name.   It calls the function
 * capitalize() on the name
 * @see query_name()
 * @see set_name()
 * @see efun::capitalize()
 */
string query_cap_name() {
   if (!_id->name) {
      return "Someone";
   }
   return capitalize(_id->name);
}

void create() {
   setup_id_class();
}

/* alias stuff */
/**
 * This method sets the entire alias array.   It overrides any current alias
 * definitions and sets the alias arry.   This should be used carefully
 * if at all.   Use add_alias instead.
 * @param str the new alias array
 */
void set_aliases(string *str) {
   _id->alias = str;
}

/**
 * This method adds an alias to the object.   An alias for the object is the
 * part used as the main noun for the object. ie in the case of a "green
 * bath tub", "bathtub" might be an alias for the object. and "green"
 * and "bath" would be adjectives. The value set with add_alias is also
 * used with present. This means you can make up an alias for an object
 * that players cannot normaly access but you can
 * get a handle on with present. For an exmple of this look at the money
 * object.
 * <p>
 * The function takes an array or a string as an argument.   If an array
 * is passed in each element of the array is added to the alias
 * array, if it is a string then the string elemnt is added to the
 * array.
 * @param str the alias(s) to add
 * @see set_name()
 * @see remove_alias()
 * @see set_aliases()
 * @see query_alias()
 * @example
 * set_name("tub");
 * set_short("green bath tub");
 * add_alias("bathtub");
 * add_adjective("green");
 * add_adjective("bath");
 */
void add_alias(mixed str) {
   if (pointerp(str)) {
      _id->alias += str;
      return;
   }
   if (member_array(str, _id->alias) == -1) {
      _id->alias += ({ str });
   }
}

/**
 * This method removes an alias from the object.
 * @see add_alias()
 * @see query_alias()
 * @param str the alias to remove
 * @return 1 if the alias is found, 0 if not
 */
int remove_alias(string str) {
   int i;

   i = member_array(str, _id->alias);
   if (i == -1) {
      return 0;
   }
   _id->alias = _id->alias[0..i - 1] + _id->alias[i + 1..];
   return 1;
}

/**
 * This method returns the current list of aliases.  If faux aliases are
 * allowed, they are included, unless the optional parameter is passed.
 * @param no_faux 1 to ignore faux aliases (optional)
 * @see add_alias()
 * @see add_faux_alias()
 * @see query_faux_alias()
 * @see query_unique_faux_alias()
 * @see remove_alias()
 * @return the array of aliases (array of strings)
 */
varargs string *query_alias( int no_faux ) {
  if( no_faux ||
      !sizeof( _id->unique_faux_alias ) ||
      !faux_id_allowed() )
    return _id->alias;
  else
    return _id->alias + _id->unique_faux_alias;
}

/**
 * This method adds a faux alias to the object.   Faux aliases are
 * ignored by objects that ignore identifiers.
 * <p>
 * The function takes an array or a string as an argument.   If an array
 * is passed in each element of the array is added to the faux alias
 * array, if it is a string then the string element is added to the
 * array.  Elements that are already real aliases of the object are ignored.
 * Duplication in this list is allowed; a separate list without duplicates
 * is maintained to improve performance.
 * @param str the alias(s) to add
 * @see query_faux_alias()
 * @see query_unique_faux_alias()
 * @see add_alias()
 * @see add_faux_adjective()
 * @see remove_faux_alias()
 * @see ignore_identifiers()
 * @see parse_command_id_list()
 * @example
 */
void add_faux_alias(mixed str) {
   if (pointerp(str)) {
      foreach( string aka in str )
        add_faux_alias( aka );
      return;
   }

   // don't duplicate real aliases
   if( member_array( str, _id->alias ) != -1 )
     return;

   // allowed duplication in the _faux_alias list is intentional
   _id->faux_alias += ({ str });

   // no duplication allowed in the unique list
   _id->unique_faux_alias = _id->unique_faux_alias | ({ str });
}

/**
 * This method removes a faux alias from the object.  Note that, if the
 * alias has been added more than once, only one instance is removed
 * from the array.  If the last instance of an alias is removed from the
 * list, it is also removed from the unique list.
 * @see add_faux_alias()
 * @see add_faux_adjective()
 * @see query_alias()
 * @see query_unique_faux_alias()
 * @param str the faux alias to remove
 * @return 1 if the alias is found, 0 if not
 */
int remove_faux_alias(string str) {
   int i;

   i = member_array(str, _id->faux_alias);
   if (i == -1) {
      return 0;
   }

   // only remove one instance from the faux list
   _id->faux_alias = _id->faux_alias[0..i - 1] +
                      _id->faux_alias[i + 1..];

   // only remove it from the unique list if there are no more instances
   // of it in the faux list.
   if( member_array(str, _id->faux_alias) == -1 ) {
      _id->unique_faux_alias -= ({ str });
   }

   return 1;
}

/**
 * This method returns the current list of faux aliases, with duplicates
 * included.
 * @see add_faux_alias()
 * @see remove_faux_alias()
 * @see query_unique_faux_alias()
 * @return the array of faux aliases (array of strings)
 */
string *query_faux_alias() {
  return _id->faux_alias;
}

/**
 * This method returns the unique list of faux aliases.  It is this list
 * that id() uses.
 * @see query_faux_alias()
 * @see id()
 * @return the array of faux aliases (array of strings) without duplication
 */
string *query_unique_faux_alias() {
  return _id->unique_faux_alias;
}

/**
 * This method is used by the id function.   It determines whether or
 * not an object in the calling list ignores identifiers.
 * If none do, then faux aliases may be used.
 * @return 1 if faux ids may be used
 * @see add_faux_alias
 * @see add_faux_adjective
 * @see ignore_identifiers
 */
int faux_id_allowed() {
  foreach( object ob in previous_object(-1) ) {
    if( objectp(ob) && ob->ignore_identifier() )
      return 0;
  }
  return 1;
}

/**
 * This method is used by the present efun.   It determines if the
 * passed in string is an alias or the name of the object.
 * @param str the value to check
 * @return 1 if the name matches
 */
int id(string str) {
   return (str == _id->name ||
          (member_array(str, query_alias()) != -1) );
}

/** @ignore yes */
int full_id(string str) {
   string *words;
   string name;
   string adjective;

   words = explode(str, " ") - ({ "" });

   name = words[<1];
   words = words[0.. < 2];

   if (!id(name)) {
      if (!id_plural(name)) {
         return 0;
      }
   }
   foreach(adjective in words) {
      if (!id_adjective(adjective)) {
         return 0;
      }
   }
   return 1;
}

/* plural stuff */
/**
 * This method sets the compete array of plurals.   This shod
 * not be used, use add_plural and remove_plural instead.
 * @see add_plural()
 * @see remove_plural()
 * @see query_plurals()
 */
void set_plurals(string *str) {
   _id->plurals = str;
}

/**
 * This method adds a plural onto the object.   The plurals will be
 * used in plural name matching by thje find_match simul_efun
 * @see /secure/simul_efun->find_match()
 * @see set_plurals()
 * @see remove_plural()
 * @see query_plurals()
 * @see add_plurals()
 */
void add_plural(mixed str) {
   if (pointerp(str)) {
      _id->plurals += str;
   } else if (_id->plurals) {
      if (member_array(str, _id->plurals) == -1) {
         _id->plurals += ({ str });
      }
   } else {
      _id->plurals = ({ str });
   }
}

/**
 * This method removes a plural from the object.
 * @param str the plural to remove
 * @see add_plural()
 * @see query_plurals()
 * @see set_plurals()
 */
void remove_plural(string str) {
   int i;

   i = member_array(str, _id->plurals);
   if (i != -1) {
      _id->plurals = _id->plurals[0..i - 1] + _id->plurals[i + 1..];
   }
}

/**
 * This method adds multiple plurals to the object.
 * @param str the array of plurals to add
 * @see add_plural()
 * @see remove_plural()
 * @see query_plurals()
 */
void add_plurals(string *str) {
   _id->plurals += str;
}

/**
 * This method returns the complete list of plurals for the
 * object.
 * @return the complete array of plurals
 */
string *query_plurals() {
   return _id->plurals;
}

/**
 * This method is similar to the id function, except this will check the
 * plural names instead of the aliases and the name.
 * @param str the name to check
 * @return 1 if the name matches one of the plurals, 0 otherwise
 * @see id()
 * @see id_adjective()
 */
int id_plural(string str) {
   return (member_array(str, _id->plurals) != -1);
}

/**
 * This method sets all the adjectives for the object.   This method
 * should not be used, please use add_adjective instead.
 * @see add_adjective()
 * @see remove_adjective()
 * @see query_adjectives()
 * @param str the array of adjectives
 */
void set_adjectives(string *str) {
   _id->adjectives = str;
}

/**
 * This method adds an adjective to the object.   The adjectives are used
 * for the object matching routines.    For example, if you have a "green
 * bath tun" then "green" and "bath" are adjectives.
 * @see add_alias()
 * @see remove_adjective()
 * @see query_adjectives()
 * @see set_adjectives()
 */
void add_adjective(mixed str) {
   int i;

   if (pointerp(str)) {
      for (i = 0; i < sizeof(str); i++) {
         add_adjective(str[i]);
      }
      return;
   }
   if (stringp(str)) {
      str = explode(str, " ");
   }

   for (i = 0; i < sizeof(str); i++) {
      if (member_array(str[i], _id->adjectives) == -1) {
         _id->adjectives += ({ str[i] });
      }
   }
}

/**
 * This method will remove an adjective from the object.
 * @see add_alias()
 * @see add_adjective()
 * @see query_adjectives()
 * @param str the adective(s) to remove
 */
void remove_adjective(mixed str) {
   int i;

   if (pointerp(str)) {
      for (i = 0; i < sizeof(str); i++) {
         remove_adjective(str[i]);
      }
   } else {
      if ((i = member_array(str, _id->adjectives)) != -1) {
         _id->adjectives = delete(_id->adjectives, i, 1);
      }
   }
}



/**
 * This method adds a faux adjective to the object.   Faux adjectives are
 * ignored by objects that ignore identifiers.
 * <p>
 * The function takes an array or a string as an argument.   If an array
 * is passed in each element of the array is added to the faux adjective
 * array, if it is a string then each word in the string element is added
 * to the array.  Elements that are already real adjectives of the object
 * are ignored.
 * @see add_adjective()
 * @see remove_faux_adjective()
 * @see ignore_identifiers()
 */
void add_faux_adjective(mixed str) {

   if (pointerp(str)) {
      foreach( string adj in str )
         add_faux_adjective( adj );
      return;
   }

   if( stringp( str ) ) {
      str = explode(str, " ");
   }

   // don't duplicate real adjectives
   str -= _id->adjectives;

   foreach( string adj in str ) {
      // duplication desired in faux list...
      _id->faux_adjectives += ({ adj });

      // ...but not in unique list.
      _id->unique_faux_adjectives = _id->unique_faux_adjectives |
                                    ({ adj });
   }
}

/**
 * This method will remove a faux adjective from the object.  Note that,
 * if the adjective has been added more than once, only one instance is
 * removed from the array.
 * @see add_faux_alias()
 * @see add_faux_adjective()
 * @see query_adjectives()
 * @param str the adective(s) to remove
 */
void remove_faux_adjective(mixed str) {
   int i;

   if (pointerp(str)) {
      foreach( string adj in str )
         remove_faux_adjective( adj );
      return;
   }

   i = member_array(str, _id->faux_adjectives);
   if( i == -1 )
     return;

   _id->faux_adjectives = _id->faux_adjectives[0..i-1] +
                          _id->faux_adjectives[i+1..];

   // if it's the last instance of this adjective, remove it from the
   // unique list, too.
   if( ( i = member_array( str, _id->faux_adjectives ) ) == -1 )
     _id->unique_faux_adjectives -= ({ str });
}

/**
 * This method returns the current list of faux adjectives, including
 * duplicates.
 * @see add_faux_adjective()
 * @see remove_faux_adjective()
 * @see query_adjectives()
 * @see query_unique_faux_adjectives()
 * @return the array of faux adjectives (array of strings)
 */
string *query_faux_adjectives() {
  return _id->faux_adjectives;
}

/**
 * This method returns a unique list of faux adjectives.
 * @see query_adjectives()
 * @see id_adjective()
 * @see remove_faux_adjective()
 * @return the array of faux adjectives (array of strings)
 */
string *query_unique_faux_adjectives() {
  return _id->unique_faux_adjectives;
}

/**
 * This method returns the current list of adjectives associated with
 * this object.  If faux ids are allowed, then faux adjectives are
 * included if the optional parameter is not 1.
 * @param no_faux 1 to ignore faux adjectives
 * @return the complete list of adjectives
 */
varargs string *query_adjectives( int no_faux ) {
  if( no_faux ||
      !sizeof( _id->unique_faux_adjectives ) ||
      !faux_id_allowed() )
    return _id->adjectives;
  else
    return _id->adjectives + _id->unique_faux_adjectives;
}

/**
 * This method is similar to the id function, except this will check the
 * adjectives instead of the aliases and the name.
 * @param str the adjective to check
 * @return 1 if the name matches one of the adjectives, 0 otherwise
 * @see id()
 * @see id_plural()
 */
int id_adjective(string word) {
   return (member_array(word, query_adjectives()) != -1);
}

/**
 * This method sets all the plural adjectives for the object.   This method
 * should not be used, please use add_plural_adjective instead.
 * @see add_plural_adjective()
 * @see remove_plural_adjective()
 * @see query_plural_adjectives()
 * @param str the array of plural adjectives
 */
void set_plural_adjectives(string *str) {
   _id->plural_adjectives = str;
}

/**
 * This method adds an plural adjective to the object.   The plural
 * adjectives are used for the
 * object matching routines.  Plural adjectives should be used very rarely,
 * they are used in cases where the plural of a name is determined by the
 * adjective rather than it's alias.  For example: pair of shoes, pairs
 * of shoes.
 * @see add_alias()
 * @see remove_plural_adjective()
 * @see query_plural_adjectives()
 * @see set_plural_adjectives()
 */
void add_plural_adjective(mixed str) {
   int i;

   if (pointerp(str)) {
      for (i = 0; i < sizeof(str); i++) {
         add_plural_adjective(str[i]);
      }
      return;
   }
   if (stringp(str)) {
      str = explode(str, " ");
   }
   for (i = 0; i < sizeof(str); i++) {
      if (member_array(str[i], _id->plural_adjectives) == -1) {
         _id->plural_adjectives += ({ str[i] });
      }
   }
}

/**
 * This method will remove an plural adjective from the object.
 * @see add_alias()
 * @see add_plural_adjective()
 * @see query_plural_adjectives()
 * @param str the adective(s) to remove
 */
void remove_plural_adjective(mixed str) {
   int i;

   if (pointerp(str)) {
      for (i = 0; i < sizeof(str); i++) {
         remove_plural_adjective(str[i]);
      }
   } else {
      if ((i = member_array(str, _id->plural_adjectives)) != -1) {
         _id->plural_adjectives = delete(_id->plural_adjectives, i, 1);
      }
   }
}

/**
 * This method returns the current list of plural adjectives associated with
 * this object.
 * @return the complete list of plural adjectives
 */
string *query_plural_adjectives() {
   return _id->plural_adjectives;
}

/**
 * This method is similar to the id function, except this will check the
 * plural adjectives instead of the aliases and the name.
 * @param str the name to check
 * @return 1 if the name matches one of the plurals, 0 otherwise
 * @see id()
 * @see id_plural()
 */
int id_plural_adjective(string word) {
   return (member_array(word, _id->plural_adjectives) != -1);
}

/* ok parse command stuff */
/** @ignore yes */
string *parse_command_id_list() {
   return ({ _id->name, file_name(this_object()) }) + query_alias();
}

/** @ignore yes */
string *parse_command_plural_id_list() {
   return query_plurals();
}

/** @ignore yes */
string *parse_command_adjectiv_id_list() {
   return query_adjectives();
}

/** @ignore yes */
string *parse_command_plural_adjectiv_id_list() {
   return query_plural_adjectives();
}

/** @ignore yes */
object query_parse_id(mixed *arr) {
   if (arr[P_THING] == 0) {
      return this_object();
   }
   if (arr[P_THING] < 0) {      /* specific object case */
      arr[P_THING]++;
      if (arr[P_THING] != 0) {
         return 0;
      }
      arr[P_THING] = -10321;
      return this_object();
   }
   arr[P_THING]--;              /* lots of objects case */
   if (arr[P_THING] != 0) {
      return this_object();
   }
   arr[P_THING] = -10101;
   return this_object();
}

/** @ignore yes */
object query_frac_parse_id(mixed *arr) {
   if (arr[P_THING] < 0) {
      arr[P_THING]++;
      if (arr[P_THING] != 0) {
         return 0;
      }
      arr[P_THING] = -10235;
      return 0;                 /* it refered to me and I am pretty
                                 * depressed about it.
                                 * I want to break free!
                                 */
   }
   if (arr[P_THING] == 0) {
      if ((arr[P_MAX_NUM] * arr[P_TOP]) / arr[P_BOT] > arr[P_CUR_NUM]++) {
         return this_object();
      } else {
         return 0;
      }
   }
   if ((arr[P_THING] * arr[P_TOP]) / arr[P_BOT] > arr[P_CUR_NUM]++) {
      return this_object();
   }
   return 0;
}
