/* a different attempt by Ember to make an accountable auto_loading system */
/* uses old system, with a different front end */
/* 
 *  30-Oct-93    created by E
 *  24-Oct-96    Fiddled by Pinkfish to create the alter ego - Gumboot.
 *  05-Feb-97    Changed by Olorin to not trust find_call_out
 */

#define ITEM_TRACKER ("/obj/handlers/item_tracker")

inherit "/obj/monster";

object my_player;
mixed total_als;      /* generated total auto-load-string */
object *save_obs;
object *unhandled_obs;
object *done_obs;

int grecos_day_off;

void setup() {
  seteuid("Root");
  if (!grecos_day_off) {
    set_name("greco");
    set_short("Greco the Departure Gecko");
    set_long("A small mottled grey gecko whose job it is to record all of "
             "the baggage of players so they can be restocked when they "
             "come back.  Greco is currently scribbling in " 
             + query_possessive() + " notebook furiously.\n");
  } else {
    /* Gumboot.  Greco's happy replacement. */
    set_name("gumboot");
    set_short("Gumboot");             
    set_long("A big black lizard who looks rather confused about what "
             + query_pronoun() + " is supposed to be doing.  Nevertheless, "
             + query_pronoun() + " is trying to look busy, scrawling "
             "something into a huge log book.\n"); 
  }                      
              
  add_property("determinate", "");
  add_property("no_attack", 1);
  set_gender(1+random(2));  /* Greco (or Gumboot) the androgynous gecko */
  basic_setup("gnome", "warrior", 30);
  set_language("general");

} /* setup() */

void set_grecos_day_off(int day_off) {
  grecos_day_off = day_off;
  setup();
} /* set_grecos_day_off() */

int query_grecos_day_off() {
  return grecos_day_off;
} /* query_grecos_day_off() */

void get_rid_of(object player, int verbose) {
  int i;

  catch(set_grecos_day_off("/obj/monster/greco"->query_grecos_day_off()));
  if (verbose) {
    if (!grecos_day_off) {
      tell_object(player, "Greco the Departure Gecko starts inspecting "
                          "all your items and scribbling on a "
                          "notepad.\n");
    } else {
      tell_object(player, "This is Greco's day off.\nGumboot, Greco's "
                          "distant relative, is here to take your "
                          "information.  Gumboot looks very confused "
                          "and sticks the pencil in "+
                          query_possessive()+" ear.\n");
    }
  }
  my_player = player;
  unhandled_obs = save_obs = all_inventory(player);
  total_als = ({ });
  done_obs = ({ });
  for (i = 0; i < sizeof(save_obs); i++) 
    call_out("get_ob_al", 1, save_obs[i], verbose);
  call_out("finish_player", 2, verbose);
} /* get_rid_of() */

void get_ob_al(object ob, int verbose)
{
  mixed *als;

  /* this is call_outed for each object in the player's inventory */
  /* it should add to the autoload array and the done_obs array */
  unhandled_obs -= ({ ob }); 
  als = my_player->fragile_auto_str_ob(ob);
  done_obs += ({ ob });
  if (sizeof(als) == 0) return;
  total_als += als;
} /* get_ob_all() */

void handle_dead_ob(object ob) {
  int value;
  mixed *als;
  string obname;

  catch(ob->move("/room/broken"));
  value = 1000; /* random anonymous replacement value */
  catch(value = ob->query_value());
  obname = "unknown object";
  catch(obname = ob->short(1));
  catch("/obj/misc/al_receipt"->set_object(file_name(ob)));
  "/obj/misc/al_receipt"->set_obname(obname);
  "/obj/misc/al_receipt"->set_value(value);
  als = "/global/auto_load"->
    fragile_auto_str_ob(find_object("/obj/misc/al_receipt"));
  als[0] = 1;
  // This is a bit of a hack.  It could easily break if the receipt
  // inheritance changes.
  if (!undefinedp(als[2][1]["::"]["cloned by"])) {
    als[2][1]["::"]["cloned by"] = "greco";
  }
  log_file("GRECO", "%s: %s - Broken object at logout: %O\n",
           ctime(time()), my_player?my_player->query_name():"0", ob);
  total_als += als;
} /* handle_dead_ob() */

