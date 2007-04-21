/*  -*- LPC -*-  */

#define WWW_SURVEY_HANDLER "survey.c"

#include <http.h>
#include <log.h>
#include <player_handler.h>

#define MAX_IDLE_TIME 60*60 // 1 hour

#define DEMOTEXT "<Font color=red> Demo only </font>"

private nosave mapping globvars;

/* should be in globvars */
#define LAST_USE (globvars[user])[0]

private nosave int nq;


protected string startup(mapping args, string user);
protected string menu(string user);

protected mixed playerq = ({
  ({ "Now, in an average block of playing,"
       "which activity absorbs most of your time?", 
       ({ "Hack and Slash", "Socialization", "Doing Quests", "Exploring", "Other" }) }),
    ({ "Now, in an average block of playing,"
         "which activity do you devote the least time to?", 
         ({ "Hack and Slash", "Socialization", "Doing Quests", "Exploring", "Other" }) }),
    ({ "Before Quest XP was removed, in an average block of playing,"
         "which activity absorbed most of your time?", 
         ({ "Hack and Slash", "Socialization", "Doing Quests", "Exploring", "Other" }) }),
    ({ "Before Quest XP was removed, in an average block of playing,"
         "which activity did you devote the least time to?", 
         ({ "Hack and Slash", "Socialization", "Doing Quests", "Exploring", "Other" }) }),
    ({ "How would you prefer to be able to spend your time, "
         "if the opportunities for each of the following were unlimited?",
         ({ "Hack and Slash", "Socialization", "Doing Quests", "Exploring", "Other" }) }),
    ({ "Rank the following for how much they contribute to your fun as a player "
         "on the mud "
         "(eg. put 1 beside your favourite item, 2 beside your second  favourite "
         "item, etc...)",
         ({ "Social interaction with other players (eg. talker, chatting)",
              "In-character interaction with other players (roleplaying)",
              "Getting new/higher skills", "New commands", "New spells", 
              "New items (eg. Quest items, T-shop items, unusual items)",
              "Completing Quests", "Exploring new areas", 
              "New NPCs for killing", "New NPCs to interact with",
              "PK fights/stealing" }), "number" }),
    ({ "What was your main way to advance skills when Quests all gave XP?",
         ({ "Idle XP", "Quest XP", "Guild command XP", "Killing XP", "TM's" }) }),
    ({ "If all ways to get XP (except idle) gave it at the same rate, "
         "what way would you prefer to get XP?",
         ({ "Quest XP", "Guild command XP", "Killing XP" }) }),
    ({ "Has the removal of XP, quest items and rewards changed your "
         "interest in doing Quests?  Choose the response that most reflects your "
         "opinion: ",
         ({ "I no longer have any interest in Quests",
              "I have decreased interest in Quests",
              "I still have the same interest in Quests",
              "I have increased interest in Quests" }) }),
    ({ "Would you like XP for all Quests?",
         ({ "Yes","No","Don't Care" }) }),
    ({ "Is Quest XP important enough for you, that you would accept "
         "inconvenience and limitations on your play to get it back?",
         ({ "Absolutely", "Probably, Yes","Probably Not","No way" }) }),
    ({ "Would it be a good thing to limit the rate at which people "
         "can do Quests for XP?",
         ({ "Absolutely", "Probably, Yes","Probably Not","No way" }) }),
    ({ "If you had to choose to do Quests for XP or Kill for XP and you "
         "were only allowed to switch every hour or so, would that "
         "be better than no Quest XP?",
         ({ "Absolutely", "Probably, Yes","Probably Not","No way" }) }),
    ({ "If you had to choose to do Quests for XP or Kill for XP, and you "
         "were only allowed to switch every hour or so, and Killing always "
         "gave more XP would you still do quests?",
         ({ "Absolutely", "Probably, Yes","Probably Not","No way" }) }),
    ({ "Do you think special commands (eg. ambush, impale) "
         "should be limited to a single guild, "
         "or a small subset of the guilds?",
         ({ "Yes, even more than now", "Yes","Less than now","Not at all" }) }),
    ({ "How many alternate characters do you currently have "
         "(ie. not counting any that deleted)?",
         ({ "None", "1", "2", "3", "4", "5 or more", "10 or more" }) }),
    ({ "How many alternate characters do you currently have, "
         "that you play at least once a month?",
         ({ "None", "1", "2", "3", "4", "5 or more", "10 or more" }) })
    });

