#include "soul.h"

mapping soul_data;

mapping query_soul_data() { return soul_data; }

void do_save() { save_object(SAVE_FILE); }

mixed *
map(mixed *arr, string fun) {
    return map_array(arr, fun, this_object());
}

void compile(string source_file) {
    COMPILER->update_commands(SOUL_DEF_DIR + "/" + source_file, soul_data);
}

void create() {
    int i;
    string *files;
    seteuid((string)"/secure/master"->creator_file(file_name(this_object())));
    if (!restore_object(SAVE_FILE)) {
	soul_data = ([ ]);
	files = get_dir(SOUL_DEF_DIR+"/");
	for (i = 0; i < sizeof(files); i ++) {
	    compile(files[i]);
	}
    }
}

/*
 * The soul parsing stuff.
 */

object *match_living(string name) {
    object ob, *obs;
    if (!name || name == "") return ({ });
    if (name == "everyone") {
	return (filter_array(users(), "is_vis", this_object()) -
		({ this_player() }));
    }
    if (sizeof(obs = find_match(name, environment(this_player())))) {
	return (filter_array(obs,
				"is_alive_and_vis", this_object()) -
		({ this_player() }));
    }
    ob = find_living(lower_case(name));
    if (ob) {
	return ({ ob });
    }
    return ({ });
}

status is_vis(object ob) {
    return (ob->short() != 0) && (environment(ob) != 0);
}

status is_alive_and_vis(object ob) { return living(ob) && is_vis(ob); }

varargs string match_adj(string scanfor, mapping data1, mapping data2) {
    string foo, bar;
    foo = S(data1["adverbs"]);
    bar = S(data2["adverbs"]);
    if (scanfor == "")
      if (bar[0] == '#' && strlen(bar) > 2)
        bar = bar[2..1000];
    if (foo[0] == '#' || bar[0] == '#') {
	return scanfor;
    }
    if (sscanf("," + bar + "," + foo + ",",
	       "%s," + scanfor + "%s,", foo, bar)) {
        if (bar && strlen(bar)) {
            return scanfor + bar;
        }
        return scanfor;
    }
    return 0;
}

string  adj;
object *target;

status cmd_soul(string verb, string arg) {
    mapping data, info;
    string *args;

    data = soul_data[verb];

    if (!data) {
        return 0;
    }

    target = ({ });

    if (!arg) {
	info = data["noargs"];
	adj = "nothing";
	args = ({ arg = "" });
    }
    if (!info) {

	if (!args) {
	    args = explode(arg, " ");
	}

	if (data["prepositions"]) {
	    args = args - (string *)data["prepositions"];
	}

	if (sizeof(args) == 1) {
	    /*
	     * We have either a living name or an adjective.
	     */

	    target = match_living(arg);

	    if (!sizeof(target)) {
		info = data["undirected"];
		if (!info) {
		    write("You must do that to somebody.\n");
		    return 1;
		}
		adj = match_adj(arg, data, info);
	    } else {
		info = data["directed"];
		if (!info) {
		    write ("You can't " + verb + " at someone.\n");
		    return 1;
		}
		adj = match_adj("", data, info);
	    }
	} else {
	    target = match_living(args[0]);
	    if (sizeof(target)) {
		info = data["directed"];
		if (!info) {
		    write ("You can't " + verb + " at someone.\n");
		    return 1;
		}
		adj = match_adj(implode(args[1 .. 1000], " "), data, info);
	    } else {
		target = match_living(args[sizeof(args) - 1]);
		if (sizeof(target)) {
		    info = data["directed"];
		    if (!info) {
			write ("You can't " + verb + " at someone.\n");
			return 1;
		    }
		    adj = match_adj(implode(args[0 .. sizeof(args) - 2], " "),
				    data, info);
		} else {
		    info = data["undirected"];
		    if (!info) {
			write("You must do that to somebody.\n");
			return 1;
		    }
		    adj = match_adj(arg, data, info);
		}
	    }
	}
    }
    if (!adj) {
	write ("I don't know how to " + verb + " that way!\n");
	return 1;
    }
    RELAYER->transmit(info, adj, target);
    return 1;
}
