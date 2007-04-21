/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: board_handler.c,v 1.71 2003/05/06 22:31:22 ceres Exp $
 */
/**
 * Keeps track of all the bulletin board notes and related information.
 *
 * @index bulletin
 * @author Pinkfish
 */
#include <board.h>
#include <mail.h>
#include <clubs.h>
#include <player_handler.h>

//#define DEBUG 1

#define T_MIN 0
#define T_MAX 1
#define T_TIMEOUT 2
#define DEFAULT_MIN 10
#define DEFAULT_MAX 80
#define DEFAULT_TIMEOUT 14
#define ARCHIVE_DIR "/open/boards/"
// maximum archive file size
#define MAX_ARCHIVE_SIZE 1048576

#define BACKUPS_DIR "/save/board_backups/"
#define NEWSRC_SAVE_DIR "/save/newsrc/"
#define BACKUP_TIME_OUT (7 * 24 * 60 * 60)
#define BOARD_HANDLE_VERSION 1

#ifdef USE_RAMDISK
#define CACHE_SIZE 10
#else
#define CACHE_SIZE 250
#endif

#define CACHE_TIMEOUT 1800
#define NEWSRC_SAVE_DELAY 300

class newsrc {
  int cached;
  int changed;
  mapping newsrc;
  string *kill;
  string *board_order;
}

class read_access_cache {
   int read_time;
   int read_access;
}

private int num;
private int board_version;
private mapping archives;
private mapping boards;
private mapping priv;
private mapping security;
private mapping timeouts;
private nosave string *_allowed_objects;
private nosave mapping message_cache;
private nosave mapping _newsrc_cache;
private nosave int total_reads, cache_hits, ram_hits, clean_out;
private nosave int _ram_exists;
private nosave int _newsrc_reads, _newsrc_cache_hits;
private mapping _idiots;
private nosave mapping _read_access_cache;

string query_archive(string board);
protected int zap_message(string board, int num, string name);
int can_delete_message(string board, int off, string pname);
void save_me();

void create() {
  int number;
  int last;
  string line;
  string *lines;

  seteuid(master()->creator_file(file_name(this_object())));
  num = 1;
  boards = ([ ]);
  security = ([ ]);
  _read_access_cache = ([ ]);
  priv = ([ ]); /* Private boards... only lords or specified people
                 * can read. */
  timeouts = ([ ]);
  archives = ([ ]);
  message_cache = ([ ]);
  _newsrc_cache = ([ ]);

  _allowed_objects = ({
    // These are the objects allowed to post...
    BOARD_HAND,
      "/obj/misc/board",
      "/obj/misc/board_mas",
      "/obj/handlers/applications_handler",
      "/w/ceres/board",
      "/w/ceres/board_mas",
      "/www/boards",
      "/www/secure/nboards",
      "/obj/handlers/club_handler",
      "/obj/handlers/deity_handler",
      "/obj/handlers/folder_handler",
      "/obj/handlers/playtesters",
      "/obj/handlers/error_tracker",
      "/obj/handlers/twiki",
      "/d/forn/utils/error_tracker",
      "/d/forn/handlers/peer_review",
      "/d/sur/utils/error_tracker",
      "/d/cwc/utils/error_tracker",
      "/d/Ankh-Morpork_dev/utils/error_tracker",
      "/d/liaison/utils/error_tracker",
      "/d/am/utils/error_tracker",
      "/d/guilds/priests/items/desk",
      "/d/ram/error_tracker",
      "/d/ram/cool/bank",
      "/d/special/error_tracker",
      "/d/klatch/utils/error_tracker",
      "/d/guilds/error_tracker",
      "/d/special/utils/error_tracker",
      "/d/am/patrician/pat_applications",
      "/d/am/patrician/pat_complaints",
      "/cmds/player/news",
      "/cmds/player/apply",
      "/d/guilds/assassins/Ankh-Morpork/admin/vote_room",
      "/d/special/player_shops/tarnach's_office",
      "/std/shops/player_shop/office",
      "/d/playtesters/handlers/applications",
      "/d/guilds/warriors/Ankh-Morpork/voting_booth",
      "/secure/cmds/creator/errors",
      "/obj/handlers/player_council",
      "/d/guilds/wizards/utils/board_hand",
      "/obj/handlers/complaints_handler",
      "/obj/handlers/project_management",
      "/d/guilds/thieves/Ankh-Morpork/rooms/ground/voting_booth",
      "/d/underworld/utils/error_tracker"      
      });

  _idiots = ([ ]);

  if (file_size(BOARD_FILE +".gz") > 0) {
     unguarded((: cp, BOARD_FILE +".gz", BACKUPS_DIR +"boards."+ time() :));
  }
  lines = unguarded((: get_dir, BACKUPS_DIR +"boards.*" :));
  if ( sizeof( lines ) > 6 ) {
    last = time() - BACKUP_TIME_OUT;
    foreach(line in lines) {
      sscanf(line, "boards.%d", number);
      if (number < last)
        unguarded((: rm, BACKUPS_DIR + line :));
    }
  }
  if(!unguarded((: restore_object, BOARD_FILE+".gz" :)))
    unguarded((: restore_object, BOARD_FILE :));

  call_out("expire_boards", 5);
} /* create() */

/**
 * This method returns the file name of the message to use in retreiving
 * the message.
 * @param board the board to get the message from
 * @param num the number of the mesage to download
 */
