inherit "/std/object";

#define REV_OB "/w/pinkfish/board_master"

void setup() {
  set_name("board");
  set_short("reversi board");
  set_long(
"A large green reversi board.  It has some black and white markers "+
"all over it.\n");
}

void init() {
  ::init();
  add_action("do_start", "st*art");
  add_action("do_join", "j*oin");
  add_action("do_take", "tu*rn");
  add_action("do_list", "li*st");
  add_action("do_rank", "ra*nk");
  add_action("do_delete", "d*elete");
  add_action("do_show", "sh*ow");
  add_action("do_view", "v*iew");
}

int do_start(string str) {
  return (int)REV_OB->do_start(str);
}

int do_join(string str) {
  return (int)REV_OB->do_join(str);
}

int do_take(string str) {
  return (int)REV_OB->do_take(str);
}

int do_list(string str) {
  return (int)REV_OB->do_list(str);
}

int do_rank(string str) {
  return (int)REV_OB->do_rank(str);
}

int do_show(string str) {
  return (int)REV_OB->do_show(str);
}

int do_view(string str) {
  return (int)REV_OB->do_view(str);
}

int do_delete(string str) {
  return (int)REV_OB->do_delete(str);
}
