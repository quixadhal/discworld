/*
 * Default language functions used by parse_command() in non -o mode
 */
string *parse_command_id_list() {
  return ({ "one", "thing" });
} /* parse_command_id_list() */

string *parse_command_plural_id_list() {
  return ({ "ones", "things", "them" });
} /* parse_command_plural_id_list() */

string *parse_command_adjectiv_id_list() {
  return ({ "the", "a", "an" });
} /* parse_command_adjectiv_id_list() */

string *parse_command_prepos_list() {
  return ({ "in", "on", "at", "along", "upon", "by", "under", "behind",
            "beside", "into", "onto", "inside", "within", "from" });
} /* parse_command_prepos_list() */

string parse_command_all_word() {
  return "all";
} /* parse_command_all_word() */