private string get_filename(string board, int num) {
   string fixed_board;

   fixed_board = replace(board, ({" ", "_", "'", ""}));
 
   // return the filename in the appropriate subdirectory.
   return BOARD_DIR + fixed_board + "/" + num;
} /* get_filename() */

/**
 * This method returns the file name of the ramdisk cach of the message
 * the message.
 * @param board the board to get the message from
 * @param num the number of the mesage to download
 */
private string get_ram_filename(string board, int num) {
   string fixed_board;

   fixed_board = replace(board, ({" ", "_", "'", ""}));

   if(!_ram_exists) {
     _ram_exists = file_size(BOARD_RAM_DIR);
   }

   // return the filename in the appropriate subdirectory.
   // If BOARD_RAM_DIR exists we return the ramdisk file otherwise
   // we return the standard file.
   if(_ram_exists == -2)
     return BOARD_RAM_DIR + fixed_board + "/" + num;
   else
     return BOARD_DIR + fixed_board + "/" + num;
}

/**
 * Returns the names of all the boards.
 * @return the names of all the boards
 */
string *query_boards() {
   return keys(boards);
} /* query_boards() */

/**
 * This method tests to see if the board exists.
 * @param board the name of the board to check
 * @return 1 if the board exists, 0 if it does not
 */
int is_board(string board) {
   if (boards[board]) {
      return 1;
   }
   return 0;
} /* is_board() */

private int query_lord(string str) {
   return (int)master()->query_lord(str);
} /* query_lord() */

/**
 * This method checks to see if the specified person is an idiot or
 * not.
 * @param name the name to check
 * @return 1 if they are an idiot
 */
int is_idiot(string name) {
   if (!_idiots) {
      _idiots = ([ ]);
   }
   if (_idiots[name]) {
      if (_idiots[name] > time()) {
         return 1;
      }
      map_delete(_idiots, name);
   }
   return 0;
} /* is_idiot() */

/**
 * This method adds an idiot to the current list of idiots.
 * @param idiot the idiot to add
 * @param length the amount of days to add them for
 */
void add_idiot(string idiot, int length) {
   _idiots[idiot] = time() + (60 * 60 * 24 * length);
   save_me();
} /* add_idiot() */

/**
 * This method returns if they are an idiot and when their period of
 * idiocy runs out.
 * @param idiot the idiot to check
 * @return 0 if not an idiot, the time left otherwise
 */
int query_idiot(string idiot) {
   return _idiots[idiot];
} /* query_idiot() */

/**
 * Check to see if read access is allowed.
 * @param board the board to check
 * @param previous the previous object
 * @param name the name of the person doing stuff
 * @return 1 if it is allowed, 0 if not
 */
int test_can_read(string board, object previous,
                      string name) {
   int bit;
   int ret;

   /* For the moment we will just log all bad access... */
   if (member_array(base_name(previous), _allowed_objects) == -1 &&
       board != "announcements") {
#ifdef DEBUG
      log_file("BAD_BOARD", ctime(time()) + " (read): [" + board + "] " +
               base_name(previous) +
               sprintf(" (%O)\n", this_player()->query_name()));
#endif
      return 0;
   }
   bit = priv[board] & B_PRIV_TYPE_MASK;
   // The default is easy.
   if (bit == 0) {
      return 1;
   }

   // Check the cache.
   if (_read_access_cache[board] &&
       !undefinedp(_read_access_cache[board][name])) {
      return _read_access_cache[board][name];
   }

   // Default, you can read!
   ret = 1;
   if (bit == B_PRIV_ACCESS_RESTRICTED) {
      ret = query_lord(name) ||
         (member_array(name, security[board]) != -1);
   } else if (bit == B_PRIV_ACCESS_RESTRICTED_METHOD) {
      if (query_lord(name)) {
         ret = 1;
      } else {
         // This will now need to check the method and function to call.
         if (sizeof(security[board]) == 2) {
            ret = call_other(security[board][1],
                           security[board][0],
                           B_ACCESS_READ,
                           board, previous, name);
         }
      }
   }

   if (!_read_access_cache[board]) {
      _read_access_cache[board] = ([ ]);
   }
   _read_access_cache[board][name] = ret;
   return ret;
} /* test_can_read() */

/**
 * Check to see if write access is allowed.
 * @param board the board to check
 * @param previous the previous object
 * @param name the name of the person doing stuff
 * @return 1 if it is allowed, 0 if not
 */
int test_can_write(string board, object previous,
                      string name) {
  int bit;
  
  if (is_idiot(name)) {
     return 0;
  }

  if (member_array(base_name(previous), _allowed_objects) == -1) {
#ifdef DEBUG
    log_file("BAD_BOARD", ctime(time()) + " (write): " + base_name(previous) +
                          sprintf(" (%O)\n", this_player()));
#endif
    return 0;
  }
  bit = priv[board] & B_PRIV_TYPE_MASK;
  if (bit == B_PRIV_ACCESS_RESTRICTED_FILE) {
    return member_array(base_name(previous), security[board]) != -1;
  }
  if (bit == B_PRIV_ACCESS_RESTRICTED_METHOD) {
    if (query_lord(name)) {
      return 1;
    }
    // This will now need to check the method and function to call.
    if (sizeof(security[board]) == 2) {
      return call_other(security[board][1],
                        security[board][0],
                        B_ACCESS_WRITE,
                        board, previous, name);
    }
    return 0;
  }
  if (bit == B_PRIV_ACCESS_RESTRICTED ||
      bit == B_PRIV_READ_ONLY) {
    return query_lord(name) ||
           (member_array(name, security[board]) != -1);
  }
  // Default, you can write!
  return 1;
} /* test_can_write() */

