(nestle
 *((adverbs     affectionately,lovingly,curiously)
   (directed
    *((mymsg    [You nestle up to $OB_NAME $ADJ])
      (theirmsg [$TP_NAME nestle up to  $OB_NAME $ADJ])))))
$$
(nibble
 *((adverbs     ear,finger,lip,arm,leg,toe,nose)
   (directed
    *((mymsg    [You nibble on $OB_NPOSS $ADJ])
      (theirmsg [$TP_NAME nibbles on $OB_NPOSS $ADJ])))))
$$
(nod
 *((adverbs     solemnly,happily,brightly,curtly,patiently,slowly,knowingly,
                vigorously,tentatively)
   (undirected
    *((mymsg    [You nod $ADJ ])
      (theirmsg [$TP_NAME nods $ADJ])))
   (directed
    *((mymsg    [You nod $ADJ at $OB_NAME])
      (theirmsg [$TP_NAME $ADJ at $OB_NAME])))))
$$
(noise
 *((undirected
    *((adverbs  bungle,flurgle,zongle-wop,wop,whirr,burble,frog,moo,train,
                hairy,twisted,wap,pop,rat,mouse,chicken,womble,guppy,
                cow,rabbit,kangaroo,gumby)
      (theirmsg [$TP_NAME makes a $ADJ noise])
      (mymsg    [You make a $ADJ noise]))
   (noargs
    *((theirmsg [$TP_NAME makes a funny noise])
      (mymsg    [You make a funny noise])))))
$$
(nudge
 *((directed
    *((theirmsg [$TP_NAME nudges $OB_NAME])
      (mymsg    [You nudge $OB_NAME]))))
$$
(nuzzle
 *((directed
    *((theirmsg [$TP_NAME nuzzles $OB_NAME affectionately])
      (mymsg    [You nuzzle $OB_NAME affectionately]))))
$$
