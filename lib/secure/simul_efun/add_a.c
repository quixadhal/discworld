string add_a(string s) /* adds 'a' or 'an' on to the string as appropriate */
{
   int i;

   i = 0;
   while (s[i] == ' ') i++;

   switch (s[i]) {
   case 'a':
   case 'e':
   case 'i':
   case 'o':
   case 'u':
   case 'A':
   case 'E':
   case 'I':
   case 'O':
   case 'U':
      return "an " + extract(s, i);
   default:
      return "a " + extract(s, i);
   }
} /* add_a() */
