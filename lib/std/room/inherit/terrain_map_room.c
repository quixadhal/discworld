/**
 * The overall room inherit for a 'mapped area'
 * all mapped area rooms should inherit from this.
 * <p>
 * MUST OVERRIDE:
 * <p>
 * query_map_file()
 * @author  Dek
 * @started March 2001
 */
#include <room.h>
#include <login_handler.h>

private int     _x_coord = -1;
private int     _y_coord = -1;
private int     _installed = 0;
private string  _extra_long = "";
private object  _map_handler;
private mapping _features = ([ ]);
private string  _outside_types;

string query_feature_item_desc(string title);
int    do_gocoords(int x, int y);
void add_extra_look(object ob);
void setup_room_chat();
object query_chatter();
varargs void room_chat(mixed* args, object chatob);
void add_item(mixed type, mixed desc);
void remove_item(string name);

/**
 * This method returns the co-ordinates of this room.
 */
mixed *query_coords() { return ({ _x_coord, _y_coord }); }


// We set the coords to -1 -1 here, to account for rooms that are
// not virtual. Rooms that adjoin map areas need to inherit this
// file, hence the need to do this.
void create()

{
   _x_coord = -1;
   _y_coord = -1;

}

/**
 * This needs to be called by the upper inherit to set us to womble.
 */
void setup_room() {
   add_extra_look(this_object());
   setup_room_chat();
} /* setup_room() */

/**
 * Setup the main coordinates for this room. Please note that these
 * are not DW coords, but local ones for the use of the handler. 
 */
void set_coords(int x, int y)

{
   _x_coord = x;
   _y_coord = y;
}


void init()

{
   tell_creator(this_player(), "Coords: (%d, %d)\n", _x_coord, _y_coord);
   if (this_player()->query_creator()) {
      add_command("gocoords", "<number'x'> <number'y'>", (: do_gocoords($4[0], $4[1]) :));
   }
}


/**
 * This method returns the map handler associated with this room.
 */
object query_map_handler()

{
   return _map_handler;
}


/**
 * This is a command used by creators to go to specific locations in the grid.
 */
int    do_gocoords(int x, int y)

{
   object   room = query_map_handler()->load_room_at(x, y);
   string   msgin = this_player()->query_mmsgin();
   string   msgout = this_player()->query_mmsgout();
   
   
   if (!room) {
     this_player()->add_failed_mess(this_object(), 
                           "No room at those coordinates.\n", ({ }));
     return 0;
   }

   msgin = replace(msgin, ({ "$N", this_player()->a_short() }) );
   msgout = replace(msgout, ({ "$N", this_player()->a_short() }) );
   if (!this_player()->move_with_look(room, msgin, msgout))
      add_succeeded_mess("Move attempt failed.\n");
   
   return 1;
}
   

/**
 * Added for convenience. BEWARE! When called, this will dest every single 
 * room in this terrain, dumping everything in the void. When I think of
 * a decent way to handle this, I will.
 */
void reset_handler()

{
   if (query_map_handler())
      query_map_handler()->reset_handler();
}


/**
 * Receive messages from the handler.
 * @Param mess_class the class of the message
 * @param message the actual message
 */
void receive_message(string mess_class, string message)

{
   string   *bits = explode(mess_class, ":");
   mapping  feature;
   int      range = -1;
   
   if (bits[0] == "feature") {
      feature = _features[bits[1]];
      if (!feature)
         return;
      
      if (sizeof(bits) >= 3)
         range = to_int(bits[2]);
         
         if ((range >= 0) && (range != feature["range"]))
            return;
      message = replace(message, ({ "$D", feature["direc"] }));
   }
   tell_creator("dek", "receive_message\n");
   tell_room(this_object(), message);
}


/**
 * This makes one of the features in the terrain send us a chat.
 */
void do_a_feature_chat()

{
   string   *chats = ({ });
   string   feature_chat;
   string   title;
   mapping  feature;
   object   handler = query_map_handler();
   
   foreach(title, feature in _features) {
      feature_chat = handler->get_a_feature_chat(title, feature["range"], 
                                                        feature["direc"]);
      if (feature_chat) {
         chats += ({ feature_chat });
      }
   }
   if (sizeof(chats)) {
      tell_room(this_object(), chats[random(sizeof(chats))] + "\n");
   }
}

/**
 * This method will print an outside chat.
 */ 
void do_an_outside_chat()

{
   string chat = query_map_handler()->get_an_outside_chat(_outside_types);

   if (chat && strlen(chat))
      tell_room(this_object(), chat + "\n");
}


/**
 * This sets the map handler, OBSOLETE. This should be done by overriding query_map_handler()
 * in the main inherited file. It doesn't get set fast enough otherwise.
 * @param handler the new handler
 */
void set_map_handler(mixed handler)

{
   if (objectp(handler))
      _map_handler = handler;
   else
      _map_handler = load_object(handler);
}


/**
 * Checks to see if the feature can be viewed or not.
 * @param type the type of the feature
 * @return 1 if it can be seen
 */
int can_view_feature(string type)

{
   return 1;
}


/**
 * Can we go to rooms of type x? Override if you want to use it - 
 * 1 means yes.
 * @param type the type of room to check
 */
int can_exit_to(string type)

{
   return 1;
}


/**
 * Can we enter from rooms of type x? Override if you want to use it - 
 * 1 means yes.
 * @param type the type of room to check
 */
int can_enter_from(string type)

{
   return 1;
}


/**
 * Do we *want* outside and feature chats? Override if you want to use 
 * 1 means yes.
 */
int do_outside_chats()

{
   return 1;
}


