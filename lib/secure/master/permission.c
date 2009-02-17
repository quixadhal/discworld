/*  -*- LPC -*-  */
/*
 * $Id: permission.c,v 1.12 2003/03/21 02:25:20 ceres Exp $
 */

int add_senior( string word ) {
   if ( !sizeof( filter( previous_object( -1 ), (: interactive( $1 ) :) ) ) ) {
      user_event( "inform", this_player()->query_name() +
            " illegally attempted to call add_senior( "+ word +
            " )", "cheat" );
      unguarded((: write_file, "/log/CHEAT", ctime( time() ) +
            ": illegal attempt to call add_senior( "+ word +
            " ).\n"+ back_trace() :));
      return 0;
   }
   if ( PLAYER_HANDLER->test_user( word ) &&
        query_leader( previous_object(-1) ) ) {
      write_file( "/log/PROMOTIONS", ctime( time() ) +": "+ word +
            " was promoted to Senior by "+ geteuid( this_interactive() ) +
            "\n" );
      if ( ( positions[ word ] != TRUSTEE ) &&
            ( positions[ word ] != DIRECTOR ) )
         positions[ word ] = SENIOR;
      save_object( "/secure/master" );
      return 1;
   }
   return 0;
} /* add_senior() */

int remove_senior(string str) {
  if (!sizeof(filter(previous_object(-1), (: interactive($1) :)))) {
    user_event( "inform", this_player()->query_cap_name()+
                " illegally attempted to call remove_senior("+str+")",
                "cheat");
    unguarded((: write_file, "/log/CHEAT", ctime(time())+": Illegal attempt "
                 "to call remove_senior("+str+").\nBacktrace: "+
                 back_trace() :));
    return 0;
  }
  if (query_leader(previous_object(-1)) &&
      positions[str] == SENIOR) {
    write_file("/log/DEMOTIONS", ctime(time())+": "+str+
               " was demoted from Senior by "+geteuid(this_interactive())+
               "\n");
    map_delete(positions, str);
    save_object("/secure/master");
    return 1;
  }
  return 0;
} /* remove_senior() */

int add_director(string str) {
  if (!sizeof(filter(previous_object(-1), (: interactive($1) :)))) {
    user_event( "inform", this_player()->query_cap_name()+
                " illegally attempted to call add_director("+str+")",
                "cheat");
    unguarded((: write_file, "/log/CHEAT", ctime(time())+
               ": Illegal attempt to call add_director("+str+
               ").\nBacktrace: "+ back_trace() :));
    return 0;
  }
  if (PLAYER_HANDLER->test_user(str) &&
      query_trustee(previous_object(-1))) {
    write_file("/log/PROMOTIONS", ctime(time())+": "+str+
               " was promoted to Leader by "+ geteuid(this_interactive())+
               "\n");
    if (positions[str] != TRUSTEE)
      positions[str] = DIRECTOR;
    save_object("/secure/master");
    return 1;
  }
  return 0;
}

int remove_director(string str) {
  if (!sizeof(filter(previous_object(-1), (: interactive($1) :)))) {
    user_event( "inform", this_player()->query_cap_name()+
                " illegally attempted to call remove_director("+str+")",
                "cheat");
    unguarded((: write_file, "/log/CHEAT", ctime(time())+
               ": Illegal attempt to call remove_director("+str+
               ").\nBacktrace: "+ back_trace() :));
    return 0;
  }
  if (query_trustee(previous_object(-1)) &&
      positions[str] == DIRECTOR) {
    write_file("/log/DEMOTIONS", ctime(time())+": "+str+
               " was demoted from Leader by "+geteuid(this_interactive())+
               "\n");
    map_delete(positions, str);
    save_object("/secure/master");
    return 1;
  }
  return 0;
}

