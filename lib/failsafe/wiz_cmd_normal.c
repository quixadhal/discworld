inherit "/failsafe/simul_efun";
inherit "/failsafe/ls";
inherit "failsafe/more_file";
inherit "failsafe/path";
inherit "failsafe/le";

#define ALIGN(str,x) extract(str+"                                   ",0,(x))
#define MASTER "/secure/master"
 
int earmuffs;
 
static wiz_commands() {
    add_action("makedir","mkdir");
    add_action("removedir","rmdir");
    add_action("chmoddir","chmod");
    add_action("chowndir","chown");
    add_action("function","call");
    add_action("update","update");
    add_action("discard", "discard");
    add_action("rm_file","rm");
    add_action("load","load");
    add_action("dest","dest");
    add_action("clone","clone");
    add_action("shutdown_game", "shutdown");
    add_action("local_cmds", "localcmd");
    add_action("show_stats", "stat");
    add_action("home", "home");
    add_action("display_wl", "wizlist");
    add_action("do_trace", "trace");
    add_action("trans", "trans");
    add_action("indent", "indent");
    }
     
/*
** commands in the app_commands will go to all players with
** level >= 20. This includes players who have opted to
** remain players rather than advancing to wizard. As such
** we must be careful in what we place here
*/
 
static app_commands() {
    add_action("whereis","whereis");
    add_action("get_pathof","pathof");
    add_action("get_creator","creator");
    add_action("get_inv","inv");
    add_action("teleport","goto");
    add_action("list_people","people");
    add_action("earmuffs", "earmuffs");
    }
 
/* These commands go to ALL players. Note that master.c
** gives only limited read/write access to dirs anyway,
** so the apparent security problems are NOT a problem
*/
 
static all_commands() {
    add_action("tail_file","tail");
    add_action("what_dir","pwd");  
    add_action("list_files","ls");
    add_action("change_dir","cd");
    add_action("exa_file","cat");
    add_action("more_file","more");
    add_action("edit","ed");
    add_action("le", "le");
    add_action("set_home_dir", "homedir");
    }
 
whereis(string str) {
object *ov,e;
int i;
 
   notify_fail("Where is what?\n");
   ov = wiz_present(str,this_player());
   if (!ov) return 0; 

   for (i = 0; i < sizeof(ov); i++) {
      write(desc_object(ov[i]) + " is : \n");
      e = ov[i];
      while (e = environment(e))
         write("  in " + desc_object(e) + " (" + file_name(e) + ")\n");
      }
   return 1;
   }
 
