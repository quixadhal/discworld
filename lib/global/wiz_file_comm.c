inherit "/std/basic/ls";
inherit "/global/le";
inherit "/global/wiz_object_comm";
inherit "/global/player";
inherit "/global/wiz_info_comm";

#define MASTER "/secure/master"
 
static mixed in_editor;
 
varargs object *wiz_present(string str, object onobj, int nogoout);
string desc_object(mixed o);
string desc_f_object(object o);

static void wiz_commands() {
  add_action("makedir","mk*dir");
  add_action("removedir","rmd*ir");
  add_action("update","upd*ate");
  add_action("discard", "di*scard");
  add_action("rm_file","rm");
  add_action("cp_file", "cp");
  add_action("mv_file", "mv");
  add_action("load","lo*ad");
  add_action("clone","cl*one");
  add_action("indent", "indent");
  add_action("execute", "exe*c");
  wiz_object_comm::wiz_commands();
  wiz_info_comm::wiz_commands();
} /* wiz_commands() */
     
/*
** commands in the app_commands will go to all players with
** level >= 20. This includes players who have opted to
** remain players rather than advancing to wizard. As such
** we must be careful in what we place here
*/
 
static void app_commands() {
  add_action("du", "du");
  add_action("grep", "gr*ep");
  add_action("reg_ex_grep", "rg*rep");
  wiz_object_comm::app_commands();
  wiz_info_comm::app_commands();
} /* app_commands() */
 
/* These commands go to ALL players. Note that master.c
** gives only limited read/write access to dirs anyway,
** so the apparent security problems are NOT a problem
*/
 
static void all_commands() {
  add_action("tail_file","ta*il");
  add_action("what_dir","pw*d");  
  add_action("list_files","ls");
  add_action("change_dir","cd");
  add_action("exa_file","cat");
  add_action("do_more","mo*re");
  add_action("edit","ed");
  add_action("le", "le");
  add_action("set_home_dir", "homed*ir");
  add_action("do_sar","sar");
  wiz_object_comm::all_commands();
  wiz_info_comm::all_commands();
} /* all_commands() */
 
static int mv_file(string str) {
  string *filenames, dest, *fnames, source;
  int loop, fs, pos;

  if (this_player(1) != this_player()) return 0;

  if(!str) {
    notify_fail("Usage : mv file [file|dir...]\n");
    return 0;
  }
  fnames = explode(str, " ");
  filenames = get_files(implode(fnames[0..sizeof(fnames) -2], " "));
  if(!sizeof(filenames)) {
    notify_fail("Usage : mv file [file|dir...]\n");
    return 0;
  }
  dest = fnames[sizeof(fnames) - 1];
  dest = get_path(dest);
  if(!dest) {
    write("No destination\n");
    return 1;
  }
  for(loop = 0; loop < sizeof(filenames); loop++) {
    str = filenames[loop];
    if(file_size(str) == -1) {
      write("No such file : " + str + "\n");
      continue;
    }
    fs = file_size(dest);
    if(fs == -2) {
      string *names;

      names = explode(str, "/");
      fs = file_size(dest + "/" + names[sizeof(names) - 1]);
      if(fs != -1) {
        write("file exists " + dest + "/" + names[sizeof(names) - 1] + "\n");
        continue;
      }
      rename(str, dest + "/" + names[sizeof(names) - 1]);
    }
    else {
      if(fs != -1) {
        write("File exists : " + dest + "\n");
        continue;
      }
      rename(str, dest);
    }
  }
  write("Ok.\n");
  return 1;
} /* mv_file() */

