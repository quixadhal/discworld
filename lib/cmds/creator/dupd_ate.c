/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: dupd_ate.c,v 1.1 1998/01/06 05:25:41 ceres Exp $
 * $Log: dupd_ate.c,v $
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* dupdate command by wodan */

inherit "/cmds/base";

#include <creator.h>

string other;

int do_it(string str){
  object ob, *obs;
  string *all, *dil, file, fnob /* FileName ob */;

  obs = WIZ_PRESENT->wiz_present(str, this_player());
  if(!sizeof(obs))
    return 0;
  foreach(ob in obs){
    fnob = file_name(ob);
    all = inherit_list(ob);
    foreach(file in all){
      if (other){
        dil = deep_inherit_list(find_object(file));
        if (!((member_array(other, dil) != -1) || other == file))
          continue;
      }
      do_it(file);
    }
    ob->dest_me();
    if (ob)
      ob->dwep();
    if (ob)
      destruct(ob);
    ob = load_object(fnob);
    printf("Updated %s.\n", WIZ_PRESENT->desc_f_object(ob));
  }
  return 1;
}

int cmd(string str){
  object *tmp;

  other = 0;
  notify_fail("usage: dupdate <object> [<object>]\n");
  if (!str)
    return 0;
  sscanf(str, "%s %s", str, other);
  if (other){
    tmp = WIZ_PRESENT->wiz_present(other, this_player());
    if (sizeof(tmp)){
      other = file_name(tmp[0]);
      other += ".c";
    } 
    else
      return 0;
  }
  return do_it(str);
}




