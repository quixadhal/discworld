(cackle
 *((noargs
    *((theirmsg [$TP_NAME throws back $TP_POSS head and cackles with glee])
      (mymsg    [You throw back your head and cackle with glee])))
   (undirected
    *((adverbs with glee,maniacaly,mischeviously,nastily,
	       depairingly,womblely,backwards)
      (theirmsg [$TP_NAME cackles $ADJ])
      (mymsg    [You cackle $ADJ])))))
$$
(calm
 *((prepositions [down])
   (undirected
    *((theirmsg [$TP_NAME calms down])
      (mymsg    [You calm down])))
   (directed
    *((theirmsg [$TP_NAME calms $OB_NAME down])
      (mymsg    [You calm $OB_NAME down])))))
$$
(caper
 *((prepositions [around])
   (noargs
    *((theirmsg [$TP_NAME capers around madly])
      (mymsg    [You caper around madly])))
   (undirected
    *((adverbs fool,chicken,womble,jester,bimbo,blonde,sulam,seductress,
	       eager person,vivacious person,madman,madwoman,madperson,
	       frog on the run,happy chap,polite gentleperson,woman,
	       no we aren't sexist)
      (theirmsg [$TP_NAME capers around like a $ADJ])
      (mymsg    [You caper around like a $ADJ])))))
$$
(caress
 *((directed
    *((adverbs ,gently,strangely,violently,firmly,seductively,suggestively,
	       slightly,sensually,reluctantly,politely,lovingly,vigorously,
	       eagerly,vivaciously)
      (theirmsg [$TP_NAME caresses $OB_NAME $ADJ])
      (mymsg    [You caress $OB_NAME $ADJ])))))
$$
(challenge
 *((directed
    *((obmsg    [You turn $TP_NAME into a fish])
      (theirmsg [$OB_NAME turns $TP_NAME into a fish])
      (mymsg    [Make your own challenges to $OB_NAME])))))
$$
(cheer
 *((undirected
    *((adverbs  enthusiasticly,excitedly,quietly,calmly,carefully,politely,
             wildly,oddly,unconvincingly,happily,slightly,up)
      (theirmsg [$TP_NAME cheers $ADJ])
      (mymsg    [You cheer $ADJ])))
   (directed
    *((adverbs  enthusiasticly,excitedly,quietly,calmly,carefully,politely,
             wildly,oddly,unconvincingly,happily,slightly,up)
      (theirmsg [$TP_NAME cheers $ADJ $OB_NAME])
      (mymsg    [You cheer $ADJ $OB_NAME])))))
$$
(choke
 *((noarg
    *((theirmsg [$TP_NAME chokes])
      (mymsg    [You choke])))
   (directed
    *((force    [breath pain])
      (thiermsg [$TP_NAME chokes $OB_NAME])
      (mymsg    [You choke $OB_NAME])))))
$$
(chuckle
 *((undirected
    *((adverbs  politely,nastily,demonically)
      (theirmsg [$TP_NAME chuckles $ADJ])
      (mymsg    [You chuckle $ADJ])))))
$$
(clap
 *((undirected
    *((adverbs  briefly,loudly,enthusiasticly,halfheartedly)
      (theirmsg [$TP_NAME claps $TP_POSS $ADJ])
      (mysg     [You clap your hands $ADJ])))))
$$
(comfort
 *((directed
    *((adverbs  sincerly,wholeheartly,mischeviously,halfheartedly,vaguely,
                eagerly,completely,lovingly,gently,tenderly,sadly,
                in earnest,with your frog,in the ear,in the leg,
                twice,anxiously,carefully,delicately,like someone else)
      (theirmsg [$TP_NAME comforts $OB_NAME $ADJ])
      (mymsg    [You comfort $OB_NAME $ADJ])))))
$$
(confess
 *((prepositions [tp])
   (adverbs     #,to being a witch)
   (undirected
    *((theirmsg [$TP_NAME confess's $ADJ])
      (mymsg    [I confess $ADJ])))
   (directed
    *((theirmsg [$TP_NAME confess's to $OB_NAME $ADJ])
      (mymsg    [You confess to $OB_NAME $ADJ])))))
$$
(congrat
 *((directed
    *((adverbs  ,heartily,warmly)
      (theirmsg [$TP_NAME congratulates $OB_NAME $ADJ])
      (mymsg    [You congratlate $OB_NAME $ADJ])))
   (noargs
    *((theirmsg [$TP_NAME congratulates everyone])
      (mymsg    [You congratulate everyone])))))
$$
(cough
 *((undirected
    *((adverbs  ,happily,loundly,quietly,excitedly,disgustingly,
                sadly,pinkly)
      (theirmsg [$TP_NAME coughs $ADJ])
      (mymsg    [You cough $ADJ])))))
$$
(cower
 *((undirected
    *((adverbs  in a corner,fearfully,slyly,shyly,in shame,nastily)
      (theirmsg [$TP_NAME cowers $ADJ])
      (mymsg    [You cower $ADJ])))))
$$
(cringe
 *((undirected
    *((adverbs  terror,embarassment)
      (theirmsg [$TP_NAME cringes in $ADJ])
      (mymsg    [You cringe in $ADJ])))))
$$
(croak
 *((noargs
    *((theirmsg [$TP_NAME croaks (ribbit)])
      (mymsg    [Ribbit])))))
$$
(cross
 *((undirected
    *((adverbs  fingers,legs,eyes,nose)
      (theirmsg [$TP_NAME crosses $TP_POSS $ADJ])
      (mymsg    [You cross your $ADJ])))))
$$
(cry
 *((noargs
    *((theirmsg [$TP_NAME crys loudly])
      (mymsg    [Wahhhhhhhh])))))
$$
(cuddle
 *((directed
    *((theirmsg [$TP_NAME cuddles $OB_NAME])
      (mymsg    [You cuddle $OB_NAME])))))
$$
(curtsey
 *((prepositions [before to])
   (adverbs      ,gracefully,humbly,quickly,low,slightly
   (undirected
    *((theirmsg  [$TP_NAME curtseys $ADJ])
      (mymsg     [You curtsey $ADJ])))
   (directed
    *((theirmsg  [$TP_NAME curtsies before $OB_NAME $ADJ])
      (mymsg     [You curtsey before $OB_NAME $ADJ]))))
$$
