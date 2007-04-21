#include <http.h>
#include <board.h>
#undef DAYS
#undef MONTHS
#include <config.h>
#include <mail.h>
#include <login_handler.h>
#include <login.h>
#include <clubs.h>
#include <player_handler.h>
#include <playtesters.h>

#undef SAVE_FILE
#define SAVE_FILE "/save/www/boards"

#define THREADING 0

// One day...
#define TIME_INTERVAL_FOR_POSTING (24 * 60 * 60)
// Maximum number of posts per time period
#define TIME_POSTING_MAXIMUM 5

class player_info {
    mapping newsrc;
    int newsrc_changed;
    string *current_order;
    int current_order_changed;
}

class message_cache {
    string message;
    int last_read;
}

class player_cache {
    string* order;
    int last_read;
    int new_index;
}

private nosave string *_allowed_creator_boards = ({ "commonroom", "learning",
  "development", "dwclone",
  "mudlib", "ram", "am", "sur", "klatch", "forn",
  "special projects",
  "cwc",
  "fluffos",
  "playerhelp",
  "discipline",
  "applications",
  "style", "liaison", "guilds", "underworld"});

nosave string *_allowed_playtester_boards = ({
  "ptforum", "playtesters", "playtester_bugs",
});

nosave string *_allowed_boards = ({ "frog", "newspaper", "warriorsguild",
  "thievesguild", "priestsguild", "assassinsguild",
  "witchesguild", "wizardsguild", "announcements",
  "philosophers", "never wending story",
  "alt.fan.pratchett", "flame", "fluff",
  "ankh morpork council",
  "ankh morpork magistrates",
  "klatch council",
  "klatch magistrates",
  "the green slab",
  "the am daily",
  "equality", "player killers", "role-playing", "tarnach's" });
private nosave mapping _board_pics;
/*
 * This is a mapping containging function pointers which
 * control access to special boards.  eg: philosphers.
 */
private nosave mapping _special_access_boards;
private nosave mapping _read_protected_boards;
private nosave string *no_post_boards = ({ "announcements" });

/* Two globals to do terrible things with data. */
// we have three now.
private nosave string board;
private nosave string html_board;
private nosave mixed messages;

private nosave mapping _subjects;
private nosave mapping _messages;
private nosave mapping _board_order;

mapping posted_stuff;

class posted_information {
    int time_first_posted;
    int count;
}

private int is_allowed_special_access(string board, string name);
private string subjects(class http_request req, int threaded);
private string read(string str, class http_request req, int post_ok,
                    int threaded);
private string new_message(class http_request req, int threaded);
private string followup(string str, class http_request req, int threaded);
private string delete_message(string str, class http_request req, int threaded);
private string postform(string subject, string body, string note,
                        string action, class http_request, int threaded);
private string post(string str, class http_request req, int threaded);
private string create_header(string title);
private string board_summary(class http_request req, int threaded);
private string complete_board_summary(class http_request req, int threaded);
private int is_allowed_access(string board, string name);
private string mail_reply(string note, class http_request req, int threaded);
private string send_mail(string note, class http_request req, int threaded);
private string remove_control_chars(string str);
private void do_post_note(string name);
private void do_delete_note(string name);
private string can_post_note(string name, string board);
private int can_followup_note(string name, string board, mixed *stuff);
private string read_order(class http_request req, int threaded);
private string *get_allowed_boards(string name);
string* get_user_board_order(string user);
private int has_new_messages(string board,
                             string user,
                             mapping newsrc);
private void add_board_picture(string board, string picture);

void create() {
    string* boards;
    string name;

    _subjects = ([ ]);
    _messages = ([ ]);
    _board_order = ([ ]);
    seteuid("WWW");
    /* This is used to flush out newsrc's... */
    _special_access_boards = ([
      "philosophers" : (: PLAYER_HANDLER->test_property($1, "philosopher") :),
      "tarnach's" : (:
             "/d/special/player_shops/tarnach's_office"->query_employee($1) :),
     ]);
     _read_protected_boards = ([
    ]);
    _read_protected_boards = ([
      "ankh morpork magistrates" : 1,
      "klatch magistrates" : 1,
      "the green slab" : 1,
      "the am daily" : 1,
    ]);
    seteuid(getuid());
    /* Do -this every 10 minutes... */
    unguarded( (: restore_object(SAVE_FILE) :) );
    if (!posted_stuff) {
        posted_stuff = ([ ]);
    }
    boards = CLUB_HANDLER->query_boards();
    _allowed_boards += boards;
    foreach (name in boards) {
       _read_protected_boards[name] = 1;
    }

    // Frogs!
    _board_pics = ([ ]);
    add_board_picture("default", "pics/tfrog-news.gif");

    // Custom frogs!
    // Guilds
    add_board_picture("assassinsguild", "pics/boards/assassin_frog0.gif");
    add_board_picture("assassinsguild", "pics/boards/assassin_frog4.gif");
    add_board_picture("assassinsguild", "pics/boards/assassin_frog9.gif");
    add_board_picture("priestsguild", "pics/boards/priest_frog0.gif");
    add_board_picture("priestsguild", "pics/boards/priest_frog4.gif");
    add_board_picture("priestsguild", "pics/boards/priest_frog9.gif");
    add_board_picture("thievesguild", "pics/boards/thief_frog0.gif");
    add_board_picture("thievesguild", "pics/boards/thief_frog4.gif");
    add_board_picture("thievesguild", "pics/boards/thief_frog4b.gif");
    add_board_picture("thievesguild", "pics/boards/thief_frog9.gif");
    add_board_picture("warriorsguild", "pics/boards/warrior_frog0.gif");
    add_board_picture("warriorsguild", "pics/boards/warrior_frog4.gif");
    add_board_picture("warriorsguild", "pics/boards/warrior_frog9.gif");
    add_board_picture("witchesguild", "pics/boards/witch_frog0.gif");
    add_board_picture("witchesguild", "pics/boards/witch_frog4.gif");
    add_board_picture("witchesguild", "pics/boards/witch_frog9.gif");
    add_board_picture("wizardsguild", "pics/boards/wizard_frog0.gif");
    add_board_picture("wizardsguild", "pics/boards/wizard_frog4.gif");
    add_board_picture("wizardsguild", "pics/boards/wizard_frog9.gif");
    // Domains
    add_board_picture("am", "pics/boards/am_frog6.gif");
    add_board_picture("klatch", "pics/boards/klatch_frog5.gif");
    add_board_picture("liaison", "pics/boards/liaison_frog7.gif");
    // Clubs
    add_board_picture("club pcdc", "pics/boards/pcdc_frog5.gif");
    // Random boards
    add_board_picture("ankh morpork magistrates",
                      "pics/boards/ammagistrate_frog4.gif");
    add_board_picture("announcements", "pics/boards/news_frog1.gif");
    add_board_picture("announcements", "pics/boards/news_frog2.gif");
    add_board_picture("announcements", "pics/boards/news_frog4.gif");
    add_board_picture("announcements", "pics/boards/news_frog9.gif");
    add_board_picture("development", "pics/boards/development_frog6.gif");
    add_board_picture("discipline", "pics/boards/discipline_frog0.gif");
    add_board_picture("discipline", "pics/boards/discipline_frog5.gif");
    add_board_picture("discipline", "pics/boards/discipline_frog7.gif");
    add_board_picture("equality", "pics/boards/equality_frog4.gif");
    add_board_picture("flame", "pics/boards/flame_frog0.gif");
    add_board_picture("flame", "pics/boards/flame_frog1.gif");
    add_board_picture("flame", "pics/boards/flame_frog4.gif");
    add_board_picture("flame", "pics/boards/flame_frog9.gif");
    add_board_picture("fluff", "pics/boards/fluff_frog4.gif");
    add_board_picture("fluff", "pics/boards/fluff_frog9.gif");
    add_board_picture("learning", "pics/boards/learning_frog6.gif");
    add_board_picture("mudlib", "pics/boards/mudlib_frog3.gif");
    add_board_picture("newspaper", "pics/boards/newspaper_frog5.gif");
    add_board_picture("never wending story", "pics/boards/nw_frog0.gif");
    add_board_picture("never wending story", "pics/boards/nw_frog2.gif");
    add_board_picture("never wending story", "pics/boards/nw_frog4.gif");
    add_board_picture("philosophers", "pics/boards/philosopher_frog0.gif");
    add_board_picture("philosophers", "pics/boards/philosopher_frog4.gif");
    add_board_picture("player killers", "pics/boards/pk_frog2.gif");
    add_board_picture("player killers", "pics/boards/pk_frog2b.gif");
    add_board_picture("player killers", "pics/boards/pk_frog4.gif");
    add_board_picture("role-playing", "pics/boards/rp_frog4.gif");
    add_board_picture("role-playing", "pics/boards/rp_frog4b.gif");
    add_board_picture("style", "pics/boards/style_frog7.gif");
    add_board_picture("style", "pics/boards/style_frog7b.gif");
    add_board_picture("style", "pics/boards/style_frog7c.gif");
    add_board_picture("style", "pics/boards/style_frog7d.gif");

    add_board_picture("klatch council", "pics/boards/klatch_council_frog0.gif");
    add_board_picture("klatch magistrates", "pics/boards/klatch_council_frog0.gif");
    add_board_picture("tarnach's", "pics/boards/tarnachs_frog.gif");
    // Clean up the cache every 30 minutes.
    call_out("clean_up_cache", 30 * 60);
} /* create() */

