/* formatting inventories for player, monster and corpse contents */

/* self true if inventory, else an examine. */
string query_living_contents(int self)
{
  string s, fish, *strs;
  object wpn, *arms, *carry, *conts, money, held, *tmp_conts;
  int coins, i, col;
  
  s = "";
  strs = conts = tmp_conts = ({ });

  tmp_conts = all_inventory(this_object());
  for(i=0; i<sizeof(tmp_conts); i++)
    if((fish = (string)tmp_conts[i]->short()) && (fish != ""))
      conts += ({ tmp_conts[i] });

  wpn = (object)this_object()->query_weapon();
  held = (object)this_object()->query_held_ob();
  arms = (object *)this_object()->query_armours();
  money = present("Some Money For Me", this_object());

  if(!arms) arms = ({ });
  
  if(wpn) {
    strs += ({ "Wielding: ", (string)wpn->short(0) + ".\n"});
  }

  if(held) {
    strs += ({ "Holding : ", (string)held->short(0) + ".\n" });
  }

  if(sizeof(arms))
    if(fish = query_multiple_short(arms))
      strs += ({ "Wearing : ", fish  + ".\n" });
      
  carry = conts - arms - ({ wpn }) - ({ money }) - ({ held }) - 
          (object *)this_object()->query_labels();
  
  if(sizeof(carry))
    if(fish = (string)this_object()->query_contents(0, carry))
      strs  += ({ "Carrying: ", fish });

  col = (int)this_player()->query_cols()-11;
  for(i=0; i<sizeof(strs); i+=2) {
    s += sprintf("%s%-=*s", strs[i], col, strs[i+1]);
  }
  if(self) {
    if(money) fish = (string)money->short(0);
    else fish = 0;
    if(!sizeof(strs)) s = "You are empty handed.\n";
    s += "Your purse contains " + (fish?fish:"only moths") + ".\n";
  } else if(money) {
    coins = (int)money->query_number_coins();    
    s += capitalize((string)this_object()->query_possessive()) + " purse is ";
    switch(coins) {
      case 0..10:    s += "home to only moths!\n"; break;
      case 11..100:  s += "tinkling with coins.\n"; break;
      case 101..300: s += "bulging with coins.\n"; break;
      default:       s += "fit to burst!\n";
    }
  }
  return s;
}
