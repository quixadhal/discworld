/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: mudlist.c,v 1.3 2002/07/18 23:21:36 ceres Exp $
 * $Log: mudlist.c,v $
 * Revision 1.3  2002/07/18 23:21:36  ceres
 * Added baselib
 *
 * Revision 1.2  2000/03/21 21:04:38  ceres
 * Fixed typo
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
/*    /cmds/player/mudlist.c
 *    from the Nightmare IV LPC Library
 *    gives information about muds
 *    created by Descartes of Borg 950623
 *    Modified for Discworld by Turrican 1-11-95
 */

#define INTERMUD_D "/net/intermud3/intermud"

int cmd(string str) {
  mixed *info;
  string *list;
  mapping borg;
  string mud;
  
  if( str && str != "") {
    mapping tmp;
    string opt, tmpstr;
    
    tmp = (mapping)INTERMUD_D->GetMudList();
    if( str[0] != '-' ) str = "-n " + str;
    opt = str[1..1];
    str = str[3..];
    borg = ([]);
    foreach(mud, info in tmp) {
      int x, y, z;
      
      switch(opt) {
      case "m":
        x = 5;
        break;
      case "d":
        x = 7;
        break;
      case "n":
        x = 0;
        break;
      }
      tmpstr = (x ? info[x] : mud);
      z = strlen(str = replace_string(lower_case(str), " ", ""));
      y = strlen(tmpstr = replace_string(lower_case(tmpstr), " ", ""));
      if( str == tmpstr ) {
        borg = ([ mud : info ]);
        break;
      }
      else if( y > z && tmpstr[0..z-1] == str && info[0] == -1 ) 
        borg[mud] = info;
    }
  }
  else {
    borg = ([ ]);
    foreach( mud, info in (mapping)INTERMUD_D->GetMudList() )
      if( info[0] == -1 ) borg[mud] = info;
  }
  if( !sizeof(borg) ) {
    write(mud_name()+" does not have any MUD matching "+str+" in "
          "its mudlist.\n");
    return 1;
  }
  else if( sizeof(borg) == 1 ) {
    string msg, svc;
    int val, comma = 0;
    
    mud = keys(borg)[0];
    msg = "\nDetailed information on %^GREEN%^" + mud + "%^RESET%^:\n";
    msg += "Server: " + borg[mud][7] + " (" + borg[mud][8] + ")\n";
    msg += "BaseLib: " + borg[mud][6] + "\n";
    msg += "Library: " + borg[mud][5] + "\n";
    msg += "Status: " + borg[mud][9] + "\n";
    msg += "Admin E-mail: " + borg[mud][10] + "\n";
    msg += "Services: ";
    foreach(svc, val in borg[mud][11]) {
      if( val == 1 ) {
        if( comma ) msg += ", " + svc;
        else {
          msg += svc;
          comma = 1;
        }
      }
    }
    msg += "\nHost: " + borg[mud][1] + "\n";
    msg += "Telnet port: " + borg[mud][2] + "\n";
    if( borg[mud][11]["http"] ) 
      msg += "HTTP port (World Wide Web): " + borg[mud][11]["http"]+"\n";
    if( borg[mud][11]["ftp"] ) 
      msg += "FTP port (File Transfer): " + borg[mud][11]["ftp"] + "\n";
    if( borg[mud][11]["rcp"] )
      msg += "RCP port (Remote Creator): " + borg[mud][11]["rcp"] + "\n";
    write(msg);
    return 1;
  }
  list = ({});
  foreach(mud, info in borg)
    list += ({ sprintf("%:-15s %:-10s %:-15s %:-15s %s %d",
                       mud, info[8], info[7], info[5], info[1], info[2]) });
  list = sort_array(list, 1);
  list = ({ mud_name() + " recognizes " + sizeof(borg)+" muds"
              " matching your query: ", "" }) + list;
  previous_object()->more_string(implode(list, "\n")+"\n", "Mudlist");
  return 1;
}

int alphabet(string a, string b) {
    if((a = lower_case(a)) == (b = lower_case(b))) return 0;
    else if(a > b) return 1;
    else return -1;
}

void help() {
    message("help", "Syntax: <mudlist>\n"
      "        <mudlist -dmn [arg]>\n\n"
      "Without any arguments, it gives a full listing of all muds "
      "with which this mud is capable of communication through "
      "tell, mail, finger, rwho, and other intermud services.  "
      "With arguments, <mudlist> requires one and only one option "
      "which must be one of the following:\n"
      "\t-d [driver]: List only muds using the named driver\n"
      "\t-m [mudlib]: List only muds using the named mudlib\n"
      "\t-n [mudname]: List only the muds with the name given\n\n"
      "Note that the argument need not be complete, for example:\n"
      "\t mudlist -n idea\n"
      "will list IdeaExchange as well as any other mud whose name "
      "begins with the string \"idea\".\n\n"
      "See also: finger, mail, rwho, tell", this_player());
}

void clean_up() {
  destruct(this_object());
}

void reset() {
  destruct(this_object());
}

void dest_me() {
  destruct(this_object());
}
