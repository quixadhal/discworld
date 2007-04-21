#include <clubs.h>
inherit "/std/room/controller/club_room_controller";
#include <room/club_room.h>

void setup() {
   set_save_file("/d/admin/save/club_room");
   set_board_associated(1);
   add_controlled_ownership("main room",
                            CLUB_ROOM_CONTROLLER_DISCUSSED,
                            CLUB_ROOM_OWNED_POSITION,
                            (: CLUB_HANDLER->query_president_position($2) :));
} /* setup() */
