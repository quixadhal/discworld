/*  Created by Dragonlord  -  5 October 1996  */

#define TP this_player()
#define TPCAP this_player()->one_short()
#define TO this_object()

inherit "std/object";

void setup(){
   set_name("bear");
   set_short("teddy bear");
   add_alias("teddy");
   add_adjective("teddy");
   set_long("This is a cute and cuddly teddy bear.  Just the sort to give "
     +"someone you love to remind them of you.  It looks very soft and "
     +"huggable.\n");
   set_weight(10);
   set_value(25);
   set_main_plural("teddy bears");
   add_plural(({"bears", "teddys"}));
}  /* setup() */

void init() {
   TP->add_command("hug", TO);
   TP->add_command("cuddle", TO);
}

int do_hug() {
   write("You hug the teddy bear close to you and feel all warm and fuzzy "
     "inside.\n");
   say(TP->one_short()+ " hugs the teddy bear close and smiles happily.\n");
   TP->add_succeeded_mess(TO, "", ({ }) );
   return 1;
}

int do_cuddle() {
  do_hug();
   TP->add_succeeded_mess(TO, "", ({ }) );
   return 1;
}