do_update(object *ov) {
   string pname, dummy, err;
   int i, j;
   object *invent, rsv, env, dup, loaded;

   "room/void"->bingle_bingle();

   rsv = find_object("room/void");  /* RSV = Room Slash Void */
   if (!rsv) /* Die in horror */
   {
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
         call_other(pname, "dummy");
         ov[i] = find_object(pname);
      } else {
         loaded = find_object(pname);
         if (loaded) loaded->dest_me();
         if (loaded) loaded->dwep();
         if (loaded) destruct(loaded);

         dup = clone_object(pname);
         if (dup) {
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
     write("Updated " + desc_object(ov[i]) + ".\n");
   }
   return 1;
}
 
update(string str) {
string tring;
object ob, *val;
    notify_fail("No such object.\n");
    tring = str;
    if (!str) str = file_name(environment());
    else str = get_path(str);
    ob = find_object(str);
    if (!ob) val = wiz_present(tring, this_player());
    else val = ({ ob });
    if (!val) return 0;
    if (do_update(val))
      return 1;
    return load(str);
}

static discard_obj;	/* This is for querying about objects who don't */
                        /* want to be destructed */

affirmative(s)	/* returns true if s is a 'yes' equiv response */
{
   s = lower_case(s);
   return (s == "y" || s == "yes" || s == "ok" || s == "please");
}

handle_error(erm, desc)	/* is there an error?  print it */
{
   if (erm) {
      write(desc + " failed : " + erm);
   }
}

discard(str)
{
   string err;

   if (!str) {
      notify_fail("Discard what?\n");
      return 0;
   }
   str = get_path(str);
   discard_obj = find_object(str);
   if (!discard_obj) {
      if (file_size(str + ".c") < 0) notify_fail("No such file.\n");
      else notify_fail("That object is not loaded.\n");
      return 0;
   }
   err = catch(discard_obj->dest_me());
   handle_error(err, "dest_me");
   if (discard_obj) {
      write("That object has a violent objection to being dested.\n");
      write("Are you sure you want to do this? ");
      input_to("no_discard");
      return 1;
   }
   write("Ok.\n");
   return 1;
}

no_discard(s)
{ 
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
}

load(str) {
    if (!str) {
        write("Load what ?\n"); 
        return 1;
        }
    str = get_path(str);
    if (!str) return 1;
    if (file_size(str +".c")<0 && file_size(str)<0) {
       notify_fail("No such object.\n");
       return 0;
    }
    str -> dummy();
    write("Ok.\n");
    return 1;
    }
 
edit(str) {
    if (!str) {
        ed();
        return 1;
        }
    str = get_path(str);
    if (!str) return 1;
    ed(str);
    return 1;
    }
 
teleport(str) {
object ob;
    if (!str) {
        write("Teleport where ?\n");
        return 1;
        }
    str = this_player()->expand_nickname(str);
    ob = find_living(str);
    if (ob)
        str = file_name(environment(ob));
    else {
        str = get_path(str);
        if (!find_object(str)) 
                catch(str->dummy());
	if (!find_object(str)) {
	    write(str+"\n");
	    str = 0;
	    }
        }
    if (!str) {
        notify_fail("No such monster or room.\n");
        return 0;
        }
    this_player()->move_player("X",str);
    write("Ok.\n");
    return 1;
    }
 
get_pathof(str) {
   object *ov;
   int i;

   notify_fail("Pathof what?\n");
   ov = wiz_present(str,this_player());
   if (!ov) return 0;

   for (i = 0; i < sizeof(ov); i++) {
      if (!ov[i]) continue;
      if (sizeof(ov) > 1) {
         write("Path of " + desc_object(ov[i]) + " in " + desc_object(environment(ov[i])) + ":\n");
      }
      write(file_name(ov[i]) + "\n");
   }
   return 1;
}
 
get_inv(str) {
   object *ov,obj;
   int i;
   string s2;
 
   notify_fail("Inv of what?\n"); /* thanks for the great error message ! */

   if (!str) {
     ov = ({ this_player() });
   } else {
     if (sscanf(str, "of %s", s2) == 1) {
       write("You don't need the 'of' when using inv.\n");
       str = s2;
     }
     ov = wiz_present(str,this_player());
   }
   if (!ov) return 0;

   for (i = 0; i < sizeof(ov); i++) {
        if (!ov[i]) continue;
        write("Inv of " + desc_object(ov[i]) + " in " + 
            desc_object(environment(ov[i])) + ":\n");
        obj = first_inventory(ov[i]);
        while (obj) {
            write("   " + desc_object(obj) + "\n");
         obj = next_inventory(obj);
      }
   }
   return 1;
}
 
get_creator(str) {
   object *ov;
   int i;
 
   notify_fail("Creator of what?\n");
   ov = wiz_present(str,this_player());
   if (!ov) return 0;

   for (i = 0; i < sizeof(ov); i++) {
      write("Creator of " + desc_object(ov[i]) + ": " +
      "secure/master"->creator (ov[i]) + " uid: "+getuid(ov[i])+
      ".\n");
   }
   return 1;
}

static object *dest_obj; 
static int objn, majd;

ask_dest()
{
   if (!pointerp(dest_obj) || objn >= sizeof(dest_obj)) {
      write("No more things to dest.\n");
      dest_obj = 0;    /* wipe array to free memory */
      return;
   }
   write("Dest object " + desc_object(dest_obj[objn]) + " ? ");
   input_to("dest_answer");
   return;
}

dest_answer(s)
{
   string err, shrt;

   if (affirmative(s)) {
      if (majd) {
         shrt = dest_obj[objn]->short();
         err = catch(dest_obj[objn]->dwep());
         handle_error(err, "DWEP");
         if (dest_obj[objn]) {
            write("It REALLY doesn't want to be dested.\n");
            err = catch(destruct(dest_obj[objn]));
            handle_error(err, "destruct()");
         }
         majd = 0;
         if (dest_obj[objn]) write("It didn't dest.\n");
         else {
            say(this_player()->query_cap_name()+" disintegrates "+
               (shrt ? shrt : "something") +".\n"); 
            write("Ok.\n");
         }
         objn++;
         ask_dest();
         return;
      } else {
         err = catch(dest_obj[objn]->dest_me());
         handle_error(err, "dest_me");
         if (dest_obj[objn]) {
            write("This object does NOT want to be dested.  Are you sure? ");
            majd = 1;
            input_to("dest_answer");
            return;
         } 
         write("Ok.\n");
         objn++;
         ask_dest();
         return;
      }
   }
   write("Ok.  Not destd.\n");
   objn++;
   ask_dest();
   return;
}
         
dest(str) {
   object *ob;
   int i;

   string qstr, err, shrt, dobj;
   dest_obj = ({ });

   notify_fail("Can't find " + str + " to dest.\n");
    
   if (sscanf(str, "query %s", qstr) == 1) {
      dest_obj = wiz_present(qstr, this_player());
      if (!dest_obj) return 0;
      objn = 0;
      majd = 0;	/* MAJOR dest needed */
      ask_dest();
      return 1;
   }

   ob = wiz_present(str,this_player());
   if (!ob) return 0;

   for (i = 0; i < sizeof(ob); i++) {
      if (interactive(ob[i]) && sizeof(ob) != 1) {
         write("You DON'T destruct " + ob[i]->query_name() + ".\n");
         continue;
      }
      shrt = ob[i]->short();
      dobj = desc_object(ob[i]);
      err = catch(ob[i] -> dest_me());
      handle_error(err, "dest_me");
      if (ob[i]) 
         dest_obj += ({ ob[i] });
      else {
         write("You destruct " + dobj + ".\n");
         say(this_player()->query_cap_name()+" disintegrates "+
            (shrt ? shrt : "something") + ".\n"); 
      }
   }
   if (sizeof(dest_obj) > 0) {
      objn = 0;
      majd = 0;
      ask_dest();
      return 1;
   }
   return 1;
}
  
clone(str) {
    object ob;
    string err;

    if (!str) {
        write("Clone what ?\n");
        return 1;
    }

    str = get_path(str);
    if (!str) return 0;

    if (file_size(str) < 0 && file_size(str + ".c") < 0) {
       notify_fail("No such file.\n");
       return 0;
    }
/*    err = catch(ob = clone_object(str));
    handle_error(err, "clone_object()"); */
    ob = clone_object(str);
    if (ob) {
       err = catch(ob -> move(environment(this_player())));
       handle_error(err, "move(environment())");
       err = catch(ob -> move(this_player()));
       handle_error(err, "move(this_player())");
       write("Ok.  Object "+file_name(ob)+" cloned and put in "+
          (environment(ob)==this_object() ? "you" : 
           (environment(ob)==environment() ? "here" : desc_object(ob)))+
          ".\n");
       say(this_player()->query_cap_name()+" fetches something from another dimension.\n");
    } else {
       write("Failed to clone.\n");
    }
    return 1;
}
 
what_dir() {
    write(current_path+"\n");
    return 1;
    }
 
static change_dir(str) {
    str = get_path(str);
    if (!str) return 1;
    if (file_size(str) != -2) 
        write("Bad directory : " + str + ".\n");
    else
        current_path = str;
    write(current_path+"\n");
    return 1;
    }
 
query_path() { return current_path; }
 
static exa_file(str) {
    if (!str) {
        write("Cat what file ?\n");
        return 1;
        }
    str = get_path(str);
    if (!str) return 1;
    cat(str);
    return 1;
    }
 
static list_files(str) {
    if (!str) str = current_path;
    else str = get_path(str);
    if (!str) return 1;
    ls(str);
    return 1;
    }
 
static rm_file(str) {
    int fsize;
 
    if (!str) {
        notify_fail("remove which file ?\n");
        return 0;
        }
    str = get_path(str);
    if (!str) {
       return 1;
    }
    fsize = file_size(str);
    if (fsize == -1) {
       notify_fail("No such file or directory.\n");
       return 0;
    }
    if (fsize == -2) { 
       if (!rmdir(str)) {
          notify_fail("Couldn't rm directory.\n");
          return 0;
       }
    } else if (!rm(str)) {
       notify_fail("Can't remove file.\n");
       return 0;
    }
    write("Ok.\n");
    return 1;
}

/* This is a bit pointless now.... 
phase(dir) {
int tmp;
string *doors;
    tmp = environment(this_player())->get_door(dir);
    if (tmp == -1) {
        write("No door in that direction!\n");
        return 1;
        }
    doors = environment()->query_doors();
    if (!doors || sizeof(doors)==0) {
        write("This room is not configured correctly. Contact the creator.\n");
        return 1;
        }
    write("You phase through the "+dir+" door.\n");
    this_player()->move_player("#phases through the "+dir+" door#"+
        doors[tmp][1],"phase "+dir);
    return 1;
    }
*/
 
static tail_file(str) {
  if (!str) {
    write("Tail what file ?\n");
    return 1;
  }
  str = get_path(str);
  if (!str) return 1;
  if (!tail(str))
    write("No such file.\n");
  return 1;
}
 
list_people() {
object ob;
int i;  
string sh;

  write("This mud is running on "+query_host_name()+"\n");
  write("Some useless info : "+query_load_average()+"\n"); 
  ob = users();
  for (i=0;i<sizeof(ob);i++)
    write(ALIGN(query_ip_name(ob[i]), 20)+
           ALIGN(ob[i]->query_name(), 15)+ALIGN(ob[i]->query_level()+"", 3)+
           (environment() ? file_name(environment(ob[i]))+" "+
                getuid(environment(ob[i])):"(None)")+"\n");
  return 1;
}
 
desc_object(mixed o){
string str;
 if (!o) return "** Null-space **";
 if (str = o->short()) return str;
 if (str = o->query_name()) return str;
 return file_name(o);
}
 
/*
 * is now an efun.  have fun, win awards.
 *
print_object(object o, string s) {
   int i;
   string hand, next, prep;

   hand = "You have called print_object().  Have a nice day.";
    
   if (s) {
      prep = s;
      next = s + "  ";
   } else {
      prep = "Got ";
      next = "      ";
   }
 
   if (intp(o)) {
      write(prep + "int " + o + "\n");
      return hand;
   }
   else if (stringp(o)) {
      write(prep + "string \"" + o + "\"\n");
      return hand;
   }
   else if (objectp(o)) {
      write(prep + "object " + desc_object(o) + "\n");
      return hand;
   }
   else if (pointerp(o)) {
      write(prep + "array of " + sizeof(o) + " elements:\n");
      prep = sizeof(o);
      for (i = 0; i < prep && i < 30; i++)
         print_object(o[i], next);
      if (i < prep) 
         write(prep + "... and (too much) more ...\n");
      return hand;
   }
   write(prep + "unknown type!\n");
   return hand;
}
*/
 
std_arg(string s) {
   int i;
   string s1;
   object *ov;
 
   if (sscanf(s,"%d",i)== 1)
      return i;
 
   ov = wiz_present(s,this_player());
   if (ov && sizeof(ov)) {
      if (sizeof(ov) == 1) return ov[0];
      else return ov;
   }
   if (sscanf(s,"\"%s\"",s1)) return s1;
   return s;
}
 
wzpresent2(string str, object onobj) {
int i;
object o, obj;
string s1, s2;
 
   if (pointerp(onobj)) {
      o = ({ });
      for(i = 0; i < sizeof(onobj); i++)
         o += wzpresent2(str,onobj[i]);
      return o;
      }
   
   if (str == "all")
        return all_inventory(onobj);
 
/* every fish */
 
   if (sscanf(str,"every %s",s1) == 1) {
      obj = all_inventory(onobj); 
      o = ({ });
      for (i=0;i<sizeof(obj);i++)
         if (obj[i]->id(s1)) o += ({ obj[i] });
      return o;
   }
 
   if (obj = present(str,onobj)) return ({ obj });
   for (obj = first_inventory(onobj); obj; obj = next_inventory(obj)) {
      if (sscanf(file_name(obj), "%s"+str+"#%d", s1, i)) {
         return ({ obj });
      }
      if (sscanf(file_name(obj), "%s#%s", s1, s2) && s2 == str) {
        return ({ obj });
      }
   }
   return ({ });
}
 
wiz_present(string str, object onobj) {
int i,j;
object *o2,*o3;
string s1,s2;
 
   if (!str || !onobj) {
      return 0;
      }
 
/* all the simple ones first */
 
   if (str[0] == '@')  {
     if (o2 = find_living(
       this_player()->expand_nickname(extract(str, 1)))) return ({ o2 });
     notify_fail("Unable to find living object:"+extract(str,1)+".\n");
     return 0;
     }
 
   if (str == "me") return ({ this_player() });
 
   if (str == "here") return ({ environment(this_player()) });
 
   if (str == "everyone") return users();
 
   if (str[0] == '/') {
       if (o2 = find_object(str)) return ({ o2 });
       notify_fail("No loaded object:"+str+".\n");
       return 0;
       }
 
/* (fish) == environment of fish */
 
   if (str[0] == '(' && str[strlen(str) - 1] == ')') {
      o2 = wiz_present(extract(str,1,strlen(str) - 2),onobj);
      if (!o2) return o2;
      for (i = 0; i < sizeof(o2); i++) o2[i] = environment(o2[i]);
      return o2;
      }
 
/* handle "fish on fish2" */
 
   if (sscanf(str,"%s on %s",s1,s2) == 2 ||
       sscanf(str,"%s in %s",s1,s2) == 2) {
      o2 = wiz_present(s2, onobj);
      if (!o2) return 0;
      o3 = ({ });
      for (i = 0; i < sizeof(o2); i++) 
        o3 += wzpresent2(s1,o2[i]);
      return o3;
      }
 
/* fish and fish2 */
 
   if (sscanf(str,"%s and %s",s1,s2) == 2) {
      o2 = wiz_present(s1,onobj);
      o3 = wiz_present(s2,onobj);
      for (i=0;i<sizeof(o2);i++)  /* remove duplicates ... */
        if (member_array(o2[i], o3) < 0) o3 += ({ o2[i] });
      return o3;
      }
 
/* fish except fish2 */
 
   if (sscanf(str,"%s except %s",s1,s2) == 2) {
      o2 = wiz_present(s1,onobj);
      o3 = wiz_present(s2,onobj);
      for (i=0;i<sizeof(o3);i++)
         if ((j=member_array(o3[i], o2)) > -1)
            o2 = delete(o2,j);
      return o2;
      }
 
   if (sscanf(str,"player %s",s1)) return ({ find_player(s1) });
 
   if (!(o2 = wzpresent2(str,onobj))) {
     o2 = wzpresent2(str, environment(onobj));
   }
 
   if (o2) return o2;
 
/* check for a find_match locally */
 
   o2 = find_match(str,onobj,"dummy",1);
   if (sizeof(o2) > 0) return o2;
 
/* default to return find_living ...  */
 
   if (o2 = find_living(str)) 
      return ({ o2 });

   s1 = this_player()->expand_nickname(str);
   if (o2 = find_living(s1))
      return ({ o2 });

   return 0;
   }
 
static function(str) {
   /* call fish(x,y,z) object */
   string fn,args,os;
   string *argv;
   object *ov,retobj;
   int i;
 
   if (!str || str == "" || sscanf(str,"%s(%s) %s",fn,args,os) != 3) {
      notify_fail("USAGE : call lfun(arg[,arg[,arg...]]) object[s]\n");
      return 0;
      }
 
   if (args && args != "") {
      argv = explode(args,",");
      for(i=0; i < sizeof(argv); i++) argv[i] = std_arg(argv[i]);
      } 
   else argv = ({ });
   notify_fail("Can't find object "+os+".\n");
   ov = wiz_present(os,this_object());
 
   if (!ov) return 0;
 
   if (sizeof(argv) < 3) argv += allocate(3 - sizeof(argv));
   for (i=0; i < sizeof(ov); i++) {
      if (sizeof(ov) > 1) 
         write("*** function on " + desc_object(ov[i]) + " ***\n");
      retobj = call_other(ov[i],fn,argv[0],argv[1],argv[2]);
      write("Returned: ");
      print_object(retobj);
      }
   return 1;
   }
 
static makedir(string str) {
 
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
}
 
static removedir(str) {
   int fsize;
 
   if (!str || str == "") {
      notify_fail("Remove what dir?\n");
      return 0;
   }
   str = get_path(str);
   if (!str)
      return 1;
 
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
      notify_fail("Couldn't remove dir.\n");
      return 0;
   }
   write("Ok.\n");
   return 1;
}