/**
 * Check to see if delete is allowed.
 * @param board the board to check
 * @param previous the previous object
 * @param name the name of the person doing stuff
 * @return 1 if it is allowed, 0 if not
 */
int test_can_delete(string board, object previous,
                      string name) {
  int bit;

  if (member_array(base_name(previous), _allowed_objects) == -1) {
#ifdef DEBUG
    log_file("BAD_BOARD", ctime(time()) + " (write): " + base_name(previous) +
                          sprintf(" (%O)\n", this_player()));
#endif
    return 0;
  }
  bit = priv[board] & B_PRIV_TYPE_MASK;
  if (bit == B_PRIV_ACCESS_RESTRICTED_FILE) {
    return query_lord(name) ||
           member_array(base_name(previous), security[board]) != -1;
  }
  if (bit == B_PRIV_ACCESS_RESTRICTED ||
      bit == B_PRIV_READ_ONLY) {
    return query_lord(name) ||
           (member_array(name, security[board]) != -1);
  }
  if (bit == B_PRIV_ACCESS_RESTRICTED_METHOD) {
    if (query_lord(name)) {
      return 1;
    }
    // This will now need to check the method and function to call.
    if (sizeof(security[board]) == 2) {
      return call_other(security[board][1],
                        security[board][0],
                        B_ACCESS_DELETE,
                        board, previous, name);
    }
  }
  // Default, you can delete if your a lord!
  return query_lord(name);
} /* test_can_delete() */

private string query_destination_eaten_note(string  board, object previous,
                                            string name) {
  int bit;

  bit = priv[board] & B_PRIV_TYPE_MASK;
  if (bit == B_PRIV_ACCESS_RESTRICTED_METHOD) {
    // This will now need to check the method and function to call.
    if (sizeof(security[board]) == 2) {
      return call_other(security[board][1],
                        security[board][0],
                        B_ACCESS_DELETE,
                        board, previous, name);
    }
  }
  return 0;
} /* query_destination_eaten_note() */

/**
 * Get the subjects for the specifed board.  The subjects are
 * returns in a special array format.  See the include file for the
 * defines to get at the members of the array.
 * @see /include/board.h
 * @param name the board name to lookup
 * @return the subject array
 */
varargs mixed get_subjects(string name, string person) {
   string pl;
   int bit;

   if (file_name(previous_object()) == BOARD_WEB ||
       file_name(previous_object()) == "/w/ceres/cmds/test") {
      pl = person;
   } else if (this_player()) {
      pl = (string)this_player()->query_name();
   } else {
      pl = "unknown";
   }
   if (!test_can_read(name, previous_object(), pl)) {
      return ({ });
   }
   if (!boards[name] || bit) {
      return ({ });
   }
   return boards[name];
} /* get_subjects() */

/**
 * Get the text of a specific message.  This will look up the
 * text on a board with the given number and return that to the
 * caller.  The number is the offset into the subject array in
 * which to get the message from.
 * @param board the board name to get the message from
 * @param num the message number to use
 * @return the message or 0 if it failed
 */
string get_message(string board, int num, string person) {
  string name, rname;

  if (file_name(previous_object()) == BOARD_WEB) {
     name = person;
  } else  if (file_name(previous_object())[0..10] !=
              "/obj/misc/board"[0..10]) {
    name = "unknown";
  } else {
    name = (string)this_player()->query_name();
  }

  if(board == "lordboard") {
    catch(log_file("/d/admin/log/LORDBOARD", "%s read by prev %s player %s\n",
                   ctime(time()), base_name(previous_object()),
                   this_player()->query_name())); 
  }
  
  if (!test_can_read(board, previous_object(), name)) {
    return 0;
  }

  if (num < 0 || num >= sizeof(boards[board])) {
    return 0;
  }

  if(!message_cache) {
    message_cache = ([ ]);
  }

  total_reads++;

  // if it's not in the cache put it in there.
  if(message_cache[boards[board][num][B_NUM]]) {
    cache_hits++;
    return message_cache[boards[board][num][B_NUM]];
  }

  name = get_filename(board, boards[board][num][B_NUM]);
  if (file_size(name) <= 0) {
    return 0;
  }
  
  if(!clean_out && sizeof(keys(message_cache)) > CACHE_SIZE)
    clean_out = call_out("clean_cache", 10);

#ifdef USE_RAMDISK
  rname = get_ram_filename(board, boards[board][num][B_NUM]);
  if(rname != name) {
    if(file_size(rname) == -1) {
      unguarded((: cp, name, rname :));
    } else
      ram_hits++;
    name = rname;
  }
#endif
  
  if(!_ram_exists || _ram_exists == -1) {
    message_cache[boards[board][num][B_NUM]] = unguarded((: read_file, name :));
    return message_cache[boards[board][num][B_NUM]];
  } else {
    return unguarded((: read_file, name :));
  }
} /* get_message() */

void clean_cache() {
  int i;
  int count;
  int *list;
  string name;

  // prevent the cache getting too big.
  count = sizeof(keys(message_cache));
  list = sort_array(keys(message_cache), 0);
  for(i=0; i < (count - CACHE_SIZE); i++) {
    map_delete(message_cache, list[i]);
  }

  foreach(name in keys(_newsrc_cache)) {
    if(!_newsrc_cache[name]->changed &&
       _newsrc_cache[name]->cached < time() - CACHE_TIMEOUT) {
      map_delete(_newsrc_cache, name);
    }
  }

  clean_out = 0;

   foreach (name in keys(_read_access_cache)) {
      _read_access_cache[name] = ([ ]);
   }
} /* clean_cache() */