int add_trustee(string str) {
  if (!sizeof(filter(previous_object(-1), (: interactive($1) :)))) {
    user_event( "inform", this_player()->query_cap_name()+
                " illegally attempted to call add_tristee("+str+")",
                "cheat");
    unguarded((: write_file, "/log/CHEAT", ctime(time())+": Illegal attempt "
                 "to call add_trustee("+str+").\nBacktrace: "+
                 back_trace() :));
    return 0;
  }
  if (PLAYER_HANDLER->test_user(str) &&
      query_trustee(previous_object(-1))) {
    write_file("/log/PROMOTIONS", ctime(time())+": "+str+
               " was promoted to Administrator by "+
               geteuid(this_interactive())+"\n");
    positions[str] = TRUSTEE;
    save_object("/secure/master");
    return 1;
  }
  return 0;
}

int remove_trustee(string str) {
  if (!sizeof(filter(previous_object(-1), (: interactive($1) :)))) {
    user_event( "inform", this_player()->query_cap_name()+
                " illegally attempted to call remove_trustee("+str+")",
                "cheat");
    unguarded((: write_file, "/log/CHEAT", ctime(time())+": Illegal attempt "
                 "to call remove_trustee("+str+").\nBacktrace: "+
                 back_trace() :));
    return 0;
  }
  if (query_trustee(previous_object(-1)) &&
      positions[str] == TRUSTEE) {
    write_file("/log/DEMOTIONS", ctime(time())+": "+str+
               " was demoted from Administrator by "+
               geteuid(this_interactive())+"\n");
    map_delete(positions, str);
    save_object("/secure/master");
    return 1;
  }
  return 0;
}

varargs mixed creator_file(string path, int author);

nomask protected int check_domain(mixed ob, string func, string path, int mask) {
  object master_ob;
  string master, domain, *bits = explode(path, "/") - ({ "", "." });
  int ret;

  if (sizeof(bits) < 2)
    return (mask & READ_MASK);
  
  domain = bits[1];
  if (objectp(ob))
    ob = geteuid(ob);

  /* Is it the domain itself? */
  if (ob == creator_file(path))
    return 1;

  /* Check the domain master object, if any. */
  master = "/d/" + domain + "/master";
  if (!(master_ob = find_object(master)) && checked_master[master]) {
    return (mask & READ_MASK);
  }
  if (!master_ob && !checked_master[master] &&
      catch(master_ob = load_object(master))) {
    checked_master[master] = 1;
  }
  if (master_ob) {
    ret = master_ob->check_permission(ob, bits, mask);
    if (ret == -1) {
      /* This means the path was locked. We deny access. */
      return 0;
    }
    /* Senior Creators have access to the domains, except when explicitly
       locked. */
    if ((sizeof(bits) > 2) && (bits[2] != "master.c") &&
        (bits[2] != "master.o") && (bits[2] != "master") && query_senior(ob)) {
      return 1;
    }
    
    return (ret || (mask & READ_MASK?master_ob->valid_read(bits, ob, func):
            master_ob->valid_write(bits, ob, func)));
  }
  return (mask & READ_MASK);
}

nomask protected int check_creator(mixed ob, string func, string path, int mask) {
  string master, creator, *bits = explode(path, "/") - ({ "", "." });
  object master_ob;

  if (sizeof(bits) < 2)
    return (mask & READ_MASK);
  
  creator = bits[1];
  if (objectp(ob))
    ob = geteuid(ob);

  /* Creators are not allowed to give out write perms to their dirs using
     the granting system, they should write their own master object if
     they want to do this. */
  /* Err, why this?  Why not use the standard system?
   * - pf
   */
  if (mask & GRANT_MASK) {
    if (ob == creator) {
      return 1;
    } else {
      return 0;
    }
  }

  /* The mbox file is private. Only the owner can read it. */
  if ((mask & READ_MASK) && sizeof(bits) >= 3 && bits[2] == "mbox" &&
      file_size(sprintf("/w/%s/mbox", bits[1])) != -2)
    return ob == bits[1];

  /* Reading is allowed in creator dirs, and writing is allowed if it's
     the owner doing it. */
  if ((mask & READ_MASK) || (ob == creator_file(path)))
    return 1;

  /* Check the master object, if any. */
  master = "/w/" + creator + "/master";
  if (!(master_ob = find_object(master)) && checked_master[master])
    return 0;
  if (!master_ob && !checked_master[master] &&
      catch(master_ob = load_object(master)))
    checked_master[master] = 1;
  if (master_ob)
    return (master_ob->check_permission(ob, bits, mask) ||
            master_ob->valid_write(bits, ob, func));
  return 0;
}

