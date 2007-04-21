/**
 * A handler to deal with all the living objects on the disc.  
 * @author Wodan
 * @changed Pinkfish March 16th
 * After wodan didn't add any autodoc comments and made some bits quite
 * inefficent
 */
private nosave mapping _lnames = ([]);
private nosave mapping _players = ([]);

#ifdef 0
void create() {
  object player, *players;

  players = filter(efun::users(), (: $1 && inherits("/std/container", $1) :));
  foreach(player in players) {
    _players[player->query_name()] = player;
  }
}
#endif

/** 
 * This method checks through the living names in the handler and 
 * removes an invalid entries.
 * @param names an array of strings which contains the names of 
 * the creatures to check in the array.
 */
private void check_lnames(string* names) {
   string key;

   foreach (key in names) {
      mixed bing = _lnames[key];
       
      // If the values are an array, then remove any zeros and check
      // to see if we can clean up. 
      if (arrayp(bing)) {
         bing -= ({ 0 }); 

         if (sizeof(bing)) {
            _lnames[key] = bing;
         }
         else { 
            // It's empty, so clean it up. 
            map_delete(_lnames, key);
         }
      }
      else { 
         // We got an undefined or non-array value, so let's delete
         // the key from the mapping. 
         map_delete(_lnames, key);
      }
   }
} /* check_lname() */

/**
 * Same checks as in check_lname(), but for the _players
 * mapping instead.
 * @see check_lnames()
 */
private void check_players(string* names) {
   string key;
   
   foreach (key in names) {
      mixed bing = _players[key];     

      if (arrayp(bing)) {
         bing -= ({ 0 }); 

         if (sizeof(bing)) {
            _players[key] = bing;
         }
         else { 
            map_delete(_players, key);
         }
      }
      else { 
         map_delete(_players, key);
      }
   }
} /* check_lname() */

/**
 * Removes all the not very useful things from the huge mapping.
 */
void remove_garbage(){
  string* names;
  int i;

  names = keys(_lnames);
  for (i = 0; i < sizeof(names); i += 50) {
     call_out((: check_lnames :), i / 25, names[i..i+50]);
  }
  names = keys(_players);
  for (i = 0; i < sizeof(names); i += 50) {
     call_out((: check_players :), i / 25, names[i..i+50]);
  }
}

/**
 * Enables commands on the object.
 * @param ob the object to enable the commands on
 */
void enable_commands(object ob){
}

/**
 * This method registers the living name with the specific object.
 * @param name the name to register
 * @param ob the object to register it with
 */
void set_living_name(string name, object ob){
  if (!stringp(name) || !objectp(ob)) {
    return ;
  }
  if(!_lnames[name]) {
    _lnames[name] = ({ob});
  } else {
    _lnames[name] += ({ob});
  }

  if ( ob == master() ) {
     log_file( "LIVINGS", "%O (%s) became a living. %O\n", ob,
         name, call_stack(0) );
  }
}

/**
 * This method returns the name of all the named livings in the game.
 * Do not use this method except for debugging.
 */
object *named_livings(){
  object *ret;

  ret = keys(_lnames);
  ret = map(ret, (:$1?(_lnames)[$1]:0:));
  reset_eval_cost(); 
  ret = filter(ret, (:$1:));
  reset_eval_cost(); 
  ret = implode(ret, (:$1 + $2:));
  reset_eval_cost(); 
  ret = filter(ret, (:$1:));
  return ret;
}

/**
 * Returns the living object associated with the name.
 * @return the living object
 */
object find_living(string it) {
  if(_lnames[it]) {
    _lnames[it] = filter(_lnames[it], (: $1 :));
    if(sizeof(_lnames[it]))
      return _lnames[it][<1];
  }
  return 0;
}

/**
 * Returns the player associated with the name.
 * @return the player object
 */
object find_player(string it) {
  object *people;
  int t = real_time();
  object *tmp;

  if(_players[it]) {
    // temporarily removed this
    // if(reference_allowed(_players[it]))
      return _players[it];
    return 0;
  }

  //reset_eval_cost();
  people = filter(efun::users(), (: $1 && $1->query_name() == $(it) &&
                                  inherits("/std/container", $1):));
  
  if(sizeof(people)) {
    _players[it] = people[0];
    if(real_time() - t > 1)
      log_file("GARBAGE", "find_player took %d seconds to find %s (1)\n",
               real_time() - t, it);
    // --temporarily removed this.. 
    // if(reference_allowed(_players[it]))
      return _players[it];
  }
  if(!_lnames[it]) {
    if(real_time() - t > 1)
      log_file("GARBAGE", "find_player took %d seconds to find %s (2)\n",
               real_time() - t, it);
    return 0;
  }

  tmp = filter(_lnames[it], (: $1 && userp($1) :));

  if(real_time() - t > 1)
    log_file("GARBAGE", "find_player took %d seconds to find %s (3)\n",
             real_time() - t, it);

  if(sizeof(tmp))
    return tmp[0];
  return 0;
}

/**
 * These two are not here because the handler can be saved, but to keep
 * the info over updates.
 * @ignore yes
 */
mapping query_dynamic_auto_load() {
   mapping tmp;
   tmp = ([ "lnames" : _lnames,
            "players" : _players,
          ]);
   return tmp;
}

/**
 * @ignore yes
 */
void init_dynamic_arg(mapping maps) {
   object ob;
   string name;
   if (maps["lnames"])
      _lnames = maps["lnames"];
   if (maps["players"])
      _players = maps["players"];

   foreach(ob in efun::users()){
      if ( !ob ) {
        continue;
      }
      name = ob->query_name();
      _players[name] = ob;
      if(!_lnames[name]){
        _lnames[name] = ({ob});
      } else {
        _lnames[name] |= ({ob});
      }
   }
}

mixed *stats() {
  return ({ ({ "lnames", sizeof(keys(_lnames)) }),
              ({ "players", sizeof(keys(_players)) })});
}