protected mixed creatorq = ({
  ({ "How long ago did you last actively play on Discworld? (not testing)",
       ({ "< 1 Day", "< 1 Week", "< 1 Month", "< 1 Year" }) }),
    ({ "What fraction of time do you currently spend as a player, "
         "rather than your creator alt?",
    ({ "Less than a tenth", "Up to a quarter", "Around half", "More than half"
         }) }),
  ({ "Do you think players will do Quests for no reward (eg. XP, items)?",
       ({ "Absolutely", "Probably, Yes","Probably Not","No" }) }),
    ({ "Are you disappointed that Quests you make or have made may not "
         "qualify for XP?",
         ({ "Absolutely", "Yes, somewhat","Not that much","No" }) }),
    ({ "Do you think many players will consider Hack and Slash "
         "the main remaining option with no/limited Quest XP?",
         ({ "Absolutely", "Probably, Yes","Probably Not","No" }) }),
    ({ "If there are players who strongly prefer to do Quests for XP, "
         "what should we do about it?",
         ({ "Leave them with Hack and Slash XP, work on other stuff",
              "Assume they will switch to some new non-Hack and Slash XP, work "
              "on that", 
              "Permanently leave easy-if-you-know-it Quests without XP but "
              "work on new higher effort Quests for XP",
              "Work on Quest XP rate limits or Quest mode so that Quest XP can be "
              "restored pretty much across the board" }), "long" }),
    ({ "... what should we do about Quest XP right away?",
         ({ "Nothing", "Restore Quest XP but limit the rate (eg. 1 qp per hour)", 
              "Restore Quest XP but apply rate "
              "limits and some sort of Quest mode" }), "long" }),
    });

protected void create() {
  seteuid(getuid());
  globvars = ([]);
}

private varargs string create_header(string title, string host) {
  return "<HTML>\n<HEAD>\n"
    "<TITLE>" + title + "</TITLE>\n" +
    (host ? "<META http-equiv=\"Refresh\" content=\"3600;URL=http://" + host +
     "/secure/"+WWW_SURVEY_HANDLER+"?type=timeout\">" : "") +
    "</HEAD>\n<BODY bgcolor=\"#ffffff\" text=\"#000030\" link=\"#4a529c\" "
    "vlink=\"#b57339\">\n<FONT face=\"arial,helvetica\">\n"
    "<IMG align=\"left\" src=\"/pics/dw6.gif\"></A>\n<H2 color=\"#00AADD\">"
    "Playing Styles Survey "+DEMOTEXT+"</H2>"
    "\n<H3><I>When the frogs come home to roost.</I></H3>\n<BR clear=\"left\">\n"
    "<BR>\n";
} /* create_header() */

private string create_footer(class http_request req) {
  return ("/www/footer"->www_function("footer", ([ ]), req))+"</BODY>\n</HTML>\n";
}

private string remove_control_chars(string str) {
  int i;
  
  for (i = 0; i < strlen(str); i++) {
    /* This will also change the hard space back tyo a space. */
    if ((str[i] < ' '  && str[i] != '\n' && str[i] != '\r') ||
        str[i] == 160 ) {
      str[i] = ' ';
    }
  }
  return str;
} /* remove_control_chars() */

string htmlify_no_address(string str) {
  return replace(str, ({
    "&", "&amp;",
      "<", "&lt;",
      ">", "&gt;",
      "\n", "<BR>\n",
      /* " ", "&nbsp;", */
      }) );
}

