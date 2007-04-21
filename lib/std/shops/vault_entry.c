/*
 * $Locker:  $
 * $Id: vault_entry.c,v 1.11 2001/11/13 22:00:35 pinkfish Exp $
 */
/**
 * Standard vault inheritable object. There are two inheritables (this one and
 * /std/vault.c) which are provided to make it easy to setup your own
 * vaults.
 * This object should be inherited by your vault entrance area. It is used to
 * define most of the customisation for your vault.
 *
 * You must use set_vault_name and set_vault_entrance. You must also define
 * a vault proprietor NPC and use set_proprietor() to let the system know
 * which NPC it is. You should also check for this proprietors location and
 * existance every reset. You may also want to provide some protection to
 * prevent the NPC being killed too frequently.
 *
 * @author Ceres
 * @see /std/vault_entrance.c
 */
inherit "/std/room/basic_room";
inherit "/std/shops/inherit/vault_entry";

void create() {
   add_help_file("vault_entry");
   basic_room::create();
} /* create() */

/**
 * @ignore yes
 */
void init() {
  basic_room::init();
  vault_entry::init();
} /* init() */
