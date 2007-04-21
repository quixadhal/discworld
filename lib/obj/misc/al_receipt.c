/* the receipt given out for missing items */
/*
 * Modified by Jeremy, 22-Feb-97
 *    - added "kiss" command and associated changes
 */

#include <virtual.h>
#include <player.h>
#include <move_failures.h>
#include <armoury.h>

inherit "/std/object";

private int _timestamp;
private string _obname;
private string _obpath;
private string _virt_name;
private mixed _static_save;

void setup()
{
   set_name("receipt");
   set_short( "missing item receipt" );
   _obname = "unknown object";
   _obpath = "***";
   set_value(5);
   set_weight(1);
   add_adjective(({"missing", "item"}));
   add_property("writeable", 1);
   add_property("sale_value", 1);
   add_property("paper", 1);
   add_property("nosteal", 1);
   set_max_size(1000);
   _timestamp = time();
} /* setup() */

void init() {
    this_player()->add_command("kiss", this_object());
} /* init() */

/**
 * Sets the name of the object.  This will be used so the item recipt
 * can be identified.
 * @param s the object name
 */
void set_obname(string s) { _obname = s; }

/**
 * Sets the path to the object.  The path of the object for the recipt to
 * work with.
 * @param s the object path
 */
void set_object(string s) { _obpath = s; }

/**
 * This method is identical to set_object()
 * @param s the object path
 */
void set_obpath(string s) { set_object(s); }

/**
 * This method sets the virtual object name of the item.
 * @param s the virtual object name
 */
void set_virtobname(string s) { _virt_name = s; }

/**
 * This method is identical to set_virtobname()
 * @param s the virtual object name
 */
void set_virtname(string s) { set_virtobname(s); }


/**
 * This method sets the save information for the file.  It actually sets
 * both the static and dynamic information.  This assumes it is passed
 * an array of two elements, the first is the static save information and
 * the second is the dynamic save information.
 * @param m the data as defined above
 */
void set_static_save(mixed m) {
  mixed p;

  _static_save = m;
  if (_obname != "unknown object") {
    return;
  }

  // Try to find the short in here somewhere
  p = m[0];
  while (mapp(p)) {
    if (!undefinedp(p["short"])) {
      _obname = p["short"];
      return;
    }
    p = p["::"];
  }
}

/**
 * This method sets up the receipt for the specified object.
 * @param ob the object to setup the reciept for
 */
void setup_receipt(object ob) {
   mixed* bits;

   bits = AUTO_LOAD_OB->fragile_auto_str_ob(ob);
   set_obname(ob->query_name());
   set_object(bits[1]);
   set_static_save(bits[2]);
   set_value(ob->query_value());

   if(_obpath == "/obj/misc/al_receipt") {
     log_file("BAD_RECEIPTS", "%s: set by %s\n",
              ctime(time())[4..18], base_name(previous_object()));
     call_out("dest_me", 2);
   }
} /* setup_receipt() */

/**
 * When the item is sold, kill ourselves.
 */
void being_sold()
{
    call_out("dest_me", 4);
} /* being_sold() */

/** @ignore yes */
string long(string str, int light)
{
   return "This is a receipt for your missing " + _obname + ".  You can either "
      "sell it for something which may be close to replacement value (and may 
not), "
      "or give it to a creator and ask very nicely to have it replaced.\n"
      "You can also try kissing it - who knows, you may get lucky.\n";
} /* long() */

/** @ignore yes */
mapping query_dynamic_auto_load()
{
   return ([ "::" : ::query_dynamic_auto_load(),
             "obname" : _obname,
             "obpath" : _obpath,
             "obvalue" : value,
             "virt" : _virt_name,
             "timestamp" : _timestamp,
             "static" : _static_save ]);
} /* query_dynamic_auto_load() */

/** @ignore yes */
void init_dynamic_arg(mapping arg, object) {
   _obname = arg["obname"];
   if ( stringp( _obname ) ) {
      add_adjective( explode( lower_case( _obname ), " " ) );
   }
   _obpath = arg["obpath"];
   set_value(arg["obvalue"]);
   _virt_name = arg["virt"];
   _timestamp = arg["timestamp"];
   _static_save = arg["static"];
   ::init_dynamic_arg(arg["::"]);

   if(_obpath == "/obj/misc/al_receipt") {
     call_out("dest_me", 2);
   }
}