/**
 * Saves the state of the object.
 */
void save_me() {
  unguarded((: save_object, BOARD_FILE, 2 :));
} /* save_me() */

/**
 * Adds a new message onto the board.  This call can only be done from
 * verified source, like the bulletin oard objects themselves.  The
 * number used as a reply to should be the message number itself, not
 * the logical index.  If the reply_to is 0 then it is not
 * replying to anything at all.
 * @param board the board to add the message to
 * @param cap_name the name ofthe person posting
 * @param subject the subject of the message
 * @param body the main section of the text
 * @param reply_to the note the message is replying to
 * @return the note number, or 0 on failure
 */
int add_message(string board, string cap_name, string subject, string body,
                int reply_to, class reply_type bing) {
   int test;
   int irp;
   int index;
   string fname;
   string name;
   string from_mess;
   string mail_to;
   class reply_type reply;

   name = lower_case(cap_name);

   /* Check to see what should happen to the reply. */
   if (reply_to) {
      for (index = 0; index < sizeof(boards[board]); index++) {
         if (boards[board][index][B_NUM] == reply_to) {
            if (boards[board][index][B_REPLY_TYPE]) {
               // Do something special.
               reply = (class reply_type)boards[board][index][B_REPLY_TYPE];
               if (reply->type == B_REPLY_POSTER) {
                  mail_to = boards[board][index][B_NAME];
               } else if (reply->type == B_REPLY_NAMED) {
                  mail_to = reply->data;
               }
               if (mail_to) {
                  // Send mail instead.
                  MAILER->do_mail_message(mail_to,
                                          name,
                                          subject,
                                          "",
                                          body);
                  return 1;
               }
            }
         }
      }
   }

   if (!test_can_write(board, previous_object(), name)) {
      return 0;
   }

   if (!boards[board] || test) {
      return 0;
   } else {
      boards[board] += ({ ({ subject,
                                cap_name,
                                num++,
                                time(),
                                bing,
                                reply_to }) });
   }

   if (file_name(previous_object())[0..4] == "/www/") {
      from_mess = " [Web post]";
   } else {
      from_mess = "";
   }

   fname = get_filename(board, num-1);
   unguarded((: rm, fname :));
   fname = get_filename(board, num-1);

   unguarded((: write_file, fname, body :));
   save_me();

   // add this new message to the cache.
   message_cache[num-1] = body;
   // DO not update the message cache here since the num variable does not
   // actually contain the post number any more.
   // Further to this, shouldn't it be num-1 instead of num, that way it should
   // work -- Ceres.

   if (timeouts[board] && timeouts[board][T_MAX] &&
       sizeof(boards[board]) > timeouts[board][T_MAX]) {
      /* should be at most one message we eat...  */
      while (sizeof(boards[board]) > timeouts[board][T_MAX]) {
         zap_message(board, 0, 0);
         irp++;
      }
      if ((priv[board] & B_PRIV_TYPE_MASK) ==
          B_PRIV_ACCESS_RESTRICTED_METHOD) {
         // This will now need to check the method and function to call.
         if (sizeof(security[board]) == 2) {
            call_other(security[board][1],
                       security[board][0],
                       B_ACCESS_INFORM,
                       board, 0, cap_name, irp);
         }
      }
      if ( !(priv[board] & B_PRIV_NO_INFORM) ) {
         user_event( "inform",
                     sprintf( "%s posts a message to %s and %d message%s in sympathy%s",
                              cap_name, board, irp,
                              ( irp > 1 ? "s explode" : " explodes" ),
                              from_mess ),
                     "message",
                     this_player() );
      }
   } else {
      if ((priv[board] & B_PRIV_TYPE_MASK) ==
          B_PRIV_ACCESS_RESTRICTED_METHOD) {
         // This will now need to check the method and function to call.
         if (sizeof(security[board]) == 2) {
            call_other(security[board][1],
                       security[board][0],
                       B_ACCESS_INFORM,
                       board, 0, cap_name, 0);
         }
      }
      if ( !(priv[board] & B_PRIV_NO_INFORM) ) {
         user_event( "inform",
                     sprintf( "%s posts a message to %s%s",
                              cap_name, board, from_mess),
                     "message",
                     this_player() );
      }
   }
   return num-1;
} /* add_message() */

/**
 * Create a new board.
 * @param board the name of the board to create
 * @param priva is this board only allowed prviliged access?
 * @param person the person to add into the security array initialy
 * @return 0 on a failure, 1 on success
 */
int create_board(string board,
                 int priviliges,
                 string person) {
   if (boards[board]) {
      return 0;
   }
   if (!person) {
      person = this_player()->query_name();
   }
   boards[board] = ({ });
   security[board] = ({ person });
   if (priviliges) {
      priv[board] = priviliges;
   }
   save_me();
   //printf("Created board %s.\n", board);
   return 1;
} /* create_board() */

/**
 * Adds a member into the security array for a board.  This allows
 * certain people to read boards they may not normaly have
 * access to.
 * @param board the board to change the access on
 * @param name the name of the person to add to the array
 * @return 0 on failure, 1 on success
 */
