(panic
 *((noargs
    *((theirmsg [$TP_NAME panics])
      (mymsg    [You panic])))))
$$
(pat
 *((adverbs     head,back,leg,arm,nose,bottom)
   (directed
    *((theirmsg [$TP_NAME pats $OB_NAME on the $ADJ])
      (mymsg    [You pat $OB_NAME on the $ADJ]))))
$$
(pet
 *((directed
    *((theirmsg [$TP_NAME pets $OB_NAME])
      (mymsg    [You pet $OB_NAME])))))
$$
(peer
 *((adverbs     ,suspiciously,quizically,anoxiously,stupidly,warningly,
                expectantly,happily,sadly,desperately,worriedly,strangely,
                uncertainly,lopsidedly,lazily,rudely,disappointedly,
                fishily,fearfully,like a train,curiously,tentatively,
                warily,nervously,carefully,warningly,owlishly,dazedly)
   (undirected
      *((theirmsg  [$TP_NAME peers $ADJ around the room])
      (mymsg    [You peer $ADJ around the room])))
   (directed
    *((theirmsg [$TP_NAME peers $ADJ at $OB_NAME])
      (mymsg    [You peer $ADJ at $OB_NAME]))))
$$
(pinch
 *((noargs
    *((theirmsg [$TP_NAME pinches $TP_FOO])
      (mymsg    [You pinch yourself])))
   (directed
    *((advebrs  bottom,leg,arm,frog,cheek)
      (theirmsg [$TP_NAME pinchs $OB_NAME on the $ADJ])
      (mymsg    [You pinch $OB_NAME on the $ADJ])))))
$$
(ping
 *((directed
    *((force    [ack $TP_NAME])
      (theirmsg [$TP_NAME pings $OB_NAME])
      (mymsg    [You ping $OB_NAME])))))
$$
(plead
 *((adverbs     mercy,frogs,lots of money,a Decstation 5000,
                this stupid soul to go away,an X windows terminal,insanity,
                creators,bil,pinkfish's sanity)
   (undirected
    *((theirmsg [$TP_NAME pleads for $ADJ])
      (mymsg    [You plead for $ADJ])))))
$$
(point
 *((adverbs     accusingly,straight)
   (directed
    *((theirmsg [$TP_NAME point $ADJ at $OB_NAME])
      (mymsg    [You point $ADJ at $OB_NAME]))))
$$
(poke
 *((directed
    *((adverbs  #,ribs,nose,leg,frog,arm)
      (theirmsg [$TP_NAME pokes $OB_NAME in the $ADJ])
      (mymsg    [You poke $OB_NAME in the $ADJ])))))
$$
(ponder
 *((adverbs    #,the situation)
   (undirected
    *((theirmsg [$TP_NAME ponders $ADJ])
      (mymsg    [You ponder $ADJ])))))
$$
(pout
 *((undirected
    *((theirmsg [$TP_NAME pouts])
      (mymsg    [You pout])))))
$$
(puke
 *((directed
    *((theirmsg [$TP_NAME pukes on $OB_NAME])
      (mymsg    [You puke on $OB_NAME])))))
$$
(punch
 *((directed
    *((theirmsg [$TP_NAME punches $OB_NAME])
      (mymsg    [You punch $OB_NAME])))))
$$
(purr
 *((adverbs     contentedly,loudly,happily,sensuously)
   (undirected
    *((theirmsg [$TP_NAME purrs $ADj])
      (mymsg    [You purrs $ADJ])))))
$$
(push
 *((noargs
    *((theirmsg [$TP_NAME pushes $TP_FOO over])
      (mymsg    [You push yourself over]))
   (directed
    *((theirmsg [$TP_NAME pushs $OB_NAME over])
      (mymsg    [You push $OB_NAME over]))))
$$
(puzzle
 *((noargs
    *((theirmsg [$TP_NAME looks puzzled])
      (mymsg    [You look puzzled])))))
   (undirected
    *((theirmsg [$TP_NAME puzzles over $ADJ])
      (mymsg    [You puzzle over $ADJ])))))
