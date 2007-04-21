// Gototh 11/04/98

inherit "/cmds/base";

varargs int cmd(string num) {
  object player, *players;
  int division, i, max, slot;
  mapping stuff;

  division = to_int(num);
  if(!division) {
    division = 50;
  }

  stuff = ([ ]);
  players = users();
  max = 0;

  foreach( player in players )  {
    if ( !player->query_creator() )  {
      slot = player->query_level() / division;
      ++stuff[slot];
      if ( slot > max )  max = slot;
    }
    else players -= ({ player });
  }

  write("%^ORANGE%^Player level distribution:%^RESET%^\n");
  for ( i = 0; i < max; i++ )  {
    if ( stuff[i] )  {
      printf( "%5d ..%4d:\t%3d\t%3d%%\n", division * i, division * (i + 1) - 1,
              stuff[i], stuff[i] * 100  / sizeof( players ) );
    }
  }
  return 1;
}
