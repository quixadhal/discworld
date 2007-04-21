/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: salve.c,v 1.4 2001/07/07 16:40:40 tannah Exp $
 * $Log: salve.c,v $
 * Revision 1.4  2001/07/07 16:40:40  tannah
 * removed the ([]) argument passed to int_query_static_auto_load() in
 * query_static_auto_load() so that it would compile.
 *
 * Revision 1.3  2001/03/18 17:40:02  tannah
 * changed %D to <direct:object>, %I to <indirect:object>.
 *
 * Revision 1.2  2000/07/14 22:08:51  pinkfish
 * Fix up to not use global variables from inherits
 *
 * Revision 1.1  1998/01/06 04:58:17  ceres
 * Initial revision
 * 
*/
#include <drinks.h>

inherit "/obj/food";

mapping apply_effects;

void create() {
  do_setup++;
  ::create();
  do_setup--;
  apply_effects = ([ ]);
  if ( !do_setup )
    this_object()->setup();
} /* create() */

void init() {
  this_player()->add_command( "apply", this_object(), "<direct:object> to <indirect:object>");
} /* init() */

mapping query_apply_effects() { return apply_effects; }
void set_apply_effects( mapping map ) { apply_effects = map; }

int add_apply_effect( string word, int number ) {
  if ( apply_effects[ word ] )
    apply_effects[ word ] += number;
  else
    apply_effects[ word ] = number;
  return apply_effects[ word ];
} /* add_apply_effect() */

void remove_apply_effect( string word ) {
  if ( !undefinedp( apply_effects[ word ] ) )
    apply_effects = m_delete( apply_effects, word );
} /* remove_apply_effect() */

void being_joined_by( object thing ) {
  int i, that, this;
  string *words;
  mapping new_effects;
  that = (int)thing->query_amount();
  this = query_amount();
  if ( !( this + that ) )
    return;
  new_effects = (mapping)thing->query_apply_effects();
/*
 * If new_effects isn't a mapping, then there's something wrong, so it
 *     should give an error so that someone knows and can get it fixed.
 */
  words = m_indices( apply_effects );
  for ( i = 0; i < sizeof( words ); i++ )
    if ( new_effects[ words[ i ] ] ) {
      new_effects[ words[ i ] ] *= that;
      new_effects[ words[ i ] ] += this * apply_effects[ words[ i ] ];
    } else
      new_effects[ words[ i ] ] = this * apply_effects[ words[ i ] ];
  words = m_indices( new_effects );
  for ( i = 0; i < sizeof( words ); i++ )
    new_effects[ words[ i ] ] /= this + that;
  apply_effects = new_effects;
} /* being_joined_by() */

int do_apply(object *indir, string dir_s, string indir_s, mixed *args) {
    string *words;
    int i;

    if (environment(this_object()) != this_player()) {
        write("You aren't holding the " + dir_s + ".\n");
        return 0;
    }
    if (sizeof(indir) != 1) {
        // Maybe I'll let it do multiple items some day...
        write("You can only " + query_verb() + " to one thing at a time.\n");
        return 0;
    }
    if (living(indir[0])) {
        words = m_indices( apply_effects );
        for (i = 0; i < sizeof(words); i++) {
            indir[0]->add_effect(words[i], apply_effects[words[i]] * query_amount());
        }
    }
    this_player()->add_succeeded(indir[0]);
    move("/room/rubbish");
    return 1;
}

mapping query_static_auto_load() {
  if ( ( explode( file_name( this_object() ), "#" )[ 0 ] != "/obj/salve" )
      && !query_continuous() )
    return 0;
return int_query_static_auto_load();
} /* query_static_auto_load() */

mapping query_dynamic_auto_load() {
  return ([
    "::" : ::query_dynamic_auto_load(),
    "apply_effects" : apply_effects + ([ ]),
  ]);
} /* query_dynamic_auto_load() */

void init_dynamic_arg( mapping map ) {
  if ( map[ "::" ] )
    ::init_dynamic_arg( map[ "::" ] );
  apply_effects = map[ "apply_effects" ];
} /* init_dynamic_arg() */

mixed *stats() {
  int i;
  string *words;
  mixed *args;
  args = ::stats();
  words = m_indices( apply_effects );
  for ( i = 0; i < sizeof( words ); i++ )
    args += ({ ({ "apply effect#"+ i, words[ i ] +" ("+
        apply_effects[ words[ i ] ] +")" }) });
  return args;
} /* stats() */