/**
 * Setup the feature chats and basic room chats.
 */
varargs void setup_room_chat(mixed *args, object chatobj)

{
   if (do_outside_chats() && !query_chatter()) {
      room_chat(({ 60, 120, ({ "#do_a_feature_chat",
                                  "#do_an_outside_chat" }) }) );
   }

   // For some reason, the chatting won't start if we don't kick-start it.
   call_out("make_chat", 10);
}


// Kick-start the bloody chatter. Damn' thing won't do anything else.
/** @ignore yes */
void make_chat()

{
  if (query_chatter()) {
     query_chatter()->make_chat();
  }
}

// override to generate modify_exits between rooms of various types.
/** @ignore yes */
mixed *query_to_same(string type) { return 0; }
/** @ignore yes */
mixed *query_to_other(string type) { return 0; }
/** @ignore yes */
mixed *query_from_other(string type) { return 0; }


// The next three are prettification functions for the above, and should
// never be called directly.
/** @ignore yes */
mixed *get_to_same(string from, string to, string type)

{ 
   mixed *result = query_to_same(type);
   
   if (arrayp(result))
      return map(result, (: replace($1, ({ "$T", $2, "$F", $3 }) ) :), to, from);
   
   return 0;
}


/** @ignore yes */
mixed *get_to_other(string from, string to, string type)

{ 
   mixed *result = query_to_other(type);
   
   if (arrayp(result))
      return map(result, (: replace($1, ({ "$T", $2, "$F", $3 }) ) :), to, from);
   
   return 0;
}


/** @ignore yes */
mixed *get_from_other(string from, string to, string type)

{ 
   mixed *result = query_from_other(type);
   
   if (arrayp(result))
      return map(result, (: replace($1, ({ "$T", $2, "$F", $3 }) ) :), to, from);
   
   return 0;
}


/**
 * Called by the handler to set our extra_look strings.
 */
void set_extra_long(string extra)

{
   _extra_long = extra;
}


void set_outside_types(string types)

{
   _outside_types = types;
}


// This handles all the extra parts of the room desc which are
// inserted by the handler.
/** @ignore yes */
string extra_look() 

{
   string   result = _extra_long;
   string   title;
   mapping  feature;
   
   foreach (title, feature in _features) {
      result += query_map_handler()->query_feature_desc(title, feature["range"], 
                                    feature["direc"]) + "\n";
   }
   return result;
}


// For internal use. Talks to the handler to convert a pathname of type:
// /w/dek/map.c:3:4 to something useable.
/** @ignore yes */
mixed query_room(string room_path)

{
   string   *bits = explode(room_path, ":");
   object   room;
   object   handler;
   
   if (sizeof(bits) < 3)
      return room_path;
   
   room = load_object(bits[0]);
   handler = room->query_map_handler();
   
   return handler->query_room(room_path);
}


/**
 * Add a new visible feature to the room, so it knows to query the handler
 * for descriptions and so on. The zone info is needed as an easy way for
 * the handler to get access to which rooms see which features. 
 * @param title the title of the feature
 * @param direc the direction of the feature
 * @param range the range to the feature
 * @param items any add_items associated with the feature
 */
void add_feature(string title, string direc, int range, mixed items)

{
   _features[title] = (["direc":direc, "range":range, "items":items]);
   add_item(items, (: query_feature_item_desc($(title)) :));
}


/**
 * Removes the specified feature.
 * @param title the feature to remove
 */
void remove_feature(string title)

{
   mapping  feature = _features[title];
   mixed    items;
   
   if (!feature)
      return;
   
   map_delete(_features, title);
   items = feature["items"];
   if (stringp(items))
      remove_item(items);
   else
      remove_item(items[0]);
}


/**
 * This figures out the range to the specified feature.
 * @param title the name of the feature
 */
int query_feature_range(string title)

{
   mapping   feature = _features[title];
   
   if (!feature)
      return -1;
   
   return feature["range"];
}


/**
 * This method returns the item description for the feature.
 * @param title the feature to look up
 * @return the description of the feature
 */
string query_feature_item_desc(string title)

{
   return query_map_handler()->query_feature_item_desc(title);
}


/**
 * This method returns all of our features.
 */
mapping query_features() { return _features; }

// When someone logs into the room, see if they were previously in a
// virtual. If they were, put them back in the same location.
// coords are stored as a property, so remove it when done for neatness' sake.
/** @ignore yes */
void login_restore( string name, string l_event)

{
   object player;
   string path = explode(file_name(this_object()), "#")[0];
   int    *coords, x, y;
   
   if (l_event != LOGIN)
      return;
   
   player = find_player(name);
   if (!player)
      return;

   coords = player->query_property("area_map_coords");
   player->remove_property("area_map_coords");
   if (!coords)
      return;
   
   x = coords[0];
   y = coords[1];
   
   LOGIN_HANDLER->remove_static_login_call(name, "login_restore", path);
   player->move(query_map_handler()->load_room_at(x, y));
}

// If someone quits here, and this is a virtual, store their coords in a
// property, and flag the login handler so it knows to talk to us when
// they return.
/** @ignore yes */
void event_quit(object player)

{
   string name;
   string path = explode(file_name(this_object()), "#")[0];
   
   if (!player)
      return;
      
   if (!interactive(player))
      return;
   
   if ((_x_coord < 0) || (_y_coord < 0))
      return;
      
   name = player->query_name();
   player->add_property("area_map_coords", query_coords());
   LOGIN_HANDLER->add_static_login_call(name, "login_restore", path);
}

int query_installed() { return _installed; }
void set_installed() { _installed = 1; }