static int cp_file(string str) {
  string *filenames, text, dest, *fnames;
  int loop, fs;

  if (this_player(1) != this_player()) return 0;

  if(!str) {
    notify_fail("Usage : cp file [file|dir...]\n");
    return 0;
  }
  fnames = explode(str, " ");
  filenames = get_files(implode(fnames[0..sizeof(fnames) -2], "/"));
  if(!sizeof(filenames)) {
    notify_fail("Usage : cp file [file|dir...]\n");
    return 0;
  }
  dest = fnames[sizeof(fnames) - 1];
  dest = get_path(dest);
  if(!dest) {
    write("No destination\n");
    return 1;
  }
  for(loop = 0; loop < sizeof(filenames); loop++) {
    str = filenames[loop];
    text = read_file(str);
    if(!text) {
      write("No such file : " + str + "\n");
      continue;
    }
    fs = file_size(dest);
    if(fs == -2) {
      string *names;

      names = explode(str, "/");
      fs = file_size(dest + "/" + names[sizeof(names) - 1]);
      if(fs != -1) {
        write("file exists " + dest + "/" + names[sizeof(names) - 1] + "\n");
        continue;
      }
      write_file(dest + "/" + names[sizeof(names) - 1], text);
    }
    else {
      if(fs != -1) {
        write("File exists : " + dest + "\n");
        continue;
      }
      write_file(dest, text);
    }
  }
  write("Ok.\n");
  return 1;
} /* cp_file() */

static int do_update(object *ov) {
   string pname, dummy, err;
   int i, j;
   object *invent, rsv, env, dup, loaded;

  if (this_player(1) != this_player()) return 0;

   "room/void"->bingle_bingle();

   rsv = find_object("room/void");  /* RSV = Room Slash Void */
   if (!rsv) { /* Die in horror */
      notify_fail("The void is lost!\n");
      return 0;
   }
 
   for (i = 0; i < sizeof(ov); i++) {
      if (!ov[i]) continue;
      env = environment(ov[i]);
      invent = all_inventory(ov[i]);

      for (j = 0; j < sizeof(invent); j++) 
         invent[j]->move(rsv);

      pname = file_name(ov[i]);
      if (sscanf(pname, "%s#%d", pname, dummy) != 2) { /* a room ? */
         ov[i] -> dest_me();
         if (ov[i]) ov[i]->dwep();
         if (ov[i]) destruct(ov[i]);
         file_size("/secure/master");
         if (!ov[i])
           ov[i] = find_object(pname);
         call_other(pname, "??");
         ov[i] = find_object(pname);
      } else {
         loaded = find_object(pname);
         if (loaded) loaded->dest_me();
         if (loaded) loaded->dwep();
         if (loaded) destruct(loaded);

         dup = clone_object(pname);
         if (dup && ov[i]) {
            ov[i] -> dest_me();
            if (ov[i]) ov[i]->dwep();
            if (ov[i]) destruct(ov[i]);
            ov[i] = dup;
         }
      }

     if (!ov[i]) {
        write("I seem to have lost your object.\n");
        return 1;
     }

     for (j = 0; j < sizeof(invent); j++)
        if (invent[j]) invent[j]->move(ov[i]);

     if (env) ov[i]->move(env);
     write("Updated " + desc_f_object(ov[i]) + ".\n");
   }
   return 1;
} /* do_update() */
 
int update(string str) {
  string tring, *filenames, err;
  object ob, *val, *obs;
  int loop, loop2;

  notify_fail("No such object.\n");
  tring = str;
  if (!str || str == "here") { 
    str = file_name(environment());
    sscanf(str, "%s#%d", str, loop);
    filenames = ({ "/" + str });
    str = "here";
  }
  else {
    filenames = get_cfiles(str);
  }
  if(sizeof(filenames) == 0) {
    val = wiz_present(tring, this_player());
    if(!sizeof(val)) {
      notify_fail("No matching objects/filenames\n");
      return 0;
    }
    return do_update(val);
  }
  obs = ({ });
  for(loop = 0; loop < sizeof(filenames); loop++) {
    str = filenames[loop];
    ob = find_object(str);
    if (!ob) {
      if(file_size(str) >= 0) {
        if (!(err = catch(str->bing_with_me())))
          write("Loaded " + str + "\n");
        else
          write("Failed to load "+str+", error: "+err+"\n");
      }
      else {
        val = wiz_present(tring, this_player());
        obs += val;
      }
    }
    else {
      obs += ({ ob });
    }
  }
  if (!obs)
    return 0;
  else
    return do_update(obs);
} /* update() */

/* This is for querying about objects who don't want to be destructed */
static object discard_obj;
 
