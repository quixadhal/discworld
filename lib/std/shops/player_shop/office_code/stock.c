/******************************************************************************
* This file contains stock-related functions
*****************************************************************************/

/**
* @ignore yes 
* Keep track of items bought.
*/
void adjust_bought(string item, int amt)
{
   if (previous_object() && previous_object() != find_object(_storeroom))
   {
      LOG_ERROR("office.c", "adjust_bought("+item+","+amt+")");
      return;
   }
   _list[item][CHART_BOUGHT] += amt;
   save_list();
}
/* adjust_bought() */

/**
* @ignore yes 
* Keep track of items sold.
*/
void adjust_sold(string item, int amt)
{
   if (previous_object() && previous_object() != find_object(_storeroom))
   {
      LOG_ERROR("office.c", "adjust_sold("+item+","+amt+")");
      return;
   }
   _list[item][CHART_SOLD] += amt;
   save_list();
}
/* adjust_sold() */

/**
* @ignore yes 
* Maintain the list of stock dealt in by this shop.
*/
private int do_chart(mixed *args, string pattern)
{
   int number;
   string word = this_player()->query_name();
   
   if (sizeof(args) > 1) number = args[1];
   
   if (pattern == CHART_ADDITEM)
   {
      if(query_on_list(args[0]))
      {
         tell_object( this_player(), args[0] +
            " are already on the sales list.\n" );
         return 1;
      }
   }
   else if (!query_on_list(args[0]))
   {
      tell_object(this_player(), args[0] +" are not on the sales list.\n");
      return 1;
   }
   
   switch (pattern)
   {
   case CHART_ADDITEM :
      /* Start dealing in this item */
      add_succeeded_mess("$N put$s "+ args[0] +" on the sales list.\n");
      put_on_list(args[0]);
      shop_log(CHARTLOG, word, "put "+ args[0] +" on the sales list", PAID);
      break;
   case CHART_REMITEM :
      /* Stop dealing in this item */
      add_succeeded_mess("$N take$s "+args[0] +" off the sales list.\n");
      take_off_list( args[0] );
      shop_log(CHARTLOG, word, "took "+args[0] +" off the sales list", PAID);
      break;
   case CHART_MAXITEM :
      /* Set maximum stock */
      if (number < 1)
      {
         tell_object(this_player(), "Maximum must be greater than zero.\n");
         return 1;
      }
      add_succeeded_mess("$N set$s the maximum stock of "+ args[0] +" to "+
         number +".\n");
      set_max(args[0], number);
      shop_log(CHARTLOG, word, "set max of "+ args[0] +" to "+ number, PAID);
      break;
   case CHART_BUYITEM :
      /* Set buy price */
      if (number < 1)
      {
         tell_object(this_player(), "Price must be greater than zero.\n");
         return 1;
      }
      add_succeeded_mess("$N set$s the buying price of "+ args[0] +" to "+
         number +".\n");
      set_buy(args[0], number);
      shop_log(CHARTLOG, word, "set buy of "+ args[0] +" to "+ number, PAID);
      break;
   case CHART_SELLITEM :
      /* Set sell price */
      if (number < 1)
      {
         tell_object(this_player(), "Price must be greater than zero.\n");
         return 1;
      }
      add_succeeded_mess("$N set$s the selling price of "+ args[0] +" to "+
         number +".\n");
      set_sell(args[0], number);
      shop_log(CHARTLOG, word, "set sell of "+ args[0] +" to "+ number, PAID);
      break;
   case CHART_ASSIGN_ITEM :
      /* Assign item to cabinets */
      if (member_array( number, _list[args[0]][CHART_CAB] ) != -1)
      {
         tell_object(this_player(), args[0]+ 
            " are already assigned to cabinet "+ number+ ".\n");
         return 1;
      }
      if (number < 1 || number > _num_cabinets)
      {
         tell_object(this_player(), "That cabinet does not exist.\n");
         return 1;
      }
      add_chart_cabinet(args[0], number);
      add_succeeded_mess("$N assign$s "+ args[0] +" to use cabinet "+
         number +".\n");
      shop_log(CHARTLOG, word, "assigned "+ args[0] +
         " to cabinet "+ number, PAID);
      break;
   case CHART_UNASSIGN_ITEM :
      /* Stop item using cabinets */
      if (member_array( number, _list[args[0]][CHART_CAB] ) == -1)
      {
         tell_object(this_player(), args[0]+ 
            " are not assigned to cabinet "+ number+ ".\n");
         return 1;
      } 
      remove_chart_cabinet(args[0], number);
      add_succeeded_mess("$N stop$s "+ args[0] +" using cabinet "+
         number +".\n");
      shop_log(CHARTLOG, word, "unassigned "+ args[0] +
         " from cabinet "+ number, PAID);
      break;
   }
   return 1;
}
/* do_chart() */

