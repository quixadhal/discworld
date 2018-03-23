/*  -*- LPC -*-  */
/*
 * $Id: logging.c,v 1.13 2002/05/05 17:55:54 wodan Exp $
 */
#include <type.h>
#include <runtime_config.h>

/*
#include <localtime.h>
varargs string timestamp(int the_time, int use_local) {
    string ret;
    string zone;
    mixed *bits;

    if(undefinedp(the_time))
        the_time = time();

    bits = localtime(the_time);
    if( !use_local ) {
        // Add GMT offset if we aren't using local time
        bits = localtime(the_time + bits[LT_GMTOFF]);
        zone = "GMT";
    } else {
        zone = bits[LT_ZONE];
    }
    ret = sprintf("%04d-%02d-%02d %02d:%02d:%02d %s",
            bits[LT_YEAR], bits[LT_MON]+1, bits[LT_MDAY],
            bits[LT_HOUR], bits[LT_MIN], bits[LT_SEC],
            zone);
    return ret;
}
*/

/*
 * returns the owner of ob (for error logging)
 * all calls to this should call get_prg_name()
 */
string get_wiz_name(mixed file) {
    if (!previous_object())
        return "root";
    if (objectp(file))
        file = file_name(file);
    file = explode(file, "/") - ({ "" });
    if (file[0] == "w") {
        if (file_size("/w/" + file[1]) != -2) {
            return "root";
        } else {
            return file[1];
        }
    }
    if (file[0] == "d") {
        return "dom";
    }
    return "root";
} /* get_wiz_name() */

string get_dom_name(mixed file) {
    if (objectp(file))
        file = file_name(file);
    file = explode(file, "/") - ({ "" });
    if (file[0] == "d" && sizeof(file) >= 2)
        return file[1];
} /* get_dom_name() */

/*
 * return the owner of file (for error logging)
 */
string get_prg_name(mixed file) {
    if (objectp(file))
        file = file_name(file);
    file = explode(file, "/") - ({ "" });
    if (file[0] == "w")
        return file[1];
    return "root";
} /* get_prg_name() */

int different(string fn, string pr) {
    sscanf(fn, "%s#%*d", fn);
    fn += ".c";
    return (fn != pr) && (fn != ("/" + pr));
}

string trace_line(object obj, string prog, string file, int line) {
    string ret;
    string objfn = obj ? file_name(obj) : "<none>";

    ret = objfn;
    if (different(objfn, prog))
        ret += sprintf(" (%s)", prog);
    if (file != prog)
        ret += sprintf(" in %s:%d\n", file, line);
    else
        ret += sprintf(" at line %d\n", line);
    return ret;
}

string printable_arg(mixed arg) {
    switch (typeof(arg)) {
        case ARRAY:
        case MAPPING:
        case CLASS:
        case STRING:
            if((strlen(typeof(arg)) + strlen(save_variable(arg)) + 4) <  __MAX_STRING_LENGTH__)
                return "(" + typeof(arg) + ") " + save_variable(arg);
            else
                return sprintf("(%s) <too large>", typeof(arg));
        default:
            return sprintf("(%s) %O", typeof(arg), arg);
    }
}

string trace_args(mixed *args) {
    string *tmp;

    if (!sizeof(args))
        return " (void)";
    tmp = map(args, (: printable_arg($1) :));
    return "\n(\n    "+implode(tmp, ",\n    ")+"\n)";
}

string trace_locals(mixed *args) {
    string *tmp;

    if (!sizeof(args))
        return " none.";
    tmp = map(args, (: printable_arg($1) :));
    return "\n    " + implode(tmp, ",\n    ");
}

varargs string standard_trace(mapping mp, int flag) {
    string ret;
    mapping *trace;
    int i, n;

    ret = mp["error"] + "Object: " +
        trace_line(mp["object"], mp["program"], mp["file"], mp["line"]);

    ret += "Arguments were:" + trace_args(mp["arguments"]) + "\n";
    ret += "Locals were:" + trace_locals(mp["locals"]) + "\n";
    ret += "\n\n";
    trace = mp["trace"];

    n = sizeof(trace);
    for (i = 0; i < n; i++) {
        /*  if( flag ) ret += sprintf("#%d: ", i); */
        ret += sprintf("(%d) '%s' in %s", i + 1, trace[i]["function"],
                trace_line(trace[i]["object"], trace[i]["program"],
                    trace[i]["file"], trace[i]["line"]));

        if (!flag && (strlen(ret) + strlen(trace_args(trace[i]["arguments"])) +
                    strlen(trace_locals(trace[i]["locals"])) +
                    20) < __MAX_STRING_LENGTH__) {
            ret += "Arguments were:" + trace_args(trace[i]["arguments"]) + "\n";
            ret += "Locals were:" + trace_locals(trace[i]["locals"]) + "\n";
        }
    }
    return ret;
}