int discard(string str) {
  string *file_names;
  int loop;
  string err;

  if (!str) {
    notify_fail("Discard what?\n");
    return 0;
  }
  file_names = get_cfiles(str);
  for(loop = 0; loop < sizeof(file_names); loop++) {
    str = file_names[loop];
    discard_obj = find_object(str);
    if (!discard_obj) {
      if (file_size(str) < 0) notify_fail("No such file " + str + "\n");
      else {
        write(str + " is not loaded.\n");
      }
      continue;
    }
    err = catch(discard_obj->dest_me());
    handle_error(err, "dest_me");
    if (discard_obj) {
      write("That object has a violent objection to being dested.\n");
      write("Are you sure you want to do this? ");
      input_to("no_discard");
      return 1;
    }
  }
  write("Ok.\n");
  return 1;
} /* discard() */

void no_discard(string s) { 
   string err;

   if (affirmative(s)) {
      err = catch(discard_obj->dwep());
      handle_error(err, "DWEP");
      if (discard_obj) {
         write("It REALLY doesn't want to be discarded.\n");
         destruct(discard_obj);
         if (discard_obj) {
            write("It didn't destruct.\n");
            return;
         }
      }
   }
   write("Ok.\n");
   discard_obj = 0;
} /* no_discard() */

int load(string str) {
 string *filenames, err;
 int loop;

  if (!str) {
    notify_fail("Load what ?\n"); 
    return 0;
  }
  filenames = get_cfiles(str);
  if (!sizeof(filenames)) {
    notify_fail("No such object.\n");
    return 0;
  }
  for(loop = 0; loop < sizeof(filenames); loop++) {
    str = filenames[loop];
    if (file_size(str)<0) {
      write(str + ": No such object.\n");
      continue;
    }
    if (err = catch(str->load_up_with_yellow()))
      write("Failed to load "+str+", error: "+err+"\n");
    else
      write("Loaded "+str+"\n");
  }
  write("Ok.\n");
  return 1;
} /* load() */

int do_more(string str) {
  object *things, spam;
  int egg;

  if (this_player(1) != this_player()) return 0;

  if(!str) {
    notify_fail("More which file(s)/object?\n");
    return 0;
  }
  if(sizeof(things = wiz_present(str, this_player()))) {
    str = file_name(things[0]);
    sscanf(str, "%s#%d", str, egg);
    if (file_size(str) <= 0)
      str += ".c";
  }
  return more_file(str);
} /* do_more() */

static int edit(string str) {
  string *filenames, spam, bing;
  object *things;
  int egg;

  if (this_player(1) != this_player()) return 0;

  if (!str) {
    in_editor = "(hidden)";
    ed("frog", "fini_editor");
    return 1;
  }
  /* dodgy idea, but allows 'ed here' or 'ed strawberry' */        
  if (sizeof(things = wiz_present(str, this_player()))) {
    spam = file_name(things[0]);
    sscanf(spam, "%s#%d", spam, egg);
    if (file_size(spam) < 0)
      filenames = ({ spam+".c" });
    else
      filenames = ({ spam });
  } else
    filenames = get_files(str);
  if (!sizeof(filenames)) { 
    str = get_path(str);
  } else {
    if (sizeof(filenames) > 0) {
      str = filenames[0];
      if (sizeof(filenames) > 1) {
        int loop;

        loop = 0;
        while(loop < sizeof(filenames) && file_size(filenames[loop]) < 0)
          loop++;
        if(loop >= sizeof(filenames)) {
          write("No such file.\n");
          return 0;
        }
        else {
          str = filenames[loop];
        } 
        write("Ambiguous, using : " + str + "\n");
      }
    }
  }
  if (file_size(str) == -2) {
    write("directory\n");
    return 1;
  }
  in_editor = str;
  if (!MASTER->valid_write(str, geteuid(), "frog"))
    write("[read only] ");
  ed(str, "fini_editor");
  return 1;
} /* edit() */

mixed query_in_editor() { return in_editor; }

void fini_editor() {
  in_editor = 0;
} /* fini_editor() */
 
