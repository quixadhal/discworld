/**
 * This handler deals with all the nomic rules for councils.
 * @author Pinkfish
 * @started Fri Jun 23 19:51:04 PDT 2000
 */
#include <nomic_system.h>

private mapping _rules;

void save_me();
int query_can_change_rule(string area, class nomic_rule rule);
class nomic_rule find_nomic_rule(string area, string creator, string text);

void create() {
   _rules = ([ ]);
} /* create() */

/**
 * This method adds a nomic rule into the system.
 * @param area the area the rule is added in
 * @param type the type of rule to add
 * @param creator the person that created the rule
 * @param text the text associated with the rule
 * @return the new number of the rule, NOMIC_ERROR on fai;l
 */
int add_nomic_rule(string area, int type, string creator, string text) {
   int bing;
   class nomic_rule nomic_rule;

   if (!_rules[area]) {
      return NOMIC_ERROR;
   }
   if (type < 0 || type >= sizeof(_rules[area]->new_type_num)) {
      return NOMIC_ERROR;
   }

   if (find_nomic_rule(area, creator, text)) {
      return NOMIC_ERROR;
   }

   nomic_rule = new(class nomic_rule);
   bing = _rules[area]->new_type_num[type];
   nomic_rule->id = bing;
   nomic_rule->creator = creator;
   nomic_rule->type = type;
   nomic_rule->text = text;
   nomic_rule->date_created = time();
   nomic_rule->amendments = ({ });
   _rules[area]->rules += ({ nomic_rule });
   _rules[area]->new_type_num[type] = bing + 1;
   save_me();
   return bing;
} /* add_nomic_rule() */

/**
 * This method adds a nomic rule into the system.
 * @param area the area the rule is added in
 * @param new_rule the rule that is being added
 * @param type the type of the rule
 * @return the new number of the rule, NOMIC_ERROR on fai;l
 */
protected int add_nomic_rule_class(string area,
                                   class nomic_rule new_rule,
                                   int type) {
   int bing;

   if (!_rules[area]) {
      return NOMIC_ERROR;
   }

   if (type < 0 || type >= sizeof(_rules[area]->new_type_num)) {
      return NOMIC_ERROR;
   }

   if (find_nomic_rule(area, new_rule->creator, new_rule->text)) {
      return NOMIC_ERROR;
   }

   bing = _rules[area]->new_type_num[type];
   new_rule->id = bing;
   new_rule->type = type;
   _rules[area]->rules += ({ new_rule });
   _rules[area]->new_type_num[type] = bing + 1;
   save_me();
   return bing;
} /* add_nomic_rule() */

/**
 * This method moves a rule from one type to another.
 * @param area the area the rule is added in
 * @param rule the rule to move
 * @param type the new type of the rule
 * @return the new number of the rule, NOMIC_ERROR on fai;l
 */
protected int move_nomic_rule(string area, class nomic_rule rule,
                              int type) {
   int bing;
   int i;

   if (!_rules[area]) {
      return NOMIC_ERROR;
   }
   if (type < 0 || type >= sizeof(_rules[area]->new_type_num)) {
      return NOMIC_ERROR;
   }

   bing = _rules[area]->new_type_num[type];
   for (i = 0; i < sizeof(_rules[area]->rules); i++) {
      if (_rules[area]->rules[i]->id == rule->id) {
         if (!query_can_change_rule(area, _rules[area]->rules[i])) {
            return NOMIC_ERROR;
         }
         _rules[area]->rules[i]->id = bing;
         ((class nomic_rule)_rules[area]->rules[i])->type = type;
         _rules[area]->new_type_num[type] = bing + 1;
         save_me();
         return bing;
      }
   }
   return NOMIC_ERROR;
} /* add_nomic_rule() */

/**
 * This method moves a rule from one type to another.
 * @param area the area the rule is added in
 * @param rule the rule to move
 * @param type the new type of the rule
 * @return the new number of the rule, NOMIC_ERROR on fai;l
 */
int change_nomic_rule(string area, class nomic_rule rule) {
   int i;

   if (!_rules[area]) {
      return 0;
   }

   for (i = 0; i < sizeof(_rules[area]->rules); i++) {
      if (_rules[area]->rules[i]->id == rule->id) {
         if (!query_can_change_rule(area, _rules[area]->rules[i])) {
            return NOMIC_ERROR;
         }
         rule->type = ((class nomic_rule)_rules[area]->rules[i])->type;
         rule->id = ((class nomic_rule)_rules[area]->rules[i])->id;
         _rules[area]->rules[i] = rule;
         save_me();
         return 1;
      }
   }
   return 0;
} /* add_nomic_rule() */

/**
 * This method removes a nomic rule from the system.
 * @param area the area the rule is removed in
 * @param id the id of the rule to remove
 * @return the status of the removal, NOMIC_ERROR on fai;l
 */
