(eye
 *((directed
    *((theirmsg [$TP_NAME eyes $OB_NAME over])
      (mymsg    [You eye $OB_NAME over])))))
$$
(expect
 *((directed
    *((theirmsg [$TP_NAME looks expectantly at $OB_NAME])
      (mymsg    [You look expectantly at $OB_NAME])))))
$$
(explode
 *((noargs
    *((theirmsg [$TP_NAME explodes into thousands of tiny pieces])
      (mymsg    [You explode into thousands of tiny peices])))))
$$
