/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: health.c,v 1.41 2003/05/13 20:21:07 ceres Exp $
 */
#include <drinks.h>
#include <living.h>

#undef POINTS_LOG
#ifdef USE_SURRENDER
#define SURRENDER_SHADOW "/std/shadows/misc/surrender_shadow"
#endif

varargs int adjust_xp( int number, int shared );

int hp, max_hp, gp, max_gp, xp, wimpy, *drink_info;
#ifdef USE_SURENDER
int surrender;
#endif
nosave int max;
nosave int* runaway_callout_handles;
nosave string which;
nosave mapping counts;
mapping verbs;
nosave int callingdeath;

void create() {
   max_hp = 1;
   max_gp = 1;
#ifdef USE_SURRENDER
   surrender = -1;
#endif   
   drink_info = allocate( D_SIZEOF );
   counts = ([ ]);
   verbs = ([ ]);
   callingdeath = 0;
   runaway_callout_handles = ({ });
} /* create() */

int query_hp() { return hp; }
 
varargs int set_hp( int number, object attacker ) {
   if ( number > max_hp )
      number = max_hp;
   hp = number;
   if ( ( hp < 0 ) && ( find_call_out( "do_death" ) == -1 ) )
      call_out( "do_death", 0, attacker );
   return hp;
} /* set_hp() */
 
protected int check_wimpy() {
    int hp;
  
    hp = this_object()->query_hp();
    if ( hp < 1 )
        return 0;
        
    if( 100 * hp < this_object()->query_wimpy() * 
        this_object()->query_max_hp()) {
    
        if(find_call_out("run_away") == -1) {
            // Add the runaway callout handle to our other ones
            runaway_callout_handles += ({ call_out("run_away", 0) });
        }
        return 1;
    }
    return 0;
}

#ifdef USE_SURRENDER
protected int check_surrender() {
   int hp;
   object *attackers, *all, shad;
   
   hp = this_object()->query_hp();
   if (hp < 1)
      return 0;
   
   if (hp * 100 < this_object()->query_surrender() *
       this_object()->query_max_hp()) {
     attackers = filter(this_object()->query_attacker_list(),
                        (: $1 && !$1->query_property("dead") &&
                         environment($1) == environment($2) :),
                        this_object());
     all = attackers + ({ this_object() });
     all->event_surrender(this_object(), attackers);

     shad = clone_object(SURRENDER_SHADOW);
     if (shad) {
       shad->setup_shadow(this_object(), attackers);
     }
     return 1;
   }
   return 0;
}
#endif

/*
 * Resets the flag that prevents the creation of a new do_death
 * callout
 */
void reset_callingdeath() {
  callingdeath = 0;
}

/** 
 * Returns the current value of the callingdeath variable.
 */
int query_callingdeath() { 
    return callingdeath;
} /* query_callingdeath() */ 


varargs int adjust_hp(int number, object attacker, object weapon,
                      string attack) {
    int i;

    hp += number;
    if ( hp > max_hp )
        hp = max_hp;
    if (hp <= 0 && !callingdeath) {
        /* Set the flag so that I won't create another do_death
         * callout, and then create the callout.  I'm expecting
         * do_death to reset this flag.  Not that it should need
         * to, but we want to be clean. */
        callingdeath = time();
        call_out("do_death", 0, attacker, weapon, attack);
        // Get rid of any call_outs to run away
        for( i=0; i<sizeof(runaway_callout_handles); i++ ) {
            remove_call_out( runaway_callout_handles[i] );
        }
        runaway_callout_handles = ({ });
    }

    if (hp > 0 && number < 0 && attacker && attacker != this_object()) {
#ifdef USE_SURRENDER
        if(this_object()->query_surrender() >= this_object()->query_wimpy()) {
            if (!check_surrender())
                check_wimpy();
        }
        else {
            if (!check_wimpy())
                check_surrender();
        }
#else
        check_wimpy();
#endif     
    }
    return hp;
}
 
/* undefinedp() doesn't seem to work on reference arguments.  Have to
   work around. */