/**
 * Do some work to clean up the cache, throw away any messages not read for
 * 25 minutes.
 */
void clean_up_cache() {
   class message_cache cache;
   class player_cache order;
   int num;
   string name;

   call_out("clean_up_cache", 30 * 60);
   foreach (num, cache in _messages) {
      if (cache->last_read + 25 * 60 < time()) {
         map_delete(_messages, num);
      }
   }
   foreach (name, order in _board_order) {
      if (cache->last_read + 10 * 60 < time()) {
         map_delete(_board_order, name);
      }
   }
}

string the_date(int i) {
    return ctime(i)[4..9];
} /* the_date() */

string htmlify_no_address(string str) {
    return replace(str, ({
        "&", "&amp;",
        "<", "&lt;",
        ">", "&gt;",
        " ", "&nbsp;",
        "\n", "<br>\n",
      }) );
} /*  htmlify_no_address() */

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
    int indent = 0;
    int amount; 
    int pos;

    str =  htmlify_no_address(str);
    if (strsrch(str, "http:") > 0) {
        bits = explode("#" + str, "http:");
        bits[0] = bits[0][1..];
        for (i = 1; i < sizeof(bits); i++) {
            if (sscanf(bits[i], "%s&nbsp;%s", start, end) == 2) {
                end = "&nbsp;" + end;
                if (sscanf(start, "%s<br>%s", start, extra) == 2) {
                    end = "<br>" + extra + end;
                }
            } else if (sscanf(bits[i], "%s<br>%s", start, end) == 2) {
                end = "<br>" + end;
            } else {
                start = bits[i];
                end = "";
            }
            if (start[<1] == '.') {
                start = start[0..<2];
                end = "." + end;
            }
            bits[i] = "<a href=\"http:" + start + "\">http:" + start + "</a>" +
            end;
        }
        str = implode(bits, "");
    }
    
    // Replace any non breaking spaces with normal spaces. We want them 
    // out so that browsers can wrap text properly. 
    
    str = replace( str, ({ 
        "&nbsp;", " ",                      
    }) );

    // Check to see if there are any MUD indentation codes in the post.

    if ( strsrch(str, "$I$") > -1 ) {
        bits = explode( "#" + str, "$I$" ); 
        bits[0] = bits[0][1..];

        for (i = 1; i < sizeof(bits); i++) {
            if (sscanf(bits[i], "%d=%s", amount, end) == 2) {
                if ( indent && amount == 0 ) {
                    end = "\n<!-- END OF INDENT -->\n"
                        "</blockquote>" + end; 
                } 
                
                if ( amount ) { 
                    // Ok. With $I$x=, we indent by x characters AFTER the
                    // first line has been written out.  This means
                    // we are going to have to search for the next 
                    // \n and insert the <blockquote> in after there. 

                    pos = strsrch( end, "\n" );
                
                    // If there are no carriage returns in here, then 
                    // well just have to indent at the end of this 
                    // block of text. 

                    if ( pos == -1 ) {
                        end = "\n<!-- BEGINNING OF INDENT -->\n"
                            "<blockquote>" + end;                    
                    } else {
                        end = end[0..pos - 1] + "\n<!-- BEGINNING OF INDENT -->\n"
                            "<blockquote>" + end[pos..];                    
                    }
                    indent++; 
                }
            }

            bits[ i ] = end;
        }

        if ( indent ) { 
            while( indent ) {
                bits[ <1 ] = bits[ <1 ] + "\n<!-- END OF INDENT, "
                    "CLEANUP -->\n"
                    "</blockquote>";
                indent--;
            }
        }


        str = implode( bits, "" );
    }

    return str;
} /* htmlify() */

private void add_board_picture(string board, string picture) {
   if (!_board_pics[board]) {
      _board_pics[board] = ({ });
   }
   _board_pics[board] += ({ picture });
} /* add_board_picture() */

private string query_board_picture(string board) {
   if (_board_pics[board]) {
      return _board_pics[board][random(sizeof(_board_pics[board]))];
   }
   return _board_pics["default"][random(sizeof(_board_pics["default"]))];
} /* query_board_picture() */

