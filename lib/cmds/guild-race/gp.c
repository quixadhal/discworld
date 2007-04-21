/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: gp.c,v 1.3 1998/08/19 10:16:36 pinkfish Exp $
 * $Log: gp.c,v $
 * Revision 1.3  1998/08/19 10:16:36  pinkfish
 * Add in stuff to set the command name.
 *
 * Revision 1.2  1998/03/13 16:43:58  pinkfish
 * Make it use the new system of guild help.
 *
 * Revision 1.1  1998/01/06 05:27:04  ceres
 * Initial revision
 * 
*/
/*
 * Original by Manshoon of Newmoon in February 1995
 * Rewritten for Discworld by Deutha in March 1995
 */

/*
 * This command shows a player how many of their guild points are
 * available for use for different commands. With commands using
 * query_specific_gp( string type ) instead of the straight query_gp()
 * the various stupid paradoxes of the old system will be avoided by
 * making sure that a player's available points for a given command
 * depend on the points skill most relevant.
 */

#include <skills.h>

inherit "/cmds/guild_base";

void create() {
   ::create();
   set_nroff_file("gp");
   set_command_name("gp");
} /* create() */

int cmd() {
   int i, gp, max_gp, other_gp;
   gp = (int)this_player()->query_gp();
   max_gp = (int)this_player()->query_max_gp();
   write( "You have "+ gp +" ("+ max_gp +") guild points.\n" );
   for ( i = 0; i < sizeof( STD_SKILLS ); i += SKILL_ARR_SIZE ) {
      other_gp = (int)this_player()->query_skill_bonus( STD_SKILLS[ i ]
            +".points" ) + 50;
      if ( other_gp >= max_gp ) {
         write( "* All of your guild points can be used for "+
               STD_SKILLS[ i ] +" commands.\n" );
         continue;
      }
      if ( gp + other_gp <= max_gp ) {
         write( "* You cannot use any guild points for "+
               STD_SKILLS[ i ] +" commands until\n    your points "+
               "reach "+ ( max_gp - other_gp ) +".\n" );     
         continue;
      }
      write( "* You can use "+ ( gp + other_gp - max_gp ) +" ("+ other_gp +
            ") for "+ STD_SKILLS[ i ] +" commands.\n" );
   }
   return 1;
} /* cmd() */

/*
string help() {
   return "Syntax: gp\n\n"
         "Guild points determine how many times you are able to "
         "perform some action, often using commands or spells given "
         "to you by your guild or other guilds (hence the name).  "
         "The number of guild points that you have depends on your "
         "skill bonus in one of the points skills; for example, a "
         "wizard's guild points will be determined by his bonus in "
         "magic.points, while a fighter's will be determined by "
         "fighting.points.  However, a wizard will not just use "
         "magical commands nor a fighter just use fighting commands, "
         "and the number of times you are able to perform a given "
         "command, just like how well you are able to perform it, "
         "should depend on the most relevant skill, independent of "
         "your guild.  The command \"gp\", then, shows you how many "
         "of your guild points you have available for commands in "
         "each skill category using a system that avoids needing "
         "one type of guild points for every skill category.\n\n"
         "Example:\n\n"
         "> gp\n"
         "You have 276 (338) guild points.\n"
         "* You can use 13 (75) guild points for covert commands.\n"
         "* You cannot use any guild points for faith commands until\n"
         "    your points reach 288.\n"
         "* You can use 245 (307) guild points for fighting commands.\n"
         "* All of your guild points can be used for magic commands.\n\n"
         "See also:\n"
         "  commands, skills, spells\n\n"
         "Credit goes to Manshoon of Newmoon for this solution to "
         "the guild points problem.\n";
} * help() */

/* accompanying function for in /std/living/health.c

int query_specific_gp( string gp_type ) {
   int other_gp;
   if ( !gp_type )
      return 0;
   other_gp = (int)this_object()->query_skill_bonus( gp_type
         +".points" ) + 50;
   if ( other_gp >= max_gp )
      return gp;
   if ( gp + other_gp <= max_gp )
      return 0;
   return gp + other_gp - max_gp;
} query_specific_gp() */
