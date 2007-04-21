/*   -*- LPC -*-   */
/*
 * $Locker:  $
 * $Id: mail_track.c,v 1.10 2003/03/21 01:28:01 ceres Exp $
 */
/**
 * This handler keeps track of who has a mailer at the moment and sends the
 * mail to there instead.  This also keeps track of the current mailing
 * lists in the system.
 * @author Pinkfish
 * @started 1991 sometime
 * @index mailing_list
 */
inherit "std/object";

#include <mail.h>
#include <mail_track.h>
#include <player_handler.h>

#define SECURITY if (!master()->query_leader(previous_object(-1))) return 0;

int add_member(string list, string mem);
private void load_me();
private void save_me();
int query_list(string list);
int create_list(string list, string creator);

/*
 * this object keeps track of who has a mailer at the moment and sends the
 * mail to there instead.
 * bounce.
 */

/*
 * Hmmm, well I think I will add mailing list support. Sounds cute huh?
 */

nosave mixed *our_mailers;
mapping mailing_lists;

/** @ignore yes */
void setup() {
   set_name("tracker");
   set_short("mail tracker");
   add_property("determinate", "the ");
   add_property("there", "mounted on one wall");
   add_adjective("mail");
   set_long("This thing tracks mail.   Clever, innit?\n");
   reset_get();
   reset_drop();
   our_mailers = ({ });
   mailing_lists = ([ ]);
   seteuid("Root");
   load_me();
   if (!query_list("postmaster")) {
      create_list("postmaster", "pinkfish");
   }
   move(MAIL_ROOM);
} /* setup() */

/**
 * This method adds in a mailer for a specific player into the current
 * list of mailers.
 * @param ob the mailer for the player
 * @param str the player the mailer is for
 * @return return 1 if successful
 */
int add_mailer(object ob, string str) {
   int i;

   if (!stringp(str) || !objectp(ob)) {
      return 0;
   }
   i = member_array(str, our_mailers);
   if (i == -1) {
      our_mailers += ({ ob, str });
   } else {
      write(i);
      if (our_mailers[i-1]) { /* make sure it hasn't already died somehow */
         our_mailers[i-1]->dest_me(); /* we don't want two of them */
      }
      our_mailers += ({ ob, str });
   }
   return 1;
} /* add_mailer() */

/**
 * This method finds the mailer associated with a particular player.
 * @param str the player whose mailer to find.
 * @return the mailer object associated with the player
 * @see add_mailer()
 * @see delete_mailer()
 */
object find_mailer(string str) {
   int i;

   if (!stringp(str)) {
      return 0;
   }
   i = member_array(str, our_mailers);
   if (i == -1) {
      return 0;
   }
   return our_mailers[i-1];
} /* find_mailer() */

/**
 * This method removes a mailer from the current list of mailers.
 * It removes the mailer based on the object itself
 * @param ob the mailer object to remove
 * @return 0 if unable to remove
 * @see add_mailer()
 * @see find_mailer()
 */
int delete_mailer(object ob) {
   int i;

   if (!objectp(ob)) {
      return 0;
   }
   i = member_array(ob, our_mailers);
   if (i == -1) {
      return 0;
   }
   our_mailers = delete(our_mailers, i, 2);
} /* delete_mailer() */

/** @ignore yes */
void we_are_done(object me) {
   if (objectp(me)) {
      me->dest_me();
   }
} /* we_are_done() */

/**
 * This is the main mail function.  It tries to start up a mailer with
 * the default address and subject.  If their is no default address then
 * the mailer uses it's menu mode, otherwise it sends one mail and then
 * leaves.
 * @param str 0 or "" for no default address
 * @param sub the default subject
 */
int mail(string str, string sub) {
   object mailer;

   mailer = clone_object(MAILER);
   mailer->set_do_this_last(({ this_object(), "we_are_done", mailer }));
   mailer->read_mail(str, sub);
   return 2;
} /* mail() */

private void save_me() {
   unguarded((: save_object, "/save/mail_track" :));
} /* save_me() */