/* Permision handling stuff. Originally coded by who knows who.
   This now uses a system based on the previous_object() stack.
   Coded by Turrican, based on code in the Nightmare mudlib.
   - First working version on 7-10-96
   - Hacked some more on 4-4-97 */
private int check_permission(mixed ob, string func, string path,
                             mapping perms, int mask) {
  string tmp, euid;
  int i;
  mixed *stack;
  
  if (perms && sizeof(perms) && !undefinedp(perms["all"]) &&
      (perms["all"] & mask))
    return 1;

  if(path[0..21] == "/save/boards/lordboard" &&
     base_name(ob) != "/obj/handlers/board_handler") {
    catch(log_file("/d/admin/log/LORDBOARD",
                   "%s: ob %s player %s prev %s\n",
                   ctime(time())[4..18], base_name(ob),
                   this_player()->query_name(),
                   base_name(previous_object()))); 
  }
  
  if (unguarded_ob == ob) {
    if ((tmp = base_name(ob)) == "/global/player" ||
        tmp == "/global/playtester" ||
        tmp == "/global/creator" ||
        tmp == "/global/lord") {
      if((path == "/save/players/"+ob->query_name()[0..0]+"/"+ob->query_name())
         ||
         (path == "/save/players/"+ob->query_name()[0..0]+"/"+ob->query_name()+
          ".o") ||
         (path == "/save/players/"+ob->query_name()[0..0]+"/"+ob->query_name()+
          ".o.gz") ||
         (path == "/save/ramdisk/players/"+ob->query_name()[0..0]+"/"+ob->query_name()) ||
         (path == "/save/ramdisk/players/"+ob->query_name()[0..0]+"/"+ob->query_name()+
          ".o") ||
         (path == "/save/ramdisk/players/"+ob->query_name()[0..0]+"/"+ob->query_name()+
          ".o.gz"))
        return 1;
      else i = sizeof(stack = ({ ob }) + previous_object(-1));
    } else if (tmp == path)
      return 1;
    else
      i = sizeof(stack = ({ ob }));
  } else if (unguarded_ob && base_name(ob) == "/secure/simul_efun") {
    if (unguarded_ob == previous_object(1))
      i = sizeof(stack = ({ previous_object(1) }));
    else
      i = sizeof(stack = ({ ob }) + previous_object(-1));
  } else if (unguarded_ob) {
    /* Okay, unguarded object is not the calling object. We only check
       the call stack as far back as the position of the unguarded
       object. */
    stack = previous_object(-1);
    for (i = 0; i < sizeof(stack) && stack[i] != unguarded_ob; i++)
      ;
    i = sizeof(stack = ({ ob }) + stack[0..i]);
  } else
    i = sizeof(stack = ({ ob}) + previous_object(-1));
//  tell_object(find_player("turrican"),
//              sprintf("stack = %O, perms = %O, unguarded_ob = %O, func = %s\n",
//                      stack, perms, unguarded_ob, func));
  while (i--) {
    if (!stack[i])
      return 0;
    if (stack[i] == this_object())
      continue;
    if (objectp(stack[i])) {
      if (file_name(stack[i]) == "/secure/simul_efun")
        continue;
      if (!(euid = geteuid(stack[i])))
        return 0;
    } else
      euid = stack[i];
    if (euid == get_root_uid())
      continue;
    if (query_director(euid) && (mask & READ_MASK))
      continue;
    if (query_trustee(euid))
      continue;
    if (perms) {
      if (!undefinedp(perms[euid]) && (perms[euid] & mask))
        continue;

      /* If the path is explicitly locked, and the lock isn't overridden
         by other pemissions, we deny access. */
      if (!undefinedp(perms["all"]) && (perms["all"] & LOCK_MASK))
        return 0;
    }

    /* Is this a creator directory? */
    if (path[0..2] == "/w/") {
      if (check_creator(stack[i], func, path, mask))
        continue;
    } else if(path[0..2] == "/d/") {
      /* It's a domain directory. */
      if (check_domain(stack[i], func, path, mask))
        continue;
    } else {
      /* The rest of the mudlib defaults to reading allowed, unless
         paths are explitcitly locked, which is handled above. */
      return (mask & READ_MASK);
    }
    return 0;
  }
  return 1;
} /* check_permission() */

