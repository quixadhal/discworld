/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: stats.c,v 1.18 2003/05/06 17:27:47 pinkfish Exp $
 * $Log: stats.c,v $
 * Revision 1.18  2003/05/06 17:27:47  pinkfish
 * Update to allow 0 gp_inc values.
 *
 * Revision 1.17  2003/04/04 03:52:40  ceres
 * Forcibly released due to inactivity
 *
 * Revision 1.16  2002/09/02 08:07:30  ceres
 * Fixed to call do_burden_call() when tmp_str is altered
 *
 * Revision 1.15  2002/08/13 20:24:33  ceres
 * Fixed bug with negative hp regen rates
 *
 * Revision 1.14  2002/08/03 23:56:51  ceres
 * Got it wrong first time round
 *
 * Revision 1.12  2002/03/16 01:05:36  pinkfish
 * Fix up some things with undieing.
 *
 * Revision 1.11  2001/05/11 15:56:28  taffyd
 * Reworked heart_beat() again to support callingdeath to prevent NPCs dying improperly.
 *
 * Revision 1.10  2001/04/25 12:49:51  wodan
 * changed query_weight to match the calls to it
 *
 * Revision 1.9  2001/02/21 06:31:36  sin
 * Fixed the double-death AND the undead NPC bug
 *
 * Revision 1.8  2000/06/15 08:56:28  pinkfish
 * Fix up to use the config.
 *
 * Revision 1.7  2000/06/15 01:53:59  pinkfish
 * Add in some stuff for the distribution lib.
 *
 * Revision 1.6  2000/03/17 05:23:52  ceres
 * Recalculated burden when carrying capacity changes
 *
 * Revision 1.5  2000/03/09 03:20:35  ceres
 * Made hp regen rate work like gp regen rate (ie. using stat modify on appropriate skill)
 *
 * Ceres
 *
 * Revision 1.4  1999/04/14 01:32:35  ceres
 * Modified to handle weightless dead people
 *
 * Revision 1.3  1999/03/06 20:03:17  ceres
 * Made the bad npc log add the time.
 *
 * Revision 1.2  1998/08/10 10:07:44  pinkfish
 * Fix up to help non-guild players.
 *
 * Revision 1.1  1998/01/06 04:29:08  ceres
 * Initial revision
 * 
*/
/*
 * Module to handle con, dex, int, wis and str,
 *                  height and weight,
 *                  hit points and guild points,
 *                  carrying capacity.
 */
 
#include <move_failures.h>
#include <skills.h>
#include <living.h>
#include <config.h>

#define BASE 20
#define ORD1T 1
#define ORD1B 1
#define ORD2T 1
#define ORD2B 5
#define SCALING_DATA ({ 650, 150, 10, 5 })
#define DIVISOR 700

#define DEATH_WAIT_TIME 5

#define INIT_GP_INC -20

int Con, Dex, Int, Str, Wis,
    contmp, dextmp, inttmp, strtmp, wistmp,
    height, base_weight;
nosave int conbon, dexbon, intbon, strbon, wisbon, no_check;
nosave string to_zap;
nosave int gp_inc = INIT_GP_INC, hp_inc;  // increase rates for guild & hitpoints

mixed query_property(string name);

string stats_to_zap() {
   string temp;

   temp = to_zap;
   to_zap = 0;
   return temp;
} /* stats_to_zap() */

void zap_stat( string word ) {
   string dummy;
   
   if ( !to_zap ) {
      to_zap = word;
      if ( find_call_out( "zap_stat_cache" ) == -1 )
         call_out( "zap_stat_cache", 1 );
   }
   if ( sscanf( to_zap, "%s"+ word +"%s", dummy, dummy ) != 2 )
      to_zap += word;
} /* zap_stat() */

int query_no_check() { return no_check; }
void set_no_check( int i ) { no_check = i; }
 
int hp_base() {
  int base;
  base = 150 + 10 * (int)this_object()->query_con();
  if ( base < 5 ) base = 5;
  return base;
} /* hp_base() */
 
int hp_gradient() { return 4; }

int scaled_weight() {
  int i, actual, scaled;
  actual = (int)this_object()->query_weight(1);
  if ( actual < SCALING_DATA[ 0 ] )
    return actual;
  scaled += SCALING_DATA[ 0 ];
  actual -= SCALING_DATA[ 0 ];
  i = 1;
  while ( actual && ( ( i * SCALING_DATA[ 2 ] ) < 100 ) ) {
    if ( actual >= SCALING_DATA[ 1 ] ) {
      scaled += ( ( 100 - SCALING_DATA[ 2 ] * i ) * SCALING_DATA[ 1 ] )
          / 100;
      scaled += ( SCALING_DATA[ 3 ] * SCALING_DATA[ 1 ] ) / 1000;
      actual -= SCALING_DATA[ 1 ];
    } else {
      scaled += ( ( 100 - SCALING_DATA[ 2 ] * i ) * actual ) / 100;
      scaled += ( SCALING_DATA[ 3 ] * actual ) / 1000;
      actual = 0;
    }
    i += 1;
  }
  if ( actual )
    scaled += ( SCALING_DATA[ 3 ] * actual ) / 1000;
  return scaled;
} /* scaled_weight() */
 
