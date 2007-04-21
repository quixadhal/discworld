/**
 * This will print out a list of all the current quests
 * @author Ceres
 */
#include <http.h>
#include <quest_handler.h>
#include <library.h>
#include <player_handler.h>
#undef SAVE_FILE
#define SAVE_FILE "/save/www/quests"

#define TEXTS_DIR "/save/quests/"
#define NUM_COLS 4

// Minimum number of times the quest must be done before it is shown here.
#define MIN_TIMES 100
// How long the player must be on to timeout seen hints.
#define TIMEOUT (3600 * 24)
// Delay between successive hints (in player age).
#define HINT_DELAY (3600 * level)

#define MAX_LIST 20
#define MAX_HINTS ({ 10, 5, 2 })

// When sorting the list how much weight to give to a quests level as
// opposed to how many times it's been done.
#define WEIGHTING 500

mapping seen_quests;

nosave string *ql, *qt;

class seen_info {
  string which;
  int time;
  int level;
}

int sort_list(string first, string second) {
  int lfirst, lsecond;

  // sort by level
  lfirst = ( 15 - QUEST_HANDLER->query_quest_level(first)) * WEIGHTING +
    QUEST_HANDLER->query_quest_times(first);
  lsecond = (QUEST_HANDLER->query_quest_level(second) * WEIGHTING) +
    QUEST_HANDLER->query_quest_times(second);

  if(lfirst > lsecond)
    return -1;
  if(lsecond < lfirst)
    return 1;

  return 0;
}

void create() {
  int i;
  
  seteuid("Room");
  unguarded( (: restore_object(SAVE_FILE) :) );
  if (!seen_quests) {
      seen_quests = ([ ]);
   }
   ql = QUEST_HANDLER->query_quest_names();
   ql = sort_array(ql, (: sort_list($1, $2) :));
   qt = allocate(sizeof(ql));
   for(i=0; i<sizeof(ql); i++)
     qt[i] = QUEST_HANDLER->query_quest_title(ql[i]);
}

private string create_header(string title) {
  return "<HTML>\n<HEAD>\n"
    "<TITLE>" + title + "</TITLE>\n"
    "</HEAD>\n<BODY bgcolor=\"#ffffff\" text=\"#000030\" link=\"#4a529c\" "
    "vlink=\"#b57339\">\n<FONT face=\"arial,helvetica\">\n"
    "<IMG align=left src=\"/pics/dw6.gif\"></A>\n"
    "<H2>Discworld Quest Page</H2>\n"
    "<H3><I>For what it's worth.</I></H3>\n<BR clear=\"left\">\n"
    "<BR>\n";
} /* create_header() */

private string create_footer(class http_request req) {
  return ("/www/footer"->www_function("footer", ([ ]), req))+"</BODY>\n</HTML>\n";
}

private string htmlify(string str) {
   return replace(str, ({
                         "&", "&amp;",
                         "<", "&lt;",
                         ">", "&gt;",
                         "\n", "<br>\n",
                         }) );
}

private string calc_filename(string qname) {
  string fname;

  fname = lower_case(qname);
  fname = replace(fname, ({ " ", "_", "'", "" })) + ".txt";
  return fname;
}

private string get_file(string fname) {
  string tmp;
  tmp = unguarded( (: read_file, TEXTS_DIR + fname :) );
  if(tmp)
    tmp = htmlify(tmp);
  return tmp;
}

private string get_plain_file(string fname) {
  string tmp;
  tmp = unguarded( (: read_file, TEXTS_DIR + fname :) );
  return tmp;
}

private void save_file() {
  unguarded( (: save_object(SAVE_FILE) :) );
}

int can_see_hint(string name, string which, int level) {
  class seen_info tmp;
  int hints, i, age;
  
  if ("/secure/bastards"->query_suspended(name))
    return 0;

  if(!PLAYER_HANDLER->test_active(name))
    return 0;

  if(LIBRARY->query_quest_done(name, which))
    return 0;

  age = - (PLAYER_HANDLER->test_age(name));

  for(i=0; i<sizeof(seen_quests[name]); i++) {
    tmp = seen_quests[name][i];
    // remove quests which they've done or which have timedout.
    if(LIBRARY->query_quest_done(name, tmp->which) ||
       tmp->time < age - TIMEOUT) {
      seen_quests[name] = seen_quests[name][0..i-1] +
         seen_quests[name][i+1..];
       save_file();
    } else if(tmp->which == which) { // Ok, this quest is on the list.

      // they've seen this level or higher so let them see it again.
      if(tmp->level >= level)
        return 1;

      // they saw the last level too recently so tell them so.
      if(tmp->time > (age - HINT_DELAY))
        return -1;

      return 1;
    } else if(tmp->level == level) {
      hints++;
    }
  }
  return (hints < (MAX_HINTS[level-1]));
}