static int clone(string str) {
    object ob;
    string err, *filenames;
    int loop, mov;

    if (!str) {
        notify_fail("Clone what ?\n");
        return 0;
    }

    filenames = get_cfiles(str);
    if (!sizeof(filenames))  {
        notify_fail("No such file.\n");
        return 0;
    }

    for(loop = 0; loop < sizeof(filenames); loop++) {
       str = filenames[loop];
       if (file_size(str) < 0 && file_size(str + ".c") < 0) {
          notify_fail("No such file.\n");
          return 0;
       }
/*    err = catch(ob = clone_object(str));
       handle_error(err, "clone_object()"); */
       ob = clone_object(str);
       if (ob) {
          err = catch((mov = (int)ob->move(this_player())));
          handle_error(err, "move(this_player())");
          if (err || mov) {
             err = catch(ob -> move(environment(this_player())));
             handle_error(err, "move(environment())");
          }
          write("Ok.  Object "+file_name(ob)+" cloned and put in "+
             (environment(ob)==this_object() ? "you" : 
              (environment(ob)==environment() ? "here" : desc_object(ob)))+
             ".\n");
          say((string)this_player()->query_cap_name() + " fetches " +
            ((string)ob->query_short()?(string)ob->query_short():"something") +
             " from another dimension.\n");
       } else {
          write("Failed to clone.\n");
       }
    }
    return 1;
} /* clone() */
 
static int what_dir() {
  write(current_path+"\n");
  return 1;
} /* what_dir() */
 
static int change_dir(string str) {
  string *filenames;

  if (this_player(1) != this_player()) return 0;

  if (!str)  {
    if(!home_dir) {
      notify_fail("No homedir.  Use homedir to set it.\n");
      return 0;
    }
    str = home_dir;
  }
  filenames = get_files(str);
  if(sizeof(filenames) > 1) {
    notify_fail("Ambiguous directory.\n");
    return 0;
  }
  if (!sizeof(filenames)) {
    notify_fail("No such dir.\n");
    return 0;
  }
  str = filenames[0];
  if (file_size(str) != -2) 
    write("Bad directory : " + str + ".\n");
  else
    current_path = str;
  write(current_path+"\n");
  return 1;
} /* change_dir() */
 
string query_path() { return current_path; }
 
static int exa_file(string str) {
   string *filenames;
   int loop;

  if (this_player(1) != this_player()) return 0;

    if (!str) {
        notify_fail("Cat what file ?\n");
        return 0;
        }
    filenames = get_files(str);
    if (!sizeof(filenames)) {
       notify_fail(str + ": No such file.\n");
       return 0;
    }
    for(loop = 0; loop < sizeof(filenames); loop++) {
       str = filenames[loop];
       if(sizeof(filenames) > 1) {
          write("FILE : " + str + "\n");
       }
       cat(str);
    }
    return 1;
} /* exa_file() */
 
static int list_files(string str) {
    if (!str) str = current_path;
    else str = get_path(str);
    if (!str) return 1;
    ls(str);
    return 1;
} /* list_files() */
 
static int rm_file(string str) {
    int fsize, pos, loop;
    string *filenames;
 
  if (this_player(1) != this_player()) return 0;

    if (!str) {
       notify_fail("Usage: rm file [file ...]\n");
       return 0;
    }
    filenames = get_files(str);
    if (!sizeof(filenames)) {
       notify_fail("No such file : " + str + "\n");
       return 0;
    }
    for(loop = 0; loop < sizeof(filenames); loop++) {
       string temp;
       str = filenames[loop];
       if(sscanf(str, "%s/.", temp) || sscanf(str, "%s/..", temp)) {
           continue;
       }
       fsize = file_size(str);
       if (fsize == -1) {
          notify_fail("No such file or directory.\n");
          return 0;
       }
       if (fsize == -2) {
          if (!rmdir(str)) {
             notify_fail("Couldn't rm directory: " + str + "\n");
             return 0;
          }
       } else if (!rm(str)) {
          notify_fail("Can't remove file.\n");
          return 0;
       }
    }
    write("Ok.\n");
    return 1;
} /* rm_files() */

static int tail_file(string str) {
    string *filenames;
    int loop;
  if (this_player(1) != this_object()) return 0;

  if (!str) {
    notify_fail("Tail what file ?\n");
    return 0;
  }
  filenames = get_files(str);
  if (!sizeof(filenames)) {
    notify_fail(str + ": No such file.\n");
    return 0;
  }
  for(loop = 0; loop < sizeof(filenames); loop++) {
    str = filenames[loop];
    if(sizeof(filenames) > 1) {
       write("FILE : " + str + "\n");
    }
    if (!tail(str))
      write("No such file.\n");
  }
  return 1;
} /* tail_file() */