int add_allowed(string board, string name) {
  string nam;
  int board_type;

  board_type = priv[board] & B_PRIV_TYPE_MASK;
  if (sscanf(file_name(previous_object()), "/obj/misc/board%s", nam) != 1) {
    return 0;
  }
  nam = (string)this_player()->query_name();
  if (!test_can_write(board, previous_object(), nam) ||
      (board_type != B_PRIV_ACCESS_RESTRICTED &&
       board_type != B_PRIV_READ_ONLY &&
       board_type != B_PRIV_ACCESS_RESTRICTED_FILE)) {
    return 0;
  }
  if (!PLAYER_HANDLER->test_user(name)) {
    return 0;
  }
  security[board] += ({ name });
  save_me();
  printf("Added %s to the security array for %s.\n",name, board);
  return 1;
} /* add_allowed() */

/**
 * Remove someone from the allowed array of the board.
 * @param board the board to remove the person from
 * @param name the name of the person to remove
 * @return 0 on nfailure and 1 on success
 */
int remove_allowed(string board, string name) {
  string nam;
  int i;
  int board_type;

  if (sscanf(file_name(previous_object()), "/obj/misc/board%s", nam) != 1) {
    return 0;
  }
  nam = geteuid(previous_object());
  board_type = priv[board] & B_PRIV_TYPE_MASK;
  if (!test_can_write(board, previous_object(), nam) ||
      (board_type != B_PRIV_ACCESS_RESTRICTED &&
       board_type != B_PRIV_READ_ONLY &&
       board_type != B_PRIV_ACCESS_RESTRICTED_FILE)) {
    return 0;
  }
  security[board] = delete(security[board], i, 1);
  save_me();
  printf("Removed %s from the security array for %s.\n", name, board);
  return 1;
} /* add_allowed() */

/**
 * This method sets the method to call to check for allowed postings
 * to a board setup as an method controlled post board.
 * @param board the name of the board to setup the method for
 * @param method the method to call on the object
 * @param name the object to call the method on
 * @return 0 if the method failed, 1 if it was successful
 */
int set_method_access_call(string board, string method, string name) {
   string pl;

   if (!boards[board] ||
       (priv[board] & B_PRIV_TYPE_MASK) != B_PRIV_ACCESS_RESTRICTED_METHOD) {
      return 0;
   }
   if (this_player()) {
      pl = (string)this_player()->query_name();
   } else {
      pl = "unknown";
   }
   if (query_lord(pl) ||
       file_name(previous_object()) == CLUB_HANDLER) {
      security[board] = ({ method, name });
      save_me();
      return 1;
   }
   return 0;
} /* set_method_access_call() */

/**
 * This method changes the type of the board to be a method access call
 * access restriction, instead of whatever it had before.
 * @param board the name of the board to control the access for
 */
int force_board_method_access_restricted(string board) {
   string pl;

   if (!boards[board]) {
      return 0;
   }
   if (this_player()) {
      pl = (string)this_player()->query_name();
   } else {
      pl = "unknown";
   }
   if (query_lord(pl) ||
       file_name(previous_object()) == CLUB_HANDLER) {
      priv[board] = (priv[board] & ~B_PRIV_TYPE_MASK) |
                    B_PRIV_ACCESS_RESTRICTED_METHOD;
      save_me();
      return 1;
   }
   return 0;
} /* force_board_method_access_restricted() */

/**
 * @ignore yes
 */
protected int zap_message(string board, int off, string name) {
  int num;
  string nam, archive;

  if (off < 0 || off >= sizeof(boards[board])) {
    return 0; /* out of range error */
  }
  num = boards[board][off][B_NUM];
  nam = get_filename(board, num);
  archive = query_archive(board);
  if (archive) {
    mixed *stuff;

    stuff = boards[board][off];
    // When we archive it, strip the colours out.
    unguarded((: write_file, archive,
               sprintf("\n----\nNote #%d by %s posted at %s\nTitle: '%s'\n\n",
                       off, stuff[B_NAME], ctime(stuff[B_TIME]),
                       stuff[B_SUBJECT])+unguarded((: read_file, nam :)) :));

    if(unguarded( (: file_size, archive :) ) > MAX_ARCHIVE_SIZE)
      unguarded((: rename, archive, archive+"."+time() :));
  }
  boards[board] = delete(boards[board],off,1);
  unguarded((: rm, nam :));
  save_me();
  return 1;
} /* zap_message() */

/**
 * Remove a message from a board.  The offset is the offset into the
 * subjects array.
 * @param board the board to remove the message from
 * @param off the offset to delete
 * @param override_name used by the web boards
 * @return 0 on failure and 1 on success
 */
int delete_message(string board, int off, string override_name) {
  string nam;

  if (file_name(previous_object()) == "/www/boards") {
    nam = override_name;
  } else {
    nam = this_player()->query_name();
  }

  if (!can_delete_message(board, off, nam)) {
    return 0;
  }

  return zap_message(board, off, nam);
} /* delete_message() */

/**
 * Check to see if the named person can delete the message.
 * @param pname the player name
 * @param board the board name
 * @param off the offset to delete
 * @see delete_message()
 */
int can_delete_message(string board, int off, string pname) {
  if (!boards[board]) {
    return 0;
  }

  if (off >= sizeof(boards[board])) {
    return 0;
  }

  if (!test_can_delete(board, previous_object(), pname) &&
      (lower_case(boards[board][off][B_NAME]) != lower_case(pname))) {
    return 0; /* not allowed to delete the notes */
  }

  return 1;
} /* can_delete_message() */

/**
 * Returns the security array for the given board.
 * @param board the board to get the security array for
 * @return the security array
 */
