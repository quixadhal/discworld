inherit "std/room";

void setup() {
  set_light(60);
  set_short("The Pawn Shop Storeroom");
  set_long(
"You are in the store room of the Ankh Morpork Pawn Shop.  This is where "+
"objects are left before they are moved to the shop or reclaimed.\n");
  add_exit("north", "d/am/am/pawnshop", "plain");
}
