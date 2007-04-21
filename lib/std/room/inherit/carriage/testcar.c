/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: carriage.c,v 1.5 2002/08/19 00:39:11 pinkfish Exp $
 *
 *
 */

/*** New and Improved Carriage
     Coded by Wodan, Deutha, Gototh, Terano, Pinkfish.
     Coded on 25th February, 1998

     Added a carriage bell, 24th March. Terano

     Mangled hacked and mostly rewrote by Pinkfish 2002.

***/

#include <room/carriage.h>

#define DEFAULT_WAIT 60
#define DEFAULT_TRAVEL 45

inherit "/std/room/basic_room";

private class route_details _route_info;
private string _carriage_name;
private string _exit_name;

private int _current_stop;
private int _next_stop;

private object _outside;
private object _fake_carriage;
private object _bell;
private object _car_driver;
private object _handler;

private string _n_name;
private string _p_name;

int do_ring();
string query_environ_long();

/*
This mapping holds a few things:
The name of the location of the stop, the path of the stop,
how long it takes to get there from the last stop and how long
it will wait there.

ie:

route_info= (\ "Lancre Town", "/d/ram/lancre.c",100,60 \),
            (\ "Ohulan Cutash", "/d/ram/oc.c",100,45 \)
            (\ "Ankh Morpork", "/d/am/blue.c",200, 75 \)


This info is then referenced by an index.

*/

void create() {
    do_setup++;
    ::create();
    do_setup--;

    set_light(60);
    set_keep_room_loaded(1);
    set_short( "inside a carriage" );
    add_property( "no teleport", 1 );

    add_property("determinate", "");
    add_property("no map", 1);
    add_property("no godmother", 1);

   add_item("window", (: query_environ_long :) );
   add_item( "carriage", "The carriage looks fairly old and barely "
     "road worthy." );

   _bell = clone_object("/std/object");
   _bell->set_name("bell");
   _bell->set_short("bell");
   _bell->add_property("there","hanging here");
   _bell->set_long("This is a bell you could probably ring if the "
                 "driver has fallen asleep and you want the carriage to go "
                 "somewhere.\n");
   _bell->reset_get();
   _bell->reset_drop();
   _bell->move( this_object() );

   if (!do_setup) {
      this_object()->setup();
      this_object()->reset();
   }

} /* setup() */


void init() {

  this_player()->add_command("ring", _bell, "<direct:object>", (: do_ring :) );

}//end init();

/**
 * This method should be overriden by higher level carriages to setup
 * the driver correctly.
 * @return the driver to use for this carriage
 */
object create_carriage_driver() {
   return 0;
}

/**
 * This method returns the name of the route the carriage is on.
 * @return the name of the carriage route.
 */
string query_carriage_name() { return _carriage_name; }

/**
 * This method returns the details of the current stop.
 * @return the details of the current stop
 */
class stop_list_details query_current_stop() { return _route_info->stops[_current_stop]; }

/**
 * This method returns the details of the next stop.
 * @return the details of the next stop
 */
class stop_list_details query_next_stop() { return _route_info->stops[_next_stop]; }

/**
 * This method returns the object that is the outside of us.
 * @return the environment of the carriage
 */
object query_outside() { return _outside; }

/**
 * This method returns the entire details of the route used by this
 * carriage.
 * @return the entire route details
 */
class route_details query_route_map() { return _route_info; }

/**
 * This method returns the indexes of the current stop and the
 * next stop.
 * @return ({ current stop, next stop })
 */
int *query_stop_info() { return ({ _current_stop, _next_stop }); }

/**
 * This is called by the handler to update the current route information
 * @param name the name of the rotue
 * @param new_route_info the new route to use
 * @param handler the handler for the carriage
 */
void update_route( string name,
                  class route_details new_route_info,
                  object handler)
{
   _handler = handler;
   _route_info = new_route_info;
   // Mess with the stop stuff.
   if (_current_stop >= sizeof(_route_info)) {
      _current_stop = _current_stop % sizeof(_route_info->stops);
   }
   _next_stop = (_current_stop + 1) % sizeof(_route_info->stops);
}

/**
 * This is called by the handler to originaly setup the current route
 * information.
 * @param name the name of the rotue
 * @param new_route_info the new route to use
 * @param handler the handler for the carriage
 */
int make_carriage( string name,
                   class route_details new_route_info,
                   object handler)
{

    _handler = handler;
    _carriage_name = name;
    _route_info = new_route_info;

    _current_stop = 0;
    _next_stop = 1;


  if (!stringp( _carriage_name )) {
     log_file("NEW_CARRIAGE","%O: Invalid name: %O", this_object(),_carriage_name);
     return 0;
  }

  if (!classp( _route_info )) {
     log_file("NEW_CARRIAGE","%O: Invalid class: %O", this_object(), _route_info);
     return 0;
  }

  call_out("make_stop",5);
  _current_stop = 0;
  _next_stop = (_current_stop + 1) % sizeof(_route_info->stops);