string *query_security(string board) {
  string *str;

  str = security[board];
  if (!str) {
    return str;
  }
  return copy(str);
} /* query_security() */

/**
 * Complete erase a board.
 * @param board the board to delete
 * @return 0 on failure and 1 on success
 */
int delete_board(string board) {
  string nam;

  if (!boards[board]) {
    return 0;
  }
  nam = geteuid(previous_object());
  if (!query_lord(nam) ||
      file_name(previous_object()) == CLUB_HANDLER) {
    return 0; /* not allowed to delete the notes */
  }
  while (sizeof(boards[board])) {
    if (!zap_message(board, 0, 0)) {
        return 0;
    }
  }
  map_delete(boards, board);
  map_delete(security, board);
  map_delete(archives, board);
  map_delete(timeouts, board);
  save_me();
  return 1;
} /* delete_board() */

/**
 * The names of all the boards.
 * @see query_boards()
 * @return the list of all the boards
 */
string *list_of_boards() {
  return keys(boards);
} /* list_of_boards() */

/**
 * Change the time before a message automatic gets deleted off a
 * board.
 * @param board the name of the board to set the timeout for
 * @param timeout the timeout (in seconds)
 * @return 0 on failure and 1 on success
 */
int set_timeout(string board, int timeout) {
  string nam;

  if (!boards[board]) {
    return 0;
  }
  nam = geteuid(previous_object());
  if (!test_can_write(board, previous_object(), nam)) {
    return 0; /* not allowed to delete the notes */
  }
  if (!timeouts[board]) {
    timeouts[board] = ({ DEFAULT_MIN, DEFAULT_MAX, timeout });
    return 1;
  }
  timeouts[board][T_TIMEOUT] = timeout;
  save_me();
  printf("Set the automagic timeout to %d days for %s.\n", timeout, board);
  return 1;
} /* set_timeout() */

/**
 * Sets the minimum number of message to keep on a board.  If there is less
 * than this
 * number then they will not be auto deleted.
 * @param board the board to set the minimum for
 * @param min the number of message to keep
 * @return 0 on failure and 1 on success
 */
int set_minimum(string board, int min) {
  string nam;

  if (!boards[board]) {
    return 0;
  }
  nam = geteuid(previous_object());
  if (!test_can_write(board, previous_object(), nam)) {
    return 0; /* not allowed to delete the notes */
  }
  if (!timeouts[board]) {
    timeouts[board] = ({ min, DEFAULT_MAX, DEFAULT_TIMEOUT });
    return 1;
  }
  timeouts[board][T_MIN] = min;
  save_me();
  printf("Set the minimum number of messages to %d for %s.\n", min, board);
  return 1;
} /* set_minimum() */

/**
 * Set the maximum number of message before they start being auto deleted
 * when someone posts to the board.
 * @param board the board to set the maximum for
 * @param max the maximum number of messages
 * @return 0 if it failed and 1 on success
 */
int set_maximum(string board, int max) {
  string nam;

  if (!boards[board]) {
    return 0;
  }
  nam = geteuid(previous_object());
  if (!test_can_write(board, previous_object(), nam)) {
    return 0; /* not allowed to delete the notes */
  }
  if (!timeouts[board]) {
    timeouts[board] = ({ DEFAULT_MIN, max, DEFAULT_TIMEOUT });
    return 1;
  }
  timeouts[board][T_MAX] = max;
  save_me();
  printf("Set the maximum number of messages to %d for %s.\n", max, board);
  return 1;
} /* set_maximum() */

/**
 * Set the archive file location.  This is where all deleted messages
 * wil be stored.
 * @param board the board to set the archive for
 * @param file the file name to set it to
 * @return 0 on failure and 1 on success
 */
int set_archive(string board, string file) {
  string nam;

  if (!boards[board]) {
    return 0;
  }
  nam = geteuid(previous_object());
  if (!test_can_write(board, previous_object(), nam)) {
    return 0; /* not allowed to delete the notes */
  }
  archives[board] = file;
  save_me();
  printf("Set the archive file to %s for %s.\n", file, board);
  return 1;
} /* set_archive() */

/**
 * Return the timeout time of the board.
 * @param board the board to get the timeout for
 * @return the timeout in seconds
 * @see set_timeout()
 */
int query_timeout(string board) {
  if (!timeouts[board]) {
    return 0;
  }
  return timeouts[board][T_TIMEOUT];
} /* query_timeout() */

/**
 * Return the minimum number of message allowed on the board.
 * @param board the board to get the minimum numbr of message for
 * @return 0 on failure, the minimum number of messages on success
 */
int query_minimum(string board) {
  if (!timeouts[board]) {
    return 0;
  }
  return timeouts[board][T_MIN];
} /* query_minimum() */

/**
 * Return the maximum number of message allowed on the board.
 * @param board the board to get the maximum numbr of message for
 * @return 0 on failure, the maximum number of messages on success
 */
int query_maximum(string board) {
  if (!timeouts[board]) {
    return 0;
  }
  return timeouts[board][T_MAX];
} /* query_maximum() */

/**
 * Return the archive file location for the board.
 * @param board the board to get the archive location for
 * @return the archive file location, 0 on failure
 */
string query_archive(string board) {
  if (!boards[board]) {
    return 0;
  }
  if (undefinedp(archives[board])) {
    return ARCHIVE_DIR+board;
  }
  return archives[board];
} /* query_archive() */