static int makedir(string str) {
 
  if (this_player(1) != this_object()) return 0;
   if (!str || str == "") {
      notify_fail("Make what directory?\n");
      return 0;
      }
   str = get_path(str);
 
   if (!str)
      return 1;
 
   if (file_size(str) != -1) {
      notify_fail(str + " already exists.\n");
      return 0;
      }
 
   if (!mkdir(str)) {
      notify_fail("Couldn't make dir.\n");
      return 0;
   }
   write("Ok.\n");
   return 1;
} /* makedir() */
 
static int removedir(string str) {
   string *filenames;
   int fsize, loop;
 
  if (this_player(1) != this_object()) return 0;
   if (!str || str == "") {
      notify_fail("Remove what dir?\n");
      return 0;
   }
   filenames = get_files(str);
   if (!sizeof(filenames)) {
      notify_fail("No such directory : " + str + "\n");
      return 0;
   }
 
   for(loop = 0; loop < sizeof(filenames); loop++) {
      str = filenames[loop];
      fsize = file_size(str);
      if (fsize == -1) {
         notify_fail(str + " doesn't exist.\n");
         return 0;
      }
      if (fsize != -2) {
         notify_fail(str + " is not a directory.\n");
         return 0;
      }
      if (!rmdir(str)) {
         notify_fail("Couldn't remove dir : " + str + "\n");
         return 0;
      }
   }
   write("Ok.\n");
   return 1;
} /* removedir() */
 
static int set_home_dir(string str) {
  if (this_player(1) != this_object()) return 0;
    if (str) home_dir = get_path(str);
    write("Home directory set to "+home_dir+".\n");
    return 1;
} /* set_home_dir() */
 
static int execute(string str) {
  mixed  err, ret;
  object ob;
  string file, wiz_dir;
        
  if (this_player(1) != this_object()) return 0;
  if (!this_player()) return 0;
  if (!str) {
    notify_fail("usage:  exec <lpc code>\n");
    return 0;
    }   
  wiz_dir = "/w/" + (string)this_player()->query_name();
  if (file_size(wiz_dir)!=-2) {
    notify_fail("Directory: " + wiz_dir + " does not exist.\n");
    return 0;
    }
  file = wiz_dir + "/exec_tmp";
  if (find_object(file)) file->dest_me();
  if (file_size(file+".c")>0) rm(file+".c");
  write_file(file+".c",
    "create() { seteuid(geteuid(this_player())); }\n" +
    "dest_me() { destruct(this_object()); }\n" +
    "do_call() {\n"+ str + ";\n}\n");
  err = catch(ret = (mixed) file->do_call());
  if (err==0) printf("\nReturns: %O\n", ret);
  if (find_object(file)) file->dest_me();
  rm(file+".c");
  return 1;
} /* execute() */

int grep(mixed str) {
  int i, j, num;
  string *files, *bit;
  string s1, s2, s3, s4;

  if (this_player(1) != this_object()) return 0;
  num = 1;
  if (sscanf(str,"-n %s", str) == 1)
    num = 0;
  if (sscanf(str, "%s -n %s", s1, s2) == 2) {
    num = 0;
    str = s1+" "+s2;
  }
  if (sscanf(str, "%s %s", s1, s2) != 2) {
    notify_fail("Usage: grep pattern <files>\n");
    return 0;
  }

  files = get_files(s2);
  if(!sizeof(files)) {
    notify_fail("File(s) " + s2 + " not found.\n");
    return 0;
  }
  for (i=0;i<sizeof(files);i++)
    if (file_size(files[i]) > 0)
      if (sscanf(read_file(files[i]), "%s"+s1+"%s", s3, s4) == 2) {
        j = 0;
        str = read_file(files[i], 0, 900);
        write("File : "+files[i]+"\n");
        while (str && num) {
          while (sscanf(str, "%s"+s1+"%s", s3, s4) == 2) {
            bit = explode(s3,"\n");
            printf("%4d: %s\n", j+sizeof(bit), bit[sizeof(bit)-1]+s1+
                                explode(s4,"\n")[0]);
            j += sizeof(bit);
            str = implode(explode(s4,"\n")[1..1000],"\n");
          }
          j = ((j/900)+1)*900;
          str = read_file(files[i],j, 900);
        }
      }
  return 1;
} /* grep() */