void reset_hp() {
  int new_max, skill_bon;
  string report;

  if ( no_check )
    return;
  skill_bon = (int)this_object()->query_skill_bonus( "other.health" );
  new_max = hp_base() + skill_bon * hp_gradient();
  if ( !base_weight ) {
    if ( (string)this_object()->query_name() == "object" )
      return;
    report = file_name( this_object() ) +" ("+
        (string)this_object()->query_name() +"): race object is "+
        (string)this_object()->query_race_ob();
    if ( environment() ) {
      if ( file_name( environment() ) == "/room/void" )
        return;
      report += "; in "+ file_name( environment() );
      this_object()->move( "/room/void", "$N gets booted into the void for "+
          "being a bad NPC." );
    }
    report += ".\n";
    log_file( "BAD_NPC", ctime(time()) + " " + report );
    return;
  }
   new_max = ( new_max * scaled_weight() ) / DIVISOR;
   if ( new_max < 1 )
      this_object()->set_max_hp( 1 );
   else
      this_object()->set_max_hp( new_max );
} /* reset_hp() */
 
void reset_gp() {
   string guild_ob;

   if ( no_check )
      return;
   guild_ob = (string)this_object()->query_guild_ob();
   if (!guild_ob) {
      guild_ob = query_property("backup guild");
   }
   if ( !guild_ob ) {
      this_object()->set_max_gp( 50 +
            (int)this_object()->query_skill_bonus( "other.points" ) );
   } else {
      guild_ob->set_gp( this_object() );
   }
} /* reset_gp() */

void calc_inc_rates() {
   string guild_ob;

   hp_inc = sqrt((int)this_object()->stat_modify(100, "other.health")) - 7;

   guild_ob = (string)this_object()->query_guild_ob();
   if ( !guild_ob ) {
     guild_ob = query_property("backup guild");
     if (!guild_ob) {
       guild_ob = CONFIG_DEFAULT_GUILD;
     }
   }
   /* Believe it or not, this works... See the comment in the skills module. */
   gp_inc = sqrt((int)this_object()->stat_modify(100,
         (string)guild_ob->query_gp_skill())) - 7;
   // The inc cannot be less than 0.
   if(hp_inc < 0) {
     hp_inc = 0;
   }
   if(gp_inc < 0) {
     gp_inc = 0;
   }
} /* calc_inc_rates() */

int *query_inc_rates() { return ({ gp_inc, hp_inc }); }

void heart_beat() {
   int hp;
   int calling_death; 

   hp = this_object()->query_hp();

   if ( hp < 0 ) { 
       calling_death = this_object()->query_callingdeath();

        if ( calling_death ) { 

           // Oops.. if for some reason query_callingdeath() was set to 
           // true.. it means that the do_death() callout has been lost 
           // somehow.
           //
           // So let's die now instead.
           
           // tell_creator( "taffyd", "In do_death()\n" );

           if ( time() > calling_death + DEATH_WAIT_TIME ) { 
                // tell_creator( "taffyd", "DEATH_WAIT triggered\n" );
                this_object()->do_death();
            }
        } else {
            this_object()->do_death();
        }
   }
   else { 
      if ( gp_inc == INIT_GP_INC ) { 
         calc_inc_rates();
      }

      this_object()->adjust_gp( gp_inc );
      this_object()->adjust_hp( hp_inc );
   }
} /* heart_beat() */

