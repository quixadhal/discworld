/* this is [als]'s wonder trig object */

int *sintab;

create()
{
   sintab = ({
      0, 17452, 34899, 52335, 69756, 87155, 104528, 121869, 139173, 156434,
      173648, 190808, 207911, 224951, 241921, 258819, 275637, 292371, 
      309016, 325568, 342020, 358367, 374606, 390731, 406736, 422618, 
      438371, 453990, 469471, 484809, 500000, 515038, 529919, 544639, 
      559192, 573576, 587785, 601815, 615661, 629320, 642787, 656059, 
      669130, 681998, 694658, 707106, 719339, 731353, 743144, 754709, 
      766044, 777145, 788010, 798635, 809016, 819152, 829037, 838670, 
      848048, 857167, 866025, 874619, 882947, 891006, 898794, 906307, 
      913545, 920504, 927183, 933580, 939692, 945518, 951056, 956304, 
      961261, 965925, 970295, 974370, 978147, 981627, 984807, 987688, 
      990268, 992546, 994521, 996194, 997564, 998629, 999390, 999847, 1000000
   });
}

int sin(int angle)
/* return sin(angle in degrees) * 1000000 */
{
/* for 'huge' angles this may be slow, but for the small angles we are expecting
   it is fine */

   while (angle < 0) angle += 360;
   while (angle >= 360) angle -= 360;
   
   if (angle >= 270) return -sintab[360 - angle];
   if (angle >= 180) return -sintab[angle - 180];
   if (angle >= 90) return sintab[180 - angle];
   return sintab[angle];
}

int cos(int angle)
{
   return sin(90 - angle);
}

int tan(int angle)
{
/* 1000 * 1e6 is the maximum ... so we have to divide the bottom by 1000 :(
   losing accuracy along the way of course. */

   return 1000 * sin(angle) / (sin(90 - angle) / 1000);
}
