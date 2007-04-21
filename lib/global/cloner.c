/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: cloner.c,v 1.6 2002/03/21 11:23:20 taffyd Exp $
 * $Log: cloner.c,v $
 * Revision 1.6  2002/03/21 11:23:20  taffyd
 * Made it only do one lot of writes at once, so that any objects which add multiple cloner entries at once don't cause lots of disk writes.
 * Also made it use previous_object() in the log_file() if there's no this_player().
 *
 * Revision 1.5  2001/03/24 01:53:24  ceres
 * Added illegal object stuff
 *
 * Revision 1.4  2000/11/01 07:12:44  ceres
 * Oops, that didn't work, try it htis way instead
 *
 * Revision 1.2  1998/02/22 03:48:07  presto
 * No changes
 *
 * Revision 1.1  1998/01/06 04:54:05  ceres
 * Initial revision
 * 
*/
/**
 * Used for mapping deleted files to new ones when things are changed.
 * Updated to a more tidy mechanism by Ceres 14/3/96
 * <p>
 * Usage: call add_mapping(from, to) /global/cloner
 */
#define SAVE_FILE "/save/cloner"
#define SAVE_FILE_DELAY 15
/* How long a mapping lasts, currently 3 months */
#define LIFE_TIME 7257600

inherit "/std/object";

mapping changes;
mapping illegal;

private nosave int _save_file_call_out;

/**
 * This method returns the list of currently moved objects.
 */
mapping query_changes() {
   return changes;
} /* query_changes() */

/**
 * Saves the current configuration.
 */
void save_file() {
   _save_file_call_out = 0; 
   unguarded((: save_object, SAVE_FILE:));
} /* save_file() */

/**
 * Loads the current configuration.
 */
void load_file() {
   if (file_size(SAVE_FILE + ".o") > 0)
      unguarded((: restore_object, SAVE_FILE:));
} /* load_file() */

void setup() {
   changes = ([]);
   illegal = ([]);
   load_file();
   _save_file_call_out = 0; 
} /* setup() */

/**
 * Try and clone the passed in object.  If the name exists in the changes
 * array then the new file name is cloned instead of the old one.
 * @param word the name of the file to clone
 * @return the cloned object
 * @see other_file()
 * @see list_mappings()
 */
object clone( string word ) {
   string new_file;
   object thing;

   if ( changes[ word ] && new_file = changes[ word ][ 0 ] )
      word = new_file;
   catch( thing = clone_object( word ) );
   return thing;
} /* clone() */

/**
 * This method returns the mapped name of the object.  If no mapping
 * exists the passed in value is returned directly.
 * @param word the object name to get a mapping for
 * @return the file to use
 * @see clone()
 * @see list_mappings()
 */
string other_file( string word ) {
   if ( changes[ word ] )
      return changes[ word ][ 0 ];
   return word;
} /* other_file() */

/**
 * Use this function to add a new mapping of one filename to another.
 * Means that if an object of type 'from' is attempted to be cloned
 * an object of type 'to' will be cloned instead.
 * @param from the old object name
 * @param to the new object name
 * @return 1 on success, 0 on failure
 * @see remove_mapping()
 * @see list_mappings()
 */
int add_mapping( string from, string to ) {
  string obj_name; 
  if(from[<2..] == ".c")
    from = from[0..<3];
  if(to[<2..] == ".c")
    to = to[0..<3];
  if((file_size(to) < 1) && (file_size(to+".c") < 1)) {
    write("Destination file does not exist\n");
    return 0;
  }
  changes[from] = ({to, time()});

  if ( !_save_file_call_out ) { 
        _save_file_call_out = call_out( (: save_file :), SAVE_FILE_DELAY ); 
  }

  if ( !this_player() ) {
      obj_name = file_name( previous_object() ); 
  }
  else {
      obj_name = this_player()->query_cap_name();
  }
  write("Mapping of "+from+" to "+to+" added.\n");
  log_file("CLONER", "Mapping of "+from+" to "+to+" added by "+
           obj_name +" at "+ctime(time())+".\n");
  remove_call_out("clean_up");
  call_out("clean_up", 120 + random(500));
  return 1;
} /* add_mapping() */

/**
 * Removes a mapping from the object.  This looks to see if an mapping
 * exists for the object and removes it if it does.
 * @param from the object to remove the mapping from
 * @return 1 on success and 0 on failure
 * @see add_mapping()
 * @see list_mappings()
 */
int remove_mapping( string from ) {
   string obj_name;

   if ( changes && changes[ from ] ) {
      map_delete( changes, from );
      save_file();
      write( "Mapping of " + from + " removed.\n" );
      if ( !this_player() ) {
          obj_name = file_name( previous_object() ); 
      }
      else {
          obj_name = this_player()->query_cap_name();
      }
      log_file( "CLONER", "Mapping of " + from + " removed by " +
          obj_name + " at " + ctime( time() ) + ".\n" );
      return 1;
   }
   write("No mapping found for " + from + ".\n");
   return 0;
} /* remove_mapping() */

/**
 * This method returns a list of all the current mappings.  If a
 * pattern is given, then only files which contain that string will
 * be returned.
 * @param str the pattern to search for
 * @return the list of mappings
 * @see add_mapping()
 * @see remove_mapping()
 */
string list_mappings( string str ) {
   string from, retval;
   retval = "";
   foreach(from in keys(changes)) {
      if (!str ||
          strsrch(from, str) != -1 ||
          strsrch(changes[from][0], str) != -1)
         retval += from + " -> " + changes[from][0] + "\n";
   }
   return retval;
} /* list_mappings() */

/**
 * This method is used to add objects that we don't want to be reloaded.
 * This is for those objects that are clones of /std/object or /obj/armour
 * or something but which are wrongly configured.
 *
 * @example add_illegal("/obj/armour", "blue robe", 
                        "/obj/armours/robe.arm);
 *
 * @param basename the base filename of the object
 * @param short the short name of the object
 * @param replacement the replacement object.
 * @return 1 for success, 0 for failure
 */
int add_illegal(string basename, string short, string replacement) {
  if(!illegal) 
    illegal = ([ ]);
 
  if(illegal[basename + ":" + short])
    return 0;
  
  if((file_size(replacement) < 1) && (file_size(replacement+".c") < 1))
    return 0;

  illegal[basename + ":" + short] = replacement;
  save_file();
  return 1;
}

/**
 * Remove an item from the illegal array
 * @param basename the basename of the object.
 * @param short the shortname of the object.
 * @return 1 for success, 0 for failure
 */
int remove_illegal(string basename, string short) {
  if(!illegal)
    return 0;
  
  if(!illegal[basename+":"+short])
    return 0;
  
  map_delete(illegal, basename+":"+short);
  save_file();
  return 1;
}

/**
 * List the illegal objects.
 */
mapping query_illegal() {
  return illegal;
}

/**
 * Determine if a given object is illegal.
 * @param basename the basename of the object.
 * @param short the shortname of the object.
 * @return 1 for success, 0 for failure
 */
string illegal_thing(string basename, string short) {
  if(!illegal)
    return 0;
  return illegal[basename + ":" + short];
}

/** @ignore yes */
void clean_up() {
   string from;
   mixed  *to;
   
   foreach(from, to in changes)
      if ((to[1] < time() - LIFE_TIME) ||
          ((file_size(to[0]) < 1) && (file_size(to[0] + ".c") < 1)))
         map_delete(changes, from);
   save_file();
} /* clean_up() */
