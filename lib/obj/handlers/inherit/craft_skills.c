/**
 * This is an inheritable file to deal with the more dynamic nature of
 * craft skills.  So we can add in new ones without disturbing the balance
 * everything.
 * @author Pinkfish
 * @started Thu Nov 11 18:51:01 PST 1999
 */
#include <skills.h>

private string *_craft_skill_tree;

void add_craft_skill(string name);

void create() {
   _craft_skill_tree = ({ });

   add_craft_skill("crafts.smithing.gold");
   add_craft_skill("crafts.smithing.silver");
   add_craft_skill("crafts.smithing.black.tool");
   add_craft_skill("crafts.smithing.black.weapon");
   add_craft_skill("crafts.smithing.black.armour");
   add_craft_skill("crafts.smithing.gem.cutting");
   add_craft_skill("crafts.smithing.gem.polishing");
   add_craft_skill("crafts.smithing.gem.setting");
   add_craft_skill("crafts.mining.gem");
   add_craft_skill("crafts.mining.ore.panning");
   add_craft_skill("crafts.mining.mineral");
   add_craft_skill("crafts.hunting.tracking");
   add_craft_skill("crafts.hunting.fishing");
   add_craft_skill("crafts.hunting.trapping");
   add_craft_skill("crafts.carpentry.furniture");
   add_craft_skill("crafts.carpentry.coopering");
   add_craft_skill("crafts.carpentry.turning");
   add_craft_skill("crafts.carpentry.whittling");
   add_craft_skill("crafts.pottery.forming.throwing");
   add_craft_skill("crafts.pottery.forming.shaping");
   add_craft_skill("crafts.pottery.glazing");
   add_craft_skill("crafts.pottery.staining");
   add_craft_skill("crafts.pottery.firing");
   add_craft_skill("crafts.materials.leatherwork");
   add_craft_skill("crafts.materials.weaving");
   add_craft_skill("crafts.materials.spinning");
   add_craft_skill("crafts.materials.needlework");
   add_craft_skill("crafts.materials.dyeing");
   add_craft_skill("crafts.husbandry.plant.edible");
   add_craft_skill("crafts.husbandry.plant.herbal");
   add_craft_skill("crafts.husbandry.plant.milling");
   add_craft_skill("crafts.husbandry.plant.tree");
   add_craft_skill("crafts.husbandry.animal.grooming");
   add_craft_skill("crafts.husbandry.animal.breeding");
   add_craft_skill("crafts.culinary.cooking");
   add_craft_skill("crafts.culinary.baking");
   add_craft_skill("crafts.culinary.butchering");
   add_craft_skill("crafts.culinary.preserving");
   add_craft_skill("crafts.culinary.brewing");
   add_craft_skill("crafts.culinary.distilling");
   add_craft_skill("crafts.arts.design");
   add_craft_skill("crafts.arts.calligraphy");
   add_craft_skill("crafts.arts.drawing");
   add_craft_skill("crafts.arts.painting");
   add_craft_skill("crafts.arts.printing");
   add_craft_skill("crafts.arts.sculpture");
   add_craft_skill("crafts.points");
} /* create() */

private mixed *add_craft_skill_int(string *bits,
                                   mixed *arr) {
   int pos;

   pos = member_array(bits[0], arr);
   if (pos == -1) {
      pos = sizeof(arr);
      arr += ({ bits[0], 0, 0, ({ }) });
   }
   if (sizeof(bits) > 1) {
      arr[pos + SKILL_BIT] = add_craft_skill_int(bits[1..],
                                                 arr[pos + SKILL_BIT]);
   }
   return arr;
} /* add_craft_skill_int() */

private void add_craft_skill(string name) {
   string *bits;

   bits = explode(name, ".");
   _craft_skill_tree = add_craft_skill_int(bits, _craft_skill_tree);
} /* add_craft_skill() */

/**
 * This method returns the craft skill tree.
 * @return the crafts skill tree.
 */
mixed *query_crafts_skill_tree() {
   return _craft_skill_tree;
} /* query_crafts_skill_tree() */
