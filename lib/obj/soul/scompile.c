#include "soul.h"

static string  chunk;
static mapping soul_data;

void error(string msg) {
     write(msg + "\n");
     if (chunk) {
	 write(" at " + chunk[0 .. 70] + "\n");
     }
     destruct(this_object());
}

varargs void
update_commands(string file, mapping soul_mapping) {
    string soul_definition, *soul_cmds;
    /*
     * This is horrible. The alternatives are worse.
     */
    soul_definition = read_file(file);
    if (soul_definition) {
	soul_data = soul_mapping;
	soul_definition = implode(explode(soul_definition, "\t"), "");
	soul_cmds = explode(soul_definition, "\n");
	soul_cmds = map_array(soul_cmds, "remove_indentation", this_object());
	soul_cmds = explode(implode(soul_cmds, ""), "$$");
	filter_array(soul_cmds, "distribute_decoding", this_object());
    } else {
	error("read_file("+file+") failed");
    }
}

string remove_whitespace(string str) {
    while (sscanf(str, "    %s", str));
    sscanf(str, "  %s", str);
    sscanf(str, " %s", str);
    return str;
}

string remove_indentation(string str) {
    str = remove_whitespace(str);
    if (!(['(' : 1, '[' : 1, '*' : 1])[str[0]]) {
	return str;
    }
    return " " + str;
}

void distribute_decoding(string chunk) {
    call_out("define_action", 0, chunk);
}

mixed parse_chunk(); /* Arrrgh primal scream hate prototypes */

void parse_assoc(mapping map) {
    string index;
    mixed  value;

    /*
     * An assoc is on the form (index value) where only value may recurse.
     */

    if (sscanf(chunk, "(%s %s", index, chunk) < 2) {
        error("parse_assoc() failed at sscanf()");
    }
    value = parse_chunk();
    chunk = extract(chunk, 1);
    if (map == soul_data) {
	write ("Adding soul_command : " + index + ".\n");
    }
    map[index] = value;
}

string *parse_array() {
    string values;

    /*
     * The array is of format [val1 val2 val3] where none may recurse.
     */
     if (sscanf(chunk, "[%s]%s", values, chunk) != 2) {
         error("parse_array() failed at sscanf()");
     }
    return map_array(explode(values, " "), "fix_macros", this_object());
}

mixed fix_macros(string foo) {
    if (foo[0] == '$') {
	switch(foo) {
	  case "$ADJ"        : return ADJ;
	  case "$TP_NAME"    : return TP_NAME;
	  case "$OB_NAME"    : return OB_NAME;
	  case "$TP_PRONOUN" : return TP_PRONOUN;
	  case "$OB_PRONOUN" : return OB_PRONOUN;
	  case "$TP_POSS"    : return TP_POSSESSIVE;
	  case "$OB_POSS"    : return OB_POSSESSIVE;
          case "$TP_NPOSS"   : return TP_NPOSS;
          case "$OB_NPOSS"    : return OB_NPOSS;
          case "$OB_FOO"     : return OB_FOO;
          case "$TP_FOO"     : return TP_FOO;
	}
    }
    return foo;
}

mapping parse_mapping() {
    mapping map;
    int     i;

    /*
     * The mapping is of format *( assoc assoc assoc )
     * Here we must singlestep and trust the other routines to be robust!
     */

    map = ([ ]);

    if (!sscanf(chunk, "*(%s", chunk)) {
	error("sscanf in parse_mapping failed");
    }
    while ((chunk = remove_whitespace(chunk))[0] != ')') {
	parse_assoc(map);
    }
    chunk = extract(chunk, 1);
    return map;
}

mixed parse_chunk() {
    string value;
    chunk = remove_whitespace(chunk);
    switch(chunk[0]) {
        case '*' : return parse_mapping();
        case '[' : return parse_array();
        case '(' : error("parse_assoc() wanted from parse_chunk()");
    }
    if (sscanf(chunk, "%s)%s", value, chunk) == 2) {
	return value;
    }
    error("sscanf() failed in parse_chunk");
}

void define_action(string cmd_def) {
    chunk = remove_whitespace(cmd_def);
    parse_assoc(soul_data);
}
