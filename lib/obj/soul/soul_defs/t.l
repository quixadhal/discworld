(tackle
 *((noargs
    *((theirmsg [$TP_NAME tackles $TP_FOO to the ground])
      (mymsg    [You tackle yourself to the ground])))
   (directed
    *((theirmsg [$TP_NAME tackles $OB_NAME in the chest])
     (mymsg    [You tackle $OB_NAME in the chest])))))
$$
(tap
 *((prepositions [on the])
   (adverbs foot,head,left leg,right leg,fingers,nose,shoulder,teeth,arm)
   (undirected
    *((theirmsg [$TP_NAME taps $TP_POSS $ADJ impatiently])
      (mymsg    [You tap your $ADJ impatiently])))
   (directed
    *((theirmsg [$TP_NAME taps $OB_NAME on the $ADJ])
      (mymsg    [You tap $OB_NAME on the $ADJ])))))
$$
(taunt
 *((noargs
    *((theirmsg [$TP_NAME taunts $TP_FOO])
      (mymsg    [You taunt yourself])))
   (directed
    *((theirmsg [$TP_NAME taunts $OB_NAME])
      (mymsg    [You taunt $OB_NAME])))))
$$
(tease
 *((noargs
    *((theirmsg [$TP_NAME teases $TP_FOO])
      (mymsg    [You tease yourself])))
   (directed
    *((adverbs  mercilessly,insistantly,annoyingly,lovingly,mischeviously,
                rudely,carefully,quickly,slowly,halfheartedly,idlely,
                obnoxiously)
      (theirmsg [$TP_NAME teases $OB_NAME $ADJ])
      (mymsg    [You tease $OB_NAME $ADJ])))))
$$
(thank
 *((adverbs profusely,gratuitously,lots,slightly,reservedly)
   (directed
    *((theirmsg [$TP_NAME thanks $OB_NAME $ADJ])
      (mymsg    [You thanks $OB_NAME $ADJ])))))
$$
(think
 *((adverbs #)
   (noargs
    *((theirmsg [$TP_NAME thinks carefully])
      (mymsg    [You think carefully])))
   (undirected
    *((theirmsg [$TP_NAME thinks $ADJ])
      (mymsg    [You think $ADJ])))))
$$
(throw
 *((directed
    *((theirmsg [$TP_NAME throws $OB_NAME to the floor in a happy embrace])
      (mymsg    [You $OB_NAME to the floor in a happy embrace])))))
$$
(tickle
 *((adverbs ,mercilessly,ruthlessly,fearlessly,quickly,sneakily,weakly,
            carefully,gently,harshly,inconcievablely,slowly)
   (directed
    *((theirmsg [$TP_NAME tickles $OB_NAME $ADJ])
      (mymsg    [You tickle $OB_NAME $ADJ])))))
$$
(tongue
 *((directed
    *((theirmsg [$TP_NAME pokes $TP_POSS tongue out at $OB_NAME])
      (mymsg    [You poke your tongue out at $OB_NAME])))))
$$
(tremble
 *((adverbs in fear,in nervousness,in pain,slightly,violently,insistantly,
            carefully,in a pink fit)
   (undirected
    *((theirmsg  [$TP_NAME trembles $ADJ])
      (mymsg     [You tremble $ADJ])))))
$$
(trip
 *((noargs
    *((theirmsg [$TP_NAME trips up])
      (mymsg    [You trip up]))
   (directed 
    *((theirmsg [$TP_NAME trips $OB_NAME])
      (mymsg    [You trip $OB_NAME])))))
$$
(tweak
 *((adverbs  bottom,ear,arm,leg,nose,bits)
   (directed
    *((theirmsg [$TP_NAME tweaks $OB_NAME $ADJ])
      (mymsg    [You tweak $OB_NAME $ADJ])))))
$$
(twiddle
 *((adverbs thumbs,fingers,hair,nose)
   (undirected
    *((theirmsg [$TP_NAME twiddles $TP_POSS $ADJ])
      (mymsg    [You twiddle your $ADJ])))))
$$
(twitch
 *((adverbs nose,arm,body,finger,igloo)
   (undirected
    *((theirmsg [$TP_NAME twitches $TP_POSS $ADJ])
      (mymsg    [You twitch your $ADJ])))))
