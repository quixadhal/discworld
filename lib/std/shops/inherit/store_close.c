/**
 * The inherit file for store closings. This should be inherited along with 
 *  /std/shop or /std/item-shop. Or any other room of your choice, really.
 *  @author Rodion
 */

#include <weather.h>
#include <climate.h>
#include <am_time.h>

void open_up();
void close_up();
private string _keeper;
private string *_outside_rooms;
private string *_exits;
private string _burglar;
private string _file;
private int _flag;
private int _open;
private int _closed;
private nosave object _keeper_ob;

/** @ignore */
void event_weather(int whats_changed){
   if(whats_changed >= 8){
      if (_flag) {
         call_out("night_time_check", 1);
      } else {
         call_out("day_time_check", 1);
      }
   }
} /* event_weather() */

/** @ignore */
protected void night_time_check(){
   int bing;

   bing = AM_TIME_HANDLER->query_am_hour(time() );
   if (bing > 12 && bing < 23 && !_closed) {
      _open = 0;
      _closed = 1;
      close_up();
   }
   if (bing > 0 && bing < 12 && !_open) {
      _closed = 0;
      _open = 1;
      open_up();
   }
} /* night_time_check() */

/** @ignore */
protected void day_time_check(){
   int bing;

   bing = AM_TIME_HANDLER->query_am_hour(time() );
   if (bing > 0 && bing < 12 && !_closed) {
      _open = 0;
      _closed = 1;
      close_up();
   } 
   if (bing > 12 && bing < 23 && !_open) {
      _closed = 0;
      _open = 1;
      open_up();
   }
} /* day_time_check() */


/**
 * This function sets up the closing code for stores.  This allows you to 
 *  close and lock up the place, update external rooms, send the shopkeepers 
 *  home, and catch burglars who try to sneak in.
 *  <p>
 *  If the "night" flag is 1, the store will close at night. If it's 0, the 
 * store will close during the day.
 * <p>
 * The shopkeeper name should be the string filename of the shopkeeper. 
 * This can be set to "none" for no shopkeeper (regular 
 * event_person_say stuff will be used to simulate it.)
 * <p>
 * *outside is an array of exit, room path pairs that connect to the shop.  
 * That is, if you moved east to get into the shop, the exit string will be 
 * "east", "/path/of/room/we/got/here/from".  THIS IS REQUIRED, so that 
 * players will be moved to the right place.
 * <p>
 * The burglar_func parameter is the name of a function that will be called 
 * when someone 
 * enters the shop after it is closed.  It will be called with the argument 
 * object ob, where ob is the person who entered.  For no burglar handling, 
 * this argument can be 0.
 *  
 * @param night Whether the store closes during the night or day.
 * @param shopkeeper The file name of the shopkeeper for the store.
 * @param outside The exit, room pairs corresponding to outside rooms.
 * @param burglar_func The function called when store is burgled.
 * @example 
 *    set_night_close(1, "/d/sur/chars/bob", ({"east", PATH+"flibble_room"}), "catch_thief");
 *    ...
 *    set_night_close(0, "none", ({"up", "/d/ram/Snorkle/wibble_room"}));
 */
void set_night_close(int night, string shopkeeper, string *outside, 
    string burglar_func){
   int i;

   _flag = night;
   WEATHER->weather_notify( this_object(), NOTIFY_DAY );
   _keeper = _file = shopkeeper;
   
   //check to see if outside exists.
   if (sizeof(outside) && !(sizeof(outside)%2)) {
      _exits = allocate(sizeof(outside)/2);
      _outside_rooms = allocate(sizeof(outside)/2);
      //split it up so that each room outside has a corresponding
      //exit, in the proper arrays... easier to deal with this way.
      for(i=0; i<sizeof(outside); i++) {
         if(!i%2)
            _outside_rooms[i] = outside[i];
         else
            _exits[i-1] = outside[i];
      }
   }
   _burglar = burglar_func;
} /* set_night_close() */

/**
 * This method closes the shop! It sends the shopkeeper home, changes the 
 * incoming exits to be closed and locked, and moves all players in the store 
 * out.
 */
void close_up(){
   object *obs;
   object *people;
   object bing;
   object *keeper_ob;
   int i;

   if (_keeper == "none") {
     event (this_object(), "person_say", "The shopkeeper says: ", 
       "I'm sorry, it's time to close!", "common");    
     tell_room (this_object(), "The shopkeeper ushers you out of "
          "the store.\n");
   }
   else {
      keeper_ob = children(_keeper);
      if (!sizeof(keeper_ob)) {
         return;
      }
      if (keeper_ob[0]->query_fighting()) {
         call_out("close_up", 60);
         return;
      }
      keeper_ob[0]->do_command("'I'm sorry, it's time to close!");
      keeper_ob[0]->do_command(":ushers you out of the store.");
      keeper_ob[0]->move("/room/rubbish");
   }
   
   obs = all_inventory( this_object() );
   people = filter(obs, (: $1->query_property("player") :));
   
   foreach(bing in people){
      bing->move_with_look(_outside_rooms[0]);
   }
   
   for(i=0; i<sizeof(_outside_rooms); i++){
      _outside_rooms[i]->modify_exit(_exits[i], ({"open", 0, "locked", 1, 
          "difficulty", 4}));
   }
} /* close_up() */

/**
 * This method opens the store, summoning the shopkeeper from the void and
 * unlocking and opening the various exits.
 */
void open_up(){
   int i;

   for(i=0; i<sizeof(_outside_rooms); i++){
     _outside_rooms[i]->modify_exit(_exits[i], ({"open", 1, "locked", 0, 
        "difficulty", 4}));
   }
   if (_keeper == "none") {
      return;
   }
   _keeper_ob = clone_object(_file);
   _keeper_ob->move(this_object());
} /* open_up() */

/** @ignore */
void event_enter(object ob, string message, object from){
   if (!_burglar) {
      return;
   }
   if (!_closed) {
      return;
   }
   if (ob->query_creator()) {
      return;
   }
   if (!ob->query_property("player")) {
      return;
   }
   call_out(_burglar, 1, ob);
} /* event_enter() */

/** @ignore yes */
mapping query_dynamic_auto_load() {
   return ([ "keeper ob" : _keeper_ob ]);
}

/** @ignore yes */
void init_dynamic_arg(mapping map) {
   _keeper_ob = map["keeper ob"];
}