string www_request(string str, mapping args, class http_request req) {
  int threaded;
  
  if (!undefinedp(args["board"])) {
    board = args["board"];
    html_board = replace(board, " ", "+");
    if (!is_allowed_access(board, req->user)) {
      return "<b>Access to "+board+" is denied.</b>\n"+
  ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
    }

    threaded = to_int(args["threaded"]);
    switch (args["type"]) {
    case "read" :
      return read(args["note"], req, 0, threaded);
    case "followup" :
      return followup(args["note"], req, threaded);
    case "post" :
      return post(args["note"], req, threaded);
    case "new" :
      return new_message(req, threaded);
    case "eat" :
      return delete_message(args["note"], req, threaded);
    case "summary" :
      return board_summary(req, threaded);
    case "summary-all" :
      return complete_board_summary(req, threaded);
    case "mailreply" :
      return mail_reply(args["note"], req, threaded);
    case "sendmail" :
      return send_mail(args["note"], req, threaded);
    case "readorder" :
      return read_order(req, threaded);
    default :
      return subjects(req, threaded);
    }
  }
  return "<b>Incorrect argument!</b>"+
    ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
} /* www_request() */

private string create_header(string title) {
    return "<html><head>"
    "<title>" + title + "</title>"+
    "</head><body bgcolor=\"#ffffff\" TEXT=\"#000030\" LINK=\"#4a529c\" "
    "VLINK=\"#b57339\"><font face=\"arial,helvetica\">";
} /* create_header() */

private string *get_next_new_note(string user, mapping newsrc,
                                  class http_request req, int threaded) {
    string *allowed;
    //string *boards;
    int pos;
    mixed *new_stuff;
    string bing;
    int changed;

    allowed = get_user_board_order(user);
    // So killed boards don't get linked to.
    //allowed -= BOARD_HAND->list_killfile(req->user);
    foreach (bing in allowed) {
        if (is_allowed_special_access(bing, user)) {
           pos = has_new_messages(bing, user, newsrc);
           if (pos > 0) {
              new_stuff = BOARD_HAND->get_subjects(bing, req->user);
              pos = new_stuff[<pos][B_NUM];
              if (changed) {
                 BOARD_HAND->set_newsrc(req->user, newsrc);
              }
              return ({ "<a href=\"boards.c?board=" +
                replace_string(bing, " ", "+") +
                "&amp;type=read&amp;threaded=" + threaded +
                          "&amp;note=" + pos + "\">Next new</a>" });
           }
        }
    }
    return ({ });
} /* get_next_new_note() */

private string show_line(mixed stuff, class http_request req, mapping newsrc,
                         int threaded) {
  string ret;

  if (_subjects[stuff[B_NUM]]) {
     ret = _subjects[stuff[B_NUM]];
  } else {
     ret = sprintf("<li><a href=\"boards.c?board=%s&amp;type=read&amp;"
                   "threaded=" + threaded + "&amp;note=%d\">%s</a> ("
                "<a href=\"/" + (req->user?"secure/":"") +
                "finger.c?player=%s\">%s</a>, %s)</a>",
                html_board, stuff[B_NUM],
                htmlify_no_address(stuff[B_SUBJECT]), stuff[B_NAME],
                capitalize(stuff[B_NAME]),
                the_date(stuff[B_TIME]));
     _subjects[stuff[B_NUM]] = ret;
  }
  if (req->user && newsrc[board] < stuff[B_TIME] &&
      req->user != lower_case(stuff[B_NAME])) {
    ret += " <b>[new]</b> ";
  }
  return ret + "\n";
}

private string find_followups(int thread, int start, class http_request req,
                              mapping newsrc, int threaded) {
  int i;
  string ret;
  mixed followups;

  followups = filter(messages[start..], (: $1[B_REPLY_NUM] == $2 :), thread);

  if(!sizeof(followups))
    return "";

  ret = "<ul>\n";
  for(i=0; i<sizeof(followups); i++) {
    ret += show_line(followups[i], req, newsrc, threaded);
    ret += find_followups(followups[i][B_NUM], i, req, newsrc, threaded);
  }
  messages = filter(messages, (: $1[B_REPLY_NUM] != $2 :), thread);

  ret += "</ul>\n";

  return ret;
}

private string subjects(class http_request req, int threaded) {
    int i;
    mixed *stuff;
    string ret;
    mapping newsrc;
    string *tmp;

    ret = create_header("Board: "+board);
    ret += "<h2><img src=\"http:/external/" +
           query_board_picture(board) + "\" alt=\" \">"
    "Notes on the " +
    capitalize(board) + " board</h2>\n";
    stuff = (mixed *)BOARD_HAND->get_subjects(board, req->user);
    if (req->user) {
        newsrc = BOARD_HAND->query_newsrc(req->user);
        if (!newsrc) {
            newsrc = ([ ]);
        }
    }

    tmp = ({ });
    if (req->user) {
        if (member_array(board, no_post_boards) == -1) {
                tmp += ({ "<a href=\"boards.c?type=new&threaded=" + threaded +
                            "&amp;board=" + html_board + "\">New&nbsp;note</a>" });
        }
    }
    tmp += ({ "<a href=\"boards.c?type=summary&amp;threaded=" + threaded +
                "&amp;board=" + html_board + "\">Summary</a>",
                "<a href=\"boards.c?board=frog&amp;type=summary-all&amp;"
                "threaded=" + threaded + "\">All&nbsp;Boards</a>" });
    if(threaded)
      tmp += ({ "<a href=\"boards.c?board=" + board + "&amp;threaded=0\">"
                  "Subjects&nbsp;by&nbsp;Date</a>"});
    else
      tmp += ({ "<a href=\"boards.c?board=" + board + "&amp;threaded=1\">"
                  "Subjects&nbsp;by&nbsp;Thread</a>"});
      
    ret += "<font size=-1><strong>[&nbsp;" + implode(tmp, " | ") +
      "&nbsp;]</strong></font><br clear=both>\n";

    if (!sizeof(stuff)) {
        return ret + "<b>The board is completely empty.</b><p>" +
          "/www/footer"->www_function("footer", ([ ]), req)+"</body></html>";
    }

    if(!threaded) {
      ret += "<ol>";
      for (i=0;i<sizeof(stuff);i++) {
        ret += show_line(stuff[i], req, newsrc, 0) + "\n";
      }
      ret += "</ol><br>";
    } else {
      messages = stuff;
      ret += "<ul>\n";
      for(i=0; i<sizeof(messages); i++) {
        ret += show_line(messages[i], req, newsrc, 1);
        ret += find_followups(messages[i][B_NUM], i, req, newsrc, 1);
      }
      ret += "</ul>\n";
    }
    ret += "<font size=-1><strong>[&nbsp;" + implode(tmp, " | ") +
      "&nbsp;]</strong></font><br clear=both>\n";

    return ret + ("/www/footer"->www_function("footer", ([ ]), req)) +
      "</body></html>";
} /* subjects() */

private int count_lines(string str) {
    return sizeof(explode(str, "\n"));
} /* count_lines() */

