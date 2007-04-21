/**
 *  Check command
 *  @author Drakkos
 *  @started 18/07/2002 01:56
 *
 */


inherit "/cmds/base";
#include <playtesters.h>

string query_applicant_text();
string pt_protected();

varargs int cmd(string check, string name) {
  object ob = environment (this_player());
  int min, max;
  mixed *chats;
  mixed *chatsd, *chatsn;
  string day, night;
  string ret = "";
  int count;
  mixed *args, *args1; 

  if (ob->query_property ("restrict check")) {
    this_player()->add_failed_mess (this_object(), "The use of the check "
      "command in this room has been restricted.\n", ({ }));
    return 0;
  }
  
  switch (check) {
    case "chats": 
      args= ob->query_room_chats();  
      chatsd = ob->query_room_day_chats();
      chatsn = ob->query_room_night_chats();     
      if (!args && !sizeof (chatsd) && !sizeof (chatsn)) {
        tell_object (this_player(), "There are no chats defined here.\n");
        return 1;
      }
      
      if (args) {      
         min = args[0];
         max = args[1];
         if (!arrayp(args[2])) {
            ret += "%^BOLD%^%^RED%^Error in the room chats!%^RESET%^\n";
            chats = ({ });
         } else {
            chats = args[2];
         }
      } else {
         chats = ({ });
      }
      
      if (sizeof (chats)) {
        ret = "This room has the following chats, set to occur "
          "at the rate of one every " + min + " to " + max + " seconds:\n\n";
          
        for (int i = 0; i < sizeof (chats); i++) {
          if (!stringp(chats[i])) {
             ret += "%^BOLD%^%^RED%^Broken chat setup in room.%^RESET%^\n";
             break;
          }
          ret += (i + 1) + ". " + chats[i];
          ret += "\n";
        }
      }
            
      if (sizeof (chatsd)) {
        min = chatsd[0];
        max = chatsd[1];
        if (!arrayp(chatsd[2])) {
           ret += "%^BOLD%^%^RED%^Error in the room day chats!%^RESET%^\n";
           chats = ({ });
        } else {
           chats = chatsd[2];
        }
             
        ret += "\n\nThis room has the following day chats, set to occur "
          "at the rate of one every " + min + " to " + max + " seconds:\n\n";
          
        for (int i = 0; i < sizeof (chats); i++) {
          ret += (i + 1) + ". " + chats[i];
          ret += "\n";
        }
      }   
           
      if (sizeof (chatsn)) {
        min = chatsn[0];
        max = chatsn[1];
        if (!arrayp(chatsd[2])) {
           ret += "%^BOLD%^%^RED%^Error in the room night chats!%^RESET%^\n";
           chats = ({ });
        } else {
           chats = chatsn[2];
        }
             
        ret += "\n\nThis room has the following night chats, set to occur "
          "at the rate of one every " + min + " to " + max + " seconds:\n\n";
          
        for (int i = 0; i < sizeof (chats); i++) {
          ret += (i + 1) + ". " + chats[i];
          ret += "\n";
        }
      }  
        
    break;
    case "long":
      day = ob->query_day_long();
      night = ob->query_night_long();
        
      ret = "Day long description:\n\n" + day;
      ret += "\n\nNight long description:\n\n" + night;      
    break;
    case "items":
      ob = ob->query_item();
      
      if (!ob) {
        ret = "%^CYAN%^There are no add_items defined in this room.%^RESET%^\n";
      }
      else {
        args = ob->query_shrt();
        args1 = ob->query_lng();
        
        count = sizeof (args);

        ret += "%^YELLOW%^Main items:%^RESET%^\n";
        
        for (int i = 0; i < count; i++) {
          if (!sizeof (args[i])) {
            continue;
          }
          
          ret += "%^BOLD%^" + args[i] + "%^RESET%^: " + (functionp (args1[i]) ?  ("*" + evaluate (args1[i]) + "*") : args1[i]);
          ret += "\n\n";     
        }
        args = ob->query_day_items();
        if (!sizeof(args)) {
           ret += "%^CYAN%^There are no day items defined in this room.%^RESET%^\n";
        } else {
          ret += "%^YELLOW%^Day Items:%^RESET%^\n";
          for (int i = 0; i < sizeof (args) ; i=i+2) {
            ret += "%^BOLD%^" + args[i] + "%^RESET%^: " + (functionp (args[i+1]) ?  
              ("*" + evaluate (args[i+1]) + "*") : args[i+1]);
            ret += "\n\n";       
          }        
        }
        args = ob->query_night_items();
        if (!sizeof(args)) {
           ret += "%^CYAN%^There are no night items defined in this room.%^RESET%^\n";
        } else {
          ret += "%^YELLOW%^Night Items:%^RESET%^\n";
          for (int i = 0; i < sizeof (args) ; i=i+2) {
            ret += "%^BOLD%^" + args[i] + "%^RESET%^: " + (functionp (args[i+1]) ?  
              ("*" + evaluate (args[i+1]) + "*") : args[i+1]);
            ret += "\n\n";       
          }        
        }
      }
    break;
    case "day items":
      args = ob->query_day_items();
      if (!sizeof (args)) {
        ret += "There are no day items defined here.\n";
      }
      else {
        for (int i = 0; i < sizeof (args) ; i=i+2) {
          ret += args[i] + ": " + (functionp (args[i+1]) ?  
            ("*" + evaluate (args[i+1]) + "*") : args[i+1]);
          ret += "\n\n";       
        }        
      }
    break;    
    case "night items":
      args = ob->query_night_items();
      if (!sizeof (args)) {
        ret += "There are no night items defined here.\n";
      }
      else {
        for (int i = 0; i < sizeof (args) ; i=i+2) {
          ret += args[i] + ": " + (functionp (args[i+1]) ?  
            ("*" + evaluate (args[i+1]) + "*") : args[i+1]);
          ret += "\n\n";       
        }        
      }    
    break;    
   
    case "protected":
      if (!PLAYTESTER_HAND->query_exec_access(this_player()->query_name())) {
        this_player()->add_failed_mess (this_object(), "This facility is "
          "available only to PT Executives.\n", ({ }));
        return 0;
      }
      else {
        ret = pt_protected();
      }
    break;
  }    

  write("$P$Items$P$" + ret);
  return 1;
} /* cmd() */

