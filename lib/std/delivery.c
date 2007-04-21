/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: delivery.c,v 1.2 2003/03/21 01:49:47 ceres Exp $
 *
 *
 */

/**
 * Inheritable Delivery System
 * <p>
 * This object provides a way of storing and sending objects to 
 * players while they are not online.  It is roughly based on
 * the concepts found in Post Offices that support parcel
 * depositing, and also Bernita's Flower Shop. (Which delivers
 * flowers to people when they log on.)
 * <p>
 * This object is easy to use, and can be combined with most
 * other objects on the Disc. (ie, /obj/monster and /std/room).
 * See delivery_room.c for a pre-made room which handles the 
 * delivery of objects.
 * <p>
 * Note that this system uses static login calls.  These 
 * must be configured first and turned on manually in order for
 * the delivery system to work.
 * <p>
 * In order to use this inheritable the following things must be
 * done:
 * <ul>
 * <li>The object that uses it created, and this object inherited.
 * <li>The save file set using set_save_file().
 * <li>Any extra settings such as delivery message and delay configured.
 * <li>setup_delivery() called on the object to finalize setup.
 * </ul>
 * @author Taffyd
 */
 
#include <player_handler.h>
#include <login.h>
#include <move_failures.h>
 
inherit "/global/auto_load";

class delivery_item {
    object delivery_ob;
    string sent_by;
    int submit_time;
    int delay_time;
}

protected void set_delivery_delay(int new_time);
protected int valid_delivery(class delivery_item delivery, string person);
protected void deliver_item(string who, object ob, string sent_by);
protected void set_burdened_mess(string s);
protected void set_delivery(mapping x);
protected void set_delivery_mess(string s);
protected void set_save_file(string str);
public int query_delivery_delay();
public string query_burdened_mess();
public string query_delivery_mess();
public string query_save_file();
public varargs mixed query_delivery(string person);
public void add_delivery(mixed who, mixed sender, object item, int delay);
public void check_delivery(mixed person, string type);
public void clear_delivery();
public void setup_delivery();
public void load_file();
public void save_file();

private mapping _delivery;
private mixed _item_save;

private nosave string _save_file;
private nosave object _cont;
private nosave string _delivery_mess;
private nosave string _burdened_mess;
private nosave int _delivery_delay;

/** @ignore yes */
void create() {   
    _delivery = ([ ]);
    _save_file = "";
    _delivery_delay = 5;
    _delivery_mess = "\nA small womble wearing a bright green hat "
        "strides up to you confidently and hands you $N.  She "
        "mutters something about \"delivery\" and \"$S\", before "
        "scurrying away.\n";
    _burdened_mess = "\nA small womble wearing a bright purple "
        "hat strides up to you, and places \"$N\" on the ground.  She "
        "mutters something about a gift from \"$S\", and the runs off.\n";
    _cont = clone_object("/std/container");
    call_out("tidy_up", 30 + random(30));
} /* create() */        

/** 
 * This method adds the object into the login handler's static login calls 
 * list.  It only ever needs to be called _once_, and doesn't need to be 
 * called every time the object loads. 
 * <p>
 * Once this is done, the object will automatically be loaded whenever 
 * someone logs on.
 * <p> 
 * This must be called in order for the delivery system to work at all.
 */
public void setup_delivery() {   
    LOGIN_HANDLER->add_static_login_call("all", "check_delivery", 
        base_name(this_object()));
} /* setup_delivery() */

/** 
 * This method is used to query the save file in which the delivery data
 * is stored.
 *
 * @return  the path of the save file used to store delivery details.
 */
public string query_save_file() {
    return _save_file;    
} /* query_save_file() */

/** 
 * This method sets the save file to a file name so that auto loading and 
 * delivery details can be successfully saved.
 * @param str the new path.
 */
protected void set_save_file(string str) {
    _save_file = str;       
} /* set_save_file() */

/** 
 * This method is used to remove all deliveries currently stored within the 
 * delivery system.
 * <p>
 * Be very careful when using this function! It will save the cleared
 * delivery mapping after it has been called. 
 */
