(idle
    *((adverbs  ,like Pinkfish,like only Bastian can,like biggles,
            like sloths do,like a gherkin,like everyone else is,nonchalantly,
            quickly,unbelievably well,without using your hands,upside-down,
            hyperactively,standing still,artfully,around,intensely)
      (undirected
          *((mymsg  [You idle $ADJ])
           (theirmsg  [$TP_NAME idles $ADJ])))))
$$
(ignore
     *((undirected
         *((mymsg   [You ignore the world])
           (theirmsg   [$TP_NAME ignores the world])))
       (directed
         *((mymsg  [You ignore $OB_NAME])
           (theirmsg  [$TP_NAME ignores $OB_NAME])))))
$$