static chmoddir(arg) {
}

static chowndir(arg) {
  if (!MASTER->high_programmer(this_player()->query_name())) return 0;
  
}
 
static shutdown_game(arg) {
        if (!arg) {
                write("Argument required.\n");
                return 1;
                }
        write("Ok... shutting down game... \n");
        log_file("GAME_LOG", "Game shutdown by "+
        this_player()->query_name()+" at "+ctime(time())+
        ": "+arg+"\n");
        shutdown();
        return 1;
        }
 
static local_cmds() {
        write("Current local commands:\n");
        localcmd();
        return 1;
        }
 
show_stats(str) {
object ob;
int i;

    str = expand_nickname(str);
    ob = find_living(str);
    if (!ob) {
        write("No such living thing.\n");
        return 1;
        }
    ob = ob->stats();
    for (i=0;i<sizeof(ob);i++)
        if (ob[i][1])
            write(ob[i][0]+": "+ob[i][1]+"\n");
    return 1;
    }
 
set_home_dir(str) {
    if (str) home_dir = get_path(str);
    write("Home directory set to "+home_dir+".\n");
    return 1;
    }
 
home() {
    if (!home_dir) {
        write("Home directory not set. Use \"homedir <path>\".\n");
        return 1;
        }
    teleport(home_dir+"/workroom");
    return 1;
    }
 
