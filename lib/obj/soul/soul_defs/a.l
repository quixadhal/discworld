(ack
 *((undirected
    *((theirmsg [$TP_NAME ack's])
      (mymsg    [You ack])))))
$$
(accuse
 *((prepositions [of])
   (adverbs #)
   (noargs
    *((theirmsg [$TP_NAME accuses the butler])
      (mymsg    [The butler did it!])))
   (undirected
    *((theirmsg [$TP_NAME accuses $ADJ])
      (mymsg    [You accuse $ADJ])))
   (directed
    *((theirmsg [$TP_NAME accuses $OB_NAME of $ADJ])
      (mymsg    [You accuse $OB_NAME of $ADJ])))))
$$
(apologize
 *((prepositions [to])
   (adverbs profusely,happily,sadly,reservedly,carefully,slowly,perfunctorily,
	    curtly,visciously,sulkily,drunkenly,unhappily,professionally,
	    pompously,wholeheartedly,partially,wearily,fishily,facetiously,
	    unnecessarily,spontaneously,twice,thoroughly,humbly)
   (undirected
    *((theirmsg [$TP_NAME apologizes $ADJ])
      (mymsg    [You apologize $ADJ])))
   (directed
    *((theirmsg [$TP_NAME apologizes $ADJ to $OB_NAME])
      (mymsg    [You apologize $ADJ to $OB_NAME])))))
$$
(applaud
 *((adverbs wholeheartedly,loudly,softly,vaguely,briefly)
   (undirected
    *((theirmsg [$TP_NAME applauds $ADJ])
      (mymsg    [You applaud $ADJ])))
   (directed
    *((theirmsg [$TP_NAME applauds $OB_NAME $ADJ])
      (mymsg    [You applaud $OB_NAME $ADJ])))))
$$
(agree
 *((prepositions [with])
   (adverbs wholeheartedly,partially,wearily)
   (undirected
    *((theirmsg [$TP_NAME agrees $ADJ])
      (mymsg    [You agree $ADJ])))
   (directed
    *((theirmsg [$TP_NAME agrees $ADJ with $OB_NAME])
      (mymsg    [You agree $ADJ with $OB_NAME])))))
