/**
 * Addalt - adds a record about a player's alt character.
 * @author Fiona
 */
#include <creator.h>
#include <playerinfo.h>

inherit "/cmds/base";

// Command entry point
int add_alt(string s1, string s2) {
  string *alts, alt;

  s1 = this_player()->expand_nickname(lower_case(s1));
  alts = explode(lower_case(s2)," ") - ({ "" }) - ({ 0 });

  foreach(alt in alts)
    write(PLAYERINFO_HANDLER->add_alt(this_player(), s1, alt));

  return 1;
}

// Answer the command patterns
mixed *query_patterns()
{
    return ({
        "<word'player-name'> <string'alt-name alt-name ...'>",
        (: add_alt($4[0],$4[1]) :)
    });
}