display_wl(str) {
    if (!str) str = this_player()->query_name();
    wizlist(str);
   return 1;
   }
 
do_trace(arg) {
  string *args;
  int i, j, trace_arg;
/* ok lets make this clever...
 */
  if (!arg) {
    notify_fail("Usage: trace <off|call|call_other|return|args|exec|heart_beat|apply|obj_name|number>\n");
    return 0;
  }
  args = explode(arg, " "); 
  if (!sizeof(args)) {
    notify_fail("Usage: trace <off|call|call_other|return|args|exec|heart_beat|apply|obj_name|number>\n");
    return 0;
  }
  for (i=0;i<sizeof(args);i++)
    if ((j=member_array(args[i], ({ "off", "call", "call_other", "return",
                                    "args", "exec", "heart_beat", "apply",
                                    "obj_name" }))) != -1)
      if (!j) {
        trace(0);
        write("Trace off.\n");
        return 1;
      } else
        trace_arg |= ({ 0, 1, 2, 4, 8, 16, 32, 64, 128 })[j];
    else
      if (sscanf(args[i], "%d", j))
        trace_arg |= j;
  trace(trace_arg);
  write("Trace level : "+trace_arg+"\n");
  return 1;
}

int trans(string str) {
object ob;
    str = (string)this_player()->expand_nickname(str);
    if (!str || !(ob = find_living(str))) {
        write("Transport who ?\n");
	return 1;
	}
    ob->event_say(this_object(),
        "A service shaft opens in the wall and you are grabed by two large "+
        "burly service men.\n");
    ob -> move_player("X", file_name(environment(this_player())));
    return 1;
    }

int earmuffs(string str) {
  if (!str) str = "off";
  if (earmuffs && str == "on") {
    write("You already have earmuffs on.\n");
    return 1;
  } else if (!earmuffs && str == "off") {
    write("You don't have earmuffs on.\n");
    return 1;
  }
  if (str == "on") {
    earmuffs = 1;
    write("Ok. Earmuffs on.\n");
  } else if (str == "off") {
    earmuffs = 0;
    write("Ok. Earmuffs off.\n");
  } else
    write("Use \"on\" or \"off\".\n");
  return 1;
}

query_earmuffs() { return earmuffs; }

static indent(str) {
    if (!str) {
        notify_fail("Indent which file ?\n");
        return 0;
        }
    str = get_path(str);
    if (!str) return 1;
    cindent(str);
    write("ok.\n");
    return 1;
    }