private void load_me() {
   string *list_names, name;
   mixed *list;
   
   unguarded((: restore_object, "/save/mail_track" :));
   list_names = keys(mailing_lists);
   if (sizeof(list_names) && !classp(mailing_lists[list_names[0]])) {
      foreach (name, list in mailing_lists) {
         mailing_lists[name] = new(class mailing_list, members : list[0],
                                                controllers : list[1], creator : list[2]);
      }
      save_me();
   }
} /* load_me() */

/**
 * This method returns the current mailing lists available on the
 * handler.
 * @return the array of available mailing lists
 */
string *query_mailing_lists() {
   return keys(mailing_lists) +
     "/secure/master"->query_domains() + 
      ({ "seniors", "lords", "hlords" });
} /* query_mailing_lists() */

/**
 * This method checks to see if the specified list exists or not.
 * @return 1 if the list exists, 0 if not
 * @param list the list name to check
 * @see query_mailing_lists()
 * @see query_members()
 * @see query_member()
 */
int query_list(string list) {
   return member_array(list, query_mailing_lists()) != -1;
} /* query_list() */

/**
 * This returns the current members of the specified mailing list.
 * @return the members of the mailing list
 * @param list the mailing list to get the memebrs of
 * @see query_mailing_lists()
 * @see query_list()
 * @see query_member()
 */
string *query_members(string list) {
  string *tmp;
  
  switch (list) {
  case "playtesters" :
    tmp = (string *)"/obj/handlers/playtesters"->query_playtesters();
    return ({ "/d/playtesters/master"->query_lord() }) + sort_array(tmp, 1);
  case "seniors" :
    tmp = (string *)master()->query_all_seniors();
    return tmp;
  case "lords" :
    tmp = (string *)master()->query_all_leaders();
    return tmp;
  case "hlords" :
    tmp = ((string *)master()->query_administrators() -
           ({ "Admin", "Root", "offler" }));
    return tmp;
  default :
  }
  if (file_size("/d/"+ list) == -2) {
    tmp = (string *)("/d/"+ list +"/master")->query_members() +
      ({ (string)("/d/"+ list +"/master")->query_lord() });
  }
  else if (mailing_lists[list]) {
    tmp = copy(mailing_lists[list]->members);
  }

  if(tmp)
    return filter_array(tmp, (: PLAYER_HANDLER->test_active($1, 1) :));
  
  return ({ });
}

/**
 * This method checks to see if the given player is in the selected
 * mailing list.
 * @param list_name the mailing list name
 * @param name the name of the player in the list
 * @return 1 if the player is a member, 0 if not
 * @see query_mailing_lists()
 * @see query_list()
 * @see query_members()
 */
int query_member(string list_name, string name) {
   class mailing_list list;

   list = mailing_lists[list_name];

   if (list) {
      return member_array(name, list->members) != -1;
   }
   if (file_size("/d/" + list_name) == -2) {
      return (int)("/d/" + list_name + "/master")->query_member(name);
   }
   if (list_name == "seniors") {
      return (int)master()->query_senior(name);
   }
   if (list_name == "lords") {
      return (int)master()->query_leader(name);
   }
   if (list_name == "hlords") {
      return (int)master()->query_administrator(name);
   }
} /* query_member() */

/**
 * This method returns all the current controllers of the mailing list.
 * Controllers can add and remove members from a list.
 * @param list the mailing list to query the controllers for
 * @return the array of controllers
 */
string *query_controllers(string list) {
   if (file_size("/d/"+ list) == -2) {
      return ({ (string)("/d/"+ list +"/master")->query_lord() });
   }
   if (mailing_lists[list]) {
      return copy(mailing_lists[list]->controllers);
   }
   return ({ });
} /* query_controllers() */

/**
 * This method checks to see if the given player is a controller.
 * @param list_name the name of the list
 * @param name the name to check to see if they are a controller
 * @see query_controllers()
 * @see query_member()
 */
int query_controller(string list_name, string name) {
   class mailing_list list = mailing_lists[list_name];
   if (!list) {
      return 0;
   }
   return member_array(name, list->controllers) != -1;
} /* query_controller() */

/**
 * This method determines the creator of the list.
 * @return the creator of the list
 * @param list_name the list to get the creator of
 * @see create_list()
 */
string query_creator(string list_name) {
   class mailing_list list = mailing_lists[list_name];
   
   if (!classp(list)) {
      return 0;
   }
   return list->creator;
} /* query_creator() */