int remove_nomic_rule(string area, int id) {
   class nomic_rule nomic_rule;

   if (!_rules[area]) {
      return NOMIC_ERROR;
   }
   foreach (nomic_rule in _rules[area]->rules) {
      if (nomic_rule->id == id) {
         if (!query_can_change_rule(area, nomic_rule)) {
            return NOMIC_ERROR;
         }
         _rules[area]->rules -= ({ nomic_rule });
         save_me();
         return 1;
      }
   }
   return 0;
} /* remove_nomic_rule() */

/**
 * This method by passes the standard nomic rules check, it can be used
 * to remove an immutable rule.
 */ 
int int_remove_nomic_rule(string area, int id) {
   class nomic_rule nomic_rule;

   if (!_rules[area]) {
      return NOMIC_ERROR;
   }
   foreach (nomic_rule in _rules[area]->rules) {
      if (nomic_rule->id == id) {
         _rules[area]->rules -= ({ nomic_rule });
         save_me();
         return 1;
      }
   }
   return 0;
} /* remove_nomic_rule() */

/**
 * This method returns a nomic rule for the specified area.
 * @param area the area to get the rule in
 * @param id the id of the rule to match
 * @return 0 if failed, the class if succeeded
 */
class nomic_rule query_nomic_rule(string area, int id) {
   class nomic_rule nomic_rule;

   if (!_rules[area]) {
      return 0;
   }
   foreach (nomic_rule in _rules[area]->rules) {
      if (nomic_rule->id == id) {
         return copy(nomic_rule);
      }
   }
   return 0;
} /* query_nomic_rule() */

/**
 * This method returns all the nomic rules for the area.
 * @param area the area to get the rule sin
 * @return all the rules
 */
class nomic_rule* query_all_nomic_rules(string area) {
   if (!_rules[area]) {
      return ({ });
   }

   return copy(_rules[area]->rules);
} /* query_all_nomic_rules() */

/**
 * This method searches through for the specified rule.  This stops the
 * same rule being added more than once by some error in the code.
 * @param area the area the rule is in
 * @param creator the creator of the rule
 * @param text the text of the rule
 * @return the rule if it is found, 0 if not
 */
class nomic_rule find_nomic_rule(string area, string creator, string text) {
   class nomic_rule rule;

   if (!_rules[area]) {
      return 0;
   }

   foreach (rule in _rules[area]->rules) {
      if (rule->creator == creator && rule->text == text) {
         return rule;
      }
   }

   return 0;
} /* find_nomic_rule() */

/**
 * This method creates an new nomic area.
 * @param area the new nomic area name
 * @return 1 on success, 0 on failure
 */
int create_area(string area) {
   class nomic_area new_area;

   if (_rules[area]) {
       return 0;
   }

   new_area = new(class nomic_area);
   new_area->new_type_num = NOMIC_DEFAULT_TYPE_NUMS;
   new_area->rules = ({ });
   _rules[area] = new_area;
   return 1;
} /* create_area() */

/**
 * This method returns the number for the corresponding type name.
 * @param name the name of the area to get the number of
 * @return the type number
 */
int query_type_number(string name) {
   switch (lower_case(name)) {
   case "immutable" :
      return NOMIC_TYPE_IMMUTABLE;
   case "general" :
      return NOMIC_TYPE_GENERAL;
   case "citizen" :
      return NOMIC_TYPE_CITIZEN;
   default :
      return NOMIC_ERROR;
   }
} /* query_type_number() */

/**
 * This method returns the name of the corresponding type name.
 * @param type_no the type number
 * @return the type name string
 */
string query_type_name(int type_no) {
   switch (type_no) {
   case NOMIC_TYPE_IMMUTABLE :
      return "immutable";
   case NOMIC_TYPE_GENERAL :
      return "general";
   case NOMIC_TYPE_CITIZEN :
      return "citizen";
   default :
      return "error";
   }
} /* query_type_name() */

/**
 * This method checks to see if the specified rule can be changed.
 * @param area the area in which the rule will be changed in
 * @param rule the rule to check
 * @return 1 if it can be changed, 0 if not
 */
int query_can_change_rule(string area, class nomic_rule rule) {
   return rule->type != NOMIC_TYPE_IMMUTABLE;
} /* query_can_change_rule() */

/**
 * This method returns the rule as a string.
 * @param rule the rule to make a string
 * @return the string version of us
 */
string rule_as_string(int indent, class nomic_rule rule) {
   string ret;
   class nomic_amendment amend;

   ret = "$I$" + (indent + 5) + "=" + sprintf("%*s", indent, "") +
          (rule->id?rule->id+"":"(" +
          NOMIC_HANDLER->query_type_name(rule->type) + ")") + " by " +
          capitalize(rule->creator) + " (" +
          ctime(rule->date_created) + "):\n" + rule->text +
          "\n";
   foreach (amend in rule->amendments) {
      ret += "$I$" + (indent + 10) + "=" + sprintf("%*s", indent + 3, "") +
             capitalize(amend->amender) + " (" +
             ctime(amend->date_amended) + "):\n" +
             amend->text + "\n";
   }

   return ret;
} /* rule_as_string() */