private string read(string str, class http_request req, int post_ok,
                    int threaded) {
    int num, i, b_num;
    int next_thread = 1;
    mixed stuff;
    string ret;
    string *tmp;
    mapping newsrc;
    string mess;
    string middle;
    string line;
    class message_cache cache;

    ret = create_header("Note "+str+" on board "+board);
    stuff = (mixed *)BOARD_HAND->get_subjects(board, req->user);
    if (sscanf(str, "%d", b_num) != 1) {
        return ret + "<b>Incorrect argument!</b>" +
        ("/www/footer"->www_function("footer", ([ ]), req)) +
        "</body></html>";
    }

    num = -1;
    for (i=0;i<sizeof(stuff);i++) {
        if (stuff[i][B_NUM] == b_num) {
            num = i+1;
        }
    }

    if (num < 1 || num > sizeof(stuff)) {
        return ret + "<b>No note of that number</b>" +
        ("/www/footer"->www_function("footer", ([ ]), req)) +
        "</body></html>";
    }

    num --;
    if (req->user) {
        newsrc = BOARD_HAND->query_newsrc(req->user);
        if (!newsrc) {
            newsrc = ([ ]);
        }
        if (newsrc[board] < stuff[num][B_TIME]) {
            newsrc[board] = stuff[num][B_TIME];
            BOARD_HAND->set_newsrc(req->user, newsrc);
        }
    }

    tmp = ({ });
    if (num<sizeof(stuff)-1) {
      tmp += ({ "<a href=\"boards.c?board="+html_board+
                  "&amp;type=read&amp;threaded=" + 0 + "&amp;note="+
                  stuff[num+1][B_NUM]+ "\">Next</a>" });

      for(i=num+1; i<sizeof(stuff); i++) {
        if(stuff[i][B_REPLY_NUM] == stuff[num][B_NUM] ||
           (stuff[num][B_REPLY_NUM] &&
            stuff[i][B_REPLY_NUM] == stuff[num][B_REPLY_NUM])) {
          tmp += ({ "<a href=\"boards.c?board="+html_board+
                      "&amp;type=read&amp;threaded=1&amp;note="+stuff[i][B_NUM]+
                      "\">Next&nbsp;in&nbsp;Thread</a>" });
          next_thread = 0;
          break;
        }
      }
      if(next_thread) {
        tmp += ({ "<a href=\"boards.c?board="+html_board+
                    "&amp;type=read&amp;threaded=1&amp;note="+
                    stuff[num+1][B_NUM]+ "\">Next Thread</a>" });
        
      }
    } else if (req->user) {
      tmp += get_next_new_note(req->user, newsrc, req, threaded);
    }

    if (num>=1) {
      tmp += ({ "<a href=\"boards.c?board="+html_board+
                  "&amp;type=read&amp;threaded=" + threaded + "&amp;note="+
                  stuff[num-1][B_NUM]+ "\">Previous</a>" });
    }
    tmp += ({ "<a href=\"boards.c?board="+html_board+"&amp;threaded=0"
                "\">Subjects&nbsp;by&nbsp;Date</a>" });
    tmp += ({ "<a href=\"boards.c?board="+html_board+"&amp;threaded=1"
                "\">Subjects&nbsp;by&nbsp;Thread</a>" });

    if (req->user) {
        if (member_array(board, no_post_boards) == -1) {
            tmp += ({ "<a href=\"boards.c?board=" + html_board +
              "&amp;type=followup&amp;threaded=" + threaded + "&amp;note=" +
              stuff[num][B_NUM] + "\">Followup</a>",
              "<a href=\"boards.c?board=" + html_board +
              "&amp;type=new&amp;threaded=" + threaded + "\">New&nbsp;note</a>" });
        }
        tmp += ({ "<a href=\"boards.c?board=" + html_board +
              "&amp;type=eat&amp;threaded=" + threaded + "&amp;note=" +
                    stuff[num][B_NUM] +"\">Eat&nbsp;note</a>",
          "<a href=\"boards.c?board=" + html_board +
          "&amp;type=mailreply&amp;note=" +
          stuff[num][B_NUM] + "\">Mail&nbsp;reply</a>",
         "<a href=\"boards.c?board=frog&amp;type=summary&amp;threaded=" +
                    threaded + "\">Summary</a>",
         "<a href=\"boards.c?board=frog&amp;type=summary-all&amp;threaded=" +
                    threaded + "\">All&nbsp;Boards</a>"
                    });
    }

    mess = BOARD_HAND->get_message(board, num, req->user);
    if (!stringp(mess)) {
        mess = "Horribly broken message with eggs for ears.\n";
    }
    ret += "<h2><img src=\"http://discworld.imaginary.com/external/" +
             query_board_picture(board) + "\" alt=\" \">"+
       htmlify_no_address(stuff[num][B_SUBJECT])+"</h2>\n";
    line = "<font size=-1><strong>[&nbsp;"+implode(tmp, " | ")+"&nbsp;]</strong></font>\n";
    ret += line;
    if (_messages[b_num]) {
       ret += "<h3>Note #" + (num + 1) + (_messages[b_num]->message);
       _messages[b_num]->last_read = time();
    } else {
       middle = " by <a href=\"/" + (req->user?"secure/":"") +
       "finger.c?player="+
       stuff[num][B_NAME]+"\">"+capitalize(stuff[num][B_NAME])+"</a> at "+
       ctime(stuff[num][B_TIME])+" on board " + board + "</h3>\n<p>"+
       ""+htmlify(strip_colours(mess))+
       "</p>\n";

       ret += "<h3>Note #" + (num + 1) + middle;
       cache = new(class message_cache);
       cache->message = middle;
       cache->last_read = time();
       _messages[b_num] = cache;
    }
    return ret + line +
    ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
} /* read() */

private string followup(string str, class http_request req, int threaded) {
    int num, i, b_num;
    mixed stuff;
    string ret;
    string subject;

    ret = create_header("Note "+str+" on board "+board);
    stuff = (mixed *)BOARD_HAND->get_subjects(board, req->user);
    if (sscanf(str, "%d", b_num) != 1 || !req->user) {
        return ret+"<b>Incorrect argument!</b>"+
        ("/www/footer"->www_function("footer", ([ ]), req)) +
        "</body></html>";
    }

    num = -1;
    for (i=0;i<sizeof(stuff);i++) {
        if (stuff[i][B_NUM] == b_num) {
            num = i+1;
        }
    }

    if (num < 1 || num > sizeof(stuff)) {
        return ret+"<b>No note of that number</b>"+
        ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
    }

    num --;
    if (!can_followup_note(req->user, board, stuff[num])) {
        return ret + "<b>Unable to followup to this message, too many messages "
        "posted today (Maximum " + TIME_POSTING_MAXIMUM + ")</b>" +
        ("/www/footer"->www_function("footer", ([ ]), req)) +
        "</body></html>";
    }

    ret += "<h2><img src=\"http://discworld.imaginary.com/external/" +
            query_board_picture(board) + "\" alt=\" \">"
    "Followup to Note #" +
    (num + 1) + " on board " + board + "<br>'" +
    htmlify_no_address(stuff[num][B_SUBJECT]) + "'</h2>\n";

    if (sscanf(stuff[num][B_SUBJECT], "Re:#%d %s", i, subject) != 2) {
        subject = "Re:#1 "+ stuff[num][B_SUBJECT];
    } else {
        subject = "Re:#"+ ( i + 1 ) +" "+ subject;
    }

    ret += postform(subject,
                    "On " + ctime(stuff[num][B_TIME]) + ", " +
                    stuff[num][B_NAME] +
                    " wrote:\n> " +
                    implode(explode(strip_colours(sprintf("%-=76s",
              BOARD_HAND->get_message(board, num, req->user))), "\n"), "\n> "),
                    str, "post", req, threaded);
    return ret + ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
} /* followup() */