mixed permission_match_path(mapping m, string path) {
  string p, *bits;
  int i, size;
  mapping found = ([]);

  if (!sizeof(m)) {
    return 0;
  }
  bits = explode(path, "/") - ({ "", "." });
  p = "";

  if (!undefinedp(m["/"])) {
    found += m["/"];
  }

  size = sizeof(bits);
  for (i = 0; i <= size; i++) {
    if (!undefinedp(m[p])) {
      mapping old = copy(found);

      if (sizeof((found += m[p])) != (sizeof(old) + sizeof(m[p]))) {
        string euid;
        int mask;
        
        found = old;
        foreach (euid, mask in m[p]) {
          if (!undefinedp(found[euid]))
            found[euid] |= mask;
          else
            found[euid] = mask;
        }
      }
    }
    if (i < size)
      p = p + "/" + bits[i];
  }
  if (sizeof(found)) {
    return found;
  } else {
    return 0;
  }
} /* permission_match_path() */

int valid_grant(object euid, string path, int mask) {
  string domain, master, director;
  int result;
  object master_ob;

  if (path[0] != '/')
    path = "/" + path;
  result = check_permission(euid, 0, path,
                            permission_match_path(permissions, path),
                            GRANT_MASK);
  if (!result || (mask & (READ_MASK|WRITE_MASK)))
    return result;
  if (sscanf(path, "/d/%s/%*s", domain) != 2 &&
      sscanf(path, "/d/%s", domain) != 1)
    return 0;
  master = "/d/" + domain + "/master";
  if (!(master_ob = find_object(master)) && checked_master[master])
    return 0;
  if (!master_ob && !checked_master[master] &&
      catch(master_ob = load_object(master))) {
    checked_master[master] = 1;
    return 0;
  }
  director = master->query_director();
  if(!director)
    director = master->query_lord();
  return (query_director(previous_object(-1) + ({ euid })) &&
          (member_array(director, map(previous_object(-1),
                                  (: geteuid($1) :))) != -1));
}

mapping query_permissions() {
  string *doms, master;
  int i;
  mapping blue, tmp;
  object master_ob;

  doms = get_dir("/d/");
  doms -= ({ "lost+found" });
  blue = ([ ]);
  for (i=0;i<sizeof(doms);i++) {
    master = "/d/"+doms[i]+"/master";
    if (!(master_ob = find_object(master)) && checked_master[master])
      continue;
    if (!master_ob && !checked_master[master] &&
        catch(master_ob = load_object(master))) {
      checked_master[master] = 1;
      continue;
    }
    tmp = (mapping)master_ob->query_access();
    if (mapp(tmp))
      blue += tmp;
  }
  return permissions + blue;
} /* query_permissions() */

protected int add_permission(string euid, string path, int mask) {
  string *bits, master;
  object master_ob;

  if (path[0..2] == "/d/") {
    /* A domain...  */
    bits = explode(path, "/");
    if (sizeof(bits) >= 2) {
      master = "/d/"+bits[1]+"/master";
      if (!(master_ob = find_object(master)) && checked_master[master])
        return 0;
      if (!master_ob && !checked_master[master] &&
          catch(master_ob = load_object(master))) {
        checked_master[master] = 1;
        return notify_fail("Failed to load master file.\n");
      }
      if ((mask & LOCK_MASK) &&
          !sizeof(filter(previous_object(-1),
                      (: $(master_ob)->query_lord() == geteuid($1) :))))
        return notify_fail("You are not the leader of $C$" + bits[1] +
                           ".\n");
      return (int)master_ob->add_permission(euid, path, mask);
    }
  }
  if (!permissions[path]) {
    permissions[path] = ([ euid : mask ]);
  } else {
    permissions[path][euid] |= mask;
  }
  unguarded((: save_object, "/secure/master" :));
  return 1;
} /* add_permission() */

