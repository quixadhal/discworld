#include "soul.h"

static string global_adj;
static string *their_msg;
static string *ob_msg;
static object current_you;
static object *all_targets;
static string *force_cmds;

string filter_stuff(mixed *parts, object *targets) {
    parts = map_array(parts, "handle_part", this_object(), targets);
    parts = parts - ({ "", 0 });
    return implode(parts, " ");
}

string compile_message(mixed *parts, object *targets) {
    return filter_stuff(parts, targets) + ".\n";
}

status remove_zeroes(string foo) { return foo != 0; }

void transmit(mapping info, string adj, object *ob) {
    mixed *ob_by_env;

    global_adj = adj;

    /*
     * Three parts : Tell the player, tell the target(s), tell the others.
     * The first part can be done here. The others in the filter().
     */

    current_you = 0;
    tell_object(this_player(), compile_message(info["mymsg"], ob));

    if (!ob || !sizeof(ob)) {
	say(compile_message(info["theirmsg"], 0));
    } else {
	force_cmds  = info["force"];
	their_msg   = info["theirmsg"];
	ob_msg      = info["obmsg"];
	all_targets = ob;
	ob_by_env = unique_array(ob, "query_current_room");
	filter_array(ob_by_env, "handle_room", this_object());
    }
}

object env_for_ob(object ob) { return environment(ob); }

void handle_room(object *ob) {
    /*
     * This is an array of living thingies in ONE environment.
     * Give the ob's the proper text, and then does a tell_room with
     * the ob's excluded, in case other people are watching.
     */

    if (environment(ob[0]) == environment(this_player())) {
	tell_room(environment(ob[0]),
		  compile_message(their_msg, all_targets),
		  ob + ({ this_player() }) );
	filter_array(ob, "handle_player", this_object(), all_targets);
    } else {
	tell_room(environment(ob[0]),
		  compile_message(their_msg, ob),
		  ob + ({ this_player() }) );
	filter_array(ob, "handle_player", this_object(), ob);
    }
}

void handle_player(object target, object *ob) {
    if (force_cmds) {
	string cmd;
	cmd = lower_case(filter_stuff(force_cmds, ({ this_player() })));
	call_out("force_foo", 2, ({ target, cmd }));
    }
    current_you = target;
    if (ob_msg) {
	tell_object(target, compile_message(ob_msg, ob));
    } else {
	tell_object(target, compile_message(their_msg, ob));
    }
}

void force_foo(mixed *data) {
    command(data[1], data[0]);
}

string name_of_ob(object ob) {
    if (ob == current_you) {
	return "you";
    }
    return (string)ob->query_cap_name();
}

string compile_names(object *ob) {
    if (sizeof(ob) == 1) {
	return name_of_ob(ob[0]);
    }
    return (implode(map_array(ob[1 .. 1000], "name_of_ob", this_object()), ", ") +
	    " and " + name_of_ob(ob[0]));
} 

string make_nposs(object target) {
    string poss, nam;
    nam = name_of_ob(target);
    if (nam == "you")
      return nam;
    poss = (string)target->query_name();
    switch (poss[strlen(poss) - 1]) {
      case 's' : case 'x' : case 'z' : return nam + "'";
    }    return nam + "'s";
}

string handle_part(mixed part, object *targets) {
    if (stringp(part)) {
	return part;
    } else {
	switch(part) {
	  case ADJ           : return global_adj;

	  case TP_NAME       : return (string)TP->query_cap_name();
	  case TP_PRONOUN    : return (string)TP->query_pronoun();
	  case TP_POSSESSIVE : return (string)TP->query_possessive();
          case TP_NPOSS      : return make_nposs(this_player());
	  case TP_FOO        : return TP->query_objective() + "self";

	  case OB_NAME       : return compile_names(targets);
	  case OB_PRONOUN    : return (string)OB0->query_pronoun();
	  case OB_POSSESSIVE : return (string)OB0->query_possessive();
	  case OB_NPOSS      : return make_nposs(OB0);
	  case OB_FOO        : return OB0->query_objective() + "self";
	}
    }
    return part + "";
}