private string mail_reply(string str, class http_request req, int threaded) {
    int num, i, b_num;
    mixed stuff;
    string ret;
    string subject;

    ret = create_header("Note "+str+" on board "+board);
    stuff = (mixed *)BOARD_HAND->get_subjects(board, req->user);
    if (sscanf(str, "%d", b_num) != 1 || !req->user) {
        return ret+"<b>Incorrect argument!</b>"+
        ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
    }

    num = -1;
    for (i=0;i<sizeof(stuff);i++) {
        if (stuff[i][B_NUM] == b_num) {
            num = i+1;
        }
    }

    if (num < 1 || num > sizeof(stuff)) {
        return ret+"<b>No note of that number</b>"+
        ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
    }

    num --;
    ret += "<h2><img src=\"http://discworld.imaginary.com/external/" +
      query_board_picture(board) + "\" alt=\" \">"
      "Reply mail to Note #" + (num + 1) + " on board " + board + "<br>'" +
      htmlify_no_address(stuff[num][B_SUBJECT]) + "'</h2>\n";

    if (sscanf(stuff[num][B_SUBJECT], "Re:#%d %s", i, subject) != 2) {
        subject = "Re:#1 "+ stuff[num][B_SUBJECT];
    } else {
        subject = "Re:#"+ ( i + 1 ) +" "+ subject;
    }

    ret += postform(subject,
                    "> " + implode(explode(
          strip_colours(sprintf("%-=76s", BOARD_HAND->get_message(board, num, req->user))), "\n"), "\n> "),
                    str, "sendmail", req, threaded);

    return ret +
      ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
} /* mail_reply() */

private string send_mail(string str, class http_request req, int threaded) {
    string ret;
    string body;
    string subject;
    string *rep_arr;
    string mail_to;
    int num;
    int i;
    int b_num;
    mixed *stuff;

    /* Whooo... */
    ret = create_header("Note "+str+" on board "+board);

    if (str && req->user) {
        if (sscanf(str, "%d", b_num) != 1) {
            return ret+"<b>Incorrect argument!</b>"+
            ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
        }

        stuff = (mixed *)BOARD_HAND->get_subjects(board, req->user);

        num = -1;
        for (i=0;i<sizeof(stuff);i++) {
            if (stuff[i][B_NUM] == b_num) {
                num = i+1;
                mail_to = stuff[i][B_NAME];
            }
        }

        if (num < 1 || num > sizeof(stuff)) {
            return ret+"<b>No note of that number</b>"+
            ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
        }

        num --;
    } else {
        return ret+"<b>Incorrect argument!</b>"+
        ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
    }

    rep_arr = ({ " ", "", "\n", "", "\r", ""});

    if (undefinedp(req->body->data["body"]) ||
      undefinedp(req->body->data["subject"]) ||
      replace(req->body->data["body"], rep_arr) == "" ||
      replace(req->body->data["subject"], rep_arr) == "") {
        ret += "\n<!-- " + sprintf("%O", req->body) + " -->";
        ret += "\n<!-- " + req->body->data["body"] + " -->";
        ret += "\n<!-- " + req->body->data["subject"] + " -->";
        return ret + "<b>Posted data is incorrect.</b>\n" +
        ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
    }

    body = remove_control_chars(strip_colours(req->body->data["body"]));

    /* Format it better...  */
    body = sprintf("%-=78s", body);

    subject = remove_control_chars(req->body->data["subject"]);

    if (MAILER->do_mail_message(mail_to, req->user,
        subject,
        "",
        body)) {
        return read(str, req, 1, 0);
    } else {
        return ret + "<b>Unable to mail the message. :(</b>" +
        ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
    }
} /* send_mail() */

private string delete_message(string str, class http_request req,
                              int threaded) {
    int num, i, b_num;
    mixed stuff;
    string ret;

    ret = create_header("Note "+str+" on board "+board);
    stuff = (mixed *)BOARD_HAND->get_subjects(board, req->user);
    if (sscanf(str, "%d", b_num) != 1 || !req->user) {
        return ret+"<b>Incorrect argument!</b>"+
        ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
    }

    num = -1;
    for (i=0;i<sizeof(stuff);i++) {
        if (stuff[i][B_NUM] == b_num) {
            num = i+1;
        }
    }

    if (num < 1 || num > sizeof(stuff)) {
        return ret+"<b>No note of that number</b>"+
        ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
    }

    num --;
    if (BOARD_HAND->delete_message(board, num, req->user)) {
        do_delete_note(req->user);
        event( users(), "inform", req->user +
          " eats a note off "+ board + " {web}", "message", this_object() );
        if (num + 1 > sizeof(stuff)) {
            return read(stuff[num + 1][B_NUM] + "", req, 1, threaded);
        } else if (num > 0) {
            return read(stuff[num - 1][B_NUM] + "", req, 1, threaded);
        } else {
            return subjects(req, threaded);
        }
    }
    return ret + "<b>Unable to delete note " + num + " on board " + board +
    "</b>\n" +
    ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
} /* delete_message() */

private string new_message(class http_request req, int threaded) {
    string ret, tmp;

    ret = create_header("New message on board "+board);

    if (!req->user)
      return ret + "<b>Unable to post message." +
        ("/www/footer"->www_function("footer", ([ ]), req)) +
        "</body></html>";

    tmp = can_post_note(req->user, board);
    if(tmp != "")
      return ret + "<b>Unable to post a message, " + tmp + ".</b>" +
        ("/www/footer"->www_function("footer", ([ ]), req)) +
        "</body></html>";

    ret += "<h2><img src=\"http://discworld.imaginary.com/external/" +
           query_board_picture(board) + "\" alt=\" \">New hairy message on "
            "board " + board + "</h2>\n"    ;

    ret += postform("", "", 0, "post", req, threaded);

    return ret + ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
} /* new_message() */

/*
 * This returns the number of new messages.
 */
private int has_new_messages(string board,
  string user,
  mapping newsrc) {
    mixed *stuff;
    int i;
    int tim;
    int window_bottom;
    int window_top;

    stuff = BOARD_HAND->get_subjects(board, user);
    i = sizeof(stuff) - 1;
    tim = newsrc[board];
    if (i >= 0 && stuff[i][B_TIME] > tim) {
        window_bottom = 0;
        window_top = sizeof(stuff) - 1;
        while (window_bottom < window_top) {
           i = (window_bottom + window_top) / 2;
           if (stuff[i][B_TIME] > tim) {
              window_top = i;
           } else {
              window_bottom = i + 1;
           }
        }

        // Skip over our messages.  Might need to skip one more
        while (window_bottom < sizeof(stuff) &&
               lower_case(stuff[window_bottom][B_NAME]) == user) {
            window_bottom++;
        }
  
        return sizeof(stuff) - window_bottom;
    }
    return 0;
} /* has_new_messages() */

