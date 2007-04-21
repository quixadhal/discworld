/*  -*- LPC -*-  */
/* change log */

/*  26/6/93   Add in codes by LIFE so that the object can actually be
 *            used to take photos instead of just a useless lump.
 *            Godot's original setup() is mostly kept but all the rest
 *            are my own code. 
 */

#include <weather.h>

inherit "/std/object";

int  colour;
int  black_white;
int  scene;

void setup() {
  colour = 2;
  black_white = 4;
  scene = 1;

  set_name( "box" );
  add_adjective( ({ "heavy" , "black" }) );
  set_short( "heavy black cube" );
  set_main_plural( "heavy black cubes" );
  set_long( "You see a heavy black box with a window on one side.  "
            "Looking through the window, you notice a tiny demon sitting "
            "patiently next to a miniscule painter's easel.\n" );
  set_weight( 10 );
  add_alias( ({ "cube" , "camera" }) );
  add_plurals(({ "cubes", "cameras"}));
  adjust_money( 1 , "gold" );
  add_help_file("camera");
/*
  set_read_mess("To make the demon paint the picture, simply \"use"
                " camera on <subject>\".\n", "common", 1);
 */
}
 

void init() {   
    add_command("use", "<direct:object> on <indirect:object>");
    add_command("use", "<direct:object>");
}

int query_colour()      { return colour;  }
int query_black_white() { return black_white;  }

int do_use(object *obs) {   
  int i;
  string view;
  string photo_of;
  string chars_sorted;
  string things_sorted;
  object *players;
  object *chars;
  object *things;
  object  photo;
  object  env;
  object *yes;
   
  players = ({ });
  chars = ({ });
  things = ({ });
  yes = ({ });
  view = "";
  photo_of = "";
  chars_sorted = "";
  things_sorted = "";
 
  env = environment(this_player()) ;
  photo_of = "This is a beautiful "; 

  if (env->query_light() < 60) {
    this_player()->add_failed_mess(this_object(),
                                   "The little imp whispers: Hey! "
                                   "Not everyone like you can see "
                                   "in the dark you know.\n",({ }));
    return 0;
  }
  
  if (env->query_light() >180) {
    this_player()->add_failed_mess(this_object(),
                                   "The little demon said quietly: "
                                   "Could I have a pair of sunspecs "
                                   "please! Not too much to ask, is it?\n",
                                   ({ }));
    return 0;
  }
  
  if (black_white <= 0) {
    call_out("out_of_paint", 0, env);
    this_player()->add_failed_mess(this_object(), 
                                   "The little imp gives up painting "
                                   "for you.\n",({ }));
    return 0;
  }
  
  if (colour > 1) {
    photo_of += "colour ";
    colour --;
  } else if (colour == 1) {
    photo_of += "colour ";
    call_out("out_of_colour", 2, env); 
    colour --;
  } else {
    photo_of += "black and white ";
    black_white --;
  }

  if (sizeof(obs)) {
    for (i=0;i<sizeof(obs);i++) {
      if (obs[i]->query_property("player")) {
        players += ({ obs[i] });
        this_player()->add_succeeded(obs[i]);
        yes  += ({ obs[i] });
      } else if (obs[i]->query_property("npc")) {
        chars   += ({ obs[i] });
        this_player()->add_succeeded(obs[i]);
        yes  += ({ obs[i] });
      } else {
        if (environment(obs[i]) != this_player()) {
          things  += ({ obs[i] });
          this_player()->add_succeeded(obs[i]);
          yes  += ({ obs[i] });
        }
      }
      
    }
    chars_sorted = query_multiple_short(chars);

    things_sorted = query_multiple_short(things);
    
    if (chars_sorted || things_sorted || sizeof(players)) {
      scene = 0;
    }
  }

  view = env->query_long();

  if(view[sizeof(view)-1] != 10)
    view += "\n";
  
  if(env->query_property("location") == "outside" &&
     !env->query_weather_obscured(WEATHER->query_day())) {
    view = WEATHER->weather_string(env) + ".  " + view;
  }
  
  photo_of += "picture of " + env->a_short() + ".";

  if (scene == 0) {
    if ((sizeof(chars)+sizeof(players)) == 1) {
      if (sizeof(chars) == 1) {
        view += capitalize( (string)chars[0]->query_short() ) +
          " is here smiling.\n";
      } else if (sizeof(players) == 1) {
        view += players[0]->one_short() +" is here smiling.\n";
      }
    } else if ((sizeof(players)+sizeof(chars)) > 1) {
      for(i=0;i<sizeof(players);i++) {
        view += players[i]->one_short() ;
        if (i < (sizeof(players)-1)) {
          view += ", ";
        }
      }

      if (chars_sorted != "" && sizeof(players) > 0) {
        view += ", " + chars_sorted ;
      } else if (chars_sorted != "") {
        view += capitalize(chars_sorted) ;
      }
      view += " are smiling here.\n";
    }   

    if (sizeof(things) == 1) {
      
      view += "There is a "+ things_sorted +" in the photo.\n";
    } else if ( sizeof( things ) ) {
      view += "There are " + things_sorted+" lying here in the photo.\n" ;
    }
  }
  
  photo = clone_object("/std/object");
  photo->set_name("glass");
  photo->add_adjective( ({"tiny", "square"}) );
  photo->set_short("tiny square of glass");
  photo->set_main_plural("tiny squares of glass");
  photo->set_long("/global/events"->convert_message(view));
  photo->set_read_mess(photo_of);
  photo->add_property("photographer", (string)this_player()->query_name()) ;
  photo->move(this_player());
  
  if (scene == 0) {
    this_player()->add_succeeded_mess(this_object(),
                                      "$N $V $D on $I. After some frantic "
                                      "sounds of pens scratching and "
                                      "brushes brushing, a tiny "
                                      "demon inside hands a piece of painted "
                                      "glass to $N.\n", yes);
  } else {
    this_player()->add_succeeded_mess(this_object(),
                                      "$N $V $D on the scenery. After some "
                                      "frantic sounds of pens scratching "
                                      "and brushes brushing, a "
                                      "tiny demon inside hands a piece "
                                      "of painted glass to $N.\n");
  }
  
  return 1;
}

mapping query_static_auto_load() {
  return ([ "colour" : colour,
          "black_white" : black_white ]);
}

void init_static_arg(mapping map) {
  colour = map["colour"];
  black_white = map["black_white"];
}

void out_of_paint(object room) {    
  tell_object(this_player(), "The tiny demon whispers sarcastically: "
              "Without any more paints, I can still paint "
              "you invisible picture if you like!\n");
  tell_room(room, "The tiny demon in the cube whispers something about "
            "paints to "+ this_player()->one_short() +".\n",
            this_player());
  
}

void out_of_colour(object room) {
  tell_object(this_player(), "The tiny demon whispers: There are no more "
              "colour paints. This picture is the "
              "last colour one. I can only do you "
              "black and white pictures from now!\n");
  tell_room(room, "The tiny demon in the cube whispers something about "
            "colour to "+ this_player()->one_short() +".\n",
            this_player());
}

