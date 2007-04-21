/**
 * This handler will do any mud base manipulations of the twiki
 * database.
 * @author Pinkfish
 * @started Wed May 24 17:19:06 PDT 2000
 */
#include <board.h>

void do_check_changes();

#define SAVE_FILE "/save/twiki"
#define CHECK_TIME (7 * 24 * 60 * 60)

#define TWIKI_CHANGE_FILE "/.changes"
#define TWIKI_DATA_DIRECTORY "/twiki/data/"

private nosave string* _databases;

private int _last_check;
private mapping _last_change;

void create() {
   _databases = ({ "Main", "Am", "Ram", "Cwc", "Sur", "Klatch",
                   "Forn", "Underworld", "Other", "Playtesters" });
   seteuid(getuid());
   restore_object(SAVE_FILE);
   if (_last_check + CHECK_TIME < time()) {
      do_check_changes();
   }
   call_out("do_check_changes", _last_check + CHECK_TIME - time());

   if (!_last_change) {
      _last_change = ([ ]);
   }
   call_out("do_change_inform", 60 * 5);
} /* setup() */

/**
 * This method finds the pages that have changed since the specified time.
 * @param limit the time after which to find the changes
 * @return the mapping of pages to who changed them
 */
mapping find_changed_pages(int limit, string database) {
   string* lines;
   int i;
   int tim;
   string page;
   string author;
   mixed* bits;
   mapping changes_pages;

   lines = explode(read_file(TWIKI_DATA_DIRECTORY + database + TWIKI_CHANGE_FILE), "\n");
   changes_pages = ([ ]);
   for (i = 1; i < sizeof(lines); i++) {
      bits = reg_assoc(lines[<i], ({ "([a-zA-Z][a-zA-Z0-9]+)", "[0-9]+" }), ({ 1, 2 }) );
      if (sizeof(bits[0]) == 9 || sizeof(bits[0]) == 7) {
         page = bits[0][1];
         author = bits[0][3];
         sscanf(bits[0][5], "%d", tim);
         if (tim > limit) {
            //
            // Ignore all the user page changes...
            //
            if (page[0..4] != "TWiki" &&
                page[0..9] != "Underworld" &&
                author != "PeterThoeny" &&
                page != author &&
                page != "WebPreferences") {
               //
               // Read the page and determine from that.
               //
               if (file_size("/twiki/data/" + database + "/" + page + ".txt") > 0 &&
                   strsrch(read_file("/twiki/data/" + database + "/" + page + ".txt"),
                           "\t* Login Name:") == -1) {
                  if (!changes_pages[page]) {
                     changes_pages[page] = ({ });
                  }
                  if (member_array(author, changes_pages[page]) == -1) {
                     changes_pages[page] += ({ author });
                  }
               }
            }
         } else {
            break;
         }
      } else {
         break;
      }
   }

   return changes_pages;
} /* find_changed_pages() */

/**
 * This method prints out any changes since the specified limit.
 * @param limit the time after which to show any changes
 * @return the changes since that time
 */
string find_changes(int limit) {
   mapping changes_pages;
   string ret;
   string page;
   string database;

   ret = "Changes since " + ctime(limit) + " in\n"
         "http://discworld.imaginary.com/twiki/bin/view/Main/WebHome\n\n";

   foreach (database in _databases) {
      changes_pages = find_changed_pages(limit, database);

      foreach (page in sort_array(keys(changes_pages), 1)) {
         ret += sprintf("%-30s changed by %s\n", database + "." + page,
                         query_multiple_short(changes_pages[page]));
      }
   }
   return ret;
} /* find_changes() */

/**
 * This method prints out any changes since the specified limit.
 * @param database the database to find the changes in
 * @param limit the time after which to show any changes
 * @return the changes since that time
 */
string find_changes_in_database(string database, int limit) {
   mapping changes_pages;
   string ret;
   string page;

   if (database == "Playtesters") {
      ret = "Changes since " + ctime(limit) + " in\n"
         "http://discworld.imaginary.com/twiki/pt/bin/view/Playtesters/WebHome\n\n";
   } else {
      ret = "Changes since " + ctime(limit) + " in\n"
         "http://discworld.imaginary.com/twiki/bin/view/Main/WebHome\n\n";
   }

   changes_pages = find_changed_pages(limit, database);

   foreach (page in sort_array(keys(changes_pages), 1)) {
      ret += sprintf("%-30s changed by %s\n", database + "." + page,
                      query_multiple_short(changes_pages[page]));
   }
   return ret;
} /* find_changes() */

/**
 * This method finds the specified file and shows it to us.
 * @param fname the file name to find
 * @return the text of the file, 0 if the file is not found
 */
string query_file(string fname) {
   if (strsrch(fname, ".") > 0) {
      fname = "/twiki/data/" + replace_string(fname, ".", "/") + ".txt";
   } else {
      fname = "/twiki/data/Main/" + fname + ".txt";
   }

   if (sizeof(stat(fname))) {
      return read_file(fname);
   }
   return 0;
} /* query_file() */

/** @ignore yes */
void do_check_changes() {
   string changes;

   changes = find_changes(_last_check);

   BOARD_HAND->add_message("commonroom", "Twiki Database",
                           "Changes in the Twiki database",
                           changes);
   _last_check = time();
   save_object(SAVE_FILE);
   call_out("do_check_changes", _last_check + CHECK_TIME - time());
} /* do_changes() */

/** @ignore yes */
void do_change_inform() {
   int changed;
   mapping changed_pages;
   string page;
   string* changers;
   int dosave;
   string database;

   //
   // See if the file has changed.
   //
   if (!mapp(_last_change)) {
      _last_change = ([ ]);
   }
   foreach (database in _databases) {
      if (!_last_change[database]) {
         _last_change[database] = time();
         dosave = 1;
      } else {
         changed = stat(TWIKI_DATA_DIRECTORY + database + TWIKI_CHANGE_FILE)[1];
         if (changed != _last_change[database]) {
            //
            // Do an inform about it...
            //
            changed_pages = find_changed_pages(_last_change[database], database);
            _last_change[database] = changed;
            dosave = 1;
            foreach (page, changers in changed_pages) {
               user_event( "inform",
                           sprintf( "%s changes the twiki page %s.%s",
                                    query_multiple_short(changers), database,
                                    page),
                           "message",
                           this_player() );
      
            }
         }
      }
   }
   if (dosave) {
      save_object(SAVE_FILE);
   }
   call_out("do_change_inform", 60 * 5);
} /* do_change_inform() */
