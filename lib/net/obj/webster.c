/* Ick I hate // comments.
 * Converted to this illustrious mudlib by Pinkfish.
 *
 * file:   obj/webster.c
 * author: Truilkan@Basis
 * date:   1992/10/29
 * mudlib: Basis
 *
 * if you choose to use this code in your mud, please retain the above
 * header.  if you write a help screen for this object, please give credit
 * to the original authors (Truilkan and Jacques)
 *
 * This object obeys part of the RFC for dictionary lookups.  It provides
 * an LPC object frontend (in the form of a dictionary object) to an online
 *  dictionary server.
 *
 * todo: parse the first line returned in order to be smart about
 * interpreting results of a query (successful, failed, etc).
 */

#include <socket.h>

#define DISCONNECTED "dictionary (closed)"
#define CONNECTED "dictionary (open)"

inherit "/std/socket/telnet";
inherit "/std/object";

void create() {
  telnet::create();
  object::create();
} /* create() */

void setup() {
  set_name("dictionary");
  add_alias(({ "book", "webster" }));
  set_short(DISCONNECTED);
  set_long("Its a fine dictionary with faded gold embossing.\n"
  + "You could probably do all manner of things with it such as defining,\n"
  + "completing, and spelling words.\n");
  set_verbosity(0);
} /* setup() */

void handler(string event) {
  switch (event) {
    case "open" :
      set_short(CONNECTED);
      tell_object(environment(this_object()),
        "The dictionary creaks open.\n");
      break;
    case "close" :
      tell_object(environment(this_object()),
        "The dictionary slams shut!\n");
      set_short(DISCONNECTED);
      break;
    default:
      break;
  }
} /* handler() */

int dlookup(string arg) {
  int result;

  if (!query_connected()) {
    notify_fail("The dictionary isn't open!\n");
    return 0;
  }
  say((string)this_player()->query_cap_name() + " looks up a word.\n");
  telnet::send("DEFINE " + arg + "\n");
  return 1;
} /* dlookup() */

int do_open() {
  if (query_connected()) {
    return 0;
  }
/*
  say((string)this_player()->query_cap_name() + " opens "
    + this_player()->query_possessive() + " dictionary.\n");
 */
  call_out("do_close", 5*60*60);
  return telnet::connect("129.79.254.191 2627");
} /* do_open() */

int do_close() {
  if (!query_connected()) {
    return 0;
  }
/*
  if (this_player())
    say((string)this_player()->query_cap_name() + " closes "
      + this_player()->query_possessive() + " dictionary.\n");
 */
  remove_call_out("do_close");
  return telnet::disconnect();
} /* do_close() */

int dskim(string arg) {
  int result;

  if (!query_connected()) {
    notify_fail("The dictionary isn't open!\n");
    return 0;
  }
  say((string)this_player()->query_cap_name() + " skims the dictionary.\n");
  telnet::send("ENDINGS " + arg + "\n");
  return 1;
} /* dskim() */

int dspell(string arg) {
  int result;

  if (!query_connected()) {
    notify_fail("The dictionary isn't open!\n");
    return 0;
  }
  say((string)this_player()->query_cap_name() + " searches for a word.\n");
  telnet::send("SPELL " + arg + "\n");
  return 1;
} /* dspell() */

int dcomplete(string arg) {
  int result;

  if (!query_connected()) {
    notify_fail("The dictionary isn't open!\n");
    return 0;
  }
  say((string)this_player()->query_cap_name() + " searches for a word.\n");
  telnet::send("COMPLETE " + arg + "\n");
  return 1;
} /* dcomplete() */

void init() {
  add_action("dlookup", "define");
  add_action("dskim", "endings");
  add_action("dspell", "spell");
  add_action("dcomplete", "complete"); 
  this_player()->add_command("open", this_object());
  this_player()->add_command("close", this_object());
} /* init() */

void recieve_message(string str) {
  say(str);
} /* recievr_message() */