/**
 * This method returns the name of the object.
 * @return the name of the object
 */
string query_obname() { return _obname; }
/**
 * This method returns the path of the object.
 * @return the path of the object
 */
string query_obpath() { return _obpath; }
/**
 * This method returns the timestamp of the reciept.  This is the time the
 * receipt was created.
 * @return the timestamp
 */
int query_timestamp() { return _timestamp; }
/**
 * This method returns the virtual object path name of the item.
 * @return the virtual object name
 */
string query_virt_obname() { return _virt_name; }
/**
 * This method returns the save information for the object.  THis is not
 * just the static save information, it is an array of two elements.  The
 * first is the static save information and the second is the dynamic
 * save information.
 * @return the save information
 */
mixed query_static_save() { return _static_save; }

/**
 * This will return 1 if the item is a missing item receipt.
 * @return 1 if the item is a missing item receipt
 */
int query_missing_item_receipt() {
   return 1;
}

/** @ignore yes */
mixed *stats()
{
  mixed *stuff;

  stuff = ::stats() + ({
      ({ "obname", _obname }),
      ({ "obpath", _obpath }),
      ({ "timestamp", _timestamp + " (" + ctime(_timestamp) + ")" }),
   });
  if (_virt_name)
    stuff += ({ ({ "virtname", _virt_name }) });
  return stuff;
} /* stats() */

/** @ignore yes */
int do_kiss() {
    object thing;

    if (sizeof(_static_save) < 2) {
        if (this_player()->query_creator()) {
            write("This receipt has no auto-load information, possibly "
                  "because the object was broken when the player logged "
                  "out.  It will have to be replaced by hand.\n");
        } else {
            write("Sorry, but this object is too broken to repair "
                  "with a kiss.  You'll have to contact a creator.\n");
        }
        return 1;
    }
    if ((query_cloned_by() != this_player()->query_name())
        && !this_player()->query_creator()) {
        write("The receipt whispers: I don't belong to you!\n");
        return 1;
    }

    // If we have virtual names, try and fiddle with this.
    if (_virt_name) {
       _virt_name = CLONER->other_file( _virt_name );
       if (file_size(_virt_name) <= 0) {
          // try and rewrite through the armoury
          _virt_name = ARMOURY->remap_file_path(_virt_name);
       }
       if (file_size(_virt_name) <= 0) {
           write("The receipt whispers: Sorry, I'm still broken.  "
                 "Try again some other time, or contact a creator.\n");
           return 1;
       }
    }

    if ( !catch( thing = (object)CLONER->clone( _obpath ) ) ) {
        if ( thing ) {
            thing->init_static_arg(_static_save[0]);
            thing->init_dynamic_arg(_static_save[1]);
            if (!thing->move( environment(this_object()) )
                || !thing->move( environment(environment(this_object())) )) {
                thing->add_property(VIRTUAL_NAME_PROP, _virt_name);
                write(the_short() + " suddenly transforms itself into " +
                      thing->a_short() + ".\n");
                tell_room(environment(this_player()),
                          this_player()->the_short() +
                          " kisses a piece of paper, which suddenly "
                          "transforms into " + thing->a_short() + ".\n",
                          ({ this_player() }));
                this_player()->add_succeeded_mess(this_object(), "", ({ }));
                move("/room/rubbish");
            } else {
                write("The receipt whispers: I'm going to need some "
                      "creator help.\n");
                thing->dest_me();
            }
        } else {
            write("The receipt whispers: Sorry, I'm still broken.  "
                  "Try again some other time, or contact a creator.\n");
        }
    } else {
        write("The receipt whispers: Sorry, I'm still broken.  "
              "Try again some other time, or contact a creator.\n");
    }
    return 1;
} /* do_kiss() */


varargs int move(mixed dest, string messin, string messout)  {
   if (!objectp(dest))  {
      if (!stringp(dest)  ||  (dest = find_object(dest)) == 0)  {
         return MOVE_EMPTY_DEST;
      }
   }

   if (::move(dest, messin, messout) != MOVE_OK)  {
      if (environment(dest))  {
         return move(environment(dest), messin, messout);
      }
      else  {
         return MOVE_INVALID_DEST;
      }
   }
   return MOVE_OK;
} /* move() */
