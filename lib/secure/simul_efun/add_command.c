/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: add_command.c,v 1.5 1999/04/20 20:30:03 pinkfish Exp $
 * $Log: add_command.c,v $
 * Revision 1.5  1999/04/20 20:30:03  pinkfish
 * Add a check to make sure the current player object is valid
 *
 * Revision 1.4  1999/01/23 18:23:02  terano
 * Added a reference to add_command_conversion for arby
 *
 * Revision 1.3  1998/03/24 07:28:59  pinkfish
 * Add some docs to make the simulefuns more usefully lookupable.
 *
 * Revision 1.2  1998/02/01 00:59:14  presto
 * No changes
 *
 * Revision 1.1  1998/01/06 05:13:15  ceres
 * Initial revision
 * 
*/
/*
 * A simul_efun for add_command to make life slightly easier.
 */
/**
 * Cotains some simul_efuns.
 * @author Pinkfish
 */
/**
 * A simul_efun for add_command.  This allows you to call add_command without
 * having to go this_player()->add_command.  This will only work on non-living
 * objects.
 *
 * @param verb the verb to add
 * @param pattern the pattern associated with the verb
 * @param func the funvction to call on success
 *
 * @see /global/new_parse->add_command()
 * @see help add_command_conversion
 */
varargs void add_command(string verb, string pattern, function func) {
   if (!verb) {
      write("Need to specify a verb for add_command.\n");
      return ;
   }
   if (!pattern) {
      write("Need to specify a pattern for add_command.\n");
   }
   if (this_player()) {
      this_player()->add_command(verb, previous_object(), pattern, func);
   }
} /* add_command() */

/**
 * An interface to add_succeeded_mess.  This is added to make life easier,m
 * you do not need to call this_player()->add_succeeded_mess() if you are
 * writing add_command code in a non-living object.
 *
 * @param mess the message to print
 * @param indir the indirect objects the message is for
 */
void add_succeeded_mess(mixed mess, object *indir) {
   if (!indir) {
     indir = ({ });
   }
   this_player()->add_succeeded_mess(previous_object(), mess, indir);
} /* add_succeeded_mess() */

/**
 * An interface to add_succeeded.  This is added to make life easier,m
 * you do not need to call this_player()->add_succeeded() if you are
 * writing add_command code in a non-living object.
 *
 * @param ob the object to add as succeeded
 * @see /global/new_parse->add_succeeded()
 */              
void add_succeeded_ob(object ob) {
   this_player()->add_succeeded(ob);
} /* add_succeeded_ob() */

/**
 * An interface to add_failed_mess.  This is added to make life easier,m
 * you do not need to call this_player()->add_failed_mess() if you are
 * writing add_command code in a non-living object.
 *
 * @param mess the message to print
 * @param indir the indirect objects the message is for
 *
 * @see /global/new_parse->add_failed_mess()
 */              
void add_failed_mess(mixed mess, object *indir) {
   if (!indir) {
     indir = ({ });
   }
   this_player()->add_failed_mess(previous_object(), mess, indir);
} /* add_failed_mess() */