varargs string health_string(int flag, int ref health_level) {
   int level;
   string ret;

   if ( this_object()->query_property( "dead" ) )  {
      ret = "appears to be dead";
      level = 0;
   }
   else if ( hp < max_hp / 10 )  {
      ret = "is in very bad shape";
      level = 1;
   }
   else if ( hp < max_hp / 5 )  {
      ret = "is in bad shape";
      level = 2;
   }
   else if ( hp < max_hp / 2 )  {
      ret = "is not in good shape";
      level = 3;
   }
   else if ( hp < ( max_hp - 200 ) )  {
      ret = "is slightly hurt";
      level = 4;
   }
   else  {
      ret = "is in good shape";
      level = 5;
   }
   if (!undefinedp(flag))   health_level = level;
   return ret;
} /* health_string() */
 
int query_max_hp() { return max_hp; }
 
int set_max_hp( int number ) {
  int old_hp;
  old_hp = hp;
  if ( max_hp == hp )
    hp = number;
  else
    if ( max_hp )
      hp = ( hp * number ) / max_hp;
    else
      hp = number;
  max_hp = number;
  if ( hp > max_hp )
    hp = max_hp;
  if ( ( hp < 0 ) && ( old_hp > 0 ) )
    hp = max_hp;
  return max_hp;
} /* set_max_hp() */
 
int query_gp() { return gp; }

int query_specific_gp( string gp_type ) {
   int now;
   if ( !gp_type )
      return 0;
   max = (int)this_object()->query_skill_bonus( gp_type
         +".points" ) + 50;
   if ( max >= max_gp )
      max = max_gp;
   now = gp + max - max_gp;
   which = gp_type;
   if ( find_call_out( "clear_gp_info" ) == -1 )
      call_out( "clear_gp_info", 1 );
   return now;
} /* query_specific_gp() */

void clear_gp_info() { which = 0; }
 
int set_gp( int number ) {
   gp = number;
   if ( gp > max_gp )
      gp = max_gp;
   return gp;
} /* set_gp() */
 
int adjust_gp( int number ) {
  int tmp, diff;
  string guild_ob, current_verb;
  mixed *bits;
  
  if ( gp + number < 0 )
    return -1;
  
  gp += number;
  
  if ( gp > max_gp )
    gp = max_gp;

  tmp = number;

  /* Give people automatic gp advances.
   * which is setup by query_specific_gp.
   * max is their maximum gp.
   *
   * Also, give people XP for using guild commands.
   */
  if((number < 0) && stringp(which)) {
    
    if (number > ( GP_ADVANCE_MOD * max ) / MIN_TIMES)
      number = (GP_ADVANCE_MOD * max) / MIN_TIMES;
    
    counts[ which ] -= number;
    if (counts[ which ] > GP_ADVANCE_MOD * max) {
      counts[ which ] = 0;
      
      // they get a chance of an advance not a guarantee.
      // the use of exp(skill/somevalue) causes a steep growth
      // curve at higher values thus preventing massive levels
      // in .points skills.
      number = this_object()->query_skill(which +".points");
      if(!random(to_int(exp(number/150.0))) &&
         this_object()->add_skill_level( which +".points", 1,
                                         this_object())) {
        tell_object( this_object(), "%^YELLOW%^You find yourself "
                     "more able to concentrate on this task than you "
                     "thought.%^RESET%^\n" );
      }
      
#ifdef POINTS_LOG
      log_file( "POINTS", time() +" "+
                (string)this_object()->query_name() +" gets "+
                which +" ("+ max +")\n" );
#endif
    }
    
    // Give them Xp if they used a guild command.
    number = tmp;
    guild_ob = this_object()->query_guild_ob();
    
    /*
     * If it's one of their guild commands give them 10x the GP.
     * if it's not then give then 4x the GP. Give 6x for cast since
     * 
     */
    if(guild_ob && member_array(query_verb(), guild_ob->query_commands()) != -1)
      number *= -10;
    else
      number *= -4;

    // These use all available GPs so need to give less XP.
    // These use all available GPs so need to give less XP.
    if(query_verb() == "pray" || query_verb() == "berserk" ||
       query_verb() == "spellcheck" || query_verb() == "octograve")
      number /= 2;
    
    // If it's a repeat command and repeated too quickly then
    // or if it's just another command done very quickly (eg. a;b;a;b)
    // we reduce the Xp they get.
    current_verb = query_verb();
    if(current_verb == "cast" || current_verb == "perform") {
      current_verb = this_object()->query_current_cmd();
    }

    if(verbs[current_verb]) {
      bits = verbs[current_verb];
      diff = time() - bits[0];

      // see when they last did this command. If it was more than 5 minutes
      // ago then reduce their count of repeats.
      if(diff > 300)
        bits[1] /= 2;
      if(bits[1] < 1)
        bits[1] = 1;

      if(bits[1] > 1 && bits[2] == environment())
        bits[1] += 1;

      // they're repeating the command so reduce their xp somewhat.
      if(bits[1] < 10)
         number /= (random(bits[1]) + 1);
      else
        number = 0;
      verbs[current_verb] = ({ time(), bits[1]+1, environment() });
    } else {
      verbs[current_verb] = ({ time(), 1, environment() });
    }

    if(number)
      adjust_xp(number, 1);
    
#ifdef GUILD_XP
    log_file("GUILD_XP", "%s Gave %s %d points for %s [%s, %d]\n",
             ctime(time()), this_object()->query_name(), number,
             current_verb, verbs[current_verb][0], verbs[current_verb][1]);
#endif
  }
  return gp;
}

