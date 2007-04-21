#include <player_handler.h>

#define SAVE_FILE "/secure/rcs_handler"
#define CMD_NUM 5
#define MAX_MOD_TIME (86400 * 90)

mapping _locks = ([]);

void create()  {
   unguarded((: restore_object, SAVE_FILE :));
   call_out("clean_up", 2);
}

void clean_up(string *names) {
  string name, file, *cmd;
  int changed, i;
  
  if(!names) {
    names = keys(_locks);
  }

  if(!sizeof(names))
    return 0;
  
  name = names[0];

  if(sizeof(names) > 1) {
    while(sizeof(names) > 1 && (!names[1] || !arrayp(names[1])))
      names = names[1..];
    if(sizeof(names) > 1)
      call_out("clean_up", random(15), names[1..]);
  }
  
  foreach(file in _locks[name]) {
    if(i++ > 100)
      break;
    if(file_size(file) <= 0) {
      _locks[name] -= ({ file });
      changed = 1;
    } else if((time() - stat(file)[1]) > MAX_MOD_TIME ||
            !PLAYER_HANDLER->test_creator(name)) {
      cmd = ({ "-w" + name,
                 "-u",
                 "-mForcibly released due to inactivity",
                 file[1..] });
      external_start(CMD_NUM, cmd, "read_call_back", "write_call_back",
                     "close_call_back");
      _locks[name] -= ({ file });
      changed = 1;
    }
  }

  if(!sizeof(_locks[name])) {
    map_delete(_locks, name);
    changed = 1;
  }

  if(changed)
    unguarded((: save_object, SAVE_FILE, 2 :));
}

void read_call_back(int fd, mixed mess) { return; }
void write_call_back(int fd) { return; }
void close_call_back(int fd) { return; }

void add_lock(mixed cre, string file)  {
   string name;
   if( file[0] != '/' )
      file = "/" + file;

   if (stringp(cre))  name = lower_case(cre);
   else name = cre->query_name();
   if (undefinedp(_locks[name]))
      _locks[name] = ({ file });
   else if ( member_array( file, _locks[name] ) < 0 )
      _locks[name] += ({ file });
   unguarded((: save_object, SAVE_FILE, 2 :));

   return;
}


void remove_lock(mixed cre, string file)  {
   string name;
   if( file[0] != '/' )
      file = "/" + file;

   if (stringp(cre))  name = lower_case(cre);
   else name = cre->query_name();
   if (!undefinedp(_locks[name]))  {
      _locks[name] -= ({ file });
      unguarded((: save_object, SAVE_FILE, 2 :));
   }

   return;
}
/**
 * Return a mapping of non-creators who have files locked and which files
 * those people have locked.
 */
mapping query_non_creators() {
  mapping res;
  string person;

  res = ([ ]);
  foreach(person in keys(_locks)) {
    if(!PLAYER_HANDLER->test_creator(person))
      res[person] = _locks[person];
  }
    return res;
}

string *query_locks(mixed cre)  {
   string name;

   if (stringp(cre))  name = lower_case(cre);
   else name = cre->query_name();
   return _locks[name];
}


void reset_locks(mixed cre, int start)  {
  string file, name, rcsfile, tmp;
  int idx, i, changed;

  changed = 0;
  
  if (stringp(cre))
    name = lower_case(cre);
  else
    name = cre->query_name();
  
  if (undefinedp(_locks[name]))
    return;

  for(i=0; i < sizeof(_locks[name]) && i < start + 100; i++) {
    file = _locks[name][i];
    
    idx = strsrch(file, "/", -1);
    rcsfile = file[0 .. idx] + "RCS/" + file[idx + 1 .. ] + ",v";
    
    if (file_size(rcsfile) > 0) { 
      tmp = read_file(rcsfile, 4, 1);
      if (tmp == "locks\n") { // we've found a lock
        string lockname;
        tmp = read_file(rcsfile, 5, 1);
        sscanf(tmp, "\t%s:", lockname);
        if (lockname != name)  {
          _locks[name] -= ({ file });
          changed = 1;
        }
      } else {
        _locks[name] -= ({ file });
        changed = 1;
      }
    }
  }

  if(!sizeof(_locks[name])) {
    map_delete(_locks, name);
    changed = 1;
  }
  
  if (changed)
    unguarded((: save_object, SAVE_FILE, 2 :));
  return;
}

string help() {
  return "Displays the files that are locked in your current directory.";
}
