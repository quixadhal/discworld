/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: cleanup.c,v 1.1 1998/01/06 05:11:09 ceres Exp $
 * $Log: cleanup.c,v $
 * Revision 1.1  1998/01/06 05:11:09  ceres
 * Initial revision
 * 
*/
/* command, trial out by Turrican for a commands daemon. */

inherit "/cmds/base";
inherit "/cmds/speech";

mixed cmd(string mess) {

	if("/secure/bulk_delete"->delete_files(mess))
		write("Ok.\n");
	else
		return notify_fail("Sorry.\n");
}