/**
 * This method checks to see if the board is in restricted access mode.
 * @param board the name of the board to check
 * @return 1 if it is, 0 if it is not
 */
int query_restricted_access(string board) {
   return (priv[board] & B_PRIV_TYPE_MASK) == B_PRIV_ACCESS_RESTRICTED;
} /* query_restricted_access() */

/**
 * This method checkes to see if the board is in restricted access file
 * mode.
 * @param board the name of the board to check
 * @return 1 if it is, 0 if it is not
 */
int query_restricted_access_file(string board) {
   return (priv[board] & B_PRIV_TYPE_MASK) == B_PRIV_ACCESS_RESTRICTED_FILE;
} /* query_restricted_access_file() */

/**
 * This method checks to see if the board is in a read only mode.
 * @param board the name of the board to check
 * @return 1 if it is read only, 0 if not
 */
int query_read_only(string board) {
  return (priv[board] & B_PRIV_TYPE_MASK) == B_PRIV_READ_ONLY;
} /* query_read_only() */

/**
 * This method checks to see if the board is in a no inform mode.
 * @param board the name of the board to check
 * @return 1 if it is no inform, 0 if not
 */
int query_no_inform(string board) {
  return priv[board] & B_PRIV_NO_INFORM;
} /* query_no_inform() */

/**
 * This method returns the current privilage level of the board in
 * question.  This should be used for testing only.
 * @return the current privilage level
 */
int query_privilage(string board) {
   return priv[board];
} /* query_privilage() */

/**
 * This method runs through all the messages and expires any which are
 * too old.
 */
void expire_boards() {
  string nam;
  int tim, num, *val;

  foreach (nam, val in timeouts) {
    num = 0;
    if ((tim = val[T_TIMEOUT]) &&
        (sizeof(boards[nam]) > val[T_MIN]) &&
        ((boards[nam][0][B_TIME]+(tim*(24*60*60))) < time())) {
          /* Got to delete some...  */
      while (sizeof(boards[nam]) > val[T_MIN] &&
             boards[nam][0][B_TIME]+(tim*24*60*60) < time()) {
        zap_message(nam, 0, 0);
        num++;
      }
      user_event( "inform", sprintf("Board handler removes %d messages "
                                       "from %s", num, nam), "message");
    }
  }
  if (find_call_out("expire_boards") == -1) {
    call_out("expire_boards", 60*60);
  }
} /* expire_boards() */

/** Prevents the object from being shadowed. */
int query_prevent_shadow() {
  return 1;
} /* query_prevent_shadow() */

/** The current max board number. */
int query_num() {
  return num;
} /* query_num() */

mixed *stats() {
  if(!total_reads)
    total_reads = 1;
  if(!cache_hits)
    cache_hits = 1;
  if(!ram_hits)
    ram_hits = 1;
  return  ({
    ({ "messages read", total_reads, }),
#ifdef USE_CACHE      
      ({ "cache hit percent", (cache_hits * 100) / total_reads, }),
#endif      
      ({ "ram hit percent", (ram_hits * 100) / total_reads, }),
      ({ "messages in cache", sizeof(keys(message_cache)), }),
      ({ "newsrc reads", _newsrc_reads, }),
      ({ "newsrc hit percent", (_newsrc_cache_hits * 100) / _newsrc_reads, }),
        ({ "newsrcs in cache", sizeof(keys(_newsrc_cache)), }),
          });
}

void query_cache() {
    printf("%O\n", sort_array(keys(message_cache), 0));
}

/** @ignore yes */
int load_newsrc(string player) {
  string fname, board;

  _newsrc_reads++;

  if(_newsrc_cache[player]) {
    _newsrc_cache_hits++;
    return 1;
  }

  fname = NEWSRC_SAVE_DIR+player[0..0]+"/"+player+ ".o";

  if(unguarded( (: file_size($(fname)) :)) > 0) {
    _newsrc_cache[player] =
      unguarded((: restore_variable(read_file($(fname))) :));
    if (arrayp(_newsrc_cache[player]->newsrc)) {
      _newsrc_cache[player]->newsrc = ([ ]);
      if (find_call_out("flush_newsrc") == -1) {
        call_out("flush_newsrc", NEWSRC_SAVE_DELAY);
      }
    }
    return 1;
  } else {
    _newsrc_cache[player] = new(class newsrc,
                                cached : time(),
                                kill : ({ }),
                                newsrc : ([ ]),
                                board_order : ({ }));
    if(PLAYER_HANDLER->test_property(player, NEWS_RC))
      _newsrc_cache[player]->newsrc =
        PLAYER_HANDLER->test_property(player, NEWS_RC);
    if(PLAYER_HANDLER->test_property(player, BOARD_ORDER))
      _newsrc_cache[player]->board_order =
        PLAYER_HANDLER->test_property(player, BOARD_ORDER);

    foreach(board in keys(boards)) {
      if(PLAYER_HANDLER->test_property(player, "news_kill_"+board)) {
        _newsrc_cache[player]->kill += ({ board });
      }
    }
    _newsrc_cache[player]->changed = time();
    if(find_call_out("flush_newsrc") == -1)
      call_out("flush_newsrc", NEWSRC_SAVE_DELAY);

    return 1;
  }

  return 0;
}

