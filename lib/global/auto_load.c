string *create_auto_load(object *obs) {
  string s, s1, s3;
  mixed *auto_string;
  int j, i;

  auto_string = ({ });
  for (i=0;i<sizeof(obs);i++) {
    s = s3 = 0;
    if (!catch(s=(string)obs[i]->query_auto_load()) && !s) {
      catch(s = (string)obs[i]->query_static_auto_load());
      catch(s3 = (string)obs[i]->query_dynamic_auto_load());
      if (!s && !s3)
        continue;
    } else if (!s)
      continue;
    else
      s3 = 0;
    if (sscanf(file_name(obs[i]), "%s#%d", s1, j) == 2)
      auto_string += ({ 1, s1, ({ s, s3 }) });
    else
      auto_string += ({ 0, s1, ({ s, s3 }) });
  }
  return auto_string;
} /* create_auto_load() */

void birthday_gifts() {
  int day;
  string month, s1, s2;

  if(!sizeof(find_match("birthday card", this_object())))
    if(this_object()->query_is_birthday_today())
      if(!catch(call_other("/obj/b_day/card", "??")) &&
         !catch(call_other("/obj/b_day/demon", "??"))) {
        call_out("card_arrives", 5);
      }
  sscanf(ctime(time()), "%s %s %d %s", s1, month, day, s2);
  switch (month) {
/* Special day processing */
    case "Feb" :
      switch (day) {
        case 17 :
/* world cabbage day */
          call_out("auto_clone", 0,
                    ({ clone_object("/global/special/cabbage"), ({ "bing" }),
                       this_object() }));
          this_object()->add_message(this_object(), 
                                     "%^GREEN%^Its world cabbage day!!!! "+
                                     "eat, be merry.\nPS:  Have a free "+
                                     "cabbage.\n");
          break;
      }
      break;
  }
} /* birthday_gifts() */

int query_special_day(string type) {
  string s1, s2, month;
  int day;

  sscanf(ctime(time()), "%s %s %d %s", s1, month, day, s2);
  switch (type) {
    case "cabbage day" :
      if (month == "Feb" && day == 17)
        return 1;
      return 0;
    case "test day" :
      if (month == "Feb" && day == 16)
        return 1;
      return 0;
  }
  return 0;
} /* query_day_special() */

void card_arrives() {
  object card;
  
  card = (object)"/global/cloner"->clone("/obj/b_day/card");
  card->move(this_object());
  tell_room(environment(), "You hear a rumbling in the distance. " +
      "Then, suddenly, a malformed goblin wizzes past you, " +
      "frantically pedalling a fire-engine red tricycle!\n");
  write("You feel something thrust into your hand by a greener, " +
      "wartier one.\n");
} /* card_arrives() */

object *load_auto_load(string *auto_string, object dest) {
  object ob, card, *obs;
  string name, args;
  int i;

  obs = ({ });
  if (stringp(auto_string))
    return ({ });
  if (!auto_string || !sizeof(auto_string))
    return ({ });
  for (i=0;i<sizeof(auto_string);i+=3)
    if (auto_string[i]) {
      if (!catch(call_other(auto_string[i+1], "??"))) {
        catch(ob =(object)"/global/cloner"->clone(auto_string[i+1]));
        if (ob) {
          call_out("auto_clone", 0, ({ ob,
                                 auto_string[i+2], dest }));
          obs += ({ ob });
        }
      }
    } else if (!find_object(auto_string[i+1]) && 
               !catch(call_other(auto_string[i+1], "??"))) {
      ob = find_object(auto_string[i+1]);
      call_out("auto_clone", 0, ({ ob, auto_string[i+2], dest }));
      obs += ({ ob });
    }
  return obs;
} /* load_auto_load() */

void auto_clone(mixed arg) {
  if (sizeof(arg[1]) == 1 || !arg[1][1]) {
    arg[0]->init_arg(arg[1][0]);
  } else {
    arg[0]->init_static_arg(arg[1][0]);
    arg[0]->init_dynamic_arg(arg[1][1]);
  }
  arg[0]->move(arg[2]);
} /* auto_clone() */
