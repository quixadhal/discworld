/**
 * MUD based dictionary, conforms to the DICT new protocol found in RFC 2229. 
 * See ftp://ftp.isi.edu/in-notes/rfc2229.txt for more information.
 * @author Taffyd@Discworld
 * @changes 5-2-2001 Pinkfish
 * Converted to work as a handler.
 */

#include <network.h>

// #define DNS_LOOKUP

#define DICT_SERVER_NAME "dict.org"
#define DICT_SERVER_IP "66.111.36.30"

#define DISCONNECTED 0
#define WAITING_FOR_CONNECT 1
#define READY 2
#define RECEIVING 3
#define SENDING_DEFINE 4
#define SENDING_MATCH 5
#define WAITING_FOR_DNS 6

#define INACTIVE 0
#define DEFINE 1
#define MATCH 2

#define SEARCH_MODE_ALL "*"
#define DEFAULT_SEARCH_MODE "web1913"

#define DEBUGGER "pinkfish"
//#define DEBUGGER ""

/**
 * This class handles connection details for each person using
 * the dictionary. 
 * @member fd the file descriptor of the socket currently being
 * used by the dictionary.
 * member dns_key the resolve() key used when doing a nslookup.
 */

   // string *dictionaries;
   // string *capabilities;

class request {
   string word;
   string search_mode;
   int type;
   int i;
   function callback;
}

class connection_data {
   int fd;
   int dns_key;
      
   string message;
   mixed definitions;
   mixed *working;

   string ip_address;

   int status;
   int count;
   string search_mode;    
   class request current_request;
}

inherit CLIENT;

private class connection_data _connection;
private class request* _requests;

void remove_request(class request bing);
void close_connection();
void inform_of_problem(class request bing, string str);
void send_request(class request bing);
void make_connection();
void check_send();

void create() {
   client::create();
   SetSocketType(STREAM);
   _requests = ({ });
} /* create() */

void close_socket( int fd, object who ) {
   if ( _connection->fd ) {
      eventWrite(fd, "QUIT\r\n" );
      eventAbortCallback( fd );
      close_connection();
   }
} /* close_socket() */

void do_callback(function callb, string word, mixed result, mixed* data) {
   call_out((: evaluate($1, $2, $3, $4) :), 0, callb, word, result, data);
} /* do_callback() */