int query_ed_setup() { return ed_setup; }
void set_ed_setup(int i) { ed_setup = i; }

int rec_du(string path) {
  string *files;
  int i, size, tot;
  if (this_player(1) != this_object()) return 0;
  if (path[strlen(path)-1] != '/')
    path += "/";
  files = get_dir(path+"*");
  for (i=0;i<sizeof(files);i++) {
    if (files[i] == "." || files[i] == "..")
      continue;
    size = file_size(path+files[i]);
    if (size > 0)
      tot += size;
    else if (size == -2) {
      printf("%-30s %5d\n", path+files[i], (size = rec_du(path+files[i])));
      tot += size*1024;
    }
  }
  return (tot+1023)/1024;
} /* rec_du() */

int du(string str) {
  if (this_player(1) != this_object()) return 0;
  notify_fail("Must have write access to be allowed to use du on a dir.\n");
  if (!str) {
    if(!"secure/master"->valid_write(current_path,geteuid(this_object())))
      return 0;
    printf("%-30s %5d\n", "Total:",rec_du(current_path));
    return 1;
  }
/* forget about type 2 for now ;) */
  return 0;
} /* du() */

int reg_ex_grep(string str) {
  string *files;
  string s1, *s2, *s3, pattern;
  int i, j, k, fsize;
  int done;

  if (this_player(1) != this_object()) return 0;
  notify_fail("Usage: grep <pattern> <files>\n");
  if (!str)
    return 0;

  if (sscanf(str, "%s %s", pattern, str) != 2)
    return 0;

  files = get_files(str);
  if(!sizeof(files)) {
    notify_fail("File(s) " + s2 + " not found.\n");
    return 0;
  }
  for (i=0;i<sizeof(files);i++)
    if ((fsize=file_length(files[i])) > 0) {
      done = 0;
      for (j=0;j<fsize;j+=500) {
        s1 = read_file(files[i],j,500);
        s2 = explode(s1,"\n");
        s3 = regexp(s2, pattern);
        if (sizeof(s3) && !done)
          printf("%s\n",files[i]);
        for (k=0;k<sizeof(s3);k++)
          printf("%5d: %s\n", j+member_array(s3[k], s2), s3[k]);
      }
    }
  return 1;
} /* reg_ex_grep() */

int review() {
  player::review();
  wiz_info_comm::review();
  return 1;
} /* review() */

/*
  Nanvaent Industries International
  
  LPC: Search and Replace

  Written by  : Weazel@Nanvaent Nov 92
  Modified by : Bill@Nanvaent Jan 93
  Modified by : Bill@Nanvaent+@Discworld Mar 93
*/

int do_sar(mixed str)
{
  int i, j, num;
  string *files, *bit;
  string s1, s2, s3, s4;
  if(!str)
  {
    notify_fail("Usage: sar search_string replace_string <files>\n");
    return 0;
  }
  s4 = extract(str, 0, 0);
  if(sscanf(str, s4+"%s"+s4+" "+s4+"%s"+s4+" %s", s1, s2, s3) != 3)
    if (sscanf(str, "%s %s %s", s1, s2, s3) != 3)
    {
      notify_fail("Usage: sar search_string replace_string <files>\n");
      return 0;
    }
  files = (string *)this_player()->get_files(s3);
  if(!sizeof(files))
  {
    notify_fail("File(s) " + s3 + " not found.\n");
    return 0;
  }
  for (i=0;i<sizeof(files);i++)
  {
    if (file_size(files[i]) <= 0) continue;
    write("Looking at "+files[i]+".\n");
    s4 = read_file(files[i]);
    if (s4)
    {
      s4 = replace(s4, s1, s2);
      rm(files[i]);
      write_file(files[i], s4);
    }
    else
      write("...failed...no file\n");
  }
  return 1;
}
