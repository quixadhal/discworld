(bark
 *((prepositions [at like a])
   (adverbs dog,cat,tree,wolf,hunting dog,labrador,german shepard,
	    something else that barks,bull terrier,train)
   (undirected
    *((theirmsg [$TP_NAME barks like a $ADJ])
      (mymsg    [You bark like a $ADJ])))
   (directed
    *((theirmsg [$TP_NAME barks like a $ADJ at $OB_NAME])
      (mymsg    [You bark like a $ADJ at $OB_NAME])))))
$$
(beam
 *((prepositions [at])
   (adverbs brightly,admiringly,strangely,hapilly,ecstatically,fishily,
	    like a train)
   (undirected
    *((theirmsg [$TP_NAME beams $ADJ])
      (mymsg    [You beam $ADJ])))
   (directed
    *((theirmsg [$TP_NAME beams $ADJ at $OB_NAME])
      (mymsg    [You beam $ADJ at $OB_NAME])))))
$$
(beep
 *((adverbs #,happily)
   (undirected
    *((theirmsg [$TP_NAME beeps $ADJ])
      (mymsg    [You beep $ADJ])))))
$$
(beg
 *((prepositions [like a])
   (adverbs dog,cat,frog,chicken,frisbee,cow,large bear,poodle,chihuahua,
	    train,beggar,noble man,lassie,labrador,german shepard,cannibal,
	    chimpanzee,orang-u-tang,librarian,fish,dolphin,tokzic,banshi,
	    bloodhound,st bernard,virgin mary,husky,wolf,dingo,alsation)
   (undirected
    *((theirmsg [$TP_NAME begs like a $ADJ])
      (mymsg    [You beg like a $ADJ])))
   (directed
    *((force    [smile $OB_NAME])
      (theirmsg [$TP_NAME begs like a $ADJ at $OB_NAME])
      (mymsg    [You beg like a $ADJ at $OB_NAME])))))
$$
(bing
 *((prepositions [at])
   (adverbs happily,sadly,joyfully,sagely,knowingly,quickly,loudly,carefully,
	    balefully,excitedly,nervously,brightly,uncertainly)
   (undirected
    *((theirmsg [$TP_NAME bings $ADJ])
      (mymsg    [You bing $ADJ])))
   (directed
    *((theirmsg [$TP_NAME bings $ADJ at $OB_NAME])
      (mymsg    [You bing $ADJ at $OB_NAME])))))
$$
(bird
 *((directed
    *((theirmsg
       [$TP_NAME raises $TP_POSS middle finger and gives $OB_NAME the bird])
      (mymsg
       [You raise your middle finger and give $OB_NAME the bird])))))
$$
(bite
 *((undirected
    *((theirmsg [$TP_NAME bites $OB_FOO])
      (mymsg    [You bite yourself]))
    (directed
     *((adverbs ,hard,nastily,carefuly,politely,fishily,lovingly,refreshingly,
		happily,like a train,pointlessly,objectively,sulkily,gently,
		brightly,excitedly,uncertainly,like a dog,wolfishly,
		mercilessly,ruthlessly,softly,sweetly,profesionally,hungrily,
		mischeviously,ankles,arm,leg,casually,nose,ear,toe,finger)
       (theirmsg [$TP_NAME bites $OB_NAME $ADJ])
       (mymsg    [You bite $OB_NAME $ADJ]))))))
$$
(bleed
 *((noargs
    *((theirmsg [$TP_NAME bleeds])
      (mymsg    [You bleed])))
   (undirected
    *((adverbs  ,heart,leg,arm,head,ear,nose)
      (theirmsg [$TP_NPOSS $ADJ bleeds])
      (mymsg    [Your $ADJ bleeds])))))
$$
(blink
 *((adverbs ,owlishly,slowly,carefully)
   (undirected
    *((theirmsg [$TP_NAME blinks $ADJ])
      (mymsg    [You blink $ADJ])))))
$$
(blush
 *((adverbs in embarassment,redly,slightly,worriedly)
   (undirected
    *((theirmsg [$TP_NAME blushes $ADJ])
      (mymsg    [You blush $ADJ])))))
$$
(boggle
 *((prepositions [at])
   (noargs
    *((theirmsg [$TP_NAME boggles at the concept])
      (mymsg    [You boggle at the concept])))
   (undirected
    *((adverbs #)
      (theirmsg [$TP_NAME boggles at $ADJ])
      (mymsg    [You boggle at $ADJ])))))
$$
(bop
 *((undirected
    *((theirmsg [$TP_NAME bops to the beat])
      (mymsg    [You bop to the beat])))
   (directed
    *((theirmsg [$TP_NAME bops $OB_NAME on the head])
      (mymsg    [You bop $OB_NAME on the head])))))
$$
(bounce
 *((prepositions [around])
   (noargs
    *((theirmsg [$TP_NAME bounces around])
      (mymsg    [B O U N C E ! !])))
   (adverbs excitedly,fearfully,happily)
   (undirected
    *((theirmsg [$TP_NAME bounces around $ADJ])
      (mymsg    [You bounce around $ADJ])))
   (directed
    *((adverbs carefully,wildly,pinkly,joyfully,once,slowly,suggestively,
	      wombely)
      (theirmsg [$TP_NAME bounces around $OB_NAME $ADJ])
      (mymsg    [You bounce around $OB_NAME $ADJ])))))
$$
(breakdance
 *((undirected
    *((theirmsg [$TP_NAME does a really complicated break-dance manouvre])
      (mymsg    [You do a really complicated break-dance manouvre])))))
$$
(breathe
 *((undirected
    *((adverbs quickly,seductively,painfully,nastily,slowly,once,
	       heavily,carefully,heatedly)
      (theirmsg [$TP_NAME breathes $ADJ])
      (mymsg    [You breathe $ADJ])))))
$$
(burp
 *((undirected
    *((adverbs disgustingly,faintly,loudly,rudely)
      (theirmsg [$TP_NAME burps $ADJ])
      (mymsg    [You burp $ADJ])))))
$$
(bow
 *((prepositions [to])
   (adverbs solemnly,deeply,formally,hastily,slightly,
	    insolently,clumsily,dextrously,wildly,colourfully)
   (undirected
    *((theirmsg [$TP_NAME bows $ADJ])
      (mymsg    [You bow $ADJ])))
   (directed
    *((theirmsg [$TP_NAME bows $ADJ to $OB_NAME])
      (mymsg    [You bow $ADJ to $OB_NAME])))))
$$
(burn
 *((noargs
    *((theirmsg [$TP_NAME sets fire to $TP_FOO])
      (mymsg    [You set fire to yourself])))
   (directed
    *((adverbs hair,leg,arm,nose,teeth,frog,carpet,net lag,
      fish,invis,bed,ego,eyes,house,furniture,tongue,keyboard,brain,code,
      ideas,hopes,dreams,cat,personality,desires,beliefs)
      (theirmsg [$TP_NPOSS sets fire to $OB_NAME $ADJ])
      (mymsg    [You set fire to $OB_NPOSS $ADJ]))))

      