/** @ignore yes */
void flush_newsrc(int force) {
  string fname, player, board, *board_list;
  object ob;

  board_list = map(keys(boards), (: lower_case($1) :));

  foreach(player in keys(_newsrc_cache)) {
    if(!force && (!_newsrc_cache[player]->changed ||
       (_newsrc_cache[player]->changed > time() - NEWSRC_SAVE_DELAY &&
        !find_object("/obj/shut"))))
      continue;

    fname = NEWSRC_SAVE_DIR + player[0..0] + "/" + player + ".o";
    // save their file.
    if(unguarded( (: file_size($(fname)) :)) > 0) {
      _newsrc_cache[player]->newsrc =  
        filter(_newsrc_cache[player]->newsrc, 
               (: member_array(lower_case($1), $(board_list)) != -1 :));
      if(_newsrc_cache[player]->kill)
        _newsrc_cache[player]->kill =  
          filter(_newsrc_cache[player]->kill, 
                 (: member_array(lower_case($1), $(board_list)) != -1 :));
      if(_newsrc_cache[player]->board_order)
        _newsrc_cache[player]->board_order =  
          filter(_newsrc_cache[player]->board_order, 
                 (: member_array(lower_case($1), $(board_list)) != -1 :));
      
      unguarded( (: write_file($(fname), save_variable(_newsrc_cache[$(player)]),
                               1) :));
    } else if(ob = find_player(player)) {
      // if they're around and don't have a file then write a new one
      // and clear their properties.
      unguarded( (: write_file($(fname),
                               save_variable(_newsrc_cache[$(player)]), 1) :));
      ob->remove_property(NEWS_RC);
      ob->remove_property(BOARD_ORDER);
      foreach(board in keys(boards)) {
        ob->remove_property("news_kill_" + lower_case(board));
      }
    } else {
      // they're not around and they don't have a file so just update their
      // properties.
      PLAYER_HANDLER->special_add_property(player, NEWS_RC, _newsrc_cache[player]->newsrc);
    }
    _newsrc_cache[player]->changed = 0;
  }
}

/**
 * Get someones newsrc
 *
 * @param string the name of the player
 * @return mapping their newsrc
 */
mapping query_newsrc(string player) {
  if(!load_newsrc(player))
    return ([ ]);

  _newsrc_cache[player]->cached = time();

  return _newsrc_cache[player]->newsrc;
}

/**
 * Set someones newsrc
 *
 * @param string the name of the player
 * @param mapping their new newsrc
 * @return 1 for success, 0 for failure
 */
int set_newsrc(string player, mapping newsrc) {
  if(!load_newsrc(player))
    return 0;

  _newsrc_cache[player]->newsrc = (mapping)newsrc;
  _newsrc_cache[player]->cached = time();
  _newsrc_cache[player]->changed = time();

  if(find_call_out("flush_newsrc") == -1)
    call_out("flush_newsrc", NEWSRC_SAVE_DELAY);

  return 1;
}

/**
 * Find out if a given board is in a player killfile
 *
 * @param string the name of the player
 * @param string the name of the board (in lowercase)
 * @return 1 if it is, 0 if it isn't
 */
int query_killfile(string player, string board) {
  if(!load_newsrc(player))
    return 0;

  _newsrc_cache[player]->cached = time();
  return member_array(board, _newsrc_cache[player]->kill) != -1;
}

/**
 * Add a board to someones killfile
 *
 * @param string the name of the player
 * @param string the name of the board (in lowercase)
 * @return 1 for success, 0 for failure
 */
int set_killfile(string player, string board) {

  if(!load_newsrc(player))
    return 0;

  if(member_array(board, _newsrc_cache[player]->kill) != -1)
    return 1;

  _newsrc_cache[player]->kill += ({ board });
  _newsrc_cache[player]->changed = time();
  _newsrc_cache[player]->cached = time();

  if(find_call_out("flush_newsrc") == -1)
    call_out("flush_newsrc", NEWSRC_SAVE_DELAY);

  return 1;
}

int remove_killfile(string player, string board) {

  if(!load_newsrc(player))
    return 0;

  if(member_array(board, _newsrc_cache[player]->kill) == -1)
    return 1;

  _newsrc_cache[player]->kill -= ({ board });
  _newsrc_cache[player]->changed = time();
  _newsrc_cache[player]->cached = time();

  if(find_call_out("flush_newsrc") == -1)
    call_out("flush_newsrc", NEWSRC_SAVE_DELAY);

  return 1;
}

/**
 * Retrieve a players killfile list
 *
 * @param string the players name
 * @return string * the list of boards in their killfile
 */
string *list_killfile(string player) {

  if(!load_newsrc(player))
    return ({ });

  return _newsrc_cache[player]->kill;
}

/**
 * Retrieve someones chosen board order.
 *
 * @param string the players name
 * @return the list of boards, in order
 */
string *query_board_order(string player) {
  if(!load_newsrc(player))
    return ({ });

  _newsrc_cache[player]->cached = time();
  if(!_newsrc_cache[player]->board_order)
    return ({ });

  return _newsrc_cache[player]->board_order;
}

/**
 * Set the order boards should be shown in for a given player.
 *
 * @param string a players name
 * @param string * the list of boards
 * @return 1 for success, 0 for failure
 */
int set_board_order(string player, string *new_order) {

  if(!load_newsrc(player))
    return 0;

  _newsrc_cache[player]->board_order = new_order;
  _newsrc_cache[player]->changed = time();
  _newsrc_cache[player]->cached = time();

  if(find_call_out("flush_newsrc") == -1)
    call_out("flush_newsrc", NEWSRC_SAVE_DELAY);

  return 1;
}

/** @ignore yes */
void dest_me() {
  flush_newsrc(1);
  destruct(this_object());

}
