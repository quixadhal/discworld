/*
 * this is the thingy that has all the standard commands that
 * players get.
 *
 * This keeps track of all the available commands in the mud with a path
 * and function attached to each one.
 *
 * When a player logs on the path and function is queryed off this object
 * so that even if a command changes location in the sccheme of all things
 * it can still be access without major hacks everywhere...
 */

mapping commands;

void create() {
  commands = ([
    "skills" : ({ "/std/commands/skills","skill_list" }),
    "judge" : ({ "/std/commands/judge", "judge" }),
    "ambush" : ({ "/std/commands/ambush", "ambush" }),
    "fix" : ({ "/std/commands/fix", "fix" }),
    "hide" : ({ "/std/commands/hide_in_shad", "hide_in_shadows" }),
    "sneak" : ({ "/std/commands/sneak", "sneak" }),
    "palm" : ({ "/std/commands/palm", "palm" }),
    "slip" : ({ "/std/commands/slip", "slip" }),
/* Wizard commands... */
    "remember" : ({ "/std/commands/remember", "remember" }),
    "forget" : ({ "/std/commands/forget", "forget" }),
/* As yet unwritten.
    "assess" : ({ "/std/commands/assess", "assess" }),
 */
  ]);
}

mapping query_commands() { return commands+([ ]); }

string *query_command_names() { return m_values(commands); }

mixed *query_command(string str) { return commands[str]+({ }); }
