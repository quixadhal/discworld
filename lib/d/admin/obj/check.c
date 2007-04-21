/*

 Inventory checker made by Funtime to periodically check players inventories
 17/5/1994
 
*/

#define SAVE_FILE "/d/admin/log/"

string *paths, *cloned_by;
int *times;
mapping illegal;

void dest_me()
{
   destruct(this_object());
}

void create()
{
   illegal = ([ ]);
   paths = ({ });
   cloned_by = ({ });
}

void log_file()
{
   int p;
   string *person, *filename;

   filename=m_indices(illegal);
   person=values(illegal);
   
   if(sizeof(illegal)>0) {
      unguarded((: write_file, SAVE_FILE+"illegal.txt",
                 "\nSearch carried out on "+ctime(time())+" from here.\n" :));
      
      for(p=0;p<sizeof(illegal);p++) {
         unguarded((: write_file, SAVE_FILE+"illegal.txt",
                    filename[p]+"  -  on player "+person[p]+" - cloned by "+
                    cloned_by[p]+"\n" :));
      }
   }
}

void check_sub_inv(object *things, object owner)
{
int q, r, check;
string *pre_filename, filename, *switcher;
mixed info;

   for(q=0;q<sizeof(things);q++)
   {  
      pre_filename=explode(file_name(things[q]),"#");
      
      filename=pre_filename[0];
                  
      switcher=explode(filename,"/");

      if( (switcher[0]=="w") && (!owner->query_creator() ) )
      {
         illegal=illegal+([filename:owner->query_name()]);

/* Find out who the object was cloned by */

         info = things[q]->stats();

         for(r=0; r<sizeof(info);r++)
         {
            if(info[r][0] == "cloned by")
            {
               cloned_by = cloned_by + ({ info[r][1] });
            }
         }

         /*
         Remove comment here if you want it dested.
         */
         
           destruct(things[q]);
/*
         things[q]->dest_me();
     */
       }
     
      if ( (!owner->query_creator()) && (sizeof(switcher)>2) && (switcher[0]=="obj") && (switcher[1]=="misc") )
      {
         /*
         Remove comment here if you want it dested.
         */
         switch(switcher[2]){
         
         case "mailer_ob":
         case "board_mas":
            info = things[q]->stats();

            for(r=0; r<sizeof(info);r++)
            {

               if(info[r][0] == "cloned by")
               {

                  cloned_by = cloned_by + ({ info[r][1] });
               }
            }

            illegal=illegal+([filename:owner->query_name()]);

             destruct(things[q]);

            break;
         }
      }   
   }
}

void check_inv(object player)
{
int q;
object *things, *contents, owner;

   things=deep_inventory(player);
   
   owner=player;
   check_sub_inv(things, player);
}

do_seek()
{
   int p;
   object *everyone_on;

   seteuid("Admin");

   everyone_on = users();
   
   for(p=0;p<sizeof(everyone_on);p++)
   {
      check_inv(everyone_on[p]);
   }

   log_file();

   return 1;
}
