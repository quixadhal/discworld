(dance
 *((noargs
    *((mymsg    [You do the disco duck.  Feels silly doesn't it?])
      (theirmsg [$TP_NAME does the disco duck])))
   (directed
    *((theirmsg [$TP_NAME sweeps $OB_NAME across the dance floor])
      (mymsg    [You sweep $OB_NAME accross the dance floor])))))
$$
(daydream
 *((noargs
    *((theirmsg [$TP_NAME daydreams])
      (mymsg    [You daydream])))
   (undirected
    *((adverbs  #)
      (theirmsg [$TP_NAME daydreams $ADJ])
      (mymsg    [You daydream $ADJ])))))
$$
(drool
 *((noargs
    *((theirmsg [$TP_NAME starts to drool])
      (mymsg    [You start to drool])))
   (directed
    *((theirmsg [$TP_NAME drools all over $OB_NAME])
      (mymsg    [You drool all over $OB_NAME])))))
$$
(duh
 *((noargs
    *((theirmsg [$TP_NAME screams "Duh!"])
      (mymsg    [You scream "Duh!"])))))
$$