private string time_string( int time_elapsed ) {
    string retval = "";
    int sec = time_elapsed % 60;
    int min = ( time_elapsed / 60 ) % 60;
    int hour = time_elapsed / 3600;

    if ( hour > 0 )
        retval += hour + "h";
    if ( min > 0 )
        retval += min + "m";
    if ( sec > 0 )
        retval += sec + "s";
    if ( sizeof( retval ) == 0 )
        return "0s";

    return retval;
} /* time_string() */


int do_check_quota() {
    string* bits;
    string shortdate;
    int* spam;
    int reports;
    int age;
    int curage;
    mapping spamtoo;

    // see adjust_quota() in /obj/handlers/playtesters.c
    bits = explode( ctime( time() ), " " ) - ({ "" });
    shortdate = bits[ 1 ] + " " + bits[ 4 ];

    spamtoo = PLAYTESTER_HAND->
        query_bugs( this_player()->query_name() );

    if ( mapp( spamtoo ) ) {
        spam = spamtoo[ shortdate ];

        if ( arrayp( spam ) ) {
            reports = spam[ 0 ];
            age = spam[ 1 ];
            curage = PLAYTESTER_HAND->get_age_uncached(this_player()->query_name());
            age += curage - spam[2];

            tell_object( this_player(), "This month (" + shortdate + "), you "
                         "have made " + reports + " report" +
                         ( ( reports == 1 ) ? "" : "s" ) +  " in " +
                         time_string( age ) + " online.\n" );

            //tell_object( this_player(), quota_message( reports, age ) );

            return 1;
        } else {
            tell_object( this_player(), "You cannot check your playtester "
                         "quota.\n" );
            return 1;
        }
    } else {
        tell_object( this_player(), "You cannot check your playtester "
                     "quota.\n" );
        return 1;
    }
}

int cmd_chats(object* obs) {
   mixed* chats;
   string ret;
   int i;
   object ob;

   // Find item and npc chats.
   ret = "";
   foreach (ob in obs) {
      ret += "$I$0=%^YELLOW%^$C$" + ob->the_short() + "%^RESET%^:\n";
      chats = ob->query_chat_string();
      if (sizeof(chats)) {
         ret += "$I$0=   %^BOLD%^Chats:%^RESET%^ (Chance: " + chats[0] + ")\n";
         chats = chats[1];
         for (i = 0; i < sizeof(chats); i += 2) {
            if (stringp(chats[i + 1])) {
               ret += "$I$6=   (Weight " + chats[i] + "): " + chats[i + 1] + "\n";
            } else if (arrayp(chats[i + 1])) {
               ret += "$I$6=   (Weight " + chats[i] + ") Story:\n ";
               foreach (string story in chats[i + 1]) {
                  ret += story + "\n";
               }
            } else {
               ret += "%^BOLD%^%^RED%^Bad chat type%^RESET%^";
            }
         }
      }
      chats = ob->query_achat_string();
      if (sizeof(chats)) {
         ret += "$I$0=   %^BOLD%^Attack Chats:%^RESET%^ (Chance: " + chats[0] + ")\n";
         chats = chats[1];
         for (i = 0; i < sizeof(chats); i += 2) {
            if (stringp(chats[i + 1])) {
               ret += "$I$6=   (Weight " + chats[i] + "): " + chats[i + 1] + "\n";
            } else if (arrayp(chats[i + 1])) {
               ret += "$I$6=   (Weight " + chats[i] + ") Story:\n ";
               foreach (string story in chats[i + 1]) {
                  ret += story + "\n";
               }
            } else {
               ret += "%^BOLD%^%^RED%^Bad chat type%^RESET%^";
            }
         }
      }
      chats = ob->query_chats();
      if (sizeof(chats)) {
         ret += "$I$0=   %^BOLD%^Item Chats:%^RESET%^ (Min time " + chats[0] + ", "
                "Max time " + chats[1] + ")\n";
         foreach (string story in chats[2]) {
            ret += "$I$6=   " + story + "\n";
         }
      }
   }
   write("$P$Npc/Item chats$P$" + ret);
   return 1;
}

mixed *query_patterns() {
   mixed *patterns = 
    ({ 
      "{chats|long|items|day items|night items|"
        "protected}", (: cmd($4[0]) :),
      "chats <indirect:object>",
        (: cmd_chats :),
      "quota", (: do_check_quota :),             
    });
   
    return patterns;     
} /* query_patterns() */




string pt_protected() {
  object *obs = filter (users(), (: $1->query_playtester() :));
  string ret = "";
  
  obs = filter (obs, (: $1->query_pt_protection() :));
  
  if (!sizeof(obs)) {
    tell_object (this_player(), "No-one is currently using PT Protection.\n");
  }
  else {
    ret += "The following people have PT protection enabled:\n\n";

    foreach (object ob in obs) {
      ret += sprintf ("%s: %s (%s).\n\n", ob->query_name(), 
        environment(ob)->query_short(), base_name (environment (ob)));
    }
  }
  
  return ret;
}
