/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: tactics.c,v 1.4 2002/10/24 04:19:04 ceres Exp $
 * $Log: tactics.c,v $
 * Revision 1.4  2002/10/24 04:19:04  ceres
 * fixed typo
 *
 * Revision 1.3  2002/09/21 02:29:32  ceres
 * Added attack hand
 *
 * Revision 1.2  2002/08/12 00:19:11  presto
 * Fixed typo: opponents --> opponent's
 *
 * Revision 1.1  2002/08/03 19:27:13  ceres
 * Initial revision
 *
 * Revision 1.2  2000/04/22 05:33:22  ceres
 * Fixed bug with error return
 *
 * Revision 1.1  1998/01/06 05:28:43  ceres
 * Initial revision
*/
inherit "/cmds/base";

#include <combat.h>

int cmd() {
  class tactics tactics;

  tactics = (class tactics) this_player()->query_tactics();
  
  write("Your combat options are:\n\n" );
  write("   Attitude - "+ tactics->attitude +".\n" );
  write("   Response - "+ tactics->response +".\n" );
#ifdef USE_SURRENDER
  write("   Mercy - "+ (tactics->mercy?tactics->mercy:"none") + ".\n");
#endif  
  write("   Focus - "+ (tactics->focus_zone?tactics->focus_zone:"none") + ".\n" );
#ifdef USE_DISTANCE
  write("   Target distance - "+ (tactics->ideal_distance?tactics->ideal_distance:"none") + ".\n" );
#endif  
  write("You will use "+
        (tactics->attack == "both" ? "both hands" :
         "your "+ (tactics->attack?tactics->attack:"either") +" hand" ) +
        " to attack.\n" );
  write("You will use "+
        (tactics->parry == "both" ? "both hands" :
         "your "+ (tactics->parry?tactics->parry:"either") +" hand" ) +
        " to parry.\n" );
  write("\nYou will"+ ( tactics->parry_unarmed ? " " : " not " ) +
        "attempt to parry unarmed.\n" );
  return 1;
}

int attitude(string word) {
  class tactics my_tactics;

  my_tactics = new(class tactics);
  my_tactics = (class tactics) this_player()->query_tactics();
  
  switch(word) {
  case "insane":
  case "offensive":
  case "neutral":
  case "defensive":
  case "wimp":
    if((my_tactics->attitude) == word) {
      write("Your attitude is already " + word + ".\n");
    } else {
      my_tactics->attitude = word;
      write("Your attitude is now " + word + ".\n");
      this_player()->set_tactics(my_tactics);
    }
    return 1;
  default:
    return notify_fail("Syntax: tactics attitude "
                       "insane|offensive|neutral|defensive|wimp.\n");
  }
}

int response(string word) {
  class tactics my_tactics;

  my_tactics = this_player()->query_tactics();

  switch(word) {
  case "dodge":
  case "neutral":
  case "parry":
    if(my_tactics->response == word) {
      write("Your response is already " + word + ".\n");
    } else {
      my_tactics->response = word;
      write("Your response is now " + word + ".\n");
      this_player()->set_tactics(my_tactics);
    }
    return 1;
  default:
    return notify_fail( "Syntax: tactics response dodge|neutral|parry\n");
  }
}

int parry(string word) {
  class tactics my_tactics;

  my_tactics = this_player()->query_tactics();

  switch(word) {
  case "unarmed":
    my_tactics->parry_unarmed = 1 - my_tactics->parry_unarmed;
    write("You will"+ ( my_tactics->parry_unarmed ? " " : " not " ) +
          "now attempt to parry unarmed.\n" );
    this_player()->set_tactics(my_tactics);
    return 1;
  case "left":
  case "right":
  case "both":
    if(my_tactics->parry == word) {
      write("You are already using "+
            ( word == "both" ? "both hands" :
              "your "+ word +" hand" ) +" to parry.\n" );
    } else {
      my_tactics->parry = word;
      write("You will now use "+
            ( word == "both" ? "both hands" :
              "your "+ word +" hand" ) +" to parry.\n" );
      this_player()->set_tactics(my_tactics);
    }
    return 1;
  default:
    return notify_fail( "Syntax: tactics parry "+
                        "left|right|both [hand(s)]\n        "
                        "tactics parry unarmed\n" );
  }
}