private int is_allowed_special_access(string board, string name) {
  if (_special_access_boards[board]) {
    return evaluate(_special_access_boards[board], name) ||
      PLAYER_HANDLER->test_creator(name);
  }
  if (_read_protected_boards[board]) {
    return BOARD_HAND->test_can_read(board, this_object(), name, 0);
  }
  return 1;
}  /* is_allowed_access() */

private int is_allowed_access(string board, string name) {
  if(member_array(board,_allowed_boards) != -1)
    return 1;

  if(!name)
    return 0;

  if (_special_access_boards[board]) {
    return is_allowed_special_access(board, name);
  }

  if(member_array(board, _allowed_creator_boards) != -1 &&
     PLAYER_HANDLER->test_creator(name))
    return 1;

  if (member_array(board, _allowed_playtester_boards) != -1 &&
     PLAYTESTER_HAND->query_playtester(name) || 
     PLAYER_HANDLER->test_creator(name)) {
      return 1;
  }
    
  if(board == "lordboard" && "/secure/master"->query_lord(name))
    return 1;

  return 0;
}  /* is_allowed_access() */

private string board_summary(class http_request req, int threaded) {
    string *tmp;
    mapping newsrc;
    string  *boards;
    string ret;
    //string *current_order;
    string sum;

    ret = create_header("Board Summary");

    if (!req->user) {
        return ret + "<b>Not logged on</b>\n " +
        ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
    }

    newsrc = BOARD_HAND->query_newsrc(req->user);
    if (!newsrc) {
        newsrc = ([ ]);
    }
    boards = get_user_board_order(req->user);

    tmp = ({ });
    tmp += get_next_new_note(req->user, newsrc, req, 0);
    tmp += ({ "<a href=\"boards.c?type=summary-all&amp;threaded=" + threaded +
                "&amp;board=frog\">All&nbsp;Boards</a>",
              "<a href=\"boards.c?type=readorder&amp;threaded=" + threaded +
                "&amp;board=frog\">Read&nbsp;Order</a>" });

    ret += "<h2><img src=\"http://discworld.imaginary.com/external/pics/tfrog-news.gif\" alt=\" \">Boards with new "
      "messages</h2>";
    ret += "<font size=-1><strong>[&nbsp;" + implode(tmp, " | ") +
      "&nbsp;]</strong></font>\n";
     sum = implode(map(boards, function(string board, string user, mapping newsrc, int threaded) {
                  int num;

                  num = has_new_messages(board, user, newsrc);
                  if (num && is_allowed_special_access(board, user)) { 
                     return "<dt><img src=\"http://discworld.imaginary.com/external/pics/red-ball.gif\" "
              "width=12 height=12 alt=\" o \">"
              "<a href=\"boards.c?type=subject&amp;threaded=" + threaded +
                       "&amp;board=" + replace($1, " ", "+") + "\">" +
                       capitalize($1) + "</a> [&nbsp;" + num +
                       " new message" + (num ==1 ? "" : "s") + "&nbsp;]\n";
                  } else {
                    return "";
                  }
               } ,
            req->user, newsrc, threaded), "");
    if (sum != "") {
       ret += "<dl>\n" + sum + "</dl>";
    } else {
       ret += "<br><br><strong>No boards with new messages!</strong><p>\n"
              "<h5>Enjoy it while it lasts...</h5>";
    }
    ret += "<font size=-1><strong>[&nbsp;" + implode(tmp, " | ") +
      "&nbsp;]</strong></font>\n";
    return ret + "/www/footer"->www_function("footer", ([ ]), req) +
      "</body></html>";
} /* board_summary() */

private string complete_board_summary(class http_request req, int threaded) {
    string *tmp;
    mapping newsrc;
    string  *boards;
    string ret;
    //string *current_order;
    string sum;

    ret = create_header("Board Summary");

    if (!req->user) {
        return ret + "<b>Not logged on</b>\n " +
        ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
    }

    newsrc = BOARD_HAND->query_newsrc(req->user);
    if (!newsrc) {
        newsrc = ([ ]);
    }

    boards = get_user_board_order(req->user) +
      sort_array(BOARD_HAND->list_killfile(req->user), 1);
    //current_order = BOARD_HAND->query_board_order(req->user);
    //boards = current_order + sort_array(boards - current_order, 1);
    
    tmp = ({ });
    tmp += get_next_new_note(req->user, newsrc, req, 0);
    tmp += ({ "<a href=\"boards.c?type=summary&amp;threaded=" + threaded +
                "&amp;board=frog\">"
      "Summary</a>",
      "<a href=\"boards.c?type=readorder&amp;threaded=" + threaded +
                "&amp;board=frog\">"
      "Read Order</a>" });

    /*
     * Filter out the boards which need special access but keep ones that
     * don't have new messages
     */
    boards = filter(boards, (: is_allowed_special_access($1, $2) :),
                    req->user);
    ret += "<h2><img src=\"http://discworld.imaginary.com/external/pics/tfrog-news.gif\" alt=\" \">Complete Summary</h2>";
    ret += "<font size=-1><strong>[&nbsp;" + implode(tmp, " | ") +
      "&nbsp;]</strong></font>\n";
    sum = implode(map(boards,
                      function(string board, string user, mapping newsrc, int threaded)
        {
          int num;
          if (is_allowed_special_access(board, user)) {
            num = has_new_messages(board, user, newsrc);
            return "<dt><img src=\"http://discworld.imaginary.com/external/pics/red-ball.gif\" "
              "width=12 height=12 alt=\" o \">"
              "<a href=\"boards.c?type=subject&amp;threaded=" + threaded +
              "&amp;"
              "board=" + replace($1, " ", "+") + "\">" +
              capitalize($1) + "</a> [&nbsp;" + num +
              " new message" + (num ==1 ? "" : "s") + "&nbsp;]\n";
                  } else {
                    return "";
                  }
               } ,
            req->user, newsrc, threaded), "");
    if (sum != "") {
       ret += "<dl>\n" + sum + "</dl>";
    } else {
       ret += "<br><br><strong>No boards you can access</strong><p>\n";
    }

    ret += "<font size=-1><strong>[&nbsp;" + implode(tmp, " | ") +
      "&nbsp;]</strong></font>\n";
    return ret + "/www/footer"->www_function("footer", ([ ]), req) +
      "</body></html>";
} /* complete_board_summary() */

/*
 * The action can be 'post' or 'sendmail'
 */
private string postform(string subject, string body, string note,
                        string action, class http_request req, int threaded) {
    string ret;

    if (note) {
        ret = "<form method=\"post\" action=\"boards.c?type=" + action +
        "&amp;threaded=" + threaded + "&amp;board=" + html_board +
        "&amp;note=" + note + "\">\n";
    } else {
        ret = "<form method=\"post\" action=\"boards.c?type=" + action +
        "&amp;threaded=" + threaded + "&amp;board=" + html_board +
        "\">\n";
    }
    ret += "Subject: <input name=\"subject\" type=\"text\" size=\"60\" "
        "value=\"" + replace(htmlify_no_address(subject),"\"","&#34;") + 
        "\">\n<br>";
    ret += "<font face=\"monospace\">\n";
    ret += "<textarea name=\"body\" rows=\"20\" cols=\"70\">\n";
    ret += replace_string(htmlify_no_address(body), "<br>", ""); //, "&", "&");
    ret += strip_colours(replace_string(htmlify_no_address(PLAYER_HANDLER->query_signature(req->user)), "<br>", ""));
    ret += "</textarea><br>\n</font>\n";
    if (note) {
        if (action == "post") {
            ret += "<input type=\"submit\" value=\"Post followup\">";
        } else {
            ret += "<input type=\"submit\" value=\"Mail&nbsp;reply\">";
        }
    } else {
        ret += "<input type=\"submit\" value=\"Post message\">";
    }
    ret += "<input type=\"reset\" value=\"Reset to start values\">\n</form>\n";
    return ret;
} /* postform() */

