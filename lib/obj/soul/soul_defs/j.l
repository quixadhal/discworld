(juggle
 *((adverbs     balls,sticks,battons,cats,dogs,frogs,chickens,swords,knives,
                arms,legs,balls,trees,chain saws,bottles,guppies,torches,bings,
                tree stumps,logs,zoroasters,wine glasses,water,people,fire,
                men,rhinos)
   (undirected
    *((mymsg    [You juggle some $ADJ around])
      (theirmsg [$TP_NAME juggles some $ADJ around])))))
$$
(jump
 *((adverbs     excitedly,happily,carefully,slowly,joyfully,unsteadily,
                estaticly,worriedly,sideways,absentmindedly,
                like pinkfish,like a prancing pirana,scepticaly)
   (directed
    *((adverbs  ,)
      (mymsg    [You jump $ADJ up and down on $OB_NAME])
      (theirmsg [$TP_NAME jumps up and down $ADJ on $OB_NAME])))
   (undirected
    *((mymsg    [You jump up and down $ADJ])
      (theirmsg [$TP_NAME jumps up and down $ADJ])))))
$$
