#include <clothing.h>
#include <weapon.h>

#define COST 5
#define SKILL "other.evaluating.armour"
#define LEARN_LEVEL 2
#define TEACH_LEVEL 10

inherit "/cmds/guild_base";

void create() {
  ::create();
  set_nroff_file("vurdere");
  set_command_name("vurdere");
  add_teach_skill(SKILL, 10, 2);
} /* create() */

mixed *calc_ac( object ob ) {
  int j;
  string i;
  mapping ac;
  mixed *ret, bit;
  
  ac = (mapping)ob->query_armour_class();
  ret = ({ });
  foreach ( i in keys( ac ) ) {
    if ( member_array( ac[ i ][ A_TYPE ], ({ "blunt",
                                               "sharp", "pierce" }) ) == -1 )
      continue;
    bit = ac[ i ][ A_AC ];
    if( (j = member_array( ac[ i ][ A_TYPE ], ret )) == -1 ) {
      j = sizeof( ret );
      ret += ({ ac[ i ][ A_TYPE ], ({ 0, 0, 0 }) });
    }
    j++;
    if ( intp( bit ) ) {
      ret[ j ][ 1 ] += bit / 2; /* average */
      ret[ j ][ 2 ] += bit; /* maximun */
    } else
      switch ( sizeof( bit ) ) {
      case 1 :
        ret[ j ][ 0 ] += bit[ 0 ];
        ret[ j ][ 1 ] += bit[ 0 ];
        ret[ j ][ 2 ] += bit[ 0 ];
        break;
      case 2 :
        ret[ j ][ 0 ] += bit[ 0 ];
        ret[ j ][ 1 ] += (bit[ 0 ] + bit[ 1 ] / 2);
        ret[ j ][ 2 ] += (bit[ 0 ] + bit[ 1 ]);
        break;
      case 3 :
        ret[ j ][ 0 ] += bit[ 0 ];
        ret[ j ][ 1 ] += (bit[ 0 ] + bit[ 1 ] * bit[ 2 ] / 2);
        ret[ j ][ 2 ] += (bit[ 0 ] + bit[ 1 ] * bit[ 2 ]);
        break;
      default :
        write( "Barf!\n" );
      }
  }
  return ret;
} /* calc_ac() */

int calc_sausage( int sausage, int lvl ) {
  int tmp;
  tmp = 99 - (lvl * 99) / 200;
  if( tmp < 0 )
    tmp = 0;
  return random( sausage ) * tmp / 100 - random( sausage ) * tmp / 100 +
    sausage;
} /* calc_sausage() */

string check_string( int val, int bon ) {
  int dist;
  string s;

  dist = (350 - bon) / 10;
  if( dist > (val < 0 ? -val : val) )
    return " is the same as ";
  if( val > 0 )
    s = "better than ";
  else {
    s = "worse than ";
    val -= val;
  }
  if( !val )
    return " is exactly the same as ";
  if( val < 10 )
    return " is slightly " + s;
  if( val < 20 )
    return " " + s;
  if( val < 40 )
    return " is much " + s;
  return " is amazingly " + s;
} /* check_string() */

