(waggle
 *((prepositions [at])
   (adverbs finger,arm,leg,head,nose,ear,frog,pinky,toe)
    (undirected
    *((theirmsg [$TP_NAME waggles their $ADJ])
      (mymsg    [You waggle your $ADJ])))
    (directed
     *((theirmsg [$TP_NAME waggles $TP_POSS $ADJ at $OB_NAME])
       (mymsg    [You waggle your $ADJ at $OB_NAME])))))
$$
(waltz
 *((prepositions [with])
   (directed
    *((theirmsg [$TP_NAME waltzs around the room with $OB_NAME])
      (mymsg    [You waltz around the room with $OB_NAME])))))
$$
(wave
 *((prepositions [at])
   (adverbs ,tiredly,sleepily,profunctialy,lazily,sadly,happilly,redundantly,
            forlornly,excitedly,longingly,pointlessly,your tentacles,
            pasionately,greenly)
   (noargs
    *((theirmsg [$TP_NAME waves])
      (mymsg    [You wave])))
   (undirected
    *((theirmsg [$TP_NAME wave $ADJ])
      (mymsg    [You wave $ADJ])))
   (directed
    *((theirmsg [$TP_NAME wave $ADJ to $OB_NAME])
      (mymsg    [You wave $ADJ to $OB_NAME])))))
$$
(wimper
 *((adverbs painfully,feafully,carefully,dimuatively,happily,winningly,
            in triumph,with the wombles)
   (undirected
    *((theirmsg [$TP_NAME whimpers $ADJ])
      (mymsg    [You whimper $ADJ])))))
$$
(wibble
 *((adverbs like bil,loudly,constantly,anoyingly,around,up,down,sideways)
   (undirected
    *((theirmsg [$TP_NAME wibbles $ADJ])
      (mymsg    [You wibble $ADJ])))))
$$
(whicker
 *((noargs
    *((theirmsg [$TP_NAME whickers like a horse])
      (mymsg    [You whicker like a horse])))))
$$
(whistle
 *((prepositions [at])
   (adverbs innocently,appreciatively,loudly,musically,pleasantly,
            discordantly)
   (undirected
    *((theirmsg [$TP_NAME whistles $ADJ])
      (mymsg    [You whistle $ADJ])))
   (directed
    *((theirmsg [$TP_NAME whistles $ADJ with $OB_NAME])
      (mymsg    [You whistle $ADJ with $OB_NAME])))))
$$
(wince
 *((noargs
    *((theirmsg [$TP_NAME winces])
      (mymsg    [You wince])))))
$$
(wiggle
 *((prepositions [at])
   (adverbs ,enticingly,suggestively,vigourously,arrogantly,nonchalantly,
            quickly)
   (undirected
    *((theirmsg  [$TP_NAME wiggles $TP_POSS bottom $ADJ])
      (mymsg     [You wiggle your bottom $ADJ])))
   (directed
    *((theirmsg  [$TP_NAME wiggles $TP_POSS bottom at $OB_NAME $ADJ])
      (mymsg     [You wiggle your bottom at $OB_NAME $ADJ])))))
$$
(wink
 *((prepositions [at])
   (adverbs ,suggestively,broadly,slowly,loudly,grumpily,despairingly,coyly,
            desperately,insolently,curtly,lovingly,patiently,sadisticly,
            warily,seducitive,lazily)
   (undirected 
    *((theirmsg [$TP_NAME winks $ADJ])
      (mymsg    [You wink $ADJ]))
   (directed 
    *((theirmsg [$TP_NAME winks $ADJ at $OB_NAME])
      (mymsg    [You wink $ADJ at $OB_NAME])))))
$$
(wipe
 *((prepositions [on])
   (noargs
    *((theirmsg [$TP_NAME wipes the sweat from $TP_POSS brow])
      (mysmg    [You wipe the sweat from your brow])))
   (directed
    *((adverbs hands,leg,arm,nose)
      (theirmsg [$TP_NAME wipes $TP_POSS $ADJ on $OB_NAME])
      (mymsg    [You wipe your $ADJ on $OB_NAME])))))
$$
(wobble
 *((adverbs around a bit,like jelly,unsteadily,happily,carefully,
            sideways,steadily,a bit and turn into Zellski)
   (undirected
    *((thiermsg [$TP_NAME wobbles $ADJ])
      (mymsg    [You wobble $ADJ])))))
$$
(womble
 *((adverbs #)
   (undirected
    *((theirmsg [$TP_NAME wombles around $ADJ])
      (mymsg    [You womble around $ADJ])))))
$$
(wonder
 *((adverbs #,about reality and how boring it can be)
   (undirected
    *((theirmsg [$TP_NAME wonders $ADJ])
      (mymsg    [You wonder $ADJ])))))
$$
(worship
 *((undirected
    *((abverbs   Muds,#)
      (theirmsg [$TP_NAME worships $ADJ])
      (mymsg    [You worship $ADJ])))
   (directed
    *((theirmsg [$TP_NAME worships $OB_NAME])
      (mymsg    [You worship $OB_NAME])))))