void finish_player(int verbose) {
  /* this is called after all of the auto_load call_outs are complete,
   * so it should check which objects failed to complete and write out
   * receipts for them ... */
  int i, one_more;
  object *missing_obs;

  if (find_call_out("get_ob_al") != -1) {
    call_out("finish_player", 2, verbose);
    return;
  }
  if ( sizeof( unhandled_obs ) ) {
     /* find_call_out isn't to be trusted:(
      */
    call_out("finish_player", 2, verbose);
    return;
  }
  missing_obs = save_obs - done_obs;
  if (sizeof(missing_obs)) {  
    one_more = sizeof(missing_obs);
    if (!grecos_day_off) {
      do_command("'"+(one_more==1?"One":"Some")+
                 " of your objects "+(one_more==1?"is":"are")+" broken and "
                 "will not save properly, so I will give you "+
                 (one_more==1?"a receipt for it.  This":
                              "receipts for them.  These")+
                 " can either be "
                 "sold for something vaguely like the replacement value of "
                 "the object or taken to a creator as proof of ownership "
                 "so you can ask very nicely to have it replaced.");
    } else {
      do_command("'Ummm.  I think some of these things of yours might "
                 "be broken.  Ummmm.....  Here, have some of these "
                 "receipt things.  Give them to a creator and they will "
                 "help you in trying to replace them.  Ummmm....  "
                 "Good luck.");
    }
    for (i = 0; i < sizeof(missing_obs); i++)
      handle_dead_ob(missing_obs[i]);
  }

  my_player->save_with_auto_load(total_als);
  if (verbose) {
    if (!grecos_day_off) {
      do_command( "smile brightly" );
      do_command( "'Well, everything seems to be in order.  Bye bye!" );
      do_command( "wave" );
      switch (random(3)) {
      case 0:
        tell_object(my_player, "You clamber aboard the giant spider.  It "
                    "opens the door, releases a thread of silk out into space "
                    "and, after a brief pause, climbs out after it possibly "
                    "never to be seen again ...\n");
        say(my_player->short(1) + " clambers aboard the giant spider.  It "
            "opens the door, releases a thread of silk out into space and, after "
            "a brief pause, climbs out after it.\n", my_player);
        break;
      case 1:
        tell_object(my_player, "The giant bird grabs you in its talons and leaps "
                    "out of the now open door to fly at great speed into the "
                    "black unknown ...\n");
        say(my_player->short(1) + " is grabbed by the talons of the giant bird "
            "and dragged out a door into the blackness of space.\n", my_player);
        break;
      case 2:
        tell_object(my_player, "You are thrown on to the back of the chimera.  "
                    "Clinging desperately to its mane you follow the beast out "
                    "into the inky depths.\n");
        say(my_player->short(1) + " is thrown onto the back of the chimera and "
            "dragged out into the inky depths.\n", my_player);
        break;
      }
    } else {
      do_command("smile strangely");
      do_command("'Well.  Uhhhh.  Thats it.");
      do_command("'I...  Umm.  Hope.");
      switch (random(3)) {
        case 0 :
          tell_object(my_player,
             "You clamber about a giant spider.  Gumboot starts to walk out "
             "the door and stands on one of it's feet.  You are flung from "
             "the back of the giant spider out through the door and "
             "floating meaninglessly through space.  You hope you will "
             "be seen again...\n");
          say(my_player->short(1)+" clambers aboard the giant spider.  It "
             "moves towards the door when something happens.  "+
             my_player->short(1)+" gets flung out through the door.\n");
          break;
        case 1 :
          tell_object(my_player,
             "The giant bird grabs you in it's talons and leaps out towards "
             "the now open door.\nGumboot exclaims: Wait!  I forgot to get your "
             "name!\nThe bird pauses confused and you thud into the side of "
             "the door and tumble backwards out into space, you try not "
             "notice the huge rents in your shoulder from where the talons "
             "ripped about your flesh.  Just a flesh wound.\n");
          say(my_player->short(1)+" is grabbed by the talons of the giant "
             "bird.  As they are heading for the door, Gumboot yells "
             "something at them, the bird gets disconcerted and rams "+
             my_player->short(1)+" into the door.  They tumble out into "
             "space a red line of blood showing their path.\n");
          break;
        case 2 :
          tell_object(my_player,
             "You are thrown towards the back of a chimera, you completely "
             "fail to get a hold on the mane and falls off it onto the "
             "floor.  The chimera steps backwards, convenient impaling you "
             "on a foot.  It then leaps through the door, you clutch your "
             "stomach hoping it is not permanent.\n");
          say(my_player->short(1)+" is thrown at a chimera, they don't catch "
             "hold of it, but they get impaled on it's foot when it steps "
             "back.  The chimera with a look of satisfaction dives through "
             "the door.\n");
          break;
      }
    }
  }
  tell_object(my_player, "Do come again!\n");

  // If we break here, then the player won't be destructed and then their
  // partially destructed inventory will overwrite their new inventory
  // when they idle out of the departure lounge!

  foreach( object saved_ob in done_obs ) { 
     reset_eval_cost(); 

     catch(saved_ob->set_tracked_item_status_reason("PLAYER_QUIT"));
     catch(saved_ob->dest_me());

     if ( saved_ob ) { 
         catch(saved_ob->dwep());
     }
  }
  
  // Should be the last one we need. 
  reset_eval_cost();

  catch( ITEM_TRACKER->save_all_item_states_delete_cache( my_player->query_name() ));
  catch(my_player->effects_quiting());
  catch(my_player->dest_me());
  if ( my_player ) {
    catch( my_player->dwep() );
  }
  if (my_player) {
    say(short(1) + " sighs deeply.\n");
  }
  move( "/room/rubbish", "$N appear$s.",
        "$N disappear$s in a puff of smoke." );
} /* finish_player() */
