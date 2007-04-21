/* This file is an example of the way responsive_monster can be used.

   The most important thing is the mapping respond_to_with.

   The index of r_t_w should be an array of element,
         element can be a string, 
         element can be an array of strings
   
   An element of the mapping is matched with the string a player
   says if:
     -the string the player said contains the strings of the array
      (the index of r_t_w) in the SAME order. Also, if an array of
      strings is given instead of a string, the test will be 
      succesful if one of the elements of that array appeared at
      the right place. (readmarks are ignored)

             For Example:
                  Nimmox says: I demand, that you give me attention !
 
             matched :
                  ({ "demand","attention" })          
                or
                  ({ "demand", ({"attention","help" }) })
       
             
             But doesnt match :

                  ({ "attention","demand" });
 
   If one of the indices of the mapping matches a string the player said,
   the monster will respond as is defined in the field of the mapping corres-
   ponding with the matched index.

   This field can be :

          A string : The string will be sent to the room the monster is in 
                     (with a small delay, to cause no weird effects
                      Like the monster responding for the players sees 
                      his/her/it say echo) 

A string with a "#func" format. : the function func will be called with 
                                  this_player() (the player that did the say)
                                  as argument.     

An array of strings: One of the strings is randomly selected, and treated
                     as above.




Also you can set_stop_responding(1) to stop the monster from responding
 (you might want to tell a stroy uninterrupted)

set_stop_responding(0) to allow the monster to respond again.


                          Nimmox (January 1993)
*/



inherit "/w/pinkfish/tmp/response_mon";

object weapc;
string *tale;

void setup() {
    set_name("guard");
    add_adjective(({ "big", "gate" }));
    set_short("Big gate guard");
    set_long("This is one of the common guards of Amber.  "+
     "He looks quite big, maybe you shouldn't upset him...\n");
    set_race("human");
    set_class("fighter");
    set_level(45);
    set_al(250);
    set_gender(1);
    tale = ({ "There once was a frog with 5 legs.\n",
              "This frog was very very unhappy.\n",
              "All other frogs made fun of him.\n",
              "And no lady frog wanted to marry him.\n",
              "One day, a womble appeared, and bit one of his legs off.\n",
              "Since then the frog had 4 legs, and lived happily ever after.\n"
            });


    set_respond_to_with(([ ({ "help", "me" }) : "'I can't help you." ,
                         ({ ({ "hi","hello","greetings" }) }) :
                                      ({ "'Hello $hcname$.",
                                         "emote salutes you." }),
                            ({ ({ "remove","unwield","unhold" }),
                               ({ "axe","weapon" }) }) :
                                       "#remov",
                            ({ ({ "wield","hold" }),
                               ({ "axe","weapon" }) }) :
                                       "#wiel", 
                            ({ "tell" , "story" }) :
                                      "#story" ]));
  weapc = clone_object("/obj/weapons/axe.wep");
  weapc->move(this_object());
  do_equip();
} /* setup() */

int remov() {
  init_command("unhold axe");
} /* remov() */

int wiel() {
 init_command("hold axe");
} /* wiel() */

void story(object tpl) {
  command("'Very well, "+tpl->query_cap_name()+ " I will tell you a story.\n");
  set_stop_responding(1);
  call_out("tell_story",2,0);
} /* story() */

void tell_story(int arg) {
  remove_call_out("tell_story");
  if (arg == sizeof(tale)) { 
    set_stop_responding(0);
    return ;
  }
  command("'"+tale[arg]);
  call_out("tell_story",2,arg+1);
} /* tell_story() */
