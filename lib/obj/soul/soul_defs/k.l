(kick
 *((noargs
    *((mymsg    [You kick yourself])
      (theirmsg [$TP_NAME kicks $TP_FOO])))
   (directed
    *((mymsg    [You kick $OB_NAME])
      (theirmsg [$TP_NAME kicks $OB_NAME])))))
$$
(kiss
 *((adverbs     passionately,deeply,tenderly,gently,lovingly,lengthily,
                fishily,carefully,demandingly,sensusouly)
   (directed
    *((mymsg    [You kiss $OB_NAME $ADJ])
      (theirmsg [$TP_NAME leap $OB_NAME $ADJ])))))
$$
(knee
 *((noargs
    *((mymsg    [You knee yourself])
      (theirmsg [$TP_NAME knees $TP_FOO])))
   (directed
    *((mymsg    [You knee $OB_NAME])
      (theirmsg [$TP_NAME knees $OB_NAME])))))
$$