  if(!_car_driver) {
    _car_driver = create_carriage_driver();
    if(objectp(_car_driver)) {
      _car_driver->route_setup(_carriage_name);
      _car_driver->move(this_object());
    }
  }

  return 1;
}//end make_carriage()


/**
 * Stop at the next location.
 */
void make_stop() {
   string name;
   string* name_parts;
   string np;
   string n_path;
   string path;
   int travel_time;
   int stop_time;

   name = _handler->query_stop_name(_route_info->stops[ _current_stop ]->id);
   path = _handler->query_stop_location(_route_info->stops[ _current_stop ]->id);
   _p_name = name;
   travel_time = _route_info->stops[ _next_stop ]->travel_time;
   stop_time = _handler->query_stop_delay(_route_info->stops[ _current_stop ]->id);

   _n_name = _handler->query_stop_name(_route_info->stops[ _next_stop ]->id);
   n_path = _handler->query_stop_location(_route_info->stops[ _next_stop ]->id);

   _outside = load_object(path);

   if (!_outside) {
      log_file("NEW_CARRIAGE", ctime(time())+" (%O): problem with "
        + path +" (" + name + ", " + _route_info->stops[ _current_stop ]->id + ")\n", this_object());
      _route_info->stops = _route_info->stops[0.._current_stop - 1] +
                    _route_info->stops[_current_stop + 1..];
      call_out("make_stop", 1);
      return;
   }


    /* Carriage has arrived at the new destination */

   tell_room(this_object(), "The carriage comes to a grinding "
     "halt.\n");
   if(objectp(_car_driver) && (environment(_car_driver) == this_object())) {
     _car_driver->arrived_at_to_carriage( name, _n_name );
   } else {
     tell_room(this_object(), "The carriage has arrived at " + name + ".\n");
   }

   if (!_fake_carriage) {

     _fake_carriage = clone_object("/std/object");
     _fake_carriage->set_name("carriage");
     _fake_carriage->add_property("determinate", "the " );
     _fake_carriage->set_short(_carriage_name + " Carriage");
     _fake_carriage->add_adjective( map(explode(_carriage_name, " "),
                       (: lower_case($1) :) ) );
     _fake_carriage->set_long("The carriage looks fairly old "
       "and barely road worthy.  You could probably enter "
       "it if you fancy a bumpy ride.\n");
     _fake_carriage->reset_get();
   }

   _fake_carriage->move( _outside );
   _fake_carriage->add_property("there","waiting here");

   /* Configure the inside of the carriage so passengers can disembark */
   add_exit("leave carriage", path, "corridor");

   modify_exit("leave carriage", ({
                "function", "check_can_leave",
                "enter", "$N climb$s out of the carriage.\n",
                "message", "$N climb$s out of the carriage.\n",
                "move mess", "\nYou climb out of the carriage.\n",
                "size", 190}));

   add_alias("out", "leave carriage");


   /* Display information about the carriage to people outside. */
   tell_room(_outside, "The "+ _carriage_name + " carriage arrives and "
      "pulls to a stop.\n" 
      "The driver opens the carriage door.\n");

   if(objectp(_car_driver)) { 
     _car_driver->move(_outside, "The " + _carriage_name + " carriage "
       "driver hops out of the carriage.", "The driver opens the door "
       "and leaps out.");
     _car_driver->arrived_at_to_stop( capitalize(_n_name) );
   } else {
     tell_room(this_object(), "The carriage's next stop is " + _n_name + ".\n");
   }

   /* Let new passengers enter */

   _exit_name = "enter " + lower_case(_carriage_name) + " carriage";

   _outside->add_exit(_exit_name, this_object(), "corridor"); 
   name_parts = explode(lower_case(_carriage_name), " "); 
   foreach(np in name_parts) {
      _outside->add_alias("enter " + np + " carriage", _exit_name); 
      _outside->add_alias("enter " + np + "s carriage", _exit_name); 
   }  
   _outside->add_alias("enter carriage", _exit_name);

/* Changed the modify exit to try to solve problems with messages in rooms
 * which use a linker object.  -- Furcifer, with thanks to Oaf for
 * pointing out this buglet.
 */
   if(!_outside->query_linker()) {
      _outside->modify_exit(_exit_name, ({
                   "enter", "$N enter$s the carriage.\n",
                   "message", "$N climb$s aboard the carriage.\n",
                   "move mess", "\nYou climb aboard the carriage.\n",
                   "size", 190 }) );
   } else {
      string *linked_rooms;

      _outside->modify_exit(_exit_name, ({ 
                   "enter", "$N enter$s the carriage.\n", 
                   "message", "$N climb$s aboard the carriage.\n",
                   "move mess", "\nYou climb aboard the carriage.\n",
                   "linker mess", ({ 
                      "At the carriage stop, $N $V$0=enters,enter$V$ "
                      "the carriage\n", "At the carriage stop $N "
                      "$V$0=gets off,get off$V$ the carriage.\n"}) ,
                   "size", 190 }) );

      linked_rooms = ( _outside->query_linker() )->query_rooms() 
                         - ({ base_name( _outside ) });
      foreach( string raum in linked_rooms ) {
         tell_room( raum ,
                "A carriage draws to a halt at the carriage stop.\n");
      }
   }

   /* Add the carriage to the description of the outside room */
   stop_room_chats();

   /* Start the Carriage on its way */
   call_out("give_warning", stop_time - 30, name, _n_name);
   call_out("move_on", stop_time );
} /* reach_stop() */