int add_read_permission(string euid, string path) {
  if ( base_name(previous_object()) != "/d/admin/room/access_control" &&
       base_name(previous_object()) != "/cmds/creator/perm_it") {
    user_event( "inform", this_interactive()->query_cap_name()+
                " illegally attempted to call add_read_permission("+euid+", "+
                path+")", "cheat");
    unguarded((: write_file, "/log/CHEAT", ctime(time())+": Illegal attempt "
               "to call add_read_permission("+euid+", "+path+").\nBacktrace: "+
               back_trace() :));
    return 0;
  }
  if (add_permission(euid, path, READ_MASK)) {
    write("Added read permision for "+euid+" to "+path+".\n");
    return 1;
  }
  return 0;
} /* add_read_permission() */

int add_write_permission(string euid, string path) {
  if ( base_name(previous_object()) != "/d/admin/room/access_control" &&
       base_name(previous_object()) != "/cmds/creator/perm_it") {
    user_event( "inform", this_player(1)->query_cap_name()+
                " illegally attempted to call add_write_permission("+euid+", "+
                path+")", "cheat");
    unguarded((: write_file, "/log/CHEAT", ctime(time())+": Illegal attempt "
                 "to call add_write_permission("+euid+", "+path+").\n"
                 "Backtrace: "+ back_trace() :));
    return 0;
  }
  if (add_permission(euid, path, WRITE_MASK)) {
    write("Added write permision for "+euid+" to "+path+".\n");
    return 1;
  }
  return 0;
} /* add_write_permission() */

int add_grant_permission(string euid, string path) {
  if ( base_name(previous_object()) != "/d/admin/room/access_control" &&
       base_name(previous_object()) != "/cmds/creator/perm_it") {
    user_event( "inform", this_player(1)->query_cap_name()+
                " illegally attempted to call add_grant_permission("+euid+", "+
                path+")", "cheat");
    unguarded((: write_file, "/log/CHEAT", ctime(time())+": Illegal attempt "
                 "to call add_grant_permission("+euid+", "+path+").\n"
                 "Backtrace: "+ back_trace() :));
    return 0;
  }
  if (add_permission(euid, path, GRANT_MASK)) {
    write("Added grant permision for "+euid+" to "+path+".\n");
    return 1;
  }
  return 0;
} /* add_grant_permission() */

int lock_path(string path) {
  if ( base_name(previous_object()) != "/d/admin/room/access_control" &&
       base_name(previous_object()) != "/cmds/creator/perm_it") {
    user_event("inform", this_player(1)->query_cap_name()+
               " illegally attempted to call lock_path("+path+")", "cheat");
    unguarded((: write_file, "/log/CHEAT", ctime(time())+": Illegal attempt "
               "to call lock_path("+path+").\nBacktrace: "+ back_trace() :));
    return 0;
  }
  if (add_permission("all", path, LOCK_MASK)) {
    write("Restricted access for all to "+path+".\n");
    return 1;
  }
  return 0;
} /* lock_path() */

