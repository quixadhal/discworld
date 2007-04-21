/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: shovel.c,v 1.4 1998/03/25 12:39:18 pinkfish Exp $
 * $Log: shovel.c,v $
 * Revision 1.4  1998/03/25 12:39:18  pinkfish
 * Fixe dup the index in a sepaerate thingy problem.
 *
 * Revision 1.3  1998/03/25 04:59:42  jeremy
 * More autodoc updating.
 *
 * Revision 1.1  1998/03/24 07:20:53  pinkfish
 * Initial revision
 *
 * 
 */
/**
 * This is a clonable shovel object (a "shovel" in this case being anything
 * that can be used for digging, from a garden trowel to a steam shovel).
 * The "dig" command is defined with the following syntax:
 * <ul>
 * <li> "with <direct:object> in <string>"
 * <li> "with <direct:object> in <indirect:object>"
 * <li> "with <direct:object>"
 * <li> "[in] <string> with <direct:object>"
 * <li> "[in] <indirect:object> with <direct:object>"
 * </ul><br>
 * The do_dig() function tries to call a function dig() on this_player()'s
 * environment, which should be defined as
 * <p><code>
 * int dig( object indir, string indir_s )
 * </code><p>
 * where
 * <ul>
 * <li> <i>indir</i> is the <indirect:object> matched (if any)
 * <li> <i>indir_s</i> is the <string> matched, or the string that
 *      matched <indirect:object>
 * </ul><br>
 * Note that the command can match either actual objects or just strings;
 * also note that, although it will match room items, the object passed
 * won't really be of much use (which is why the string is passed also).
 *
 * @index dig
 */

inherit "/obj/weapon";

string *patterns = ({
  "with <direct:object> in <string>",
  "with <direct:object> in <indirect:object>",
  "with <direct:object>",
  "[in] <string> with <direct:object>",
  "[in] <indirect:object> with <direct:object>"
});

/**
 * @ignore yes
 */
void init(){
  this_player()->add_command("dig", this_object(), patterns);
} /* init() */

/**
 * @ignore yes
 */
int do_dig( object *things, string dir_match, string
            indir_match, mixed *args, string pattern) {
  object *obj, indir;
  string indir_s;

  if (!objectp(environment(this_player()))) {
    return 0;
  }
  obj=this_player()->query_holding();
  if(member_array(this_object(), obj) == -1){
    this_player()->add_failed_mess(this_object(), "You must hold the $D to "
                                   "dig with it.\n", ({}));
    return 0;
  }
  if (pattern == patterns[4]) {
    if (!objectp(indir = things[0])) {
      return 0;
    }
    indir_s = args[0];
  } else if (pattern == patterns[3]) {
    indir_s = args[0];
    indir = 0;
  } else if (pattern == patterns[2]) {
    indir_s = 0;
    indir = 0;
  } else if (pattern == patterns[1]) {
    if (!objectp(indir = things[0])) {
      return 0;
    }
    indir_s = args[1];
  } else if (pattern == patterns[0]) {
    indir_s = args[1];
    indir = 0;
  } else {
    return 0;
  }
  // This is causing multiple failure messages, so I'll take it out until
  // the problem is sorted out.
  //this_player()->add_failed_mess(this_object(), "This doesn't appear to "
  //                               "be a good place for digging.\n", ({ }));
  return environment(this_player())->dig(indir, indir_s);
} /* do_dig() */

/** @ignore yes */
mapping query_static_auto_load() {
   if ( explode( file_name( this_object() ), "#" )[ 0 ] == "/obj/shovel" )
      return int_query_static_auto_load();
   return ([ ]);
} /* query_static_auto_load() */
