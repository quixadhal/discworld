/*  -*- LPC -*-  */
#include "term.h"
#include <http.h>
#include <player_handler.h>

void create() {
  seteuid(getuid());
} /* create() */

int can_reference(object thing, string user) {
   string *allowed;

   if (thing->query_invis()) {
      if (!user || thing->query_login_ob()) {
         return 0;
      }
      allowed = (string *)thing->query_allowed();
      if ( pointerp( allowed ) ) {
          if ( member_array( user, allowed ) != -1 ) {
              return 1;
          }
      }
   }
   switch ( (int)thing->query_invis() ) {
      case 3 :
         return (int)master()->query_trustee( user );
      case 2 :
         return (int)master()->query_director( user );
      case 1 :
         return (int)PLAYER_HANDLER->test_creator( user );
      default :
         return 1;
   }
} /* can_reference() */

/**
 * This function is from /www/boards.c, it effectively removes embedded
 * HTML. 
 * @param str the string to remove html from
 */
string htmlify_no_address(string str) {
    return replace(str, ({
        "&", "&amp;",
        "<", "&lt;",
        ">", "&gt;",
        /* " ", "&nbsp;", */ 
        "\n", "<br>\n",
      }) );
} /*  htmlify_no_address() */


string www_who_line(object ob, string user) {
  string ret, tmp;

  if(!can_reference(ob, user)) {
    return "";
  }
  
  ret = "<B><A HREF=\"/" + (user?"secure/":"") +
    "finger.c?player="+ob->query_name()+
    "\">"+ob->query_cap_name()+"</A></B>";
  if(ob->query_property("guest"))
    ret += " guest of Discworld";
  else if (tmp = (string)ob->query_gtitle())
    ret += htmlify_no_address(" " + tmp);
  else
    ret += " the adventurer";
  if (tmp = (string)ob->query_property("player_title"))
    ret += htmlify_no_address(", " + tmp);
  if (tmp = (string)ob->query_title())
    ret += htmlify_no_address(", " + tmp);
/*
  if (tmp = (string)ob->query_atitle())
    ret += " (" + tmp + ")";
 */

  switch(ob->query_object_type()) {
  case "T":
    ret += " <B>Trustee</B>";
    break;
  case "D":
    ret += " <B>Director</B>";
    break;
  case "S":
    ret += " <B>Senior Creator</B>";
    break;
  case "C":
    ret += " <B>Creator</B>";
    break;
  case "p":
    ret += " <B>Playtester</B>";
    break;
  }
  switch ( ob->query_invis() ) {
    case 3 :
      ret += ", <b>(Trustee Invisible)</b>";
      break;
    case 2 :
      ret += ", <b>(Director Invisible)</b>";
      break;
    case  1 :
      ret += ", <b>(Invisible)</b>";
      break;
  }
  ret += "<BR>\n";
  return ret; 
}

string www_request(string name, mapping names, class http_request req) {
  string ret;

  ret = "<html><head>"
      "<title>Discworld's player information center</title>"+
      "</head><body bgcolor=\"#ffffff\" TEXT=\"#000030\" LINK=\"#4a529c\" "
      "VLINK=\"#b57339\"><font face=\"arial,helvetica\">"
      "<h2>Player Information Centre</h2>";
  ret += "<!-- " + req->user+ "(ps: to whoever noticed this, sorry, your right) -->\n";
    
  if (mapp(names) && names["player"]) {
    string finger;

    names["player"] = htmlify_no_address(names["player"]);
    ret += "<h3>Information on "+capitalize(names["player"])+":</h3>";
    names["player"] = lower_case(names["player"]);

//    if (file_size("/d/"+names["player"]) != -1) {
//      ret += "/www/domains"->domain_info(names["player"]);
//    } else {
      if (file_size("/w/"+names["player"]+"/public_html/index.html") > 0)
        ret += "<h4><a href=\"../~"+names["player"]+"/index.html\">"+
               capitalize(names["player"])+"'s home page</a></h4>";

      finger = "/secure/finger"->www_finger_info(names["player"], req->user);
      if (finger)
        ret += finger;
      else
        ret += "Sorry, "+names["player"]+" has not visited us yet.  "
               "Perhaps you should "
               "suggest it to them.<p>\n";
//    }
  } else {
    object *players;
    string tmp;
    int i;
    
    players = filter(users(), (: $1 :));

    if(!sizeof(players))
      ret += "<h3>There is noone logged on at the moment.</h3>";
    else {
      players = sort_array(players, (: strcmp($1->query_name(), 
                                         $2->query_name()) :) );
      ret += sprintf("<h3>There are currently %d players on Discworld.</h3>",
                     sizeof(users()));
    
      for(i=0; i<sizeof(players); i++) {
        tmp = www_who_line(players[i], req->user);
        if(tmp)
          ret += tmp;
      }
    }
    ret += "<hr><form action=\"finger.c\" METHOD=GET>"+
      "<b>Query:</b> <input type=\"text\" name=\"player\" size=62><p>"+
        "<p>"+
          "<input type=\"submit\" value=\"Find info\">"+
            "<input type=\"reset\" value=\"Reset\">"+
              "</form>";
  }

  if (!req->user) {
     ret += "<hr width=50%>\n <center><b>"
            "<a href=\"http://discworld.imaginary.com:5678/secure/finger.c";
     if (mapp(names) && names["player"]) {
        ret += "?player=" + names["player"];
     }
     ret += "\">[ Login ]</a></b></center>\n";
  }

  ret = strip_colours(ret);
  ret += "/www/footer"->www_function("footer", ([ ]), req);
  return ret;
} /* www_request() */
