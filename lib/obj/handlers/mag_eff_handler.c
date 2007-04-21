/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: mag_eff_handler.c,v 1.3 2000/11/16 23:35:36 rodion Exp $
 * $Log: mag_eff_handler.c,v $
 * Revision 1.3  2000/11/16 23:35:36  rodion
 * Took out caterpillar stat-setting, as it was causing problems.
 * Rodion
 *
 * Revision 1.2  1998/04/23 14:01:25  pinkfish
 * Added documentation.  And cleanedup a little.
 *
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
*/
/**
 * This handler controls the ambient enchantment effects and effects
 * from solid grains of octarine.  Ambient enchantment is caused by
 * the use of spells in certain locations.  Ambient enchantment can
 * be blocked by use of the property "enchantment block".
 * @author Deutha
 */
/*
 * Other effects:
 * shower of lead cubes
 * gibbering eldritch shapes
 *  4-sided triangles
 *  pineapple-flavoured custard
 *  double ended circles
 */

#define BLACK_MASS 1000
#define NUM_POSS 6
#define NUM_USER 20

private mixed *_octonite;
private int _call_out_no;

protected void update_octonite();
protected void schedule_next();

/**
 * This method checks to see if the particular object is blocking the use
 * of enchantments.
 * @param thing the object to check for enchantment block
 * @return 1 if it is blocking enchanments, 0 if not
 * @see do_effect()
 * @see ambient_enchantment()
 * @see choose_effect()
 */
int block( object thing ) {
   if ( !thing->query_closed() )
      return 0;
   return (int)thing->query_property( "enchantment block" );
} /* block() */

/**
 * This method returns the current ambient enchantment for the particular
 * object.
 * @param thing the object to get the ambient enchantment for
 * @return the ambient enchantment of the object
 * @see do_effect()
 * @see choose_effect()
 * @see block()
 */
int ambient_enchantment( object thing ) {
   int i, total;
   object *tops;

   tops = ({ thing });
   while ( environment( tops[ 0 ] ) ) {
      tops = ({ environment( tops[ 0 ] ) }) + tops;
   }
   for ( i = 0; i < sizeof( tops ); i++ ) {
      total += (int)tops[ i ]->query_enchant();
      total = ( total * ( 100 - block( tops[ i ] ) ) + 50 ) / 100;
   }
/*
   tell_object( find_player( "deutha" ), "Total for "+ file_name( thing ) +
         " is "+ total +".\n" );
*/
   return total;
} /* ambient_enchantment() */

/**
 * This method chooses a random effect from the curent list of effects
 * defined in the handler.  It returns an array consisting of
 * two elements, the first is the number, the second is a random
 * number between the input number multiplied by 6 and then divided
 * by 1000.
 * @param number the intput number
 * @return an array as described above
 * @see do_effect()
 * @see ambient_enchantment()
 * @see block()
 */
int *choose_effect( int number ) {
  return ({ number, random( NUM_POSS * number ) / BLACK_MASS });
} /* choose_effect() */

/**
 * This method causes an effect to occur.  The number is the severity
 * of the effect.
 * @param number the severity of the magic infestation
 * @param name the name of the thing being effected
 * @param place the place being effected (room)
 * @see choose_effect()
 * @see ambient_enchantment()
 * @see block()
 */
void do_effect( int number, string name, object place ) {
  int i;
  string word;
  object thing, *things;

  switch( number ) {
    case 0:
      tell_room( place, "How curious.\n" );
      break;
    case 1:
      tell_room( place, name +" spins slowly in mid-air, then "+
        "vanishes with a small \"pop!\"\n" );
      break;
    case 2:
      i = random( 4 );
      tell_room( place, name +" sparkles and turns into a small "+
        ({ "yellow", "green", "hairy", "spotted" })[ i ] +" caterpillar.\n" );
      thing = clone_object( "/obj/monster" );
      thing->set_name( "caterpillar" );
      thing->set_short( ({ "yellow", "green", "hairy", "spotted" })[ i ]
        +" caterpillar" );
      thing->add_adjective( ({ "yellow", "green", "hairy", "spotted" })[ i ] );
      thing->set_main_plural( ({ "yellow", "green", "hairy", "spotted" })[ i ]
        +" caterpillars" );
      thing->add_plural( "caterpillars" );
      thing->set_long( "This is a small, common or garden caterpillar.\n" );
      thing->set_race( "caterpillar" );
/*
      thing->set_no_check( 1 );
      thing->set_max_hp( 10 );
      thing->set_hp( 10 );
      thing->set_max_weight( 1 );
      thing->set_get();
*/
      thing->set_level( 1 );
      thing->move( place );
      break;
    case 3:
      tell_room( place, name +" vanishes with a sharp \"spang!\"\n"+
        "A moment later, you hear a small thunderclap.\n" );
      break;
    case 4:
      tell_room( place, "Balefire blossoms, and "+ name +" turns into "+
        "a bunch of flowers.\n" );
      thing = clone_object( "/std/object" );
      thing->set_name( "flowers" );
      thing->set_short( "bunch of flowers" );
      thing->set_main_plural( "bunches of flowers" );
      thing->add_adjective( ({ "bunch", "bunches", "of" }) );
      thing->add_plural( "bunches" );
      thing->set_long( "A nice bunch of flowers.\n" );
      thing->set_weight( 10 );
      thing->set_value( 100 );
      thing->move( place );
      break;
    case 5:
      tell_room( place, "With a small gout of flame, "+ name +" begins "+
        "to accelerate.\n" );
      switch( (string)place->query_property( "location" ) ) {
        case "outside":
          tell_room( place, name +" vanishes into the distance.\n" );
          break;
        default:
          tell_room( place, "There is a small puff of burning dust as "+
            name +" passes through a wall.\n" );
      }
      things = users() - all_inventory( place );
      if ( sizeof( things ) > ( i = random( NUM_USER ) ) ) {
        if ( !( thing = environment( things[ i ] ) ) ) break;
        tell_object( find_player( "deutha" ),
            (string)things[ i ]->query_name() +"\n" );
        tell_room( thing, "You hear a high-pitched whine.\n" );
        tell_object( things[ i ], "Something small and very hot zooms past "+
          "you at ear level.\n" );
        if ( ( !( word = (string)things[ i ]->query_short() ) || ( word == "" )
          ) || things[ i ]->query_invis() ) break;
        tell_room( thing, word +" looks shaken.\n", things[ i ] );
      }
      break;
    default:
      tell_room( place, "Something really, really strange happens.\n" );
  }
} /* do_effect() */

