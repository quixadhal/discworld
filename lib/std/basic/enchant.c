/* enchanment stuff.  You can enchant up to a maximun of 50.
 * This will raise the ac of the armour by 50 over its start value
 * and raise the price by the amount of enchantment it has.
 */ 
int enchanted;

string enchant_string() {
  switch(enchanted) {
    case 1..10 :
      return "Dim octarine glow.\n";
    case 11..20 :
      return "Slight octarine glow.\n";
    case 21..30 :
      return "An octarine glow.\n";
    case 31..40 :
      return "Bright octarine glow.\n";
    case 41..45 :
      return "Very bright octarine glow.\n";
    case 46..50 :
      return "Brilliant pulsating octarine glow.\n";
  }
  return "";
}

void set_enchant(int i) {
  if (i>50)
    i = 50;
  if (!i)
    this_object()->set_value((int)this_object()->query_value()/enchanted);
  else if (enchanted)
    this_object()->set_value(((int)this_object()->query_value()*i)/enchanted);
  else
    this_object()->set_value((int)this_object()->query_value()*i);
  enchanted = i;
}

int add_enchant(int i) {
  if (i + enchanted > 50)
    i -= enchanted+i-50;
  if (enchanted)
    this_object()->set_value(((int)this_object()->query_value()*(i+enchanted))/enchanted);
  else if (i)
    this_object()->set_value((int)this_object()->query_value()*i);
  return (enchanted += i);
}

int query_enchant() { return enchanted; }
