/* very temporary hack to get DW off its knees on 1/12/92 by [als] */
int member_array(mixed el_match, mixed *arr)
{
   int i, bing;

   bing = -1;
   for (i = 0; i < sizeof(arr); i++) {
      if (el_match == arr[i]) {
         bing = i;
         break;
      }
   }
   return bing;
}