private string remove_control_chars(string str) {
    int i;

    for (i = 0; i < strlen(str); i++) {
        /* This will also change the hard space back to a space. */
        if ((str[i] < ' '  && str[i] != '\n' && str[i] != '\r') ||
          str[i] == 160 ) {
            str[i] = ' ';
        }
        if (i + 1 < strlen(str) &&
          (str[i] == '%' && str[i+1] == '^')) {
            str[i] = ' ';
            str[i + 1] = ' ';
        }
    }
    return str;
} /* remove_control_chars() */

private string post(string str, class http_request req, int threaded) {
    string ret;
    string tmp;
    string body;
    string subject;
    string *rep_arr;
    int num;
    int i;
    int b_num;
    int note_num;
    mixed *stuff;
    int no_quotes;
    int no_lines;

    /* Whooo... */
    ret = create_header("Note "+str+" on board "+board);

    if (!req->user || member_array(board, no_post_boards) != -1) {
        return ret + "<b>Unable to post message, access denied.</b>"+
        ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
    }

    if (str) {
        if (sscanf(str, "%d", b_num) != 1) {
            return ret+"<b>Incorrect argument!</b>"+
            ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
        }

        stuff = (mixed *)BOARD_HAND->get_subjects(board, req->user);

        num = -1;
        for (i=0;i<sizeof(stuff);i++) {
            if (stuff[i][B_NUM] == b_num) {
                num = i+1;
            }
        }

        if (num < 1 || num > sizeof(stuff)) {
            return ret+"<b>No note of that number</b>"+
            ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
        }
        num --;

        if (!can_followup_note(req->user, board, stuff[num])) {
            return ret + "<b>Unable to post this message, too many messages "
            "posted today (Maximum " + TIME_POSTING_MAXIMUM + ")</b>" +
            ("/www/footer"->www_function("footer", ([ ]), req)) +
            "</body></html>";
        }

        tmp = "> " + implode(explode(strip_colours(BOARD_HAND->get_message(board, num, req->user)),
            "\n"), "\n> ");
        note_num = stuff[num][B_NUM];
    } else {
      tmp = can_post_note(req->user, board);
      if(tmp != "")
        return ret + "<b>Unable to post a message, " + tmp + ".</b>" +
          ("/www/footer"->www_function("footer", ([ ]), req)) +
          "</body></html>";
        tmp = "test";
    }

    rep_arr = ({ " ", "", "\n", "", "\r", ""});
    tmp = replace(tmp, rep_arr);

    if (undefinedp(req->body->data["body"]) ||
      undefinedp(req->body->data["subject"]) ||
      tmp == replace(req->body->data["body"], rep_arr) ||
      replace(req->body->data["body"], rep_arr) == "" ||
      replace(req->body->data["subject"], rep_arr) == "") {
        ret += "\n<! -- " + sprintf("%O", req->body) + ">";
        ret += "\n<! -- " + req->body->data["body"] + ">";
        ret += "\n<! -- " + req->body->data["subject"] + ">";
        return ret + "<b>Posted data is incorrect.</b>\n" +
        ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
    }

    body = remove_control_chars(strip_colours(req->body->data["body"]));
    body = replace_string(body, "\r\n", "\n");
    /* Format it better...  */
    //body = sprintf("%-=78s", body);
    no_quotes = sizeof(explode(body, "\n>"));
    ret += "\n<! no quotes = " + no_quotes + ">\n";
    if (no_quotes > 5) {
        /* Strip out blank lines... */
        no_lines = sizeof(explode(replace_string(body, " ", ""), "\n") - ({ }));
        ret += "\n<! no lines = " + no_lines + ">\n";
        if (no_lines - no_quotes < no_quotes - 5) {
            return ret + "<b>You need to have as many normal "
            "lines as quoted lines.<br>NB: Notes which get around "
            "this with lots of blank space will be deleted.</b>\n" +
            ("/www/footer"->www_function("footer", ([ ]), req)) +
            "</body></html>";
        }
    }

    subject = remove_control_chars(req->body->data["subject"]);

    if (BOARD_HAND->add_message(board, capitalize(req->user),
        subject[0..80],
        body,
        note_num)) {
        if(board == "flame" || !PLAYER_HANDLER->test_creator(req->user))
            do_post_note(req->user);
        // Re-get this to move onto the nice note...
        stuff = (mixed *)BOARD_HAND->get_subjects(board, req->user);
        for (i=0;i<sizeof(stuff);i++) {
            if (stuff[i][B_NUM] == b_num) {
                // Try to move to the next note.
                if (i + 1 < sizeof(stuff)) {
                    str = stuff[i + 1][B_NUM] + "";
                }
            }
        }

        if (str) {
            return read(str, req, 1, threaded);
        } else {
          return subjects(req, threaded);
        }
    } else {
        return ret + "<b>Unable to post the note. :(</b>" +
        ("/www/footer"->www_function("footer", ([ ]), req))+"</body></html>";
    }
} /* post() */

