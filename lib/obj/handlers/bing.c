inherit "std/object";

#include "login_handler.h"

#define SAVE_NAME "/save/login_handler"
/* 4 days.  I hope... */
#define TIME_OUT (60*60*24*4)

string *names;
mixed *functions;
mixed *objects;
mixed *alignment;
static string *players_on;

void update_alginment(string name, int align);

void create() {
  names = ({ });
  functions = ({ });
  seteuid("Room");
  objects = ({ });
  alignment = ({ allocate(10), allocate(10) });
  restore_object(SAVE_NAME);
  players_on = ({ });
} /* create() */

int add_login_call(string player, mixed function, mixed obj) {
  int pos, retval;

  if((pos = member_array(player, names)) != -1) {
    if(pointerp(function) && pointerp(obj)) {
      if(sizeof(function) != sizeof(obj))
        return 0;
      functions[pos] += function;
      objects[pos] += obj;
    }
    else {
      if(pointerp(function) || pointerp(obj))
        return 0;
      if(!functions) {
        functions = ({ });
        objects = ({ });
      }
      functions[pos] += ({ function });
      objects[pos] += ({ obj });
    }
  }
  else {
    if(pointerp(function) && pointerp(objects)) {
      if(sizeof(function) != sizeof(obj))
        return 0;
      if(!functions) {
        functions += ({ });
        objects += ({ });
      }
      names += ({ player });
      functions += ({ function });
      objects += ({ obj });
    }
    else {
      if(pointerp(function) || pointerp(obj))
        return 0;
      names += ({ player });
      functions += ({ ({ function }) });
      objects += ({ ({ obj }) });
    }
  }
  catch(save_object(SAVE_NAME));
  return 1;
} /* add_login_call() */

int remove_login_call(string player, mixed function, mixed obj) {
  int pos, loop;

  if((pos = member_array(player, names)) == -1)
    return 0;
  if(stringp(function) || stringp(obj)) {
    function = ({ function });
    obj = ({ obj });
  }
  if(stringp(function) || stringp(obj))
    return 0;
  if(sizeof(function) != sizeof(obj))
    return 0;
  printf("%d,  %O\n", pos, functions[pos]);
  for(loop = 0; loop < sizeof(functions[pos]); loop++) {
    int where;

    if((where = member_array(function[pos], functions[pos])) ==
                member_array(obj[pos], objects[pos])) {
      functions[pos] = delete(functions[pos], where, 1);
      objects[pos] = delete(objects[pos], where, 1);
      loop--;
    }
  }
  if(!sizeof(functions[pos])) {
    names = delete(names, pos, 1);
    functions = delete(functions, pos, 1);
    objects = delete(objects, pos, 1);
  }
  catch(save_object(SAVE_NAME));
  return 1;
} /* remove_login_call() */

string *query_names() { return names; }
mixed *query_functions() { return functions; }
mixed *query_objects() { return objects; }

int player_logon(string player) {
  int pos, loop;

  if(lower_case(player) != (string)previous_object()->query_name())
    return 0;
  if((pos = member_array(player, players_on)) != -1)
    log_file("LOGIN", player+" logged in but was already on.\n");
  else
    players_on += ({ player });
  if((pos = member_array(player, names)) != -1) {
    for(loop = 0; loop < sizeof(functions[pos]); loop++)
      call_out("do_call", 0, ({ functions[pos][loop], objects[pos][loop],
               player, LOGIN }) );
  }
  if((pos = member_array("all", names)) != -1) {
    for(loop = 0; loop < sizeof(functions[pos]); loop++)
      call_out("do_call", 0, ({ functions[pos][loop], objects[pos][loop],
               player, LOGIN }) );
  }
  call_out("try_align", 0, player);
} /* player_login() */

void do_call(mixed data) {
  call_other(data[1], data[0], data[2], data[3]);
} /* do_call() */

int player_logout(string player) {
  int pos;

  if((pos = member_array(player, players_on)) == -1)
    return 0;
  players_on = delete(players_on, pos, 1);
  if((pos = member_array(player, players_on)) != -1) {
    int loop;
 
    for(loop = 0; loop < sizeof(functions[pos]); loop++)
      call_out("do_call", 0, ({ functions[pos][loop], objects[pos][loop],
               player, LOGOUT }));
  }
  return 1;
} /* player_logout() */

void update_alignment(string name, int align) {
  int i;
  string str1, str2;

  str1 = alignment[1][sizeof(alignment[1])-1];
  str2 = alignment[0][sizeof(alignment[0])-1];
/* First remove useless frogs. */
  for (i=0;i<sizeof(alignment[1]);i+=3)
    if (alignment[1][i+1] == name || alignment[1][i+2]+TIME_OUT < time()) {
      alignment[1] = alignment[1][0..i-1] + alignment[1][i+3..1000];
      i -= 3;
    }
  for (i=0;i < sizeof(alignment) && (align>=alignment[1][i]);i+=3);
  alignment[1] = alignment[1][0..i-1] + ({ align, name, time() }) +
                 alignment[1][i..1000];
  if (sizeof(alignment[1]) > 4*3) alignment[1] = alignment[1][3..100];

/* First remove useless frogs. */
  for (i=0;i<sizeof(alignment[1]);i+=3)
    if (alignment[1][i+1] == name || alignment[1][i+2]+TIME_OUT > time()) {
      alignment[1] = alignment[1][0..i-1] + alignment[1][i+3..1000];
      i -= 3;
    }
  for (i=0;i < sizeof(alignment) && align<=alignment[0][i];i+=3);
  alignment[0] = alignment[0][0..i-1] + ({ align, name, time() }) +
                 alignment[0][i..1000];
  if (sizeof(alignment[1]) > 4*3) alignment[1] = alignment[1][3..100];
  if (str1 && find_player(str1))
    find_player(str1)->reset_align_title();
  if (str2 && find_player(str2))
    find_player(str2)->reset_align_title();
  if (find_player(name))
    find_player(name)->reset_align_title();
} /* update_alignment() */

string *query_players_on() { return players_on; }

mixed *query_alignment() { return alignment; }

void try_align(string str) {
  if (find_player(str))
    update_alignment(str, (int)find_player(str)->query_al());
} /* try_align() */

string query_al_title() {
  string name;

  name = (string)previous_object()->query_name();
  if (member_array(name, alignment[1]) >= sizeof(alignment[1])-2)
    if ((int)previous_object()->query_gender() != 2)
      return "the lord of evil";
    else
      return "the lady of evil";
  if (member_array(name, alignment[0]) >= sizeof(alignment[0])-2)
    if ((int)previous_object()->query_gender() != 2)
      return "the lord of sweetness";
    else
      return "the lady of sweetness";
} /* query_al_title() */
