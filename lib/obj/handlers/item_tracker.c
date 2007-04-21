/*  -*- LPC -*-  */
/**
 * This handler keeps track of unique (hopefully) per player items in the game.
 * @author Shaggy
 */
#include <library.h>
#include <quest_handler.h>

inherit "/std/object";

#define QUEST_INFO_IDENT  "items"
#define MAX_COPIES    10

/* cached item data */
mapping item_cache;
mapping player_items;

/** @ignore yes */
void create() 
{
   ::create();
   item_cache = ([ ]); 
} /* create() */

/**
 * This method returns the state/location of a specific quest item
 * owned by a player.
 * <p>
 * If the item isn't actually owned by the player it returns 0
 * @param pl_name   player object or name
 * @param item      item object or identifier
 * @returns string array of state information
 * @see set_item_state()
 * @see delete_item()
 */
mixed query_item_state( mixed pl_name, mixed item ) 
{
  if (objectp(pl_name)) {
    if (!interactive(pl_name)) return 0;
    pl_name = pl_name->query_name();
  }

  if (objectp(item)) {
    if (item->query_tracked_item_owner() != pl_name) return 0;
    item = item->query_full_tracked_item_identifier();
  }
  if (!item) return 0;

  player_items = item_cache[ pl_name ];

  if (!player_items) {
    player_items = copy(LIBRARY->query_player_quest_info( pl_name, QUEST_INFO_IDENT ));
    if (!player_items) player_items = ([ ]);
    item_cache[ pl_name ] = player_items;
  }

  return player_items[ item ];
}

/**
 * This method deletes the state/location information of a specific quest item
 * owned by a player.
 * <p>
 * If the item isn't actually owned by the player it returns 0
 * @param pl_name   player object or name
 * @param item      item object or identifier
 * @returns 1 on success
 * @see set_item_state()
 */
int delete_item( mixed pl_name, mixed item ) 
{
  if (objectp(pl_name)) {
    if (!interactive(pl_name)) return 0;
    pl_name = pl_name->query_name();
  }

  if (objectp(item)) {
    if (item->query_tracked_item_owner() != pl_name) return 0;
    item = item->query_full_tracked_item_identifier();
  }
  if (!item) return 0;

  player_items = item_cache[ pl_name ];

  if (!player_items) {
    player_items = copy(LIBRARY->query_player_quest_info( pl_name, QUEST_INFO_IDENT ));
    if (!player_items) player_items = ([ ]);
    item_cache[ pl_name ] = player_items;
  }

  map_delete( player_items, item );
  return 1;
}

/**
 * This method returns the total number of known copies of a specific quest item
 * owned by a player.
 * <p>
 * If the item isn't actually owned by the player it doesn't matter.
 * @param pl_name   player object or name
 * @param item      item object or identifier
 * @returns number of copies
 * @see set_item_state()
 * @see delete_item()
 */
int query_item_copies( mixed pl_name, mixed item ) 
{
  int copy_number,n_copies;

  if (objectp(pl_name)) {
    if (!interactive(pl_name)) return 0;
    pl_name = pl_name->query_name();
  }

  if (objectp(item)) {
    item = item->query_tracked_item_identifier();
  }
  if (!item) return 0;

  player_items = item_cache[ pl_name ];

  if (!player_items) {
    player_items = copy(LIBRARY->query_player_quest_info( pl_name, QUEST_INFO_IDENT ));
    if (!player_items) player_items = ([ ]);
    item_cache[ pl_name ] = player_items;
  }

  n_copies = 0;
  for (copy_number = 1; copy_number < MAX_COPIES; copy_number++) {
    if (player_items[ item+"/"+copy_number ]) n_copies++;
  }

  return n_copies;
}

/**
 * This method sets the state/location of a specific quest item
 * owned by a player.
 * <p>
 * If the item isn't actually owned by the player it returns 0
 * @param pl_name   player object or name
 * @param item      item object or identifier
 * @returns 1 if successful
 * @see query_item_state()
 * @see delete_item()
 * @see save_all_item_states()
 */
int set_item_state( mixed pl_name, mixed item, mixed state ) 
{
  if (objectp(pl_name)) {
    if (!interactive(pl_name)) return 0;
    pl_name = pl_name->query_name();
  }

  if (objectp(item)) {
    if (item->query_tracked_item_owner() != pl_name) return 0;
    item = item->query_full_tracked_item_identifier();
  }
  if (!item) return 0;

  player_items = item_cache[ pl_name ];

  if (!player_items) {
    player_items = copy(LIBRARY->query_player_quest_info( pl_name, QUEST_INFO_IDENT ));
    if (!player_items) player_items = ([ ]);
    item_cache[ pl_name ] = player_items;
  }

  player_items[ item ] = state;
  return 1;
}

/**
 * This method returns all the information about quest items
 * owned by a player.
 * @param pl_name   player object or name
 * @returns state/location of items mapping for player
 * @see set_item_state()
 * @see delete_item()
 */
mixed query_all_item_states( mixed pl_name ) 
{
  if (objectp(pl_name)) {
    if (!interactive(pl_name)) return 0;
    pl_name = pl_name->query_name();
  }

  player_items = item_cache[ pl_name ];

  if (!player_items) {
    player_items = copy(LIBRARY->query_player_quest_info( pl_name, QUEST_INFO_IDENT ));
    if (!player_items) player_items = ([ ]);
    item_cache[ pl_name ] = player_items;
  }

  return player_items;
}

/**
 * This method saves the mapping of state/location 
 * information for items owned by a player to disk.
 * @param pl_name   player object or name
 * @param item      item object or identifier
 * @return 1 if successful
 * @see set_item_state()
 * @see query_item_state()
 * @see delete_item()
 * @see save_all_item_states_and_delete_from_cache()
 */
int save_all_item_states( mixed pl_name ) 
{
  if (objectp(pl_name)) {
    if (!interactive(pl_name)) return 0;
    pl_name = pl_name->query_name();
  }

  player_items = item_cache[ pl_name ];

  if (!player_items) {
    return 0;
  }

  LIBRARY->set_player_quest_info( pl_name, QUEST_INFO_IDENT, player_items );
  return 1;
}

/**
 * This method saves the mapping of state/location 
 * information for items owned by a player to disk and deletes
 * the entry in the cache.
 * @param pl_name   player object or name
 * @param item      item object or identifier
 * @return 1 if successful
 * @see set_item_state()
 * @see query_item_state()
 * @see delete_item()
 * @see save_all_item_states()
 */
int save_all_item_states_delete_cache( mixed pl_name ) 
{
  if (objectp(pl_name)) {
    if (!interactive(pl_name)) return 0;
    pl_name = pl_name->query_name();
  }

  player_items = item_cache[ pl_name ];

  if (!player_items) {
    return 0;
  }

  LIBRARY->set_player_quest_info( pl_name, QUEST_INFO_IDENT, player_items );
  map_delete( item_cache, pl_name );
  return 1;
}







