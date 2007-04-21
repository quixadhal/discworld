inherit "/cmds/living/l_ook";

mixed *query_patterns() {
    return ({ "<string'direction'>", (: cmd_string($4[0]) :),
              "<indirect:object>", (: cmd_object($1, $4[0]) :) });
} /* query_patterns() */
