inherit "std/room";

setup() {
  set_light(100);
  set_short("The void");
  set_long(
"The void.\n"+
"You come to the void if you fall out of a room and have no-where to go.\n"
  );
  add_exit("drum", "/d/am/am/mendeddrum");
}
