/*  -*- LPC -*-  */
/*
 * $Locker: wodan $
 * $Id: simul_efun.c,v 1.15 2000/10/19 13:22:08 taffyd Exp wodan $
 * 
*/
/* The simul_efun object. Please keep the list in alphabetical order! -*- LPC -*- */
/**
 * The simul_efun object.  This object allows you to create functions which
 * apear to act like an efun.  An efun is a function which is built into
 * the driver.  For functions which are on objects you need to specify the
 * the object name when you call them. 
 * ie: "/obj/handlers/autodoc/autodoc_handler"->add_file(frog);
 * With a simuluated efun, you would not need to specifiy a file name.  This is
 * how the functions like find_match and so on work.
 * @author Pinkfish
 */

inherit "/secure/simul_efun/add_a";
#if !efun_defined(add_action)
inherit "/secure/simul_efun/add_action";
#endif
inherit "/secure/simul_efun/add_command";
inherit "/secure/simul_efun/alt_move";
inherit "/secure/simul_efun/amtime";
inherit "/secure/simul_efun/array";
inherit "/secure/simul_efun/ctime_elapsed";
inherit "/secure/simul_efun/back_trace";
#if !efun_defined(dump_socket_status)
inherit "/secure/simul_efun/dump_socket_status";
#endif
inherit "/secure/simul_efun/extract";
inherit "/secure/simul_efun/find_match";
inherit "/secure/simul_efun/find_member";
inherit "/secure/simul_efun/find_other_call_out";
inherit "/secure/simul_efun/get_function_pointer";
inherit "/secure/simul_efun/inside_shorts";
inherit "/secure/simul_efun/mapping";
/*
 * NB: Because of the fact this file uses it, base_name is
 * inherited into modified_efuns.
 */
inherit "/secure/simul_efun/modified_efuns";
// THis in now inherited through the parser.
//inherit "/secure/simul_efun/multiple_short";
inherit "/secure/simul_efun/pk_check";
inherit "/secure/simul_efun/pl_to_ob";
inherit "/secure/simul_efun/process_value";
inherit "/secure/simul_efun/query_ident";
inherit "/secure/simul_efun/query_number";
inherit "/secure/simul_efun/roll_MdN";
#if !efun_defined(shuffle)
inherit "/secure/simul_efun/shuffle";
#endif
inherit "/secure/simul_efun/snoop_simul";
inherit "/secure/simul_efun/sqrt";
inherit "/secure/simul_efun/strip_colours";
inherit "/secure/simul_efun/str_inven";
inherit "/secure/simul_efun/debug";
inherit "/secure/simul_efun/thing_to_string";
inherit "/secure/simul_efun/unguarded";
inherit "/secure/simul_efun/virtual";

void create() {
  seteuid("Root");
  find_match::create();
}