void reset_carry_cap() {
   int i, hst_num, hst_wei, new_cap, old_loc, tot_str;
   object *contents, *dropped;

   if ( no_check ) return;
   old_loc = (int)this_object()->query_loc_weight();
   tot_str = Str + strtmp + strbon;
   new_cap = BASE;
   new_cap += ( ORD1T * tot_str ) / ORD1B;
   new_cap += ( ORD2T * tot_str * tot_str ) / ORD2B;
   new_cap = ( new_cap * (int)this_object()->query_weight(1) ) / 100;
   if ( !new_cap )
      new_cap = 1;
   this_object()->set_max_weight( new_cap );
   if ( new_cap >= old_loc ) return;
   this_object()->dest_hide_shadow();
   contents = all_inventory( this_object() ) -
      (object *)this_object()->query_armours();
   dropped = ({ });
   while ( ( old_loc > new_cap ) && sizeof( contents ) ) {
      hst_num = 0;
      hst_wei = 0;
      for ( i = 0; i < sizeof( contents ); i++ )
         if ( (int)contents[ i ]->query_complete_weight() > hst_wei ) {
            hst_wei = (int)contents[ i ]->query_complete_weight();
            hst_num = i;
         }
      if ( (int)contents[ hst_num ]->move( environment() ) == MOVE_OK ) {
         dropped += ({ contents[ hst_num ] });
         old_loc -= hst_wei;
      }
      contents = delete( contents, hst_num, 1 );
   }
   if ( sizeof( dropped ) ) {
      tell_room( environment(), capitalize( (string)this_object()->short() ) +
                " drops "+ query_multiple_short( dropped ) +" under strain.\n",
                this_object() );
      tell_object( this_object(), "Your fading strength makes you drop "+
                  query_multiple_short( dropped ) +".\n" );
   }
   this_object()->update_loc_weight();
   this_object()->calc_burden();
   if ( new_cap >= old_loc ) return;
   /* something nasty here to pin them to the ground with all that heavy
    armour */
   return;
} /* reset_carry_cap() */

void check_stats_zero() {
   if ( Int + inttmp + intbon <= 0 ||
        Wis + wistmp + wisbon <= 0 ) {
      this_object()->add_property( PASSED_OUT, 1, 500 );
      tell_object( this_object(), "You fall asleep.\n" );
   }
} /* check_stats_zero() */

void reset_all() { zap_stat( "CDISW" ); }

void reset_all2() {
   no_check = 0;
   reset_hp();
   reset_gp();
   reset_carry_cap();
   calc_inc_rates();
   check_stats_zero();  /* consequences of going to zero */
   this_object()->do_burden_call();
} /* reset_all2() */
 
int query_con() { return Con + contmp + conbon; }
int query_dex() { return Dex + dextmp + dexbon; }
int query_int() { return Int + inttmp + intbon; }
int query_str() { return Str + strtmp + strbon; }
int query_wis() { return Wis + wistmp + wisbon; }
 
int query_real_con() { return Con; }
int query_real_dex() { return Dex; }
int query_real_int() { return Int; }
int query_real_str() { return Str; }
int query_real_wis() { return Wis; }
                                   
int check( int number ) { return ( number <= 28 ); }
 
int set_con( int number ) {
  if ( !check( number ) ) number = 28;
  if ( Con != number )
    zap_stat( "C" );
  Con = number;
  return Con;
} /* set_con() */
                
int set_dex( int number ) {
  if ( !check( number ) ) number = 28;
  if ( Dex != number )
    zap_stat( "D" );
  Dex = number;
  return Dex;
} /* set_dex() */
 
int set_int( int number ) {
  if ( !check( number ) ) number = 28;
  if ( Int != number )
    zap_stat( "I" );
  Int = number;
  return Int;
} /* set_int() */
 
int set_str( int number ) {
  if ( !check( number ) ) number = 28;
  if ( Str != number )
    zap_stat( "S" );
  Str = number;
  return Str;
} /* set_str() */
 
int set_wis( int number ) {
  if ( !check( number ) ) number = 28;
  if ( Wis != number )
    zap_stat( "W" );
  Wis = number;
  return Wis;
} /* set_wis() */
 
int adjust_con( int number ) {
  if ( check( number + Con ) ) {
    Con += number;
    if ( number )
      zap_stat( "C" );
  }
  return Con;
} /* adjust_con() */
 
int adjust_dex( int number ) {
  if ( check( number + Dex ) ) {
    Dex += number;
    if ( number )
      zap_stat( "D" );
  }
  return Dex;
} /* adjust_dex() */
 
int adjust_int( int number ) {
  if ( check( number + Int ) ) {
    Int += number;
    if ( number )
      zap_stat( "I" );
  }
  return Int;
} /* adjust_int() */
 
int adjust_str( int number ) {
  if ( check( number + Str ) ) {
    Str += number;
    if ( number )
      zap_stat( "S" );
  }
  return Str;
} /* adjustr_str() */
 
int adjust_wis( int number ) {
  if ( check( number + Wis ) ) {
    Wis += number;
    if ( number )
      zap_stat( "W" );
  }
  return Wis;
} /* adjust_wis() */
 
int query_tmp_con() { return contmp; }
int query_tmp_dex() { return dextmp; }
int query_tmp_int() { return inttmp; }
int query_tmp_str() { return strtmp; }
int query_tmp_wis() { return wistmp; }
 