/**
 * This method returns the current list of octonite cystals handled
 * by this handler.
 * @return the array of octonite crystals
 * @see schedule_next()
 * @see update_octonite()
 * @see add_octonite_cyrstal()
 */
object *query_octonite_crystals() { return _octonite; }

/**
 * This method adds an octonite crystal into the current list of
 * octonite crystals.
 * @param crystal the crystal to add
 * @see query_octonite_crystals()
 * @see update_octonite()
 * @see schedule_next()
 */
void add_octonite_crystal( object crystal ) {
   int i, when;

   when = time() + roll_MdN( 5, 60 );
   if ( !sizeof( _octonite ) ) {
      _octonite = ({ crystal, when });
      schedule_next();
   } else {
      for ( i = 0; i < sizeof( _octonite ); i += 2 ) {
         if ( when < _octonite[ i + 1 ] ) {
            break;
         }
      }
      if ( i == 0 ) {
         _octonite = ({ crystal, when }) + _octonite;
         schedule_next();
         return;
      }
      if ( i >= sizeof( _octonite ) ) {
         _octonite += ({ crystal, when });
         return;
      }
      _octonite = _octonite[ 0 .. i - 1 ] + ({ crystal, when }) +
            _octonite[ i ..  ];
   }
} /* add_octonite_crystal() */

/**
 * This schedules the next update to occur.  The updates are for the
 * octonite crystals.
 * @see query_octonite_crystals()
 * @see update_octonite()
 * @see add_octonite_cyrstal()
 */
protected void schedule_next() {
   int next;

   remove_call_out(_call_out_no);
   next = _octonite[ 1 ] - time();
   if ( next > 0 ) {
      _call_out_no = call_out( (: update_octonite :), next );
   } else {
      _call_out_no = call_out( (: update_octonite :), 0 );
   }
} /* schedule_next() */

/**
 * This updatest he values associated with the octonite pebbles.  They
 * decay away slowly releasing ambient magic into the background.
 * @see query_octonite_crystals()
 * @see schedule_next()
 * @see add_octonite_cyrstal()
 */
protected void update_octonite() {
   int ambient, weight;
   object crystal;

   crystal = _octonite[ 0 ];
   if ( objectp( crystal ) ) {
      weight = 20 * (int)crystal->query_weight();
      tell_object( find_player( "deutha" ), "Current: "+ weight +".\n" );
      ambient = ambient_enchantment( crystal );
      if ( ambient > weight ) {
         weight += ( ambient - weight ) / 4;
      }
      //tell_object( find_player( "deutha" ), "Influx to: "+ weight +".\n" );
      weight -= ( weight * ( 100 - block( environment( crystal ) ) ) ) / 800;
      //tell_object( find_player( "deutha" ), "Outflux to: "+ weight +".\n" );
      if ( random( 20 ) < weight % 20 ) {
         weight += 20;
      }
      weight /= 20;
      if ( !weight )
         crystal->dest_me();
      else {
         crystal->set_weight( weight );
         crystal->set_enchant( ( 7 * (int)crystal->query_max_enchant() ) / 8 );
         crystal->set_value_info( "magic", weight * 10000 );
      }
      _octonite = delete( _octonite, 0, 2 );
      if ( sizeof( _octonite ) ) {
         schedule_next();
      }
      if ( crystal ) {
         add_octonite_crystal( crystal );
      }
   } else {
      _octonite = delete( _octonite, 0, 2 );
      if ( sizeof( _octonite ) ) {
         schedule_next();
      }
   }
} /* update_octonite() */

/**
 * This method returns the extra look information for a piece of octonite.
 * It gives different messages about size depending on its weight.
 * @param thing the object to give na extra look for
 * @return the extra information about the octonite pebbles
 */
string extra_look( object thing ) {
   if ( explode( file_name( thing ), "#" )[ 0 ] != "/obj/magic/octonite" ) {
      return "";
   }
   switch ( (int)thing->query_weight() ) {
      case 0 :
         return "";
      case 1 .. 2 :
         return "It is about the size of a pea.\n";
      case 3 .. 6 :
         return "It is about the size of a hazelnut.\n";
      case 7 .. 12 :
         return "It is about the size of a walnut.\n";
      case 13 .. 20 :
         return "It is about the size of an orange.\n";
      case 21 .. 30 :
         return "It is about the size of a grapefruit.\n";
      default :
         return "It is about the size of a pumpkin.\n";
   }
} /* extra_look() */
