/**
 * Player-run shop policy notice object.
 * This object is used by player-run shops to track management policy.
 * Essentially, it is only there to be looked at.
 * This object should not be inherited by anything, but is created by the
 * player_shop office.
 * @author Ringo
 * @started 21st September 1999
 */
inherit "/std/object";

private nosave string _office, _stock_policy;
private nosave int _type = 0;

void setup()
{
   set_name("notice");
   set_short("policy notice");
   set_long("");
   add_adjective("policy");
   reset_get();
}
/* setup() */


/**
 * @ignore yes
 * Chart display
 */
string long(string word, int dark)
{
   mapping policies= _office->get_policies(_type);
   int i = 1;
   string display;
   
   if (!_type)
   {
      display = sprintf("This is the list of shop's policies.  The policies "
         "on this notice are definitive and final.  This shop deals strictly "
         "in %s.  Items that do not fit this description should not be "
         "stocked.\n\n", "%^BOLD%^"+ _stock_policy+ "%^RESET%^");
   }
   else
   {
      display = "This is the list of management policies.\n\n";
   }
   
   if (!m_sizeof(policies))
   {
      return display;
   }
   foreach(string policy in m_indices(policies))
   {
      if(_office->query_manager(this_player()->query_name()) || 
         _office->query_retired(this_player()->query_name()) ||
         this_player()->query_creator())
      {
         display += sprintf("%d [%s]. %s\n", i++, policy,
            policies[policy][0]);  
      }
      else
      {
         display += sprintf("%d. %s\n", i++, policies[policy][0]);  
      }    
   }
   return display;
}
/* long() */


/**
 * @ignore yes
 * Where to get the data from?
 */
void set_office(string office)
{ 
   _office = office;
   _stock_policy = _office->get_stock_policy();
}
/* set_office() */


/**
 * @ignore yes
 * 1 for normal, 0 for managers
 */
void set_type(int type) { _type = type; }