private string read_order(class http_request req, int threaded) {
    string *board_names;
    string ret;
    string *current_order;
    string board;
    string bing;
    string *tmp;
    mapping new_order;
    int *order;
    int i;
    int pos;

    ret = create_header("Order of reading boards");

    if (!req->user) {
        ret += "<b>Not logged on</b>" +
        ("/www/footer"->www_function("footer", ([ ]), req)) +
        "</body></html>";
        return ret;
    }

    board_names = get_user_board_order(req->user);

    if (sizeof(req->body->data)) {
        new_order = ([ ]);
        foreach (bing in board_names) {
            board = replace_string(bing, " ", "+");
            if (req->body->data[board] &&
              req->body->data[board] != "default") {
                if (sscanf(req->body->data[board], "%d", pos) == 1) {
                    if (new_order[pos]) {
                        new_order[pos] += ({ bing });
                    } else {
                        new_order[pos] = ({ bing });
                    }
                }
            }
        }

        if (sizeof(new_order)) {
            order = sort_array(keys(new_order), 1);
            current_order = ({ });
            foreach (pos in order) {
                current_order += new_order[pos];
            }
        }
        BOARD_HAND->set_board_order(req->user, current_order);
    }


    /* If the order is not set, use the default. */
    if (!current_order) {
        current_order = BOARD_HAND->query_board_order(req->user);
    }

    ret += "<h2><img src=\"http://discworld.imaginary.com/external/pics/tfrog-news.gif\" alt=\" \">"
      "Order of reading boards</h2>\n";

    tmp = ({ });
    tmp += get_next_new_note(req->user, BOARD_HAND->query_newsrc(req->user),
                             req, 0);
    tmp += ({ "<a href=\"boards.c?type=summary&amp;threaded=" + threaded +
                "&amp;board=frog\">" +
      "Summary</a>" });
    tmp += ({ "<a href=\"boards.c?board=frog&amp;threaded=" + threaded +
                "&amp;type=summary-all\">"
              "All&nbsp;Boards</a>" });
    ret += "<font size=-1><strong>[&nbsp;" + implode(tmp, " | ") +
    "&nbsp;]</strong></font>\n";

    ret += "<blockquote>This page allows you to set the order in which the boards "
      "will be displayed for you on the web.  This sets the order "
      "for the 'next new note' button as well.  If you do not specify "
      "a postion (leave it as default) then the order will be detemined "
      "by the mud itself.  The order listed here and the board summary "
      "will always be correct.</blockquote>\n";
    ret += "<form method=post action=\"boards.c?type=readorder&amp;board=frog\">\n";



    //ret += "<table cols=2><tr>\n";
    board_names = current_order + (board_names - current_order);
    foreach (board in board_names) {
        //ret += "<td>";
        ret += "<select name=\"" + replace_string(board, " ", "+") + "\">\n";
        pos = member_array(board, current_order);
        if (pos == -1) {
            ret += "<option selected>default\n";
        } else {
            ret += "<option>default\n";
        }
        pos++;
        for (i = 1; i < sizeof(board_names) + 1; i++) {
            if (i != pos) {
                ret += "<option>" + i + "\n";
            } else {
                ret += "<option selected>" + i + "\n";
            }
        }
        ret += "</select>";
        ret += board + "<br>\n";
    }

    //ret += "</tr></table>";
    ret += "<input type=submit value=\"Set board order\">" +
    "<input type=reset value=\"Reset to start values\">\n</form>\n";

    ret += "<font size=-1><strong>[&nbsp;" + implode(tmp, " | ") +
    "&nbsp;]</strong></font>\n";

    ret += ("/www/footer"->www_function("footer", ([ ]), req)) +
    "</body></html>";

    return ret;
} /* read_order() */

string *get_allowed_boards(string name) {
    string *ret;

    ret = ({ });

    if("/secure/master"->query_lord(name))
      ret += ({ "lordboard", "complaints" });

    if (PLAYER_HANDLER->test_creator(name)) {
        ret += _allowed_creator_boards;
        ret += _allowed_playtester_boards;
    }
    if (PLAYTESTER_HAND->query_playtester (name)) {
      ret += _allowed_playtester_boards;
    }
    
    ret += _allowed_boards;

    return ret;
} /* get_allowed_boards() */

string* get_user_board_order(string user) {
    string* allowed;
    string* boards;
    class player_cache cache;

    if (_board_order[user] && _board_order[user]->order) {
       _board_order[user]->last_read = time();
       return _board_order[user]->order;
    }
    allowed = get_allowed_boards(user);
    boards = BOARD_HAND->query_board_order(user);
    allowed = boards + sort_array(allowed - boards, 1);
    allowed -= BOARD_HAND->list_killfile(user);
    cache = new(class player_cache);
    cache->order = allowed;
    cache->last_read = time();
    _board_order[user] = cache;
    return allowed;
}

/*
 * This is called when something is posted, it will increment the
 * count and fiddle with stuff.
 */
private void do_post_note(string name) {
    class posted_information bing;

    if (!undefinedp(posted_stuff[name])) {
        bing = (class posted_information)posted_stuff[name];
    } else {
        bing = new (class posted_information);
    }
    if (time() > bing->time_first_posted + TIME_INTERVAL_FOR_POSTING) {
        bing->time_first_posted = time();
        bing->count = 0;
    }
    bing->count++;
    posted_stuff[name] = bing;
    unguarded( (: save_object(SAVE_FILE) :) );
} /* do_post_note() */

/*
 * This is called when a note is eaten, it will decrement the allowed
 * number of posts.
 */
private void do_delete_note(string name) {
    class posted_information bing;

    if (!undefinedp(posted_stuff[name])) {
        bing = (class posted_information)posted_stuff[name];
        if (time() > bing->time_first_posted + TIME_INTERVAL_FOR_POSTING) {
            map_delete(posted_stuff, name);
        } else {
            bing->count--;
            posted_stuff[name] = bing;
        }
        unguarded( (: save_object(SAVE_FILE) :) );
    }
} /* do_delete_note() */

private string can_post_note(string name, string board) {
    class posted_information bing;

    if ("/secure/bastards"->query_suspended(name))  {
      return "account is suspended";
    }

    // Tests to see if you can write to the board.
    // Turn off this check.
    if (!BOARD_HAND->test_can_write(board, this_object(), name)) {
      return "no write access";
    }

    // Check to see if they are less than 6 hours old.
    if (PLAYER_HANDLER->test_age(name) > - 6 * 60 * 60) {
      return "character is too young";
    }

    // inactive players can't post.
    if(!PLAYER_HANDLER->test_active(name)) {
      return "character is inactive";
    }

    if (undefinedp(posted_stuff[name]) || (board != "flame" && PLAYER_HANDLER->test_creator(name))) {
        return "";
    }

    bing = (class posted_information)posted_stuff[name];
    if (time() > bing->time_first_posted + TIME_INTERVAL_FOR_POSTING) {
        map_delete(posted_stuff, name);
        return "";
    }


    // Upto N posts - number of days since last login.
    // Test_last returns a -ve number.
    if (bing->count < (TIME_POSTING_MAXIMUM -
        (time() - PLAYER_HANDLER->test_last(name)) / 85400)) {
        return "";
    }
    return "daily posting limit (" + TIME_POSTING_MAXIMUM + ") exceeded";
}

private int can_followup_note(string name, string board, mixed *stuff) {
    class posted_information bing;

    if ("/secure/bastards"->query_suspended(name))  {
        return 0;
    }

    // Tests to see if you can write to the board.
    if (!BOARD_HAND->test_can_write(board, this_object(), name) &&
      !stuff[B_REPLY_TYPE]) {
        return 0;
    }

    // inactive players can't post.
    if(!PLAYER_HANDLER->test_active(name))
        return 0;

    if (undefinedp(posted_stuff[name]) || (board != "flame" && PLAYER_HANDLER->test_creator(name))) {
        return 1;
    }

    bing = (class posted_information)posted_stuff[name];
    if (time() > bing->time_first_posted + TIME_INTERVAL_FOR_POSTING) {
        map_delete(posted_stuff, name);
        return 1;
    }
    // Test_last returns a -ve number.
    if (bing->count < (TIME_POSTING_MAXIMUM -
        (time() - PLAYER_HANDLER->test_last(name)) / 85400)) {
        return 1;
    }
    return 0;
} /* can_followup_note() */

void dest_me() {
    destruct(this_object());
} /* dest_me() */
