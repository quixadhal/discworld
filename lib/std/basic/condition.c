int cond,
    max_cond,
    lowest_cond;

void create() {
  max_cond = 1;
}

/* this should be masked by the object */
void break_me()
{
  this_object()->dest_me();
}

string cond_string() {
  switch ((100*cond)/max_cond) {
    case 0 :
      return "It is a wreck.\n";
    case 1..10 :
      return "It is in a very bad condition.\n";
    case 11..20 :
      return "It is in a bad condition.\n";
    case 21..30 :
      return "It is in poor condition.\n";
    case 31..40 :
      return "It is below average condition.\n";
    case 51..60 :
      return "It is average condition.\n";
    case 61..70 :
      return "It is in reasonable condition.\n";
    case 71..80 :
      return "It is in good condition.\n";
    case 81..90 :
      return "It is in very good condition.\n";
    default :
      return "It is in excellent condition.\n";
  }
}
 
void set_max_cond(int i) { max_cond = i; }
void set_cond(int i) {
  cond = i;
  if (cond < lowest_cond)
    lowest_cond = cond;
  if(cond <= 0) {
    this_object()->break_me();
    return 0;
  }
}
int adjust_cond(int i) {
  cond += i;
  if (cond < lowest_cond)
    lowest_cond = cond;
  if (cond > max_cond)
    cond = max_cond;
  if(cond <= 0) {
    this_object()->break_me();
    return 0;
  }
  return cond;
}
/* the lowest cond varible is used for repairing the armour...
 * See where it was repaired to last time, this determines how difficult
 * repairs will be...
 */
void set_lowest_cond(int i) { lowest_cond = i; }
int query_cond() { return cond; }
int query_max_cond() { return max_cond; }
int query_lowest_cond() { return lowest_cond; }

int set_percentage(int i) {
  if (i > 100) i = 100;
  cond = (max_cond*i)/100;
  lowest_cond = (cond*(random(30)+70))/100;
}