public void clear_delivery() {
    _delivery = ([ ]);
    save_file();    
} /* clear_delivery() */

/**
 * This is used to set all of the delivery mapping manually.  You should 
 * not use this method.  
 *
 * @param x the new delivery mapping.
 * @see add_delivery()
 */
protected void set_delivery(mapping x) {
    _delivery = x;
    save_file();      
} /* set_delivery() */

/**
 * This method adds a new item to the delivery mapping for the specified 
 * recipient.  This should always be used instead of set_delivery(), as it
 * makes sure that all the data structures have been configured correctly.
 *
 * @param who the person to receive the item. 
 * @param sender the person sending the file. 
 * @param item the item to be sent
 * @param delay how many seconds to wait before delivering item on the next
 * login.
 *
 * @example
 *      add_delivery(this_player(), "taffyd", frog, 360);
 * @see check_delivery()
 */
void add_delivery(mixed who, mixed sender, object item, int delay) {   
    string name, from;
    class delivery_item parcel;
    
    if (objectp(who))    
        name = who->query_name();
    else
        name = who;
        
    if (objectp(sender))    
        from = sender->query_cap_name();
    else
        from = sender;
    
    /* Create the new delivery item, configuriung the class and
       add it to the delivery mapping. */
           
    parcel = new(class delivery_item,
        delivery_ob : item, submit_time : time(), sent_by : from,
        delay_time : delay);
    
    if (undefinedp(_delivery[name]))
        _delivery[name] = ({ parcel });
    else         
        _delivery[name] += ({ parcel });
    
    log_file("DELIVERY", "%s added a new delivery item for %s, %s sent "
        "by %s.\n", previous_object()->short(), name, item->short(), 
        from);
    item->move(_cont);        
    save_file();
} /* add_delivery() */

/** 
 * This method is called by the check_delivery() function when it validates 
 * someone's deliveries.  It is used to determine whether or not it's time
 * for a delivery to be delivered, and whether or not to remove it.
 * <p>
 * @param delivery the delivery item to be tested
 * @param person the person to deliver the item to.
 * <p>
 * @return 1 if the delivery is not ready, 0 if it is. (yes, this is kind
 * of backwards.)
 */
protected int valid_delivery(class delivery_item delivery, string person) {
    
  if (delivery->submit_time + delivery->delay_time > time())
    return 1;
  
  if (!objectp(delivery->delivery_ob))
    return 0;
  
  call_out( (: deliver_item :), _delivery_delay, person, delivery->delivery_ob,
            delivery->sent_by);
  
  return 0;
} /* valid_delivery() */

/** 
 * This method is called by the login handler every time someone logs 
 * on to the Disc.  It handles the delivery of items and all sorts of 
 * nifty things.
 * <p>
 * @param person the name of the person logging on
 * @param type the type of login event
 *
 */
public void check_delivery(mixed person, string type) {    
    if (type != LOGIN && type != RECONNECT) 
        return;
    
    if (!_delivery[person])
        return;
        
    _delivery[person] = filter(_delivery[person], 
        (: valid_delivery($1, $(person)) :));
   
    if (!sizeof(_delivery[person]))
        map_delete(_delivery, person);
                
    save_file();    
} /* check_delivery() */

/**
 * This method is called when an item is being delivered to a player.  The 
 * delivery message or the burdened mess is used depending on how the item
 * is delivered, and then it is removed from the delivery mapping.
 * <p>
 * Note that this routine uses add_message() to display movement messages
 * to the player, instead of tell_object() or a traditional move message. This
 * is so that the messages can be 'clumped' together properly, otherwise 
 * it just doesn't work.
 * <p>
 * @param who the person to deliver to
 * @param ob the object to deliver
 * @param sent_by the person that send the parcel
 * @param position its location in the recepient's delivery mapping.
 */
