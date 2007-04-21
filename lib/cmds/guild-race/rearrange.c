/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: rearrange.c,v 1.5 2002/03/18 09:43:24 terano Exp $
 * $Log: rearrange.c,v $
 * Revision 1.5  2002/03/18 09:43:24  terano
 * Made the message even less open to interpreation, as players are STILL complaining they left points int he pool accidently.
 *
 * Revision 1.4  2000/03/25 06:34:32  taffyd
 * Fixed typo.
 *
 * Revision 1.3  2000/02/22 01:28:11  sin
 * added some logging.
 *
 * Revision 1.2  1998/08/19 10:20:59  pinkfish
 * Make it inherit the correct base.
 *
 * Revision 1.1  1998/01/06 05:27:04  ceres
 * Initial revision
 * 
*/

#include <playerinfo.h>

#define ME_CON points[ this_player() ][ "me_con" ]
#define ME_DEX points[ this_player() ][ "me_dex" ]
#define ME_INT points[ this_player() ][ "me_int" ]
#define ME_STR points[ this_player() ][ "me_str" ]
#define ME_WIS points[ this_player() ][ "me_wis" ]
#define POOL points[ this_player() ][ "pool" ]

inherit "/cmds/guild_base";

mapping points = ([ ]);

int cmd();

void create() {
   ::create();
   set_command_name("rearrange");
} /* create() */

void do_save() {
   int player, here;
   player = this_player()->query_real_con() +
      this_player()->query_real_dex() +
      this_player()->query_real_str() +
      this_player()->query_real_int() +
      this_player()->query_real_wis();
   here = ME_CON + ME_DEX + ME_STR + ME_INT + ME_WIS;
   if (player != here) {
      log_file("BAD_REARRANGE",
         sprintf("%s: %s had %d now has %d\n",
            ctime(time()),
            this_player()->query_name(),
            player, here));
      PLAYERINFO_HANDLER->add_entry(
         this_object(),
         this_player()->query_name(),
         "misc",
         sprintf("rearranged from %d points to %d points", player, here),
         0);
   }
   this_player()->set_con( ME_CON );
   this_player()->set_dex( ME_DEX );
   this_player()->set_int( ME_INT );
   this_player()->set_wis( ME_WIS );
   this_player()->set_str( ME_STR );
   this_player()->remove_known_command( "rearrange" );
   map_delete(points, this_player());
} /* do_save() */

void display_stats() {
   printf( "Con: %d Dex: %d Int: %d Str: %d Wis: %d Pool: %d\n",
         ME_CON, ME_DEX, ME_INT, ME_STR, ME_WIS, POOL );
} /* display_stats() */

int get_stat_bit( string word ) {
   int num;
   string stat;
   word = lower_case( word );
   if ( word == "save" ) {
      if ( POOL ) {
         write( "WARNING: You still have points left in your pool that "
		   "could be used to make your character more powerful. If you "
		   "leave these unspent now, you will not be able to get them again later. "
           "Are you sure you want to save? [y/n] " );
         input_to( "get_check" );
         return 1;
      }
      write( "Saving.\n" );
      do_save();
      return 1;
   }
   if ( word == "quit") {
      write( "Quitting.\n" );
      map_delete(points, this_player());
      return 1;
   }
   if ( word == "reset" ) {
      write( "Resetting to previous arrangement.\n" );
      return cmd();
   }
   if ( sscanf( word, "%s %d", stat, num ) != 2 )
      stat = word;
   if ( !stat || ( stat == "" ) )
      stat = "X";
   if ( num > POOL )
      if ( !POOL ) {
         write( "Cannot add to stat.  Need to add to the pool first.\n" );
         stat = "X";
      } else {
         write( "Pool has only "+ POOL +" points in it.  "+
               "Adding these to stat.\n" );
         num = POOL;
      }
   switch( stat[ 0 ] ) {
      case 'c' :
         if ( ( ME_CON + num ) < 8 ) {
            write( "Cannot set constitution to less than 8.\n" );
         } else
            if ( ( ME_CON + num ) > 23 ) {
               write( "Cannot set constitution to more than 23.\n" );
            } else {
               ME_CON += num;
               POOL -= num;
            }
         break;
      case 'd' :
         if ( ( ME_DEX + num ) < 8 ) {
            write( "Cannot set dexterity to less than 8.\n" );
         } else
            if ( ( ME_DEX + num ) > 23 ) {
               write( "Cannot set dexterity to more than 23.\n" );
            } else {
               ME_DEX += num;
               POOL -= num;
            }
         break;
      case 'i' :
         if ( ( ME_INT + num ) < 8 ) {
            write( "Cannot set intelligence to less than 8.\n" );
         } else
            if ( ( ME_INT + num ) > 23 ) {
               write( "Cannot set intelligence to more than 23.\n" );
            } else {
               ME_INT += num;
               POOL -= num;
            }
         break;
      case 's' :
         if ( ( ME_STR + num ) < 8 ) {
            write( "Cannot set strength to less than 8.\n" );
         } else
            if ( ( ME_STR + num ) > 23 ) {
               write( "Cannot set strength to more than 23.\n" );
            } else {
               ME_STR += num;
               POOL -= num;
            }
         break;
      case 'w' :
         if ( ( ME_WIS + num ) < 8 ) {
            write( "Cannot set wisdom to less than 8.\n" );
         } else
            if ( ( ME_WIS + num ) > 23 ) {
               write( "Cannot set wisdom to more than 23.\n" );
            } else {
               ME_WIS += num;
               POOL -= num;
            }
         break;
      default :
         break;
   }
   display_stats();
   write( "Enter [d|c|i|w|s|save|reset|quit] <num> : " );
   input_to( "get_stat_bit" );
   return 1;
} /* get_stat_bit() */

