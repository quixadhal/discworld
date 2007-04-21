/**
 * A small handler to keep track of corpses.
 * @author Pinkfish
 * @started Thu Jun 13 17:22:06 PDT 2002
 */
#include <player_handler.h>

#define CORPSE_SAVE_DIR "/save/corpses/"
#define CORPSE_RAM_DIR "/save/ramdisk/corpses/"
#define CORPSE_SAVE_FILE "/save/corpses/main"
#define CORPSE_OBJECT "/obj/corpse"

private mapping _corpses;
private nosave mapping _to_save;
private nosave int _save_things_id;
private int _corpse_id;

class corpse_data {
   int id;
}

void create() {
   seteuid(getuid());
   _corpses = ([ ]);
   _corpse_id = 1;
   _to_save = ([ ]);
   unguarded( (: restore_object(CORPSE_SAVE_FILE,  1) :));
   call_out("check_corpses", 10);
}

private void save_me() {
   unguarded( (: save_object(CORPSE_SAVE_FILE,  3) :));
}

/**
 * This method returns the corpse data associated with the name and
 * id of the corpse
 * @param name the name of the player
 * @param id the corpse id
 */
private class corpse_data find_corpse_data(string name, int id) {
   class corpse_data corpse;

   if (!_corpses[name]) {
      return 0;
   }
   foreach (corpse in _corpses[name]) {
      if (corpse->id == id) {
         return corpse;
      }
   }
   return 0;
}

/**
 * This method returns the corpse data associated with the specific
 * corpse objecy.
 * @param ob the corpse object to find the data for
 * @return the corpse data, 0 if not found
 */
private class corpse_data find_corpse_ob_data(object ob) {
   string name;
   int id;

   name = ob->query_ownership();
   id = ob->query_corpse_id();
   return find_corpse_data(name, id);
}

/**
 * Finds all of the corpses for the player.
 * @param player the name of the player to find the corpses of
 * @return the list of player corpses
 */
object* find_player_corpses(string player) {
   object* corpses;

   corpses = filter( children( "/obj/corpse" ),
             (: $1 && $1->query_property( "player" ) &&
                $1->query_ownership() == $2 :), player );
   return corpses;
}

private void really_save_corpses() {
  object ob, player;
  class corpse_data corpse;
  string data;
  string fname;
  
  // Zip through the list to find the corpses to save.
  foreach (ob, corpse in _to_save) {
    if(!ob)
      continue;
    
    data = ob->query_save_data();
    if (stringp(data)) {
#ifdef USE_RAMDISK
      fname = CORPSE_RAM_DIR + corpse->id;
#else
      fname = CORPSE_SAVE_DIR + corpse->id;
#endif
      unguarded((: write_file($(fname), $(data), 1) :));
    } else {
      tell_creator("pinkfish", "Bad corpse save %O %O\n", ob, data);
    }
      
    // Save the player when we save the corpse.
    player = find_player(ob->query_owner());

    if(player)
      player->save_me();
  }
  _to_save = ([ ]);
}

/**
 * This method will save the corpse data out to a nice file.
 * @param ob the object to save
 */
void save_corpse(object ob) {
   class corpse_data corpse;

   if (!ob->query_property("player") || !ob->query_ownership()) {
      return ;
   }
   corpse = find_corpse_ob_data(ob);
   if (!corpse) {
      tell_creator("pinkfish", "Cannot find the corpse to save %O [%O].\n",
                   ob, ob->query_ownership());
      return ;
   }
   _to_save[ob] = corpse;
   if (_save_things_id) {
      remove_call_out(_save_things_id);
   }
   _save_things_id = call_out((: really_save_corpses :), 2);
}

/**
 * This method creates the specific corpse.
 * @param name the player name to create it for
 * @param id the id to create
 * @return the new corpse
 */
object create_corpse(string name, int id) {
   class corpse_data corpse;
   string data, fname;
   object ob;
   
   corpse = find_corpse_data(name, id);

#ifdef USE_RAMDISK
   if(unguarded( (: file_size(CORPSE_RAM_DIR + $(corpse->id)) :)) > 0)
     fname = CORPSE_RAM_DIR + corpse->id;
#endif

   if(!fname)
     fname = CORPSE_SAVE_DIR + corpse->id;
   
   data = unguarded( (: read_file($(fname)) :));
   if (data) {
     ob = clone_object(CORPSE_OBJECT);
     ob->setup_corpse_from_save(data, name, id);
     return ob;
   }
   return 0;
}

/**
 * This goes through the current list of corpses and tries to find them,
 * if not it creates them.
 */
void check_corpses() {
   class corpse_data* corpses;
   string name;
   object* obs;
   int i;
   int j;

   //
   // We check each corpse we have in the list with the ones we can
   // find and see if there are any missing ones.
   //
   foreach (name, corpses in _corpses) {
      obs = find_player_corpses(name);
      for (i = 0; i < sizeof(corpses); i++) {
         for (j = 0; j < sizeof(obs); j++) {
            if (obs[j]->query_corpse_id() == corpses[i]->id) {
               corpses = corpses[0..i -1] + corpses[i+1..];
               i--;
            }
         }
      }
      if (sizeof(corpses)) {
         // We need to make some.
         for (i = 0; i < sizeof(corpses); i++) {
            create_corpse(name, corpses[i]->id);
         }
      }
   }
}

/**
 * This method returns the next corpse id.
 * @return the next corpse id
 */
int query_next_corpse_id() {
   int id;

   id = _corpse_id++;
   save_me();
   return id;
}

/**
 * This method registers the corpse with us.
 * @param corpse the corpse to register
 */
void register_corpse(object ob) {
   string name;
   int id;
   class corpse_data corpse;

   tell_creator("Registering %O (%O)\n", ob, ob->query_ownership());
   name = ob->query_ownership();
   if(PLAYER_HANDLER->test_creator(name)) {
     tell_creator("pinkfish", "Not registering creator corpse %O (%O)\n",
                  ob, ob->query_ownership());
     return;
   }
   id = ob->query_corpse_id();
   corpse = find_corpse_ob_data(ob);
   if (corpse) {
      save_corpse(ob);
      return ;
   }

   corpse = new(class corpse_data);
   corpse->id = id;
   if (!_corpses[name]) {
      _corpses[name] = ({ corpse });
   } else {
      _corpses[name] += ({ corpse });
   }
   save_me();
   save_corpse(ob);
}

/**
 * This method deregisters the corpse from us.  We forget about it and
 * it wombles off into the ether.
 * @param corpse the corpse to deregister
 */
void deregister_corpse(object ob) {
   string name;
   int id;
   class corpse_data corpse;
   int i;

   if (!ob->query_property("player")) {
      return ;
   }
   name = ob->query_ownership();
   id = ob->query_corpse_id();

   for (i = 0; i < sizeof(_corpses[name]); i++) {
      corpse = _corpses[name][i];
      // check and see if we already have it.
      if (corpse->id == id) {
         _corpses[name] = _corpses[name][0..i-1] + _corpses[name][i+1..];
         if (!sizeof(_corpses[name])) {
            map_delete(_corpses, name);
         }
         save_me();
         unguarded( (: rm(CORPSE_SAVE_DIR + $(id)) :));
#ifdef USE_RAMDISK
         unguarded( (: rm(CORPSE_RAM_DIR + $(id)) :));
#endif         
         return ;
      }
   }
}

mixed *stats() {
  return ({ ({ "corpses", keys(_corpses) }) });
}