#define MAX_SIZE 50000

void error_handler(mapping mp, int caught) {
    string logfile, ret, path, name, obname;
    object ob;

    logfile = caught ? "catch" : "runtime";
    ret = "--------------------\n" + timestamp() + ":  " + standard_trace(mp);

    // Catch invalid objects so we can blame them, instead of the loader.
    sscanf(mp["error"], "%*sError in loading object '%s'", obname);
    if (!obname) {
        ob = mp["object"];
        if (!ob) {
            obname = mp["program"];
        } else {
            obname = file_name(ob);
        }
    }

    // Figure out whose fault it was...
    name = get_wiz_name(obname);
    switch (name) {
        case "root":
            path = "/log/" + logfile;
            break;
        case "dom":
            // Special test to try and trace the player housing errors.
            if (obname[0..17] == "/d/am/short/flats/") {
                path = "/d/am/short/flats/" + logfile;
            } else {
                switch (obname[0..9]) {
                    case "/d/am/elm/" :
                    case "/d/am/lame" :
                        path = "/d/am/short/flats/" + logfile;
                        //path = "/d/"+implode(explode(obname, "/")[0..2], "/") + logfile;
                        break;
                    default :
                        break;
                }
            }
            path = "/d/"+get_dom_name(obname)+"/"+logfile;
            break;
        default:
            path = "/w/"+name+"/"+logfile;
            tell_creator(name, "A runtime error occured in the file %O"
                    ", logged to %s.\n", obname, path);
    }

    // Check to see if the autoload stuff is involved in this error.
    if (member_array("init_dynamic_arg", call_stack(2)) != -1 ||
            member_array("query_dynamic_auto_load", call_stack(2)) != -1) {
        path = "/d/admin/log/auto_" + logfile;
    }

    // Rotate log files that are too large.
    if (file_size(path) > MAX_SIZE) {
        unguarded((: rm, path+".old" :));
        unguarded((: rename, path, path+".old" :));
    }

    // Actually write to the chosen file.
    unguarded((: write_file, path, ret :));

    if (this_player(1) && find_object("/secure/simul_efun")) {
        this_player(1)->set_last_error(mp);
        if (!caught) {
            if (this_player(1)->query_creator()) {
                tell_object(this_player(1), mp["error"]+
                        "Object: "+trace_line(mp["object"], mp["program"],
                            mp["file"], mp["line"])+
                        "\nTrace written to "+ path +"\n");
            } else {
                tell_object(this_player(1),
                        "A runtime error occurred.\nPlease use "
                        "the \"bug\" command to report it, "
                        "describing what you tried to do when it happened.\n");
            }
        }
    }
}

/*
 * Write an error message into a log file. The error occurred in the object
 * 'file', giving the error message 'message'.
 */
void log_error(string file, string message) {
    string name, efile, epath, colour;

    if (sscanf(message, "%*sWarning:%*s") == 2) {
        if(sscanf(message, "%*sTrying to put%*s") == 2) {
            efile = "type-error";
            colour = "%^RED%^";
        } else {
            efile = "warnings";
            colour = "%^CYAN%^";
        }
    } else {
        efile = "error-log";
        colour = "%^RED%^";
    }
    if (this_player(1) && this_player(1)->query_creator()) {
        tell_object(this_player(1), colour+message+"%^RESET%^");
    }
    name = get_wiz_name(file);
    switch (name) {
        case "root":
            epath = "/log/" + efile;
            break;
        case "dom":
            epath = "/d/"+get_dom_name(file)+"/"+efile;
            break;
        default:
            epath = "/w/"+name+"/"+efile;
            break;
    }
    if (file_size(epath) > MAX_SIZE) {
        unguarded((: rm, epath+".old" :));
        unguarded((: rename, epath, epath+".old" :));
    }
    unguarded((: write_file, epath, message :));
} /* log_error() */

void do_log(string person, string text) {
    if (file_name(previous_object()) != "/std/smart_log")
        return;
    if (file_size("/w/"+person) != -2)
        return;
    unguarded((: rm, "/w/"+person+"/"+PLAYER_ERROR_LOG :));
    unguarded((: write_file, "/w/"+person+"/"+PLAYER_ERROR_LOG, text :));
} /* do_log() */

void forward_error(string file, string text) {
    if (!((file_name(previous_object()) == "/secure/cmds/creator/errors") ||
                (file_name(previous_object()) == "/www/secure/errors")))
        return;
    unguarded((: write_file, file, text :));
} /* forward_error() */
