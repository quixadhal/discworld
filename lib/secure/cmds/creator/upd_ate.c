/*  -*- LPC -*-  */
/*
 * $Id: upd_ate.c,v 1.4 1999/12/09 07:17:32 pinkfish Exp $
 */

#include <creator.h>

inherit "/cmds/base";

protected int do_update(object *ov) {
   string pname;
   object *invent;
   object rsv;
   object env;
   object dup;
   object loaded;
   object ob;
   mixed static_arg;
   mixed dynamic_arg;
   int i;
   
   rsv = load_object("room/void");  /* RSV = Room Slash Void */
   if (!rsv) { /* Die in horror */
      return notify_fail("The void is lost!\n");
   }
 
   foreach (ob in ov) {
      if (!ob) {
         continue;
      }
      if (interactive(ob)) {
         write("Can't update interactive object: " + ob->query_name() + "\n");
         continue;
      }
      env = environment(ob);
      invent = all_inventory(ob);

      for (i = 0; i < sizeof(invent); i++) {
         if (userp(invent[i]) ||
             invent[i]->query_property("unique")) {
             invent[i]->move(rsv);
         } else {
             invent = delete(invent, i--, 1);
         }
      }
  
      pname = file_name(ob);
      if (sscanf(pname, "%s#%*d", pname) != 2) { /* a room ? */
         static_arg = ob->query_static_auto_load();
         dynamic_arg = ob->query_dynamic_auto_load();
         ob->dest_me();
         if (ob) {
            ob->dwep();
         }
         if (ob) {
            destruct(ob);
         }
         if (ob) {
            return notify_fail("I can't seem to destruct the old object.\n");
         }
         ob = load_object(pname);
         if (static_arg) {
             ob->init_static_arg(static_arg);
         }
         if (dynamic_arg) {
            ob->init_dynamic_arg(dynamic_arg);
         }
      } else {
         loaded = find_object(pname);
         static_arg = ob->query_static_auto_load();
         dynamic_arg = ob->query_dynamic_auto_load();
         if (loaded) {
            loaded->dest_me();
         }
         if (loaded) {
            loaded->dwep();
         }
         if (loaded) {
            destruct(loaded);
         }
  
         dup = clone_object(pname);
         if (dup && ob) {
             ob->dest_me();
             if (ob) {
                ob->dwep();
             }
             if (ob) {
                destruct(ob);
            }
            ob = dup;
            if (static_arg) {
                ob->init_static_arg(static_arg);
            }
            if (dynamic_arg) {
                ob->init_dynamic_arg(dynamic_arg);
            }
         }
      }
      
      if (!ob) {
         printf("I seem to have lost your object.\n");
         return 1;
      }
  
      for (i = 0; i < sizeof(invent); i++) {
         if (invent[i]) {
            invent[i]->move(ob);
         }
      }
        
      if (env) {
         ob->move(env);
      }
      printf("Updated %s.\n", WIZ_PRESENT->desc_f_object(ob));
   }
   return 1;
} /* do_update() */

mixed cmd(string str) {
   string tring;
   string* filenames;
   string err;
   object ob;
   object* val;
   object* obs;
   int loop;
 
   notify_fail("No such object.\n");
   tring = str;
   if (!str || str == "here") { 
      str = file_name(environment(this_player()));
      if (str == "/room/void") {
         notify_fail("The Surgeon General warns you that it is hazardous to "
                     "update the void while standing in it.\n");
         return 0;
      }
      str = base_name(str);
      filenames = ({ "/" + str });
      str = "here";
   } else {
      filenames = this_player()->get_cfiles(str);
   }
   if (sizeof(filenames) == 0) {
      val = WIZ_PRESENT->wiz_present(tring, this_player());
      if (!sizeof(val)) {
         notify_fail("No matching objects/filenames\n");
         return 0;
      }
      return do_update(val);
   }
   obs = ({ });
   for (loop = 0; loop < sizeof(filenames); loop++) {
      str = filenames[loop];
      ob = find_object(str);
      if (!ob) {
          if (file_size(str) >= 0) {
             if (!(err = catch(load_object(str)))) {
                 printf("Loaded %s\n", str);
             } else {
                 printf("Failed to load %s, error: %s\n", str, err);
             }
          } else {
              val = WIZ_PRESENT->wiz_present(tring, this_player());
              obs += val;
          }
      } else {
          obs += ({ ob });
      }
   }
   if (!obs) {
      return 0;
   } else {
      return do_update(obs);
   }
} /* cmd() */
