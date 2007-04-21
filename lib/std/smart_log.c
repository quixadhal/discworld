/* $Id: smart_log.c,v 1.14 2003/04/23 19:39:57 pinkfish Exp pinkfish $ */

#include <log.h>
#include <db.h>
#include <config.h>
#include <playtesters.h>

/* Define this to use the old ERROR_REPORTS file system */
#undef OLD_SYSTEM

/* Define this to use the new SQL based errors system */
#define NEW_SYSTEM

//int db_fd;

void create() {
   //catch(db_fd = db_connect("localhost", "errors", CONFIG_DB_USER));
}

void smart_log(string full_type, string reporter, string text,
                      string trace, string file) {
   int i;
   string type, temp1, *j, *person, dir;
#ifdef OLD_SYSTEM
   string temp2, report;
#endif
#ifdef NEW_SYSTEM
   string query, name, category;
   int rtime;
#endif
    
   seteuid("Root");
   person = ({ });

   person = master()->query_assigned_to_directory(file);

   j = explode(file, "/") - ({ "" });
   dir = "/" + implode(j[0..<2], "/");
   // Normalise the file name.
   file = implode(j, "/");
   sscanf(full_type, "%s %s", category, full_type);
   if (!sscanf(full_type, "%s %s", type, name)) {
      type = full_type;
      name = "";
   }
   rtime = time();
   if (sizeof(person) > 0) {
      query = sprintf("INSERT INTO errors (EntryDate, DirEntryDate, "
                           "Directory, Filename, Category, Type, Name, Reporter, "
                           "Report, Runtime, AssignedTo) VALUES(%d, %d, '%s', '/%s', '%s', '%s', "
                           "'%s', '%s', '%s', '%s', '%s');", rtime, rtime, dir,
                           db_escape(file), category, type, db_escape(name), reporter,
                           db_escape(text), (trace ? db_escape(trace) : ""),
                           db_escape(person[0]));
   } else {
      query = sprintf("INSERT INTO errors (EntryDate, DirEntryDate, "
                           "Directory, Filename, Category, Type, Name, Reporter, "
                           "Report, Runtime) VALUES(%d, %d, '%s', '/%s', '%s', '%s', "
                           "'%s', '%s', '%s', '%s');", rtime, rtime, dir,
                           db_escape(file), category, type, db_escape(name), reporter,
                           db_escape(text), (trace ? db_escape(trace) : ""));
   }
   DB_HANDLER->make_sql_request("errors", CONFIG_DB_USER, "",
                                query, (: tell_creator("pinkfish", "%O %O\n", $1, $2) :));

   PLAYTESTER_HAND->report_made(reporter, type, file,
                               (sizeof(person) > 0 ?
                                 "Assigned to " + capitalize(person[0]) + "\n\n" : "Not assigned to anyone.\n\n" ) +
                               text + ( trace
                                       ? "\nError:\n" + trace : ""));
  
   //catch(db_exec(db_fd, query));

   j = explode( file, "/" );
   file = "/"+ implode( j[ 0 .. <2 ], "/" );
   for(i=0;i<sizeof(person);i++) {
      temp1 = read_file("/w/"+person[i]+"/"+PLAYER_ERROR_LOG);
      if (!temp1) {
         temp1 = "";
      }
      j = explode(temp1, "\n");
      if (!j) {
         j = ({ });
      }
      if (member_array(file, j) == -1) {
         j += ({ file });
      }
      temp1 = implode(j, "\n")+ "\n";
      master()->do_log(person[i], temp1);
   }
} /* smart_log()*/

void dest_me() {
   //catch(db_close(db_fd));
   destruct(this_object());
}

void clean_up() { dest_me(); }

void reset() { dest_me(); }
