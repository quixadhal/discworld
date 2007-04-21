/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: flu_shadow.c,v 1.1 1998/01/06 04:36:02 ceres Exp $
 * $Log: flu_shadow.c,v $
 * Revision 1.1  1998/01/06 04:36:02  ceres
 * Initial revision
 * 
*/
inherit "/std/effect_shadow";

string *soulverb;
string *souladverb;

int query_flu(){ return 1; }

/* Flu functions */

string flusoul(){
   string flu;
   soulverb = ({"sneeze",
                "cough"
   });
   souladverb = ({"violently",
                "loudly",
                "suddenly"
   });

this_player()->force_soul_command(soulverb[random(sizeof(soulverb))]+" "+souladverb[random(sizeof(souladverb))]);
   return flu;
}

/* trapped functions */

int look_me(string arg){
   if(!arg){
      if (random(100) > 30){
         tell_object(player, "You feel dizzy and bleary eyed.\n");
      }
      else{
         player->look_me();
      }
      return 1;
   }
   return 1;
}

int attack_ob(object obj){
   object *contents;
   object target;

   if(random(100) > 30){
      tell_object(player, "You feel too ill, and can't be bothered to attack.\n");
      return 1;
   }
   player->attack_ob(target);
   return 1;
}


int cast(string str){
   if(random(100) > 50){
      tell_object(player, "You lose your concentration as you feel a "+
                          "sneeze coming on.\n");
      this_player()->force_soul_command("sneeze");
      return 1;
   }
   player->cast(str);
   return 1;
}