int check_can_leave(string exit, object person) {
   return !_outside->check_cannot_leave_carriage(person);
} /* check_can_leave() */

void give_warning(string this_stop_name, string next_stop_name) {

  if(objectp(_outside) && objectp(_car_driver) && 
     (environment(_car_driver) == _outside)) {
    _car_driver->leaving_again_to_stop(next_stop_name);
  } else {
    tell_room(this_object(), "The carriage is about to leave to " +
                             next_stop_name + ".\n");
  }
} /* give_warning() */

void move_on() {

  string* name_parts;

  if(objectp(_car_driver)) {
    _car_driver->do_command("mutter");
    _car_driver->move(this_object(), "The driver hops back in to the carriage "
      "and gets ready to drive again.", "The " + _carriage_name + 
      " driver hops back onto " + _car_driver->query_possessive() + 
      " carriage.");  
  } else {
    tell_room(this_object(), "The carriage leaves the current stop.\n");
  }

  remove_exit("leave carriage");
  remove_alias("out", "leave carriage");

  if(objectp(_outside)) {
    tell_room(_outside, "The driver closes the carriage door.\n"
                         "The carriage starts moving again.\n");

    _outside->remove_exit(_exit_name);
    name_parts = explode(lower_case(_carriage_name), " "); 
    foreach(np in name_parts) {
      _outside->remove_alias("enter " + np + " carriage", _exit_name); 
      _outside->remove_alias("enter " + np + "s carriage", _exit_name); 
    }  
    _outside->remove_alias("enter carriage", _exit_name);

    if (_fake_carriage) {
      _fake_carriage->move("/std/room");
    }
    _outside->modify_item("carriage", 0);
  }

  if (objectp(_car_driver)) {
     _car_driver->next_stop_from_carriage(_n_name);	
  }

  _current_stop = _next_stop;
  _next_stop = (_current_stop + 1) % sizeof(_route_info->stops);

  /* Clear the outside object */
  _outside = 0;

  room_chat(({15, 30, ({
    "The carriage seems to be on the verge of falling apart "
      "as it shakes.",
    "The carriage hits a bump in the road and you almost "
      "fall out.",
    "The driver seems to have a talent for hitting every "
      "pothole.",
    "The driver falls off, and just manages to get back on "
      "the carriage."
  }) }) );

  this_object()->check_chat();

  call_out("make_stop", 30 );
} /* move_on() */

void dest_me() {
  string path;
  object ob;
  string* name_parts;

  if(objectp(_outside)) {
    _outside->remove_exit(_exit_name);
    name_parts = explode(lower_case(_carriage_name), " "); 
    foreach(np in name_parts) {
      _outside->remove_alias("enter " + np + " carriage", _exit_name); 
      _outside->remove_alias("enter " + np + "s carriage", _exit_name); 
    }  
    _outside->remove_alias("enter carriage", _exit_name);
    _outside->remove_extra_look(this_object());
    _outside->modify_item("carriage", 0);
  }

  if (_fake_carriage) _fake_carriage->dest_me();
  if (_bell) _bell->dest_me();
  if (_car_driver) _car_driver->dest_me();
  if (_current_stop < sizeof(_route_info)) {
     path = _handler->query_stop_location(_route_info->stops[ _current_stop ]->id);
     foreach (ob in all_inventory()) {
        if (living(ob)) {
          ob->move(path, "$N appears looking a little shocked.",
                         "$N disappears in a cloud of shockedness.");
        }
     }
  }
  
  ::dest_me();

} /* dest_me() */

/** @ignore yes */
string query_quit_handler() {
   return _handler->query_stop_location(_route_info->stops[ _current_stop ]->id);
} /* query_quit_handler() */

mixed *co() {
  return filter(call_out_info(),(: $1[0] == this_object() :));
}

int do_ring() {

  if ( sizeof( co() ) != 0 ) {
    add_succeeded_mess(({"You ring the bell.\n", "$N $V the "
      "bell.\n"}));

    if(objectp(_car_driver) && environment(_car_driver) == this_object()) {
      _car_driver->do_command("scowl");
      _car_driver->do_command("'We won't reach " + _n_name + " any faster just "
        "cause you ring that danged bell!");
    }
    return 1;
  }

  call_out("move_on",2);
  _car_driver->do_command(":wakes with a start.");
  tell_object( this_player(), "The carriage starts rolling again.\n");
  return 1;

} /* do_ring() */

string query_environ_long() {

  if ( !_outside ) {
    return "Outside the carriage, the road from " + _p_name + " to " 
      + _n_name + " swishes by.";
  } else {
  return "Outside the window you can see:\n" + _outside->long();
  }
} /* query_environ_long() */