int cmd_against( object*from, object *to ) {
  int i, j, k, l, arm_lvl;
  object *ok, *fail;
  mixed *rat, *to_ac, *zip;

  arm_lvl = (int)this_player()->query_skill_bonus( SKILL );

  if(sizeof(from) + sizeof(to) > 10) {
    add_failed_mess("You cannot compare that many items at once.\n");
    return 0;
  }

  i = (COST*(sizeof(from)+sizeof(to)));
  if(this_player()->query_specific_gp("other") < i) {
    add_failed_mess("You cannot concentrate enough to compare these "
                       "items.\n");
    return 0;
  }
  this_player()->adjust_gp( -i);

  to_ac = ({ });
  fail = ({ });
  for( i = 0; i < sizeof( to ); i++ ) {
    if( to[ i ]->query_armour() ) {
      rat = calc_ac( to[ i ] );
      to_ac += ({ to[ i ], rat });
    } else {
      fail += ({ to[ i ] });
      to = delete( to, i, 1 );
      i--;
    }
  }
  if( !sizeof( to ) ) {
    notify_fail( "None of " + query_multiple_short( fail ) +
                 " are armour.\n" );
    return 0;
  }
  fail = ({ });
  ok = ({ });
  for( i = 0; i < sizeof( from ); i++ ) {
    if( from[ i ]->query_armour() ) {
      if( !sizeof( to_ac ) ) {
        fail += ({ from[ i ] });
        continue;
      }
      rat = calc_ac( from[ i ] );
      zip = ({ });
      for( j = 0; j < sizeof( to_ac ); j += 2 ) {
        for( k = 0; k < sizeof( to_ac[ j + 1 ] ); k += 2 ) {
          if( (l = member_array( to_ac[ j + 1 ][ k ], rat )) == -1 ) {
            write( capitalize( (string)from[ i ]->short() ) +
                   check_string( 0 - (to_ac[ j + 1 ][ k + 1 ][ 0 ] +
                                      to_ac[ j + 1 ][ k + 1 ][ 1 ] +
                                      to_ac[ j + 1 ][ k + 1 ][ 2 ]) / 3,
                                 arm_lvl ) +
                   to_ac[ j ]->short() + " at protecting from " +
                   to_ac[ j + 1 ][ k ] + ".\n" );
          
          } else {
            write( capitalize( (string)from[ i ]->short() ) +
                   check_string( (rat[ l + 1 ][ 0 ] + rat[ l + 1 ][ 1 ] +
                                  rat[ l + 1 ][ 2 ]) / 3 -
                                 (to_ac[ j + 1 ][ k + 1 ][ 0 ] +
                                  to_ac[ j + 1 ][ k + 1 ][ 1 ] +
                                  to_ac[ j + 1 ][ k + 1 ][ 2 ]) / 3,
                                 arm_lvl ) +
                   to_ac[ j ]->short() + " at protecting from " +
                   to_ac[ j + 1 ][ k ] + ".\n" );
          }
          
          zip += ({ to_ac[ j + 1 ][ k ] });
        }
        for( k = 0; k < sizeof( rat ); k += 2 ) {
          if( member_array( rat[ 0 ], zip ) ) {
            write( capitalize( (string)from[ i ]->short() ) +
                   check_string( 0 - (rat[ j + 1 ][ 0 ] + rat[ j + 1 ][ 1 ] +
                                      rat[ j + 1 ][ 2 ]) / 3, arm_lvl ) +
                   to_ac[ j ]->short() + " at " + rat[ j ] + ".\n" );
          }
        }
      }
      if(member_array(from[i], ok) == -1) {
        ok += ({ from[ i ] });
      }
    } else {
      if(member_array(from[i], fail) == -1) {
        fail += ({ from[ i ] });
      }
    }
  }
  if( sizeof( fail ) ) {
    add_failed_mess( "Failed to compare " + query_multiple_short( fail ) +
                 " against " + query_multiple_short( to ) + ".\n" );
    return 0;
  }
  if( sizeof( ok ) ) {
    say( this_player()->query_cap_name() + " looks closely at " +
         query_multiple_short( uniq_array( ok + to ) ) + ".\n" );
  }
  return 1;
} /* cmd_against() */

int cmd_normal( object* from ) {
  int i, j;
  object *ok, *fail;
  mixed *rat, bing;
  string type, *types, *zones;

  ok = ({ });
  fail = ({ });
  for( i = 0; i < sizeof( from ); i++ ) {
    if( from[ i ]->query_armour() ) {
      rat = calc_ac( from[ i ] );
      for( j = 0; j < sizeof( rat ); j += 2 ) {
        bing = (rat[ j + 1 ][ 0 ] + rat[ j + 1 ][ 1 ] + rat[ j + 1 ][ 2 ]) / 3;
        bing /= 5;
        if( bing >= 10 ) {
          bing = 9;
        }
        if( bing < 0 ) {
          bing = 0;
        }
        write( from[ i ]->the_short() + ({ " is terrible",
                                       " is amazingly poor",
                                       " is pretty poor",
                                       " is poor",
                                       " is ok",
                                       " is average",
                                       " is good",
                                       " is very good",
                                       " is extremely good",
                                       " is excellent", })[ bing ] +
                                       " at stopping " + rat[ j ] +
                                       " attacks.\n" );

        ok += ({ from[ i ] });
      }
      if(!arrayp(from[i]->query_type())) {
        types = ({ from[i]->query_type() });
      } else {
        types = from[i]->query_type();
      }
      zones = ({ });
      foreach(type in types) {
        if(CLOTHING_HANDLER->query_equivilant_type(type)) {
          zones += CLOTHING_HANDLER->query_zone_names(CLOTHING_HANDLER->query_equivilant_type(type));
        } else {
          zones += CLOTHING_HANDLER->query_zone_names(type);
        }
      }

      if(!sizeof(zones)) {
      	write(from[i]->the_short() +
              " does not protect any parts of your body.\n");
      } else {
        write(from[i]->the_short() + " protects your " +
              query_multiple_short(zones, "poss") + ".\n");
      }
      continue;
    }
    fail += ({ from[ i ] });
  }
   from = uniq_array( from );

  if( !sizeof( ok ) ) {
    notify_fail( "You failed to vurdere " + query_multiple_short( from ) +
                 ".\n" );
    return 0;
  }
    ok = uniq_array( ok );
  say( this_player()->query_cap_name() + " peers carefully at " +
       query_multiple_short( ok ) + ".\n" );
  return 1;
} /* cmd() */

int teach( object ob ) {
  if( this_player()->query_skill( SKILL ) < TEACH_LEVEL )
    return -1;
  if( ob->query_skill( SKILL ) < LEARN_LEVEL )
    return -2;
  ob->add_known_command( "vurdere" );
  return 1;
} /* teach() */

mixed* query_patterns() {
   return ({ "<indirect:object:me'from'> against <indirect:object:me'to'>",
                (: cmd_against($1[0], $1[1]) :),
             "<indirect:object:me'normal'>",
                (: cmd_normal($1) :) });
} /* query_patterns() */
