/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: mysql.c,v 1.15 2003/02/19 21:02:07 pinkfish Exp $
 * $Log: mysql.c,v $
 * Revision 1.15  2003/02/19 21:02:07  pinkfish
 * Make better end of packet detection things.
 *
 * Revision 1.14  2003/02/07 02:46:03  pinkfish
 * Fix up some more issues with mysql and newlines.
 *
 * Revision 1.13  2003/02/07 02:07:08  pinkfish
 * Fix up some errors in how it handles the return of sql queries.
 *
 * Revision 1.12  2001/07/26 18:17:57  pinkfish
 * Working fine.
 *
 * Revision 1.11  2001/03/13 04:37:56  ceres
 * Fixed runtime
 *
 * Revision 1.10  2001/03/13 01:03:19  ceres
 * Fixed runtime
 *
 * Revision 1.9  2001/03/11 18:33:38  pinkfish
 * Fix p a runtime that was causing errors in the sql handling.
 *
 * Revision 1.8  2001/03/09 21:15:00  pinkfish
 * Fix up the fix.
 *
 * Revision 1.7  2001/03/09 21:01:14  pinkfish
 * Fix up issues with two many sql reuqests in a row.
 *
 * Revision 1.6  2001/03/09 20:51:51  pinkfish
 * Add in some testing stuff.
 *
 * Revision 1.5  2001/03/07 20:16:15  pinkfish
 * Add in an example and fix some small issues.
 *
 * Revision 1.4  2000/07/14 03:54:50  pinkfish
 * Change it to work over multiple chunk sends.
 *
 * Revision 1.3  2000/07/14 03:10:42  pinkfish
 * Add in some code to do debugging.
 *
 * Revision 1.2  2000/06/21 20:37:17  pinkfish
 * Fix up a biunch of things.
 *
 * Revision 1.1  2000/06/20 05:10:02  pinkfish
 * Initial revision
 *
 * Revision 1.1  1998/01/06 05:17:08  ceres
 * Initial revision
 * 
*/
/**
 * This object handles all the mysql requests for the system.
 * It connects to an external python script todo the actual mysql processing,
 * it is done this way to make the sql requests non-blocking so they do not
 * slow down the mud.
 * @author Pinkfish
 * @started Tue Jun 20 14:29:01 PDT 2000
 */
#include <network.h>

inherit CLIENT;

#include <db.h>

private int _fd;
private int _currentId;
private int _callout_id;
private mapping _requests;
private string data;

protected void setup() {
   int id;
   function func;

   _fd = eventCreateSocket("127.0.0.1", 7865);
   if (_fd < 0) {
      // This has been commented out for my sanity
      //printf("Bad fd.\n");
      remove_call_out(_callout_id);
      _callout_id = call_out("setup", 10);
      //
      // Send back an error to anyone still waiting.
      //
      foreach (id, func in _requests) {
         map_delete(_requests, id);
         evaluate(func, DB_ERROR_BAD_SOCKET, socket_error(_fd));
      }
   }
} /* setup() */

void create() {
   ::create();
   _fd = -1;
   _requests = ([ ]);
   data = "";
   setup();
} /* create() */

private void locate_value(string req, int newline) {
   mixed junk;
   function fluff;
   int index;
   int new_index;
   string test_data;
   string check_data;
	
   if(!req)
     return;

   test_data = data + req;
   index = -1;
   do {
      new_index = strsrch(test_data[index + 1..], "SQL_END_QUERY");
//tell_creator("pinkfish", "Chunk (%O %O %O)\n", test_data[index+1..50], new_index, index);
      // Not found the end marker.
      if (new_index == -1) {
         if (newline) {
            data += req + "\n";
         } else {
            data += req;
         }
         return ;
      }
      index += new_index + 1;
   } while (test_data[index - 1] == '+');
   check_data = test_data[0..index - 1];
   data = test_data[index + 13..];
//tell_creator("pinkfish", "Data: %O %O\n", check_data, data);
   catch(junk = restore_variable(check_data));
   if (pointerp(junk) && sizeof(junk)) {
//tell_creator("pinkfish", "Resolved...\n");
      fluff = _requests[junk[0]];
      map_delete(_requests, junk[0]);
      if (fluff) {
//tell_creator("pinkfish", "%O: %O %O\n", data + req, junk, fluff);
         evaluate(fluff, junk[1], junk[2]);
      }
   }
} /* locate_value() */