void eventRead(int fd, string message) {
   class connection_data dict;
   string *bits;
   string *lines;
   string line;
   string word;
   string extra;
   string dictionary_name;
   mixed *bing;
   mixed *fluff;
   int i;
   int j;
   int ignore;

   //tell_creator(DEBUGGER, "Got %O\n", message);
   /* Check to see if the message is valid. */          
   if ( !stringp( message ) ) {
      return;
   }
   
   /* Find the user for this connection */
   dict = _connection;
   lines = explode( message, "\r\n" );
   
   foreach ( line in lines ) {
      if ( !sizeof( line ) ) {
         line = "\n";
      }
      if ( line == "." ) {
         dict->definitions += ({ dict->working });
         dict->count = -1;
         continue;
      }
            
      bits = explode( line, " " );
      if ( !sizeof( bits ) ) {
         continue;
      }

      //tell_creator(DEBUGGER, sprintf("Event %s received in status %d\n[%s]\n",
                                      //bits[0], dict->status, line));

      switch ( bits[ 0 ] ) {
      case "150":
         dict->count++;
         dict->working = ({ });
         dict->status = RECEIVING;
         word = 0;
         extra = 0;
         dictionary_name = 0;
         break;
         
      case "151":                                           
         if ( dict->status == RECEIVING && 
                ( sscanf( line, "151 \"%s\" %s \"%s\"", word, extra, 
                               dictionary_name ) == 3 ) ) {
            dict->working += ({ ({ word, dictionary_name, extra }) });
         } else {
            //tell_creator( DEBUGGER, "Invalid 151 error code, %s, %s, %s.\n",
                                 //word, extra, dictionary_name );
         }
         break;
         
      case "152":
         dict->count++;
         dict->working = ({ });
         dict->status = RECEIVING;
         word = 0;
         extra = 0;
         dictionary_name = 0;
         break;

      case "220":
         if (dict->status == WAITING_FOR_CONNECT) {
            dict->status = READY;
            check_send();
         }
/*
         tell_object( who, "The dictionary's pages shuffle about. It is now "
            "ready for a query.\n" );
 */

         // dict->capabilities = explode( bits[ <2 ][1..<2], "." );
         break;
                  
      case "250":
//tell_creator(DEBUGGER, "Start...\n");
         switch(dict->current_request->type) {
         case DEFINE:
            bing = ({ });
            for ( i = 0; i < sizeof( dict->definitions ); i++ ) {
               foreach( fluff in dict->definitions[ i ] ) {
                  fluff[2] = replace( fluff[2], 
                                      ({ "    ", " ", "\t", " " }) );
                  ignore = 0;
                  for (j = 0; j < sizeof(bing); j++) {
                     if (bing[j][2] == fluff[2]) {
                        ignore = 1;
                     }
                  }
                  if (!ignore) {
                     bing += ({ fluff });
                  }
               }
            }
//tell_creator(DEBUGGER, "%O %O\n", bing, dict->definitions);
            if (dict->current_request->callback) {
               do_callback(dict->current_request->callback,
                        dict->current_request->word,
                        NETWORK_SUCCESS,
                        bing);
            }
            dict->status = READY;                        
            remove_request(dict->current_request);
                     
            break;
         case MATCH:
            if (dict->current_request->callback) {
               bing = ({ });
               for(i=0; i<sizeof(dict->definitions); i++) {
                  foreach(line in dict->definitions[i]) {
                     if(sscanf(line, "%s \"%s\"", extra, word) == 2) {
                        bing += ({ ({ extra, word }) });
                     }
                  }
               }
//tell_creator(DEBUGGER, "+++ %O\n", dict);
               do_callback(dict->current_request->callback,
                        dict->current_request->word,
                        NETWORK_SUCCESS,
                        bing);
            }
            dict->status = READY;                        
            remove_request(dict->current_request);
         }
//tell_creator(DEBUGGER, "--- Fallout!\n");
         return;
         break;
         
      case "530":
         inform_of_problem(dict->current_request, "Error from server.");
         dict->status = READY;                        
         remove_request(dict->current_request);
         //close_socket( fd, who );
         return;
         break;
         
      case "552":
         if (dict->current_request->callback) {
            do_callback(dict->current_request->callback,
                     dict->current_request->word,
                     NETWORK_SUCCESS,
                     ({ }));
         }
         dict->status = READY;                        
         remove_request(dict->current_request);
         return;
      break;
         
      default:
         if ( dict->status == RECEIVING ) {
            switch(dict->current_request->type) {
            case DEFINE:
               dict->working[<1][<1] += line;
               break;
            case MATCH:
               dict->working += ({ line });
            }
         } else
            //tell_creator(DEBUGGER, "Line not added, status " +
                               //dict->status + "\n");
         break;
      }
   }
} /* eventRead() */

void got_ip_address( string address, string ip_address, int key ) {
    tell_creator( "taffyd", "%s, %s\n", address, ip_address );

   //tell_creator(DEBUGGER, "Ig? %O %O %O %O\n", key, _connection, ip_address, address);
/*
   if ( _connection->dns_key != key ) {
       return ;
   }
 */
   
   if ( !address || !ip_address ) {
      ip_address = "127.0.0.1";
      //inform_of_problem(0, "Unable to lookup dns address.");
      //return;
   }

   //tell_creator(DEBUGGER, "Setup ip.\n");
   _connection->ip_address = ip_address;

   make_connection();
} /* got_ip_address() */

