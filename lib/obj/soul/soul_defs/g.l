(gasp
  *((adverbs    astonishment,excitement,terror,fear,fright,pain)
    (undirected
       *((theirmsg     [$TP_NAME gasps in $ADJ])
         (mymsg      [You gasp in $ADJ])))))
$$
(gesticulate
  *((noargs
     *((mymsg    [You gesticulate wildly])
       (theirmsg     [$TP_NAME gesticulates wildly]))))
$$
(gibber
    *((noargs
         *((mymsg    [You gibber madly])
        (theirmsg    [$TP_NAME gibbers madly]))))
$$
(giggle
    *((adverbs     inanely,loudly,stupidly,quietly,idiotically,fiendishly)
     (undirected
        *((mymsg   [You giggle $ADJ])
          (theirmsg   [$TP_NAME giggles $ADJ])))))
$$
(glare
    *((prepositions [at])
      (adverbs   stonily,sternly,hotly,intently,solemnly,idiotically,
                 evanly,evilly)
     (noargs
      *((mymsg    [You glare at the world in general])
         (theirmsg  [$TP_NAME glares at the world in general])))
     (undirected
         *((mymsg    [You glare $ADJ])
         (theirmsg    [$TP_NAME glares $ADJ])))
     (directed
        *((mymsg   [You glare at $OB_NAME $ADJ])
           (theirmsg    [$TP_NAME glares at $OB_NAME $ADJ]))))))
$$
(greet
    *((adverbs   happily,sadly,with open arms,ecstatically, carefully,
                 courteously,hysterically,reservedly,warmly,coldly,
                 insolently,curtly,absentmindedly)
     (directed
        *((mymsg   [You greet $OB_NAME $ADJ])
          (theirmsg   [$TP_NAME greets $OB_NAME $ADJ])))))
$$
(grimace
 *((undirected
    *((theirmsg [$TP_NAME grimaces])
      (mymsg    [You grimace])))))
$$
(grind
 *((directed
    *((adverbs  ,seductively,noisily,together,annoyingly,in terror,
                continuously,suggestively,painfully,angrily)
      (theirmsg [$TP_NAME grinds $TP_POSS teeth together $ADJ])
      (mymsg    [You grind your teeth together $ADJ])))))
$$
(grin
   *((prepositions [at])
     (adverbs   ,maniacally,evilly,wickedly,insanely,mischeviously,impishly,
                cheekily,idiotically,tentatively)
     (undirected
        *((mymsg    [You grin $ADJ])
          (theirmsg   [$TP_NAME grins $ADJ])))
     (directed
        *((mymsg    [You grin $ADJ at $OB_NAME])
          (theirmsg   [$TP_NAME grins $ADJ at $OB_NAME])))))
$$
(groan
   *((adverbs   ,loudly,resignedly,tiredly)
    (undirected
       *((mymsg   [You groan $ADJ])
         (theirmsg   [$TP_NAME groans $ADJ])))))
$$
(grope
    *((adverbs   ,#)
       (undirected
          *((mymsg   [You grope $ADJ])
          (theirmsg   [$TP_NAME gropes $ADJ])))
       (directed
          *((mymsg   [You grope $OB_NPOSS $ADJ])
            (theirmsg   [$TP_NAME gropes $OB_NPOSS $ADJ])))))
$$
(grovel
     *((prepositions [at])
          (adverbs   shamelessly,violently,unashamedly,respectfully,
                     like bastian,like a dog,sarcastically)
          (directed  
             *((mymsg  [You grovel $ADJ at $OB_NPOSS feet])
               (theirmsg   [$TP_NAME grovels $ADJ at $OB_NPOSS feet]))))
$$
(growl
     *((prepositions [at])
         (adverbs   ,menacingly,nastily,loudly,playfully)
     (undirected
        *((mymsg   [You growl $ADJ])
          (theirmsg   [$TP_NAME growls $ADJ])))
     (directed
        *((mymsg   [You growl $ADJ at $OB_NAME])
          (theirmsg   [$TP_NAME growls $ADJ at $OB_NAME])))))
$$
(grumble
    *((prepositions  [at])
      (adverbs   quietly,bitterly,menacingly,halfheartedly,seductively,
              suggestively,loudly,annoyingly,devastatingly,carefully,
              wildly,more or less,jealously)
      (undirected
           *((mymsg  [You grumble $ADJ])
            (theirmsg  [$TP_NAME grumbles $ADJ])))
      (directed
           *((mymsg  [You grumble $ADJ at $OB_NAME])
             (theirmsg  [$TP_NAME grumbles $ADJ at $OB_NAME])))))
$$
(gurgle
      *((prepositions  [at])
      (adverbs  ,noisily,rudely,loudly,mischeviously,happily,carefully,
                disgustingly,in surprise,like a train,quickly,slowly)
      (undirected
           *((mymsg  [You gurgle $ADJ])
             (theirmsg   [$TP_NAME gurgles $ADJ])))
      (directed
           *((mymsg  [You gurgle $ADJ at $OB_NAME])
             (theirmsg  [$TP_NAME gurgles $ADJ at $OB_NAME])))))
$$
