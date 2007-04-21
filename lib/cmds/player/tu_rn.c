/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: tu_rn.c,v 1.3 2002/02/24 02:35:04 ceres Exp $
 * $Log: tu_rn.c,v $
 * Revision 1.3  2002/02/24 02:35:04  ceres
 * Modified so turning while sneaking doesn't make you vis.
 *
 * Revision 1.2  1998/09/06 04:58:48  presto
 * Moved syntax message into query_patterns thingy
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
inherit "/cmds/base";

#define TP this_player()

mixed cmd(string str) {
  int i;
  mixed * facing;

  if ( !( i = ([ "half right": 1, "right": 2, "rt": 2, "around": 4, "left": 6,
      "lt": 6, "half left": 7 ])[ str ] ) ) {
    return 0;
  }
  facing = TP->query_facing();
  
  facing[ 0 ] = ( i + facing[ 0 ] ) % 8;
  write( "You reorient yourself with the world.\n" );
  if(TP->query_sneak_level())
    say( TP->one_short(1)+" reorients "+ TP->query_objective() +"self.\n");
  else
    say(TP->one_short() +" reorients "+ TP->query_objective() +"self.\n" );

  TP->set_facing(facing);
  if ( environment(TP) )
    environment(TP)->init();
  return 1;
}


mixed *query_patterns()  {
    return ({ "<string'around, [half] right, [half] left, rt, lt'>",
              (: cmd($4[0]) :) });
}