void add_seen(string name, string which, int level) {
  class seen_info seen, tmp;
  int i;
  
  if(LIBRARY->query_quest_done(name, which))
    return;
  
  seen = new(class seen_info,
             which : which,
             time : - (PLAYER_HANDLER->test_age(name)),
             level : level);

  if(seen_quests[name]) {
    for(i = 0; i < sizeof(seen_quests[name]); i++) {
      if(seen_quests[name][i]->which == which) {
        tmp = (class seen_info)seen_quests[name][i];
        if(level > tmp->level)
          tmp->level = level;
        seen_quests[name][i] = tmp;
        save_file();
        return;
      }
    }
    seen_quests[name] += ({ seen });
  } else
    seen_quests[name] = ({ seen });
  save_file();
}

string www_request(string str, mapping args, class http_request req) {
   string ret, user, fname, tmp;
   int i, j, k;

   user = req->user;

   if(!PLAYER_HANDLER->test_creator(user))
     return create_header("Quest Hints") + "You are not a creator!" +
       create_footer(req);
#ifdef 0
   if(!"/secure/master"->query_leader(user) &&
      user != "valentijn") {
     return create_header("Quest Hints") + "This page is currently only "
       "available to lords.\n" + create_footer(req);
   }
#endif   
   ret = create_header("Quest List");

   ret += "";

   if(args["param"])
     sscanf(args["param"], "%d", i);
   
   switch(args["op"]) {
   case "write":

     if(req->body->data["hint"] != "0") {
       fname = "hints/" + calc_filename(ql[i]);
       ret += "Writing hint to: " + TEXTS_DIR + fname + "<br>";
       ret += "[" + req->body->data["hint"] + "]<br>";
       unguarded((: write_file, TEXTS_DIR + fname,
                  req->body->data["hint"] + "\n", 1 :));
     }

     if(req->body->data["second_hint"] != "0") {
       fname = "second_hints/" + calc_filename(ql[i]);
       ret += "Writing second hint to: " + TEXTS_DIR + fname + "<br>";
       ret += "[" + req->body->data["second_hint"] + "]<br>";
       unguarded((: write_file, TEXTS_DIR + fname,
                  req->body->data["second_hint"] + "\n", 1 :));
     }

     if(req->body->data["final_hint"] != "0") {
       fname = "final_hints/" + calc_filename(ql[i]);
       ret += "Writing final hint to: " + TEXTS_DIR + fname + "<br>";
       ret += "[" + req->body->data["final_hint"] + "]<br>";
       unguarded((: write_file, TEXTS_DIR + fname,
                  req->body->data["final_hint"] + "\n", 1 :));
     }

     ret += "\n<p><a href=\"quests.c\">Return to main list.</a></p>\n";
     break;
   case "add":
     ret += "<p><b>Title</b>: " + QUEST_HANDLER->query_quest_title(ql[i]) +
       "<br>";
     ret += "<b>Story</b>: " + QUEST_HANDLER->query_quest_story(ql[i])+
       "</p>";
     fname = calc_filename(ql[i]);
     ret += "\n<form action=\"quests.c?op=write&param="+i+"\" "
       "method=\"post\">\n"
       "<br>First Hint:<br>\n"
       "<textarea name=\"hint\" cols=75 rows=10>" +
       get_plain_file("hints/"+fname) +
       "</textarea>\n"
       "<br>Second Hint:<br>\n"
       "<textarea name=\"second_hint\" cols=75 rows=10>" +
       get_plain_file("second_hints/"+fname) +
       "</textarea>\n"
       "<br>Final Hint: <br>\n"
       "<textarea name=\"final_hint\" cols=75 rows=10>" +
       get_plain_file("final_hints/"+fname) +
       "</textarea>\n"
       "<br><input type=\"submit\">\n"
       "</form>\n";
     break;
   case "hint":
     ret += "<h3>Quest Hint</h3>\n";
     ret += "<p><b>Title</b>: " + QUEST_HANDLER->query_quest_title(ql[i]) +
       "<br>";

     if(!can_see_hint(user, ql[i], 1)) {
       ret += "<p>Sorry, you have seen all the hints you're allowed for the "
         "moment. Why not go and complete some of those quests you've "
         "been shown then more will be available.</p>\n";
     } else {
       add_seen(user, ql[i], 1);
       
       fname = "hints/" + calc_filename(ql[i]);
       ret +=  "<p><b>First Hint</b>:<br> " + get_file(fname) + "</p>";

       fname = "second_hints/" + calc_filename(ql[i]);
       if(file_size(TEXTS_DIR + fname) > 0) {
         ret += "<b>Second Hint</b>: "
           "<a href=\"quests.c?op=second_hint&param="+i+"\">Available</a><br>";
       } else {
         ret += "<b>No second hint available</b><br>\n";

         if(file_size(TEXTS_DIR + fname) > 0)

         ret += "<b>Final Hint</b>: <a href=\"quests.c?op=final_hint&param="+i+"\">"
           "Available</a><br>";
       else
         ret += "<b>No final hint available</b><br>\n";
       }
     }
     break;
     
   case "second_hint":
     ret += "<h3>Quest Hint</h3>\n";
     ret += "<p><b>Title</b>: " + QUEST_HANDLER->query_quest_title(ql[i]) +
       "<br>";

     switch(can_see_hint(user, ql[i], 2)) {
     case 0:
       ret += "<p>Sorry, you have seen all the second hints you're allowed "
         "for the moment. Why not go and complete some of those quests you've "
         "been shown then more will be available.</p>\n";
       break;
     case -1:
       ret += "<p>You've only just seen the first hint. Why not try solving "
         "the quest before seeing the next hint.</p>\n";
       break;
     default:
       add_seen(user, ql[i], 2);
       
       fname = "second_hints/" + calc_filename(ql[i]);
       ret +=  "<p><b>Second Hint</b>:<br> " + get_file(fname) + "</p>";
       
       if(file_size(TEXTS_DIR + fname) > 0)
         ret += "<b>Final Hint</b>: "
           "<a href=\"quests.c?op=final_hint&param="+i+"\">Available</a><br>";
       else
         ret += "<b>No final hint available</b><br>\n";
     }
     
     break;

   case "final_hint":
     ret += "<h3>Final Hint</h3>\n";
     ret += "<p><b>Title</b>: " + QUEST_HANDLER->query_quest_title(ql[i]) +
       "<br>";

     switch(can_see_hint(user, ql[i], 3)) {
     case 0:
       ret += "<p>Sorry, you have seen all the final hints you're allowed for "
         "the moment. Why not go and complete some of those quests you've "
         "been shown then more will be available.</p>\n";
       break;
     case -1:
       ret += "<p>You've only just seen the second hint. Why not try solving "
         "the quest before seeing the final hint.</p>\n";
       break;
     default:
       add_seen(user, ql[i], 3);
       fname = "final_hints/" + calc_filename(ql[i]);
       ret +=  "<p><b>Final Hint</b>:<br> "+ get_file(fname) + "</p>";
     }
     break;
#ifdef 0     
   case "search":
     ret += "<h3>Search Results</h3>\n";

     tmp = "<ul>\n";
     for(i=0, j=0; i<sizeof(ql); i++) {
       if(strsrch(lower_case(ql[i]), lower_case(args["search"])) != -1 ||
          strsrch(lower_case(qt[i]), lower_case(args["search"])) != -1) {
         tmp += "<li><a href=\"quests.c?op=basic&param="+i+"\">" +
           lower_case(ql[i]) + "</a>\n";
         j++;
       }
     }

     if(!j)
       ret += "<p>No matches.</p>"
         "<form action=\"quests.c\" method=\"get\">\n"
         "<input type=\"hidden\" name=\"op\" value=\"search\">\n"
         "Search: <input type=\"text\" name=\"search\">\n"
         "</form>\n";
     else if(j > 5)
       ret += "<p>Too many matches, please refine your search.</p>"
         "<form action=\"quests.c\" method=\"get\">\n"
         "<input type=\"hidden\" name=\"op\" value=\"search\">\n"
         "Search: <input type=\"text\" name=\"search\">\n"
         "</form>\n";
     else
       ret += tmp + "</ul>\n";
     break;
#endif     
   case "basic":
     ret += "<h3>Quest Information</h3>\n";
     ret += "<p><b>Title</b>: " + QUEST_HANDLER->query_quest_title(ql[i]) +
       "<br>\n";
     ret += "<b>Story</b>: " + QUEST_HANDLER->query_quest_story(ql[i])+
       "</p>\n";

     if(PLAYER_HANDLER->test_creator(user)) {
       ret += "<p>Level: " + QUEST_HANDLER->query_quest_level(ql[i]) +
         "<br>\n";
       ret += "Times done: " + QUEST_HANDLER->query_quest_times(ql[i]) +
         "</p>\n";
       ret += "<br>Rating: " + (( 16 - QUEST_HANDLER->query_quest_level(ql[i])) *WEIGHTING +  QUEST_HANDLER->query_quest_times(ql[i])) + "\n";

     }

     fname = calc_filename(ql[i]);
     if(file_size(TEXTS_DIR + "hints/" + fname) > 0)
       ret += "<p><b>First Hint</b>: <a href=\"quests.c?op=hint&param="+i+"\">"
         "Available</a></p>";
     else {
       ret += "<p><b>No first hint available</b></p>\n";

     
       if(file_size(TEXTS_DIR + "final_hints/" + fname) > 0)
         ret += "<p><b>Final Hint</b>: <a href=\"quests.c?op=final_hint&param="+i+
           "\">Available</a></p>";
       else
         ret += "<p><b>No final hint available</b></p>\n";
     }
     break;

   case "list":
   default:
     ret += "<h3>Recommended Quests</h3>\n";
     ret += "<p>The following quests are recommended for you.</p>\n";
     ret += "<table><tr>\n<td>\n";
     for(i=0, j=0, k=0; i<sizeof(ql) && j < MAX_LIST; i++) {

       // Dont show them quests they've done already.
       if(LIBRARY->query_quest_done(user, ql[i]))
         continue;

       // Dont show inactive quests
       if(!QUEST_HANDLER->query_quest_status(ql[i]))
         continue;

       if(QUEST_HANDLER->query_quest_times(ql[i]) < MIN_TIMES)
         continue;

       // columnise the output
       if(!(k++ % (MAX_LIST / NUM_COLS)))
         ret += "</td>\n<td valign=\"top\" nowrap>\n";

       // link the quest info page.
       ret += "<li><a href=\"quests.c?op=basic&param="+i+"\">" +
         lower_case(ql[i]) + "</a>";

       j++;

     }
     ret += "</tr></table>\n";

     ret += "<form action=\"quests.c\" method=\"get\">\n"
       "<p>If you need to find assistance with a specific quest you can use "
       "the search tool to find it.</p>\n"
       "<input type=\"hidden\" name=\"op\" value=\"search\">\n"
       "Search: <input type=\"text\" name=\"search\">\n"
       "</form>\n";

     /*
      * This piece is for creators only and provides a full list of quests
      * and indicates which ones are missing hints.
      */
     if("/secure/master"->query_administrator(user) ||
        "/secure/master"->query_leader(user) ||
        user == "valentijn") {
       ret += "<hr>";
       ret += "<h3>Full quest list (for lords+ only).</h3>";
       ret += "<p>As a creator you can use this to update the hints "
         "for quests to make sure they are complete and "
         "accurate.</p>";
       
       ret += "<table><tr>\n<td>\n";
       for(i=0; i<sizeof(ql); i++) {
         // columnise the output
         if(!(i % ((sizeof(ql) + (NUM_COLS/2)) / NUM_COLS)))
           ret += "</td>\n<td valign=\"top\" nowrap>\n";
         
         // link the quest info page.
         ret += "<br><a href=\"quests.c?op=basic&param="+i+"\">" +
           lower_case(ql[i]) + "</a>";
         ret += "<br>" + qt[i] + "<br>";
         ret += "Level " + QUEST_HANDLER->query_quest_level(ql[i]) + "<br>";
         ret += "Done " + QUEST_HANDLER->query_quest_times(ql[i]) +
           " times<br>";

         ret += "Status: " + 
           (QUEST_HANDLER->query_quest_status(ql[i]) ? "Active" : "Inactive")+ 
           "<br>\n";
         
         fname = calc_filename(ql[i]);
           
         if(file_size(TEXTS_DIR + "hints/" + fname) == -1)
           ret += "<b>No</b> <a href=\"quests.c?op=add&param="+i+"\">First Hint</a>"
             "<br>";
         else
           ret += "<a href=\"quests.c?op=add&param="+i+"\">First Hint</a><br>";
         
         if(file_size(TEXTS_DIR + "second_hints/" + fname) == -1)
           ret += "<b>No</b> <a href=\"quests.c?op=add&param="+i+"\">"
             "Second Hint</a><br>";
         else
           ret += "<a href=\"quests.c?op=add&param="+i+"\">Second Hint</a>"
             "<br>";

         if(file_size(TEXTS_DIR + "final_hints/" + fname) == -1)
           ret += "<b>No</b> <a href=\"quests.c?op=add&param="+i+"\">"
             "Final hint</a><br>";
         else
           ret += "<a href=\"quests.c?op=add&param="+i+"\">Final Hint</a>"
             "<br>";
       }
       ret += "</tr></table>\n";
     }
   }
   ret += create_footer(req);
   return ret;
} /* www_request() */