/*
 * Makes all the '<' turn into $gt; and the '>' the same.  Turn
 * http addresses into real addresses...
 */
string htmlify(string  str) {
   string *bits;
   string start;
   string end;
   string extra;
   int i;

   str =  htmlify_no_address(str);
   if (strsrch(str, "http:") > 0) {
      bits = explode("#" + str, "http:");
      bits[0] = bits[0][1..];
      for (i = 1; i < sizeof(bits); i++) {
         if (sscanf(bits[i], "%s&nbsp;%s", start, end) == 2) {
            end = "&nbsp;" + end;
            if (sscanf(start, "%s<BR>%s", start, extra) == 2) {
                end = "<BR>" + extra + end;
            }
         } else if (sscanf(bits[i], "%s<BR>%s", start, end) == 2) {
            end = "<BR>" + end;
         } else {
            start = bits[i];
            end = "";
         }
         if (start[<1] == '.') {
            start = start[0..<2];
            end = "." + end;
         }
         bits[i] = "<A href=\"http:" + start + "\">http:" + start + "</A>" +
                     end;
      }
      str = implode(bits, "");
   }

   return str;
}

protected string process_questions(mixed qs, mapping args, int nq) {
  string ret;
  mixed q,r;
  int nr;

  ret = "";
  foreach (q in qs) {
    nq++;

    if (sizeof(q) == 2 || q[2]=="long") {
      if (args["q"+nq]) ret+="\n\nQuestion: q"+nq+",  Response: "
                          +args["q"+nq]+"<BR>";
    }
    else if (q[2]=="number"){
      nr=0;
      foreach (r in q[1]) {
        nr++;
      if (to_int(args["q"+nq+"r"+nr])) ret+="\n\nQuestion: q"+nq+"r"+nr+",  Response: "
                          +args["q"+nq+"r"+nr]+"<BR>";
      }
    }
    else if (q[2]=="text"){
      nr=0;
      foreach (r in q[1]) {
        nr++;
      if (sizeof(args["q"+nq+"r"+nr])) ret+="\n\nQuestion: q"+nq+"r"+nr+",  Response: "
                          +args["q"+nq+"r"+nr]+"<BR>";
      }
    }

  }

  return ret;
}


string www_request(string, mapping args, class http_request req) {
  string ret, user;
  
  if(!req->user)
    return "Error!";

  user = req->user;

  if(!master()->valid_seteuid(this_object(), user))
    return create_header("Playing Style Survey") + "Can't set euid!" +
      create_footer(req);

  seteuid(user);
  
  if(!globvars[user]) {
    globvars[user] = ({ time(), 0, 0 });
    ret = startup(args, user);
    if(ret) {
      return create_header("Playing Style Survey") + ret + create_footer(req);
    }
  }

  LAST_USE = time();
  
  ret = process_questions( playerq, args, 0 );
  if(PLAYER_HANDLER->test_creator(user)) {
    ret += process_questions( creatorq, args, sizeof(playerq) );
  }

  ret += DEMOTEXT+" Thank you!<BR>";
  globvars[user] = 0;

  return create_header("Playing Style Survey") + ret + create_footer(req);
  
  switch(args["type"]) {
  case "mail":
    ret = "mail";
    break;
  case "comment":
    ret = "comment";
    break;
  default:
    switch(args["command"]) {
    case "Forward":
      ret = "Forward";
      break;
    default:
      ret = "Default command";
    }
  }
  return create_header("Error handler", req->hostname) + ret + menu(user) + create_footer(req);
}

protected string menu(string user) {
  string ret;

  ret = "<HR>\n<FORM action=\""+WWW_SURVEY_HANDLER+"\" method=\"GET\">\n"
    "<TABLE>\n";

  return ret;
}