int attack(string word) {
  class tactics my_tactics;

  my_tactics = this_player()->query_tactics();

  switch(word) {
  case "left":
  case "right":
  case "both":
    if(my_tactics->attack == word) {
      write("You are already using "+
            ( word == "both" ? "both hands" :
              "your "+ word +" hand" ) +" to attack.\n" );
    } else {
      my_tactics->attack = word;
      write("You will now use "+
            ( word == "both" ? "both hands" :
              "your "+ word +" hand" ) +" to attack.\n" );
      this_player()->set_tactics(my_tactics);
    }
    return 1;
  default:
    return notify_fail( "Syntax: tactics attack "+
                        "left|right|both [hand(s)]\n" );
  }
}

int focus(string word) {
  class tactics my_tactics;

  my_tactics = this_player()->query_tactics();

  switch(word) {
  case "head":
  case "neck":
  case "chest":
  case "abdomen":
  case "arms":
  case "hands":
  case "legs":
  case "feet":
  case "upper body":
  case "lower body":
    if(my_tactics->focus_zone == word) {
      write("You are already focussing on your opponent's "+ word + ".\n");
    } else {
      my_tactics->focus_zone = word;
      write("You will now focus on your opponent's "+ word + ".\n");
      this_player()->set_tactics(my_tactics);
    }
    return 1;
  case "none":
    if(!my_tactics->focus_zone) {
      write("You are not currently focussing on any particular target "
            "area.\n");
    } else {
      my_tactics->focus_zone = 0;
      write("You will no longer focus on a specific target area.\n");
      this_player()->set_tactics(my_tactics);
    }
    return 1;
  default:
    return notify_fail( "Syntax: tactics focus " +
                        "upper body|lower body|head|neck|chest|abdomen|arms|hands|legs|feet|none\n");
  }
}

int distance(string word) {
  class tactics my_tactics;

  my_tactics = this_player()->query_tactics();
  switch(word) {
  case "long":
  case "medium":
  case "close":
  case "hand-to-hand":
    if(my_tactics->ideal_distance == word) {
      write("You are already set for " + word + " range.\n");
      return 1;
    }
    my_tactics->ideal_distance = word;
    write("You will now aim for "+ word + " range combat.\n");
    this_player()->set_tactics(my_tactics);
    return 1;

  case "none":
    if(!my_tactics->ideal_distance) {
      write("You are not currently aiming for any specific combat distance.\n");
    } else {
      my_tactics->ideal_distance = 0;
      write("You will no longer aim for a specific combat distance.\n");
      this_player()->set_tactics(my_tactics);
    }
    return 1;
  default:
    return notify_fail( "Syntax: tactics distance " +
                        "{long|medium|close|hand-to-hand\n");
  }
}

#ifdef USE_SURRENDER
int mercy(string word) {
  class tactics my_tactics;

  my_tactics = this_player()->query_tactics();
  switch(word) {
  case "always":
  case "ask":
  case "never":
    if(my_tactics->mercy == word) {
      write("Your mercy is already " + word + ".\n");
    } else {
      my_tactics->mercy = word;
      write("Your mercy is now "+ word +".\n" );
      this_player()->set_tactics(my_tactics);
    }
    return 1;
  default:
    return notify_fail("Syntax: tactics mercy always|ask|never\n");
  }
}
#endif

mixed *query_patterns() {
  return ({ "", (: cmd() :),
              "attitude <word'insane|offensive|neutral|defensive|wimp'>",
              (: attitude($4[0]) :),
              "response <word'dodge|neutral|parry'>", (: response($4[0]) :),
#ifdef USE_SURRENDER              
              "mercy <word'always|ask|never'>", (: mercy($4[0]) :),
#endif              
              "parry <word'left|right|both|unarmed'>", (: parry($4[0]) :),
              "attack <word'left|right|both'>", (: attack($4[0]) :),
              "focus <string'upper body|lower body|head|neck|chest|abdomen|"
              "arms|hands|legs|feet|none'>", (: focus($4[0]) :),
#ifdef USE_DISTANCE             
              "distance <word'long|medium|close|hand-to-hand|none'>",
              (: distance($4[0]) :),
#endif              
              });
}
