string
parse_args(string action, string args, int pos) {
    int arg_count, arg_num, i;
    string *t1, t2, *arg_array, temp;
 
    if(!args) {
        arg_count=0;
        args="";
        }
    else {
        arg_array=explode(args+" "," ");
        arg_count=sizeof(arg_array);
        }
    if(sscanf(action,"$#%s",temp)) 
        action=arg_count+temp;
    t1=explode(action+"$#","$#");
    action="";
    for(i=0;i<sizeof(t1)-1;i++)
        action+=t1[i]+arg_count;
    action+=t1[i];
 
    if(sscanf(action,"$*%s",temp))
        action=args+temp;
    t1=explode(action+"$*","$*");
    action="";
    for(i=0;i<sizeof(t1)-1;i++)
        action+=t1[i]+args;
    action+=t1[i];
    
    if(sscanf(action,"$%d%s",arg_num,temp)==2) {
        if(arg_num>arg_count)
            action=temp;
        else {
            if(arg_num==0)
                action=query_verb()+temp;
            else
                action=arg_array[arg_num-1]+temp;
            }
        }
    while(sscanf(action,"%s$%d%s",t2,arg_num,temp)==3) {
        if(arg_num>arg_count)
            action=t2+temp;
        else {
            if(arg_num==0)
                action=t2+query_verb()+temp;
            else
                action=t2+arg_array[arg_num-1]+temp;
            }
        }
    return action;
    }