protected void deliver_item(string who, object ob, string sent_by) {
    string new_mess;
    object player;
       
    player = find_player(who);
    
    /* Something has gone wrong... the player must have logged out before 
       they received their message. We'll just trash the item, since its
       already been removed from the delivery inheritable and placing it 
       back in is messy. (and rather pointless) */
       
    if (!player)
        ob->move("/room/rubbish");    
    
    if (ob->move(player) != MOVE_OK) {

        /* If we can't move the item into the player, then we should move it 
           into their environment... */
           
        if (environment(player)) {
            new_mess = replace(_burdened_mess, ({ "$S", sent_by, "$N", 
                "$0$" }));
            player->add_message(new_mess,  ({ ({ "my_a_short:" + 
            file_name(ob) }) }));

            ob->move(environment(player), );
        }        
        else {
            /* Eep, no environment. Trash the item, and log it. */
            ob->move("/room/rubbish", "$N appear$s in a puff of smoke.");
            log_file("DELIVERY", "%s: %s sent to the rubbish room, %s has "
                "no environment.\n", ctime(time()), ob->short(), 
                player->short());
        }
    }    
    else {
        /* Deliver everything happily. */
        new_mess = replace(_delivery_mess, ({ "$S", sent_by, "$N", "$0$" }));       
        
        player->add_message(new_mess,  ({ ({ "my_a_short:" + 
            file_name(ob) }) }));
        
        log_file("DELIVERY", "%s: %s delivered to %s.\n", ctime(time()), 
            ob->short(), who);
    }        
} /* deliver_item() */

/** @ignore yes */
int query_theft_command() {
    return -1;
} /* query_theft_command() */

void clean_delivery_mapping() {
    string *people, person;
    
    people = keys(_delivery);
    
    foreach (person in people) {
        _delivery[person] = filter(_delivery[person], 
            (: objectp($1->delivery_ob) :));
    }
    
    _delivery = filter(_delivery, (: sizeof($2) :));
} /* check_valid_delivery() */


/**
 * This method saves all of the delivery data into the save file, handling 
 * saving of the auto loading as well.
 */
public void save_file() {
  mixed *tmp;
  string player;
  class delivery_item *values;
  
  // no save file, cant save.
  if(_save_file == "")
    return;
  
  _item_save = ({ });
  
  clean_delivery_mapping();
  
  foreach (player, values in _delivery) {           
    tmp = map(values, (: $1->delivery_ob :));
        
    if (!arrayp(tmp))
      continue;
    
    _item_save += ({ player, create_auto_load(tmp, 0) });
  }
  
  tell_creator("ceres", "Saving: %s\n", _save_file);
  unguarded((: save_object, _save_file :));
  _item_save = 0;
} /* save_file() */

/**
 * This method loads retrieves data from the save file, and handles the 
 * autoloading of the items.
 */
void load_file() {
  int i, j, size;
  mixed *tmp;
  string who;
  mixed *items;

  if(_save_file == "")
    return;
  
  log_file("DELIVERY", "Attempting to load save file at %s.\n",
           ctime(time()));

  tell_creator("ceres", "Loading: %s\n", _save_file);
  if (file_size(_save_file + ".o") > -1) {
    unguarded((: restore_object, _save_file :));
    
    if (!_cont)
      _cont = clone_object("/std/container");
    
    all_inventory(_cont)->move("/room/rubbish");
    
    for (i = 0; i < sizeof(_item_save); i = i + 2) {
      who = _item_save[i];
      items = _item_save[i + 1];
      
      if (!_delivery[who])
        return;
      
      tmp = load_auto_load_to_array(items, this_player());
      
      /* This places the stored items back into the delivery
         mapping... items are stored in tmp backwards,
         which is why we do the size-i-1 thing.  */
      
      size = sizeof(tmp);
      
      for (j = 0; j < size; j++) {
        _delivery[who][j]->delivery_ob = tmp[size - j - 1];
        tmp[j]->move(_cont);
      }
    }
    
    _item_save = 0;                       
  }
} /* load_file() */

