inherit "/std/room/basic_room";

#include <money.h>

//#define DEBUGGER "presto"

#define MAX_OBJECTS 12

nosave private object *list;
nosave private int reused;

void setup() {
  set_short( "vault room" );
  set_long( "This is the vault for money.\n" );
  set_keep_room_loaded(1);
  list = ({ });
}

string long(string word, int dark) {
  if(query_verb() == "scry" )
    return "empty space.";
  
  return ::long(word, dark);
}

int no_init() { return 1; }

// add to the list of objects.
void add_to_list(object ob) {
  if(sizeof(list) >= MAX_OBJECTS) {
    ob->move("/room/rubbish");

#ifdef DEBUGGER
    if (this_player() == find_player(DEBUGGER))
      tell_creator(DEBUGGER,
                   "list too large... throwing away %O\n", ob);
#endif

    return;
  }

#ifdef DEBUGGER
  if (this_player() == find_player(DEBUGGER))
    tell_creator(DEBUGGER, "Saving %O\n", ob);
#endif

  if (member_array(ob, list) > -1)  {
    log_file("MONEY_RECYCLE", ctime(time()) + "Same object (%O) added twice, this "
             "time by %O. Call stack is %O, %O.  CMD == %s\n",
             ob, previous_object(), call_stack(2), call_stack(3),
             this_player()->query_current_cmd());
    return;
  }

  if (!environment(ob)  ||
      environment(ob)->test_remove(ob, ob->drop(this_object()),
                                   this_object()))
  {
    ob->reset_effects();
    ob->force_no_join();
    ob->move(this_object());
    ob->set_already_joined();
    ob->remove_property("dropped");
    list += ({ ob });
  }
  else
    log_file("MONEY_RECYCLE", ctime(time()) + "Could not move %O from %O (%s) to here\n",
             ob, environment(ob), environment(ob)->short());

#ifdef DEBUGGER
  if (this_player() == find_player(DEBUGGER))
    tell_creator(DEBUGGER, "list is now %O\n", list);
#endif
}

object get_money_ob() {
   object ob;

   // Don't do any recycling coz there's some bad money about.
//   return clone_object(MONEY_OBJECT);
   
   if(sizeof(list)) {
      ob = list[0];
      if(ob) {
#ifdef DEBUGGER
         if (this_player() == find_player(DEBUGGER))
           tell_creator(DEBUGGER, "Re-using %O\n", ob);
#endif
         ob->set_money_array(({ }));
         ob->reset_already_joined();
         ob->reset_no_join();
         reused++;
      }
      else
         log_file("MONEY_RECYCLE", ctime(time()) + "Found NULL entry in object list\n");
      list = list[1 .. ];
#ifdef DEBUGGER
      if (this_player() == find_player(DEBUGGER))
        tell_creator(DEBUGGER, "list is now %O\n", list);
#endif
   }
   if(!ob) {
      ob = clone_object(MONEY_OBJECT);
   }
   
   return ob;
}

object *query_list() { return list; }

mixed stats() {
  return ::stats() + ({
    ({ "list", sizeof(list) }),
      ({ "reused", reused }),
  });
}

int query_theft_command() { return -1; }
