/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: weather_room.c,v 1.1 1998/01/06 04:23:53 ceres Exp $
 * $Log: weather_room.c,v $
 * Revision 1.1  1998/01/06 04:23:53  ceres
 * Initial revision
 * 
*/
inherit "std/room";

mixed leverarr;

reset (arg) {
  if (arg) return;

  set_short("The weather room");
  set_long("The weather control center for the entire mud. On the floor should\n"+
           "be the controller itself, scattered around the room are hundred of impresive\n"+
           "flashing lights, dials and levers. You get the feeling however that they dont\n"+
           "actually do anything.\n");

  set_light(1);
  seteuid("pinkfish");
  add_property("inside");
  add_exit("pinkfish","d/Fish/pinkfish/workroom");
  add_item("lights","What did I say? there are lots of them and they are insesantly\n"+
           "flashing.\n");
  add_item("dials","Large dials with all sort of things on them. One you can see says\n"+
           "'Colour of spring', it is currently pointing at purple.\n");
  add_item("levers",
           "The levers are large multicolour protuberances which are scattered around the\n"+
           "room in random profusion.\n");
  leverarr = ({ ({ "blue",
       "The rooms spins and a small bit of printed paper falls from the\n"+
       "Weather controller saying:\n#query_rain" }),
                ({ "white",
       "The ground heaves in a major convulsion and a mound of paper falls onto\n"+
       "You from the ceiling saying:\n#query_cloud" }) });
       
}

init() {
  ::init();
  add_action("pull","pull");
  add_action("push","push");
}


pull(str) {
  int i;
  string type,rand;
  object lever, weather;

  if (str == "lever" || str == "levers")
    lever = leverarr[random(sizeof(leverarr))];
  else {
    if (!sscanf(str,"%s lever%s",type,rand)) {
      notify_fail("You have to pull a lever.\n");
      return 0;
    }
    for (i=0;i<sizeof(leverarr);i++) {
      if (leverarr[i][0] == type) {
        lever = leverarr[i];
        break;
      }
    }
  }
  if (!lever) {
    notify_fail("You need to choose and existing lever");
    return ;
  }

  weather = present("weather",this_object());
  if (!weather) {
    notify_fail("Opps the weather controller does not exist.\n");
    return 0;
  }
  sscanf(lever[1],"%s#%s",type,rand);
  write(type);
  this_player()->print_object(call_other(weather,rand));
  return 1;
}

push(str) {
  object lever;
  int i;
  string type,bing;

  if (str == "levers" && str == "lever")
    lever = leverarr[random(sizeof(leverarr))];
  else {
    if (!sscanf(str,"%s lever%s",type,bing)) {
      notify_fail("You must push an existing lever.\n");
      return 0;
    }
    for (i=0;i<sizeof(leverarr);i++)
      if (type == leverarr[i][0]) {
        lever = leverarr[i];
        break;
      }
  }
  if (!lever) {
    notify_fail("An existing lever could be usefull...\n");
    return 0;
  }

  write("You huff and you puff, but you just cant push that lever.\n");
}