int adjust_tmp_con( int number ) {
  contmp += number;
  if ( number )
    zap_stat( "C" );
   if ( contmp && !dextmp && !inttmp && !strtmp && !wistmp )
      call_out( "update_tmps", 900 );
  return contmp;
} /* adjust_tmp_con() */
 
int adjust_tmp_dex( int number ) {
  dextmp += number;
  if ( number )
    zap_stat( "D" );
   if ( !contmp && dextmp && !inttmp && !strtmp && !wistmp )
      call_out( "update_tmps", 900 );
  return dextmp;
} /* adjust_tmp_dex() */
 
int adjust_tmp_int( int number ) {
  inttmp += number;
  if ( number )
    zap_stat( "I" );
   if ( !contmp && !dextmp && inttmp && !strtmp && !wistmp )
      call_out( "update_tmps", 900 );
  return inttmp;
} /* adjust_tmp_int() */
 
int adjust_tmp_str( int number ) {
  strtmp += number;
  if ( number )
    zap_stat( "S" );
   if ( !contmp && !dextmp && !inttmp && strtmp && !wistmp )
      call_out( "update_tmps", 900 );
  return strtmp;
} /* adjust_tmp_str() */
 
int adjust_tmp_wis( int number ) {
  wistmp += number;
  if ( number )
    zap_stat( "W" );
   if ( !contmp && !dextmp && !inttmp && !strtmp && wistmp )
      call_out( "update_tmps", 900 );
  return wistmp;
} /* adjust_tmp_wis() */
 
int query_bonus_con() { return conbon; }
int query_bonus_dex() { return dexbon; }
int query_bonus_int() { return intbon; }
int query_bonus_str() { return strbon; }
int query_bonus_wis() { return wisbon; }
 
int adjust_bonus_con( int number ) {
  conbon += number;
  if ( number )
    zap_stat( "C" );
  return conbon;
} /* adjust_bonus_con() */

int adjust_bonus_dex( int number ) {
  dexbon += number;
  if ( number )
    zap_stat( "D" );
  return dexbon;
} /* adjust_bonus_dex() */
 
int adjust_bonus_int( int number ) {
  intbon += number;
  if ( number )
    zap_stat( "I" );
  return intbon;
} /* adjust_bonus_int() */

int adjust_bonus_str( int number ) {
  strbon += number;
  if ( number )
    zap_stat( "S" );
  return strbon;
} /* adjust_bonus_str() */
 
int adjust_bonus_wis( int number ) {
  wisbon += number;
  if ( number )
    zap_stat( "W" );
  return wisbon;
} /* adjust_bonus_wis() */
 
void update_tmps() {
  if ( contmp ) {
    zap_stat( "C" );
    contmp = contmp / 2;
  }
  if ( dextmp ) {
    zap_stat( "D" );
    dextmp = dextmp / 2;
  }
  if ( inttmp ) {
    zap_stat( "I" );
    inttmp = inttmp / 2;
  }
  if ( strtmp ) {
    zap_stat( "S" );
    strtmp = strtmp / 2;
  }
  if ( wistmp ) {
    zap_stat( "W" );
    wistmp = wistmp / 2;
  }
   if ( contmp || dextmp || inttmp || strtmp || wistmp )
      call_out( "update_tmps", 900 );
} /* update_tmps() */
 
int query_height() { return height; }
 
void set_height( int number ) {
  if ( number > 0 ) height = number;
} /* set_height() */

int query_base_weight() { return base_weight; }
 
void set_base_weight( int number ) {
   if ( number > 0 )
      base_weight = number;
} /* set_weight() */
 
int query_weight(int) {
  int adjust_weight;
  adjust_weight = ( ( ( Con + 3 * Str ) / 4 ) - 13 ) * ( base_weight / 30 );
  return base_weight + adjust_weight;
} /* query_weight() */
 
mixed *stats() {
  return ({
    ({ "Con", Con + conbon + contmp }),
    ({ "Dex", Dex + dexbon + dextmp }),
    ({ "Int", Int + intbon + inttmp }),
    ({ "Str", Str + strbon + strtmp }),
    ({ "Wis", Wis + wisbon + wistmp }),
    ({ "tmp Con", contmp }),
    ({ "tmp Dex", dextmp }),
    ({ "tmp Int", inttmp }),
    ({ "tmp Str", strtmp }),
    ({ "tmp Wis", wistmp }),
    ({ "bonus Con", conbon }),
    ({ "bonus Dex", dexbon }),
    ({ "bonus Int", intbon }),
    ({ "bonus Str", strbon }),
    ({ "bonus Wis", wisbon }),
    ({ "hp rate", hp_inc }),
    ({ "gp rate", gp_inc }),
    ({ "height", height }),
  });
} /* stats() */
