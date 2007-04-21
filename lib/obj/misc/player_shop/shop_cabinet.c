/**
 * Player-run shop stock cabinet object.
 * This object is used by player-run shops to store the shop's stock.
 * Each cabinet will save its own inventory, and offers a method of
 * splitting the stock up into smaller chunks.  This will help prevent
 * the Tarnach's experience where 650k of stock is being saved every
 * time there is a change in the contents.
 * This object should not be inherited by anything, but should be created
 * by the player_shop storeroom.
 * @author Ringo
 * @started 1st August 1999
 */
#include <move_failures.h>

inherit "/std/container";

private mapping _stock = ([]);

private nosave string _save_file = "";

private nosave int _call_save = 0;

private void do_save();

void create()
{
   do_setup++;
   ::create();
   do_setup--;
   if (!do_setup)
   {
      this_object()->setup();
      this_object()->reset();
   }
}
/* create() */


/**
 * Used to add items to the stock.
 * @param items The items to add.
 * @param player The player adding the items.
 * @return An array of objects that could not be added.
 */
object *add_items(object *items, object player)
{
   object *failed = ({});

   foreach( object item in items )
   {
      if (item->move(this_object()) != MOVE_OK)
      {
         failed += ({ item });
      }
   }
   if (sizeof(failed))
   {
      failed->move(player);
   }
   remove_call_out(_call_save);
   _call_save = call_out((: do_save() :), 1);
   return failed;
}
/* add_items() */


/** @ignore yes */
void dest_me()
{
   if (remove_call_out(_call_save) != -1)
   {
      do_save();
   }
   ::dest_me();
}
/* dest_me() */


/** @ignore yes */
private void do_load()
{
   if (_save_file)
   {
      if (file_size(_save_file + ".o") > 0)
      {
         unguarded((: restore_object, _save_file :));
         if (sizeof(_stock))
         {
            init_dynamic_arg(_stock,0);
         }
      }
   }
}
/* do_load() */


/** @ignore yes */
private void do_save()
{
   if (_save_file && _save_file != "")
   {
#ifdef DEBUG
      debug_printf("Saving cabinet data to %s.\n", _save_file);
#endif
      _stock = query_dynamic_auto_load();
      unguarded((: save_object, _save_file :));
   }
}
/* do_save() */


/**
 * Used to remove items from the stock.
 * @param items The items to remove.
 * @param player The player removing the items.
 * @return An array of the objects that were dropped by the player.
 */
object *remove_items(object *items, object player)
{
   object *failed = ({});

   foreach(object item in items)
   {
      if (item->move(player) != MOVE_OK)
      {
         failed += ({ item });
      }
   }
   if (sizeof(failed))
   {
      failed->move(environment(player));
   }
   remove_call_out(_call_save);
     _call_save = call_out((: do_save() :), 1);
   return failed;
}
/* remove_items() */


/**
 * Set the filename that this cabinet should use to save its inventory to.
 * @param filename The file.
 */
void set_save_file(string filename)
{
   _save_file = filename;
   do_load();
}
/* set_save_file() */
