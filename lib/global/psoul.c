#include "player.h"
/* the bit of the soul that all players should have ;) */

inherit "global/alias";
inherit "/global/nickname";

#define SOUL_OBJECT "/obj/handlers/soul"

static int time_left,    /* Time left for this round. */
           *queue_depth, /* Depth on yhe queu of the current lot of commands */
           doing_it,
           last_command;
static string *queued_commands,
              *in_command;
static mixed *interupt;

void create() {
  time_left = ROUND_TIME;
  queued_commands = ({ });
  queue_depth = ({ 0 });
  in_command = ({ });
} /* create() */

void no_time_left() {
  time_left = -ROUND_TIME;
} /* no_time_left() */

void soul_commands() {
  add_action("nosoul","nosoul",1);
  add_action("soul_com","*");
  add_action("lower_check","*", -10000);
  add_action("drunk_check","*", 10000);
  add_action("soul_force_thingy", "SOUL_FORCE", 20000);
  this_object()->add_alias("soul");
  alias_commands();
  nickname_commands();
  history_commands();
} /* soul_commands() */

int nosoul(string str) {
  if (str == "on")
    this_object()->add_property("nosoul",1);
  else if (str == "off")
    this_object()->remove_property("nosoul");
  else
    if (this_object()->query_property("nosoul"))
      this_object()->remove_property("nosoul");
    else
      this_object()->add_property("nosoul",1);
  if (this_object()->query_property("nosoul"))
    write("Soul turned off.\n");
  else
    write("Soul turned on.\n");
  return 1;
} /* nosoul() */

int soul_force_thingy(string str) {
  string str1, str2;

  if (sscanf(str,"%s %s", str1, str2) != 2)
    str1 = str;
  if (!this_object()->query_property("nosoul")) {
    if (!find_object(SOUL_OBJECT))
      if (catch(call_other(SOUL_OBJECT, "??"))) {
        write("Soul errors!  Notify a wiz at once.\n");
        write("Use nosoul to turn the soul back on when it is fixed.\n");
        this_object()->add_property("nosoul",1);
        return 0;
      }
    SOUL_OBJECT->soul_command(str1,str2);
  }
  return 1;
} /* soul_force_thingy() */

int do_soul_force(string str) {
  return command("SOUL_FORCE "+str);
} /* do_soul_force() */

int soul_com(string str) {
  string str1, str2;
  int i;
  if (sscanf(str,"%s %s", str1, str2) != 2)
    str1 = str;
/*
  str2 = expand_nickname(str2);
/* Ths soul may be crashing it... so lets find out! */
  if (exec_alias(str1, str2))
    return 2;
  if (!this_object()->query_property("nosoul")) {
    if (!find_object(SOUL_OBJECT))
      if (catch(call_other(SOUL_OBJECT, "??"))) {
        write("Soul errors!  Notify a wiz at once.\n");
        write("Use nosoul to turn the soul back on when it is fixed.\n");
        this_object()->add_property("nosoul",1);
        return 0;
      }
    i=(int)SOUL_OBJECT->soul_command(str1,str2);
    if (i)
      return 1;
  }
  return 0;
} /* soul_com() */

int soul_com_force(string str) {
  string str1,str2;
  if (file_name(previous_object()) != SOUL_OBJECT)
    return 0;
  command(str);
  return 1;
  if (sscanf(str, "%s %s", str1, str2) != 2)
    str1 = str;
  SOUL_OBJECT->soul_command(str1, str2);
} /* soul_com_force() */

void do_soul(string str, mixed bing) {
  say(str, bing);
} /* do_soul() */

int query_time_left() { return time_left; }

int adjust_time_left(int i) { return time_left += i; }

void do_flush(int first) {
  int i;
  string str;
  mixed *bing, *b2;

  if (time_left < 0 || !sizeof(queued_commands)) return ;
  if (!first) {
    bing = in_command;
    b2 = queue_depth;
    str = queued_commands[0];
    queued_commands = queued_commands[1..1000];
    queue_depth = ({ 0 });
    in_command = ({ });
    doing_it = 1;
    catch(command(str));
    doing_it = 0;
    in_command = bing;
    queue_depth = b2;
    b2[0]--;
    return ;
  }
  for (i=0;i<2 && i<sizeof(queued_commands);i++)
    call_out("do_flush", 0, 0);
  call_out("do_flush", 1, 1);
} /* do_flush() */

/*
 * This should be called each heart beat..
 */
static void flush_queue() {
  if (time_left < 0)
    time_left += ROUND_TIME;
  else
    time_left = ROUND_TIME;
  remove_call_out("do_flush");
  do_flush(1);
  if (!sizeof(queued_commands)) {
/* Ok... check to see if an interupt was set up */
    if (interupt && time_left > 0) {
      catch(call_other(interupt[1], interupt[0], 0, interupt[2]));
      interupt = 0;
    }
    doing_alias = ([ ]);
    in_alias_command = 0;
    in_command = ({ });
    queue_depth = ({ 0 });
  }
} /* flush_queue() */

void set_interupt_command(string func, mixed ob, mixed arg) {
  interupt = ({ func, ob, arg });
} /* set_interupt_command() */

int drunk_check(string str) {
  last_command = time();
  if (sizeof(in_command) && in_command[0] == str)
    return 0;
  if (this_object()->query_property(PASSED_OUT_PROP)) {
    if (str == "quit")
      return 0;
    write("You have passed out, you cant do anything.\n");
    return 1;
  }
  if (str == "stop" || str == "restart") {
/* Need to add restart here as well.... */
    queued_commands = ({ });
    if (interupt && time_left < 0)
      catch(call_other(interupt[1], interupt[0], -time_left, interupt[2]));
    write("Removed queue.\n");
    time_left = 0;
    return 0;
  }
  if (time_left < 0 || (!doing_it && sizeof(queued_commands))) {
    write("Queued command "+str+".\n");
    if (str == "quit") {
      write("If you are trying to quit and it is queueing thingys, use "+
            "'stop' to stop your commands, and or restart to start "+
            "your heartbeat.\n");
    }
    queued_commands = queued_commands[0..queue_depth[0]-1]+ ({ str })+
                       queued_commands[queue_depth[0]..10000];
    queue_depth[0]++;
    return 1;
  }
  if (interupt)
    catch(call_other(interupt[1], interupt[0], 0, interupt[2]));
  interupt = 0;
  time_left -= DEFAULT_TIME;
  add_history(str);
  in_command = ({ str }) + in_command;
  if (!sizeof(queue_depth))
    queue_depth = ({ 0, 0 });
  else
    queue_depth = ({ queue_depth[0] }) + queue_depth;
  command(str);
  in_command = in_command[1..1000];
  queue_depth[1] = queue_depth[0];
  queue_depth = queue_depth[1..1000];
  if (interupt && time_left >= 0) {
    call_other(interupt[1], interupt[0], 0, interupt[2]);
    interupt = 0;
  }
  return 1;
} /* drunk_check() */

/* Utter failure... */
int lower_check(string str) {
  time_left += DEFAULT_TIME;
  return (str == "stop");
} /* lower_check() */
