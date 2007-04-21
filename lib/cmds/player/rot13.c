/**
 * rot13 command.  Transforms input with rot13 algorithm.  Numbers are
 * transformed with rot5.  
 * @author Gruper
 */

inherit "/cmds/base";

string rot13(string str);

int cmd(string str) {
   printf("You rot: \"%s\".\n", rot13(str));
   return 1;
} /* cmd() */

string rot13(string str) {
   int i;

   for(i = 0; i < sizeof(str); i++) {
      if(str[i] >= 'A' && str[i] <= 'Z') {
         str[i] = ((13 + str[i] - 'A') % 26) + 'A';
      } else if(str[i] >= 'a' && str[i] <= 'z') {
         str[i] = ((13 + str[i] - 'a') % 26) + 'a';
      } else if(str[i] >= '0' && str[i] <= '9') {
         str[i] = ((5 + str[i] - '0') % 10) + '0';
      }
   }
   
   return str;
   
} /* rot13() */

mixed *query_patterns() {
   return ({ "<string>", (: cmd($4[0]) :) });
} /* query_patterns() */