int query_max_gp() { return max_gp; }
 
int set_max_gp( int number ) {
  if ( max_gp ) gp = ( gp * number ) / max_gp;
  else gp = number;
  max_gp = number;
  if ( gp > max_gp ) gp = max_gp;
  return max_gp;
} /* set_max_gp() */
 
int query_xp() { return xp; }

varargs int adjust_xp( int number, int shared ) {
  if(number > 0 && (xp > 10000000 || xp + number > 10000000))
    return xp;
  
  if(number > 10000 && previous_object() &&
     explode(file_name(previous_object()), "/")[0] == "w")
    log_file("/d/admin/log/CHEAT", ctime(time()) + " " +
             file_name(previous_object()) +
             " gave " + number + " Xp for " + this_object()->query_name() +
             "\n");
             
   xp += number;
   return xp;
} /* adjust_xp() */
 
int query_wimpy() { return wimpy < 30 ? wimpy : 30; }
 
int set_wimpy( int number ) {
  if ( ( number < 0 ) || ( number > 30 ) ) return -1;
  return wimpy = number;
} /* set_wimpy() */
 
#ifdef USE_SURRENDER
int query_surrender() {
  if (surrender == -1) {
    if (wimpy > 94)
      return 100;
    else
      return wimpy + 5;
  } else
    return surrender;
}

int set_surrender( int number ) {
  if ( ( number < 0 ) || ( number > 100 ) ) return -1;
  return surrender = number;
} /* set_surrender() */
#endif

int *query_drink_info() { return drink_info; }

int query_volume( int type ) {
  if ( type >= D_SIZEOF ) return 0;
  return drink_info[ type ];
} /* query_volume() */
 
int adjust_volume( int type, int amount ) {
  if ( type >= D_SIZEOF ) return 0;

  return drink_info[ type ] += amount;
} /* adjust_volume() */
 
void update_volumes() {
  int i, delta;
  delta = (int)this_object()->query_con();
  for ( i = 0; i < sizeof( drink_info ); i++ ) {
    if ( drink_info[ i ] > delta ) {
      drink_info[ i ] -= delta;
      // drinking cools you down if you're hot, eating warms you up
      // if you're cold.
      if(this_object()->query_personal_temp() >  0 && i == D_DRINK)
  this_object()->adjust_personal_temp(-(delta/3));
      else if(this_object()->query_personal_temp() <  0 && i == D_FOOD)
  this_object()->adjust_personal_temp(delta/3);
    } else
      if ( drink_info[ i ] < -delta )
        drink_info[ i ] += delta;
      else
        drink_info[ i ] = 0;
  }
} /* update_volumes() */

mapping query_counts() { return counts; }