int get_check( string word ) {
   word = lower_case( word );
   if ( word[ 0 ] != 'y' ) {
      write( "Going back to stat entering.\n" );
      return get_stat_bit( "" );
   }
   do_save();
   write( "Saving.\n" );
   return 1;
} /* get_check() */

int cmd() {
   write( "Rearranging stats.\n\n" );
   points[ this_player() ] = ([ ]);
   ME_CON = (int)this_player()->query_real_con();
   ME_DEX = (int)this_player()->query_real_dex();
   ME_INT = (int)this_player()->query_real_int();
   ME_STR = (int)this_player()->query_real_str();
   ME_WIS = (int)this_player()->query_real_wis();
   POOL = 0;
   if ( ME_CON > 8)  {
      POOL += ME_CON - 8;
      ME_CON = 8;
   }
   if ( ME_DEX > 8) {
      POOL += ME_DEX - 8;
      ME_DEX = 8;
   }
   if ( ME_INT > 8 ) {
      POOL += ME_INT - 8;
      ME_INT = 8;
   }
   if ( ME_STR > 8 ) {
      POOL += ME_STR - 8;
      ME_STR = 8;
   }
   if ( ME_WIS > 8 ) {
      POOL += ME_WIS - 8;
      ME_WIS = 8;
   }
   write( "Type \"c 3\" to add 3 points to your constitution from the pool, "+
         "or \"c -4\" to remove 4 points from constitution and add them to "+
         "the pool.  Use \"save\" to save the current state and quit.  Note "+
         "that \"%^YELLOW%^reset%^RESET%^\" will return you to your "+
         "starting position, while \"%^YELLOW%^quit%^RESET%^\" will quit "+
         "without saving any changes.\n\n" );
   return get_stat_bit( "" );
} /* cmd() */

int teach( object thing ) { return 0; }

/* This is removed form here as documetation exists elswehere
 *( Which is a duplication of this.
string help() {
   return "Syntax: rearrange\n\n"
      "Once during the life of your character you may modify your stats for "
      "free.  Stats normally take values between 8 and 23 and you can "
      "distribute your stats as you choose by typing \"rearrange\" and "
      "following the instructions.  The idea of this command is to give your "
      "character unique strengths.  You should read the help on stats to "
      "find out what each one does and carefully consider your character's "
      "future career as a member of one of the " + mud_name() + " guilds before "
      "using this command.\n\n"
      "See also:\n"
      "    score\n";
} p* help() */

int clean_up() {
  if (sizeof(points))
    return 1;
  ::clean_up();
}

void reset() {
  if (sizeof(points))
    return;
  ::reset();
}

string query_name()
{
  return "Rearrange Command";
}