/** @ignore yes */
void eventRead(int fd, string str) {
   string req;
   string* bits;

   if (strlen(str) == 1 && str[<1] == '\n') {
      locate_value("", 1);
      return ;
   }
   bits = explode(str, "\n");
//tell_creator("pinkfish", "Stuff (%O)\n", str);
   foreach (req in bits[0..<2]) {
      locate_value(req, 1);
   }
   locate_value(bits[<1], 0);
   //data += bits[<1];
   //if (str[<1] == '\n') {
      //locate_value("");
   //}
} /* eventRead() */

/** @ignore yes */
protected void eventNewConnection(int fd) {
} /* eventNewConnection() */

/** @ignore yes */
protected void eventSocketClosed(int fd) {
  _fd = -1;
  remove_call_out(_callout_id);
  _callout_id = call_out("setup", 10);
} /* eventSocketClosed() */

/** @ignore yes */
protected void eventSocketError(string str, int x) {
  //tell_creator("pinkfish", str + " -- " + socket_error(x) + " -- " + x + "\n");
  _fd = -1;
  remove_call_out(_callout_id);
  _callout_id = call_out("setup", 10);
}

/** @ignore yes */
protected void eventSocketClose(int fd) {
  _fd = -1;
  remove_call_out(_callout_id);
  _callout_id = call_out("setup", 10);
} /* eventSocketClose() */

/**
 * This is the method you call to make an sql request.  You pass in the
 * database, user and password you wish to use to connect to the
 * sql sever.  You also pass in the request you wish to make and the call
 * back function to call.
 * <p>
 * The call back function will be passed two arguements, they are a
 * 'type' and a 'data' arguement.  The type will always be an integer and
 * it registers the success or failure of the database query.  The
 * data will either be a string (in the case of an error) or it will be
 * teh returned data (in the case of request).  The returned data will
 * be an array of mappings, the mappings contain keys of the field type
 * and the data value being the returned data.
 * <p>
 * The format of the function call is:<br>
 * void return_function(int status, mapping data)
 * @example
 * void finish_request(int type, mixed* data, object person) {
 *    string ret;
 *    mapping row;
 * 
 *    if (type == DB_SUCCESS) {
 *       ret = "";
 *       foreach (row in data) {
 *          ret += sprintf("%-15s %s\n", row["Fixer"], "" + row["bing"]);
 *       }
 *    } else {
 *       ret = "Some sort of horrible error!\n";
 *    }
 *    person->more_string(ret, "details");
 * }
 *
 * DB_HANDLER->make_sql_request("errors", CONFIG_DB_USER, "", request,
 *                             (: finish_request($1, $2, $(this_player())) :));
 * @param db the database to connect to
 * @param user the user to use
 * @param pass the password to use
 * @param request the request to make
 * @param finish the call back function
 */
void make_sql_request(string db, string user, string pass, string request,
                      function finish) {
   mixed *stuff;

   if (!functionp(finish)) {
      printf("finish arguement to make_sql_request is not a function.\n");
      return ;
   }

   _requests[_currentId] = finish;
   if (_fd == -1) {
      setup();
   }
   if (_fd == -1) {
      return ;
   }
   stuff = ({ _currentId++, user, pass, db, request });
   eventWrite(_fd, save_variable(stuff) + "\n");
} /* make_sql_request() */

/**
 * This will do exciting things when we dest.
 * @ignore yes
 */
void dest_me() {
   int id;
   function func;

   foreach (id, func in _requests) {
      map_delete(_requests, id);
      catch(evaluate(func, DB_ERROR_BAD_SOCKET, "the server was dested."));
   }
   ::dest_me();
} /* dest_me() */

/** @ignore yes */
mapping query_requests() {
   return copy(_requests);
} /* query_requests() */
