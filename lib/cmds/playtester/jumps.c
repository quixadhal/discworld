/**
 * This is the command to list the jump points and do other wonderful
 * jump point manipulations.
 * @author Pinkfish
 * @started Tue Jul 16 14:03:06 PDT 2002
 */
#include <playtesters.h>

#define LOG_FILE PT_LOGS + "badge_log"
inherit "/cmds/base";

void log_attackers (object);

int do_list() {
   mapping jumps;
   string str;
   string start;
   string tmp;
   string* dest;
   object there;
   object to_there;
   object* theres;

   jumps = PLAYTESTER_HAND->query_jump_points();
   if (!sizeof(jumps)) {
      add_failed_mess("No current jumps.\n");
      return 0;
   }
   str = "Current jumps:\n";
   foreach (start, dest in jumps) {
      if ( !( there = find_object( start ) ) ) {
         catch(load_object(start));
         there = find_object( start );
         if ( !there ) {
           str += sprintf( "$I$3=%O will not load, please contact %s.\n",
                           start, "Someone happy" );
           continue;
         }
      }

      theres = ({ });
      foreach (tmp in dest) {
         if ( !( to_there = find_object( tmp ) ) ) {
            catch(load_object(tmp));
            to_there = find_object( tmp );
            if ( !to_there ) {
              str += sprintf( "$I$3=%O will not load, please contact %s.\n",
                              tmp, "Someone happy" );
              continue;
            }
         }
         theres += ({ to_there });
      }
      str += "$I$6= * From " + there->the_short() + " [" +
             explode(start, "/")[1] + "] you can get to " +
             query_multiple_short(theres, "the") + ".\n";
   }

   write("$P$Jump points$P$" + str);
   return 1;
}

int do_list_from_here() {
   string* dest;
   string here;
   string str;
   string tmp;
   object to_there;
   int pos;

   here = file_name(environment(this_player()));
   dest = PLAYTESTER_HAND->query_jump_destination(here);
   if (sizeof(dest)) {
      str = "";
      foreach (tmp in dest) {
         if ( !( to_there = find_object( tmp ) ) ) {
            catch(load_object(tmp));
            to_there = find_object( tmp );
            if ( !to_there ) {
              str += sprintf( "$I$3=%s will not load, please contact %s.\n",
                              tmp, "Someone happy" );
              return 1;
            }
         }
         str += sprintf("$I$6=   %c: %s\n", pos + 'A', to_there->the_short());
         pos++;
      }

      write("$I$3=From here you can get to:\n" + str);
      return 1;
   }
   add_failed_mess("No destinations from here.\n");
   return 0;
}

int do_jump(string id) {
   string here;
   string* dest;
   string real_dest;
   object to_there;
   int pos;

   if (strlen(id) > 1) {
      add_failed_mess("The id to jump to must be a single letter.\n");
      return 0;
   }
   id = lower_case(id);
   pos = id[0] - 'a';

   here = file_name(environment(this_player()));
   dest = PLAYTESTER_HAND->query_jump_destination(here);
   if (!sizeof(dest)) {
      add_failed_mess("There are no jump points here.\n");
      return 0;
   }

   if (pos < 0 || pos >= sizeof(dest)) {
      add_failed_mess("The id must be in 'A'..'" +
                      sprintf("%c", sizeof(dest) + 'A') + "'.\n");
      return 0;
   }

   real_dest = dest[pos];


   if (real_dest) {
      if ( !( to_there = find_object( real_dest ) ) ) {
         catch(load_object(real_dest));
         to_there = find_object( real_dest );
         if ( !to_there ) {
           write(sprintf( "$I$3=%s will not load, please contact %s.\n",
                           real_dest, "Someone happy" ) );
           return 1;
         }
      }

      catch( log_attackers( this_player() ) );

      write( "Moving you to "+ to_there->the_short() +"...\n" );
      this_player()->move_with_look( to_there,
           "$N appear$s in a gout of green fire.",
           "$N disappear$s in a puff of yellow smoke." );
      return 1;
   }
   add_failed_mess("No destinations from here.\n");
   return 0;
}

int do_armoury() {
   string env;
   string ret;
   object ob;

   env = file_name(environment(this_player()));
   if (environment(this_player())->query_armoury()) {
        ret = this_player()->query_property("pt return");
        if (ret) {
           if (strsrch(ret, "#") != -1) {
              if (!find_object(ret)) {
                 ret = 0;
              }
            } else if (!load_object (ret)) {
              ret = 0;
            }
        }
        if (!ret) {
           // Eeek.
           ret = this_player()->query_start_pos();
        }
      write( "Moving you from the pt armoury.\n");
      // Return.
      this_player()->move_with_look( ret,
           "$N appear$s in a gout of green fire.",
           "$N disappear$s in a puff of yellow smoke." );
   } else {
      catch( log_attackers( this_player() ) );
      write( "Moving you to the pt armoury.\n");    
      if (explode (env,"/")[1] != "playtesters") {
        this_player()->add_property("pt return", env);
      }
      this_player()->move_with_look( PLAYTESTER_ARMOURY,
           "$N appear$s in a gout of green fire.",
           "$N disappear$s in a puff of yellow smoke." );
   }
   return 1;
}

protected void log_attackers( object o ) {
    string s;
    object* obs;
    object e;
    
    if ( sizeof( obs = o->query_attacker_list() ) > 0 ) {
        tell_object( o, "%^BOLD%^%^WHITE%^Please stop combat before using "
                     "the jumps command to go places.%^RESET%^\n" );
        
        e = environment( o );
        obs -= ({ 0 });
        obs = filter( obs, (: environment( $1 ) == $(e) :) );

        if ( sizeof( obs ) > 0 ) {
            s = ctime( time() ) + " - " + o->query_name() + " tried to "
                "teleport out of combat from " +
                ( environment( o ) ? file_name( environment( o ) ) :
                  "NULLSPACE" ) + "; query_attacker_list() == ({ ";

            foreach ( object ob in o->query_attacker_list() ) {
                s += "\"" + ob->query_short() + "\" [" +
                    file_name( ob ) + "]";

                if ( environment( ob ) != environment( o ) )
                    s += " (not in same room: " +
                        ( environment( ob ) ?
                          file_name( environment( ob ) ) :
                          "NULLSPACE" ) + ")";
                else
                    s += " (IN SAME ROOM)";
                
                s += ", ";
            }
            
            s += "});\n\n";
            log_file( LOG_FILE, s );
            // unguarded( (: write_file( LOG_FILE, $(s) ) :) );
        }
    }
} /* log_attackers() */

mixed* query_patterns() {
   return ({ "list", (: do_list :),
             "list here", (: do_list_from_here :),
             "goto <string'id'>", (: do_jump($4[0]) :),
             "armoury", (: do_armoury :) });
}
