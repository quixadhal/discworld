inherit "/std/object";

void setup()
{   set_name("detabber");
    set_long("A long stick with absoulutely no tabs in it.\n"
             "Usage:  detab <filename>.\n");
    
}

void init()
{   this_player()->add_command("detab",this_object(),"<string>");
  
}


int do_detab(object *indirect_obs, string *dir_match, string indir_match, mixed *args, string pattern)
{   
    string file;
    string tb;
    
    tb = sprintf("\t");

    file = read_file(args[0]);

    if(file)
        {file = replace_string(file,"\t","    ");
         write_file(args[0],file,1);    
         return 1;
         }

    return 0;
}