void make_connection() {
   int key;
   int new_fd;

   //tell_creator(DEBUGGER, "Making connection.\n");
   if (!_connection || !_connection->fd) {
       if (!_connection || !_connection->ip_address) {

          _connection = new(class connection_data, fd : 0, 
                   status : WAITING_FOR_DNS, count : -1, definitions : ({ }),
                   dns_key : key );
#ifdef DNS_LOOKUP
          _connection->dns_key = resolve( DICT_SERVER_NAME, "got_ip_address" );
#else
          got_ip_address( DICT_SERVER_NAME, DICT_SERVER_IP, 0 );
#endif
       } else {
          new_fd = eventCreateSocket( _connection->ip_address, 2628 );

          if ( new_fd < 0 ) {
             inform_of_problem(0, "unable to connect");
             return;
          }
       
          _connection->fd = new_fd;
          _connection->status = WAITING_FOR_CONNECT;
   
          call_out( "close_socket", 120, new_fd, this_player() );
       }
   }
} /* make_connection() */

void close_connection() {
   int fd;

   if (_connection && _connection->fd) {
      fd = _connection->fd;
      _connection->fd = 0;
      close_socket( fd, this_player() );
      inform_of_problem(0, "socket closed");
   }
} /* close_connection() */

void check_send() {
//tell_creator(DEBUGGER, "%O\n", _connection);
   if (!_connection || _connection->fd == 0) {
      make_connection();
   } else if (_connection->status == READY && sizeof(_requests)) {
      send_request(_requests[0]);
   }
} /* check_send() */

void send_request(class request bing) {
   //tell_creator(DEBUGGER, "Sending request %O\n", bing);
   switch (bing->type) {
   case DEFINE :
      eventWrite(_connection->fd,
                 sprintf("DEFINE %s %s\r\n", bing->search_mode, 
                                              bing->word));
      _connection->status = SENDING_DEFINE;
      break;
   case MATCH :
      eventWrite(_connection->fd, sprintf("MATCH %s . %s\r\n", bing->search_mode, 
                                              bing->word));
      _connection->status = SENDING_MATCH;
      break;
   }
   _connection->current_request = bing;
   _connection->definitions = ({ });
} /* send_request() */

void remove_request(class request bing) {
   int i;

   for (i = 0; i < sizeof(_requests); i++) {
      if (_requests[i] == bing) {
         _requests = _requests[0..i - 1] + _requests[i + 1..];
         break;
      }
   }
   _connection->definitions = ({ });

   check_send();
} /* remove_request() */

void add_request(class request bing) {
   //tell_creator(DEBUGGER, "Adding request %O\n", bing);
   _requests += ({ bing });

   check_send();
} /* add_request() */

int define_word(string word, int all, function call_back) {
   class request request;

   request = new(class request);
   request->word = word;
   if (all) {
      request->search_mode = SEARCH_MODE_ALL;
   } else {
      request->search_mode = DEFAULT_SEARCH_MODE;      
   }
   request->type = DEFINE;
   request->callback = call_back;

/*
   eventWrite(dict->fd, sprintf("DEFINE %s %s\r\n", dict->search_mode, 
                                              dict->word));
*/
   add_request(request);
} /* define_word() */

int spell_word(string word, int all, function call_back) {
   class request request;

   request = new(class request);
   request->word = word;
   if (all) {
      request->search_mode = SEARCH_MODE_ALL;
   } else {
      request->search_mode = DEFAULT_SEARCH_MODE;      
   }
   request->type = MATCH;
   request->callback = call_back;

/*
   eventWrite(dict->fd, sprintf("MATCH %s . %s\r\n", dict->search_mode, 
                                              dict->current_word));
 */
   add_request(request);
}

void inform_of_problem(class request bing, string mess) {
   if (bing) {
      do_callback(bing->callback, bing->word, mess, ({ }));
      remove_request(bing);
   } else {
      foreach (bing in _requests) {
         do_callback(bing->callback, bing->word, mess, ({ }));
      }
      _requests = ({ });
   }
} /* inform_of_problem() */

void dest_me() {
   close_connection();
   client::dest_me();
} /* dest_me() */
