#include <path.h>
inherit CITYROOM;
#define DOCK "/d/am/docks/dock1"

void setup() {
  set_light(100);
  set_short("A street near the city docks. ");
  set_long(
"Ducking around crate-laden dockhands and drunk sailors, you make your way "+
"out of the hustle and bustle into the relative peace of a vendor's stall.  You "+
"politely but firmly reject his offer of a genuine ancient Pseudopolitan toga, "+
"shaking your head at his chutzpah, and continue on your way. \n");

  add_exit("east", ROOM+"mackerel2", "road");
  add_exit("south", DOCK, "road");

  add_item("docks",
"You tread gingerly as you cross the old, barnacle-bedecked remnants of the Ankh "+
"forest.  The planks are widely spaced, making you fear for the safety of small "+
"children and the troupe of midgets performing handstands to your left. \n");

  add_item("dockhand",
"This brawny fellow could do a good day's work--if he weren't so intent on "+
"shirking it.  Several of these fine chaps seem to be transporting their cargo "+
"to destinations unimagined by their employers. \n");

  add_item("vendor",
"This old chap smiles happily at you, displaying his tawdry wares as if you were "+
"a merchant prince examining jewels. \n");

  add_item("drunk",
"A rather pickled member of the seafaring community. \n");

  add_item("toga",
"You doubt that the purveyor of this item knows where Pseudopolis is.  It's a "+
"garment you wouldn't give to a beggar. \n");

  set_zone("ankh morpork");
}