protected string add_questions(mixed qs, int nq) {
  string ret, sep;
  mixed q,r;
  int nr;

  ret = "";
  foreach (q in qs) {
    nq++;
    ret +=
      "<TR>\n"
      "<TD width=20> "+nq+".</TD>"
      "<TD> "+q[0]+
      "<TR>\n"
      "<TD> &nbsp; </TD>"
      "<TD>";
    nr=0;
    sep = "";
    if (sizeof(q) == 2) {
      foreach (r in q[1]) {
        nr++;
        ret +=
          sep +"<INPUT type=\"radio\" name=\"q"+nq+"\" value=\" "+nr+" \"> "+r+"\n";
        sep = ", &nbsp; ";
      }
    }
    else if (q[2] == "long") {
      foreach (r in q[1]) {
        nr++;
        ret +=
          sep +"<INPUT type=\"radio\" name=\"q"+nq+"\" value=\" "+nr+" \"> "+r+"\n";
        sep = "<TR><TD> &nbsp; </TD><TD>";
      }
    }
    else if (q[2] == "number") {
      foreach (r in q[1]) {
        nr++;
        ret +=
          sep +"<INPUT type=\"text\" name=\"q"+nq+"r"+nr+"\" size=2> "+r+"\n";
        sep = "<TR><TD> &nbsp; </TD><TD>";
      }
    }
    else if (q[2] == "text") {
      foreach (r in q[1]) {
        nr++;
        ret +=
          sep +"<INPUT type=\"text\" name=\" q"+nq+"r"+nr+"\" size=20> "+r+"\n";
        sep = "<TR><TD> &nbsp; </TD><TD> ";
      }
    }
    ret +=
      ". </TD>";
  }

  return ret;
}

protected string startup(mapping args, string user) {
  string ret;

  ret = "<FORM method=\"GET\" action=\""+WWW_SURVEY_HANDLER+"\">\n"
    "<TABLE>\n"
    "<TR>\n"
    "<TH align=\"left\" colspan=2>Hi "+capitalize(user)+
    ",  please set your responses below:</TH>\n"
    "<TR>\n"
    "<TD BGCOLOR=\"#DDDDDD\" colspan=2> <font color=blue> Disclaimer:</font> "
    "This survey is not official: "
    "the results will not necessarily constrain the development of discworld. "
    "The discworld administration and creators take many factors into account "
    "when making big development decisions.  The purpose of this survey is to "
    "collect useful information about playing styles."
    "<TR>\n"
    "<TD  BGCOLOR=\"#00DDDD\" colspan=2> <font color=blue> Please do this survey as your "
    "main character only.</font><BR>"
    "If you have a creator alt, do the "
    "survey as that character. "
    "You will probably have to restart your browser to change character.<BR> "
    "The survey is anonymous: your name will not be associated with your responses.</TD>";
  
  if(PLAYER_HANDLER->test_creator(user)) 
    ret += "<TR>\n"
      "<TH align=\"left\" colspan=2> Player Questions:";

  ret += add_questions( playerq, 0 );

  if(PLAYER_HANDLER->test_creator(user)) {
  ret +=
    "<TR>\n"
    "<TH align=\"left\" colspan=2> Creator Questions:"
     + add_questions( creatorq, sizeof(playerq) );
  }
  

  ret +=
    "<TR>\n"
    "<TH align=\"left\" colspan=2> Click here when you are done: "
    "<INPUT type=\"submit\" value=\"Submit\"></TH>\n"
    "</TABLE>\n"
    "</FORM>\n";

  return ret;
}

void dest_me() {
  string user;

  if (sizeof(globvars)) {
    seteuid(getuid());
    foreach (user in keys(globvars)) {
      //      finish_errors(user);
    }
  }
  destruct(this_object());
}

int clean_up(int) {
  if (sizeof(globvars)) {
    return 1;
  }
  dest_me();
}

mixed *stats() {
  return ({
    ({ "users", sizeof(globvars) ? implode(keys(globvars), ", ") : "nobody" })
  });
}