/**
* @ignore yes 
* Add an item for the shop to deal in.
*/
private void put_on_list(string word)
{
   if (!_list[word])
   {
      _list[word] = ({5, 0, 20, 25, ({}), 0, 0, 0});
      save_list();
   }
}
/* put_on_list() */

/**
* @ignore yes 
* Query the average number of this item in stock.
* @param item The item to query.
* @return The average stock.
*/
int query_ave(string item)
{
   return (!_list[item])?0:copy(_list[item][CHART_AVE]);
}
/* query_ave() */

/**
* @ignore yes 
* Query the buying price of this item.
* This is the actual value, in the smallest unit of local currency
* (eg. pence, farthings etc), rather than the absolute value. 
* @param item The item to query.
* @return The buy price.
*/
int query_buy(string item)
{
   return (!_list[item])?0:copy(_list[item][CHART_BUY]);
}
/* query_buy() */

/**
* @ignore yes 
* Query the maximum number of this item allowed in stock.
* @param item The item to query.
* @return The maximum stock.
*/
int query_max(string item)
{
   return (!_list[item])?0:copy(_list[item][CHART_MAX]);
}
/* query_max() */

/**
* @ignore yes 
* Query if the shop sells this item.
* @param item The item to query.
* @return TRUE or FALSE
*/
int query_on_list(string item) { return (!_list[item])?FALSE:TRUE; }

/**
* @ignore yes 
* Query the sell price of this item.
* This is the actual value, in the smallest unit of local currency
* (eg. pence, farthings etc), rather than the absolute value. 
* @param item The item to query.
* @return The sell price.
*/
int query_sell(string item)
{
   return (!_list[item])?0:copy(_list[item][CHART_SELL]);
}
/* query_sell() */

/**
* @ignore yes 
* Set the buy price of this item.
*/
private void set_buy(string word, int number)
{
   if (!_list[word]) return;
   _list[word][CHART_BUY] = number;
   save_list();
}
/* set_buy() */

/**
* @ignore yes 
* Set the maximum number of this item allowed in stock.
*/
private void set_max(string word, int number)
{
   if (!_list[word]) return;
   _list[word][CHART_MAX] = number;
   save_list();
}
/* set_max() */

/**
* @ignore yes 
* Set the sell price of this item.
*/
private void set_sell(string word, int number)
{
   if (!_list[word]) return;
   _list[word][CHART_SELL] = number;
   save_list();
}
/* set_sell() */

/**
* @ignore yes 
* Stop the shop dealing in this item.
*/
private void take_off_list(string word)
{
   if (!_list[word]) return;
   map_delete(_list, word);
   save_list();
}
/* take_off_list() */

/**
* @ignore yes 
* Updates the chart with the average stock of an item.
*/
private void update_averages()
{
   log_file("/log/PLAYER_SHOP", "%s: %s entered update_averages (stock.c)\n",
      ctime(time()), _very_short); 
   foreach (string words in m_indices(_list))
   {
      _list[words][CHART_AVE] = (_list[words][CHART_AVE] +
         query_stock(words) + random(2)) / 2;
   }
   save_list();
}
/* update_averages() */