protected int remove_permission(string euid, string path, int mask) {
  string *bits, master;
  object master_ob;

  if (path[0..2] == "/d/") {
    /* A domain...  */
    bits = explode(path, "/");
    if (sizeof(bits) >= 2) {
      master = "/d/"+bits[1]+"/master";
      if (!(master_ob = find_object(master)) && checked_master[master])
        return 0;
      if (!master_ob && !checked_master[master] &&
          catch(master_ob = load_object(master))) {
        checked_master[master] = 1;
        return notify_fail("Failed to load master file.\n");
      }
      if ((mask & LOCK_MASK) &&
          !sizeof(filter(previous_object(-1),
                         (: $(master_ob)->query_lord() == geteuid($1) :))))
        return notify_fail("You are not the lord of $C$" + bits[1] + ".\n");
      return (int)master_ob->remove_permission(euid, path, mask);
    }
  }
  if (!permissions[path] || !permissions[path][euid]) {
    notify_fail("The euid \""+euid+"\" does not have any permissions to "
                "remove in "+path+".\n");
    return 0;
  }
  permissions[path][euid] &= ~mask;
  if (!permissions[path][euid]) {
    if (m_sizeof(permissions[path]) == 1) {
      map_delete(permissions, path);
    } else {
      map_delete(permissions[path], euid);
    }
  }
  unguarded((: save_object, "/secure/master" :));
  return 1;
} /* remove_permission() */

int remove_read_permission(string euid, string path) {
  if ( base_name(previous_object()) != "/d/admin/room/access_control" &&
      base_name(previous_object()) != "/cmds/creator/perm_it") {
    user_event( "inform", this_player(1)->query_cap_name()+
                " illegally attempted to call remove_read_permission("+euid+
                ", "+path+")", "cheat");
    unguarded((: write_file, "/log/CHEAT", ctime(time())+": Illegal attempt "
               "to call remove_read_permission("+euid+", "+path+").\n"
               "Backtrace: "+ back_trace() :));
    return 0;
  }
  if (remove_permission(euid, path, READ_MASK)) {
    write("Removed read permision for "+euid+" to "+path+".\n");
    return 1;
  }
  return 0;
} /* remove_read_permission() */

int remove_write_permission(string euid, string path) {
  if ( base_name(previous_object()) != "/d/admin/room/access_control" &&
       base_name(previous_object()) != "/cmds/creator/perm_it") {
    user_event( "inform", this_player(1)->query_cap_name()+
                " illegally attempted to call remove_write_permission("+euid+
                ", "+path+")", "cheat");
    unguarded((: write_file, "/log/CHEAT", ctime(time())+": Illegal attempt "
               "to call remove_write_permission("+euid+", "+path+").\n"
               "Backtrace: "+ back_trace() :));
    return 0;
  }
  if (remove_permission(euid, path, WRITE_MASK)) {
    write("Removed write permision for "+euid+" to "+path+".\n");
    return 1;
  }
  return 0;
} /* remove_write_permission() */

int remove_grant_permission(string euid, string path) {
  if ( base_name(previous_object()) != "/d/admin/room/access_control" &&
       base_name(previous_object()) != "/cmds/creator/perm_it") {
    user_event( "inform", this_player(1)->query_cap_name()+
                " illegally attempted to call remove_grant_permission("+euid+
                ", "+path+")", "cheat");
    unguarded((: write_file, "/log/CHEAT", ctime(time())+": Illegal attempt "
               "to call remove_grant_permission("+euid+", "+path+").\n"
               "Backtrace: "+ back_trace() :));
    return 0;
  }
  if (remove_permission(euid, path, GRANT_MASK)) {
    write("Removed grant permision for "+euid+" to "+path+".\n");
    return 1;
  }
  return 0;
} /* remove_grant_permission() */

int unlock_path(string path) {
  if ( base_name(previous_object()) != "/d/admin/room/access_control" &&
       base_name(previous_object()) != "/cmds/creator/perm_it") {
    user_event("inform", this_player(1)->query_cap_name()+
               " illegally attempted to call unlock_path("+path+")", "cheat");
    unguarded((: write_file, "/log/CHEAT", ctime(time())+": Illegal attempt "
               "to call unlock_path("+path+").\nBacktrace: "+ back_trace() :));
    return 0;
  }
  if (remove_permission("all", path, LOCK_MASK)) {
    write("Removed access restrictions for all to "+path+".\n");
    return 1;
  }
  return 0;
} /* unlock_path() */