/**
 * This method attempts to create a mailing list.
 * @return 1 if successful, 0 if not
 * @param list the name of the list to create
 * @param creator the creator of the list
 * @see query_creator()
 * @see query_members()
 * @see query_controllers()
 */
int create_list(string list, string creator) {
   SECURITY
   if (mailing_lists[list]) {
      return 0;
   }
   if (file_size("/d/" + list) == -2) {
      return 0;
   }
   if (PLAYER_HANDLER->test_user(list)) {
      return 0;
   }
   mailing_lists[list] = new(class mailing_list, members : ({ creator }),
                                          controllers : ({ creator }), creator : creator);
   save_me();
   return 1;
} /* create_list() */

/**
 * This method will delete a mailing list.
 * @param list the mailing list to delete
 * @see create_list()
 * @return 0 if failed and 1 on success
 */
protected int delete_list(string list) {
   if (!mailing_lists[list]) {
      return 0;
   }
   map_delete(mailing_lists, list);
   save_me();
   return 1;
} /* delete_list() */

/**
 * This method adds a controller to the specified mailing list.
 * @param list_name the controller to add
 * @param mem the controller to add
 * @see query_controllers()
 */
int add_controller(string list_name, string mem) {
   class mailing_list list = mailing_lists[list_name];
   
   SECURITY
   if (!classp(list)) {
      return 0;
   }
   if (member_array(mem, list->controllers) != -1) {
      return 0; /* already a controller... */
   }
   list->controllers += ({ mem });
   add_member(list_name, mem);
   save_me();
   return 1;
} /* add_controller() */

/**
 * This method removes a controller from the specified mailing list.
 * @param list_name the mailing list to remove the member from
 * @param name the member to remove
 * @see add_controller()
 * @see query_controller()
 */
int remove_controller(string list_name, string name) {
   int i;
   class mailing_list list = mailing_lists[list_name];

   SECURITY
   if (!classp(list)) {
      return 0;
   }
   if ((i = member_array(name, list->controllers)) == -1) {
      return 0;
   }
   list->controllers = delete(list->controllers, i, 1);
   save_me();
   return 1;
} /* remove_controller() */

/**
 * This method adds a member to the specified mailing list.
 * @param list_name the mailing list to add a member too
 * @param mem the member to add
 * @return 1 if successful, 0 if not
 * @see remove_member()
 * @see add_member()
 */
int add_member(string list_name, string mem) {
   class mailing_list list = mailing_lists[list_name];
   
   if (file_name(previous_object()) != MAIL_ROOM) {
      return 0;
   }
   if (!classp(list)) {
      return 0;
   }
   if (member_array(mem, list->members) != -1) {
      return 0; /* already a member... */
   }
   list->members += ({ mem });
   save_me();
   return 1;
} /* add_member() */

/**
 * This method removes a member from the specified mailing list.
 * @param list_name the mailing list to remove the member from
 * @param mem the members name to remove
 * @return 1 on success, 0 on failure
 * @see add_member()
 * @see query_member()
 * @see delete_member()
 */
int remove_member(string list_name, string mem) {
   int i;
   class mailing_list list = mailing_lists[list_name];
   
   if (file_name(previous_object()) != MAIL_ROOM) {
      return 0;
   }
   if (!classp(list)) {
      return 0;
   }
   if ((i = member_array(mem, list->members)) == -1) {
      return 0; /* not a member... */
   }
   list->members = delete(list->members, i, 1);
   if (!sizeof(list->members)) {
      delete_list(list_name);
   }
   save_me();
   return 1;
} /* remove_member() */

/**
 * This method removes a member from the list if they are either a
 * controller or a member.
 * @param list_name the list to remove the member from
 * @param mem the member to remove from the list
 * @return 1 on succes, 0 on failure
 * @see remove_member() 
 * @see remove_controller()
 */
int delete_member(string list_name, string mem) {
   class mailing_list list = mailing_lists[list_name];

   SECURITY
   if (!remove_member(list_name, mem)) {
      return 0;
   }
   remove_controller(list_name, mem);
   if (!sizeof(list->members)) {
      delete_list(list_name);
   }
   save_me();
   return 1;
} /* delete_member() */
