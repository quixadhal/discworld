/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: locate.c,v 1.21 2003/07/13 09:13:33 pinkfish Exp $
 * 
*/
inherit "/cmds/base";

#define TP this_player()

/**
 * This method zips down inside the container looking for the match.
 */
object *look_in_container(object fluff,
                          string word) {
   object frog;
   object *obs;
   object *things;

   things = match_objects_for_existence(word, ({ fluff }));
   obs = all_inventory(fluff);
   foreach (frog in obs) {
      if (sizeof(all_inventory(frog))) {
         things += look_in_container(frog, word);
      }
   }

   return things;
} /* look_in_container() */

mixed cmd( string word ) {
   int i;
   int base_num;
   string list;
   object thing;
   object *things;
   object *extra_things;

   debug_printf("%s", word);
   if (environment(TP)) {
      switch(TP->check_dark(environment(TP)->query_light())) {
      case -2:
      case -1:
        if (this_player()->query_creator()) {
          write("It is too dark to locate anything, but since you are a cre...\n");
        } else {
          return notify_fail("It's too dark to locate anything.\n");
        }
        break;
      case 1:
      case 2:
        if (this_player()->query_creator()) {
          write("It is too bright to locate anything, but since you are a cre...\n");
        } else {
           return notify_fail("It's too bright to locate anything.\n");
        }
        break;
      }
   }

   things = match_objects_for_existence( word, ({ environment( TP ) }) );
   things = filter(things, (: environment($1) == environment(TP) ||
                             environment($1) == TP :));
   extra_things = look_in_container(this_player(), word) - things;


   base_num = sizeof(things);
   
   things += extra_things;

   if ( !sizeof( things ) ) {
      return notify_fail( "There is nothing here matching \""+ word +"\".\n" );
   }

   if ( sizeof( things ) == 1 ) {
     if ( things[0] == TP ) {
       write("You are here! :)\n");
       return 1;
     }

     list = "$C$"+ (string)things[ 0 ]->the_short();
     if ( (object)things[ 0 ]->query_wielded() == TP ) {
       list += " (held in " + query_multiple_short(TP->query_holding_limbs(things[0])) + ")";
     }
     if ( (object)things[ 0 ]->query_worn_by() == TP ) {
       list += " (worn)";
     }
     if ( things[ 0 ]->query_keep() ) {
       list += " (keeping)";
     }
     word = (string)things[ 0 ]->query_full_identifier();
     if ( stringp( word ) ) {
       list += " (\""+ word +"\")";
     }
     if (environment(things[0]) == this_player()) {
       list += (things[0]->group_object() ? " are" : " is") +
         " in your inventory.\n";
     } else {
       list += (things[0]->group_object() ? " are in " : " is in ");
       list += inside_the_short(environment( things[ 0 ] )) + ".\n";
     }
     write( list );
     return 1;
   }
   
   list = "";
   foreach ( thing in things ) {
      i++;
      if ( thing == TP ) {
         list += "You (" + i + ") are here :)\n";
         continue;
      }
      if (i > 30) {
         list += "Truncated list!  Cannot list more than 30 items.\n";
         break;
      }
      list += "$C$" + (string)thing->the_short();
      if (i <= base_num) {
         list += " (" + i + ")";
      }
      if ( (object)thing->query_wielded() == TP ) {
         list += " (held in " + query_multiple_short(TP->query_holding_limbs(thing)) + ")";
         //list += " (wielded)";
      }
      if ( (object)thing->query_worn_by() == TP ) {
         list += " (worn)";
      }
      if ( thing->query_keep() ) {
         list += " (keeping)";
      }
      word = (string)thing->query_full_identifier();
      if ( stringp( word ) ) {
         list += " (\""+ word +"\")";
      }
      if (environment(thing) == this_player()) {
         list += (thing->group_object() ? " are " : " is ") +
           "in your inventory.\n";
      } else {
        list += (thing->group_object() ? " are in " : " is in ");
         list += inside_the_short(environment( thing )) + ".\n";
      }
   }
   write( "$P$Locate$P$"+ list );
   return 1;
} /* cmd() */

mixed *query_patterns() {
   return ({ "<string'object name'>", (: cmd($4[0]) :) });
} /* query_patterns() */
