(hang
     *((adverbs  shame,sorrow,despair,happiness)
       (undirected
           *((mymsg   [You hang your head in $ADJ])
             (theirmsg   [$TP_NAME hangs $TP_NPOSS head in $ADJ])))))
$$
(headbutt
     *((directed
          *((mymsg  [You headbutt $OB_NAME])
            (theirmsg  [$TP_NAME headbutts $OB_NAME]))))
$$
(headstand
      *((prepositions [on])
       (undirected
       *((mymsg  [You stand on your head])
           (theirmsg  [$TP_NAME stands on $TP_NPOSS head])))
      (directed
       *((mymsg  [You stand on $OB_NPOSS head])
          (theirmsg   [$TP_NAME stands on $OB_NPOSS head])))))
$$
(hiccup
     *((noargs
          *((mymsg   [You hiccup])
      (theirmsg   [$TP_NAME hiccups]))))
$$
(hold
     *((prepositions   [on,onto])
      (adverbs   ,close,tightly,warmly,gently,lovingly,back,up)
       (noargs
           *((mymsg   [You hold back])
          (theirmsg   [$TP_NAME holds back])))
      (directed
            *((mymsg   [You hold $OB_NAME $ADJ])
             (theirmsg  [$TP_NAME holds $OB_NAME $ADJ])))))
$$
(hop
     *((noargs
         *((mymsg   [You hop around for a bit])
           (theirmsg  [$TP_NAME hops around for a bit]))))
$$
(hug
    *((adverbs   ,tightly,warmly,gently,close)
      (directed
          *((mymsg   [You hug $OB_NAME $ADJ])
          (theirmsg   [$TP_NAME hugs $OB_NAME $ADJ])))))
$$
(hum
    *((adverbs  a merry tune,tunelessly,annoyingly,aguely,absentmindedly,
                while you work,distractedly,pointlessly,silently,loudly,
                rudely)
       (undirected
           *((mymsg   [You hum $ADJ])
             (theirmsg  [$TP_NAME hums $ADJ])))))
$$