/** 
 * This returns the current delivery message that is displayed to the 
 * recipient of a delivery.
 *
 * @see set_delivery_mess()
 * @return the delivery message
 */
public string query_delivery_mess() {
    return _delivery_mess;
} /* query_delivery_mess() */


/** 
 * This method returns how long after the login an item is delivered
 * to the recipient.
 * @return the number of seconds unt    
 *
 */
public int query_delivery_delay() {
    return _delivery_delay;
} /* query_delivery_time() */


/**
 * This method is used to specify how many seconds to wait after login 
 * until the item is delivered.  Note that if this is too long, then the
 * receipient may log out before you can give them the item.  If the 
 * receipient logs out, then the item will be destroyed.
 * <p>
 * This is set to 5 by default.
 * <p>
 * @param new_time the time in seconds after log in to deliver the message.
 */
protected void set_delivery_delay(int new_time) {
    _delivery_delay = new_time;
} /* set_delivery_time() */

/** 
 * This method sets the delivery message that is displayed to the player 
 * when they log on. $S is substituted in the string with the name of 
 * the player who sent the message, $N is replaced with the a_short()
 * of the item being sent. 
 * @param s the new delivery string
 * @see query_delivery_mess()
 * @see /std/basic/desc->a_short()
 * @example
 *     set_delivery_mess("A small frog hops up to you, and thrusts $N "
 *         "into your arms.  He holds up a large sign that reads: \""
 *         "From $S\", and then runs away.\n");
 *
 * // Recipient sees:
 * A small frog hops up to you, and thrusts a large blue box into your
 * arms.  He holds up a large sign that reads: \"From Taffyd!\", and then
 * runs away.
 */
protected void set_delivery_mess(string s) {
    _delivery_mess = s;
} /* set_delivery_mess() */

/** 
 * This method sets the message that is displayed when an item is too 
 * heavy to be placed in the victim's inventory. This uses the same format
 * as the normal delivery message.
 * <p>
 * @param s message to display
 * @see set_delivery_mess()
 * @see query_burdened_mess()
 */
protected void set_burdened_mess(string s) {
    _burdened_mess = s;    
} /* set_burdened_mess() */

/** 
 * This returns the current delivery message that is displayed to the 
 * recipient of a delivery, if it is too heavy go to in their inventory.
 * <p>
 * @see set_burdened_mess()
 * @see query_delivery_mess()
 * @return the delivery message
 */
public string query_burdened_mess() {
    return _burdened_mess;
} /* query_burdened_mess() */

/**
 * This method returns the delivery details for a particular person, or
 * the entire delivery mapping of one doesn't exist.
 * <p>
 * @param person the person's delivery details to query. 
 */
public varargs mixed query_delivery(string person) {
    if (undefinedp(_delivery[person])) {
        return copy(_delivery);
    }
    else {
        return copy(_delivery[person]);
    }
} /* query_delivery() */

/** @ignore yes */  
void dest_me() {
    if (_cont) 
        _cont->dest_me();
} /* dest_me() */

/** @ignore yes */
object query_cont() {
  return _cont;
}

/**
 * @ignore yes
 */
void tidy_up() {
  string person;
  
  foreach(person in keys(_delivery)) {
    if(!PLAYER_HANDLER->test_user(person) || !PLAYER_HANDLER->test_active(person)) {
      log_file("DELIVERY", "%s Removing delivery for %s (inactive).\n",
               this_object()->query_short(), person);
      map_delete(_delivery, person);
    }
  }
  save_file();
}

/** @ignore yes */
mixed *stats() {
    return ({
        ({ "delivery mess", replace(_delivery_mess, ({ "\n",  "" }) ) }),
        ({ "burdened mess", replace(_burdened_mess, ({ "\n",  "" }) ) }),        
        ({ "delivery delay", _delivery_delay, }),
        ({ "delivery save file", _save_file, }),
        ({ "undelivered items", _cont ? sizeof(all_inventory(_cont)) : 0 }),
    });
} /* stats() */
