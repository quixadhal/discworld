functions available and parameters:
 
set_short(string short_desc);
 
set_long(string long_desc_with_no_\n's);
 
set_light(int light);
 
set_zone(string zone_name);    /* used for wandering monsters */
 
add_exit(string direction, string destination, string exit_type);
 
add_item(string item, string description);
 
add_alias(string alias, string original);
 
 
Examples of use:
 
set_short("Corridor");
set_short("Large Plain");
 
set_long("You are travelling down a small candlelit corridor.  Candles line "+
         "the walls of the corridor.  The corridor continues to the north "+
         "and south.  There is a door further to the north.");
set_long("You are standing on a large plain.  It extends in all directions "+
         "as far as you can see.  There are small bushes here and there");
 
set_light(40);
set_light(100);
 
set_zone("Assassin Basement");
set_zone("AM Plains");
 
add_exit("north", "/room/goa/kitchen", "door");
add_exit("south", "/room/goa/corridor4", "corridor");
add_exit("north", "/room/for/plain2", "plain");
add_exit("south", "/room/am/insidecity", "gate");
 
add_item("candle", "The candles hang from the wall and provide an eerie lighting");
add_item("candles", "They line the corridor providing an errie light");
add_item("bush", "The bushes are a very boring variety indigenous to the area");
add_item("bushes", "The are dotted over the countryside.  They aren't very "+
         "interesting really");
 
add_alias("light", "candle");
add_alias("shrub", "bush");
add_alias("shrubs", "bushes");
 
NOTES:
 
Please refer to the light document to see the list of appropriate light
values for certain types of rooms.
 
Currently supported types of exits are :-
  plain,
  gate,
  corridor,
  door,
  unique
 

See also
    std.object
