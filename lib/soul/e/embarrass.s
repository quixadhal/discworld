(embarrass
  (pattern "<indirect:any-living> <string>")
  (pattern "<indirect:any-living>")
  (targeted
    (no-arguments
      (self   "You embarrass $hcname$.")
      (target "$mcname$ embarrass$s$ you.")
      (rest  "$mcname$ embarrass$s$ $hcname$.")        
    )
    (arguments
      (arguments about sex, with a stick, under a water tower,
                 about your personal problems, in a fluffy way, 
                 using inappropriate soul commands, 
                 enthusiastically, slowly, excitedly, seductively, quickly,
                 backwards, under a bridge, in a manger, quietly, loudly,
                 while talking to someone else, heaps, completely, totally,
                 about real life, you in front of your friends,
                 you in front of your parents, in front of Shalla's grandmother,
                 you about mischatting, about netsexing a cre, you about dying,
                 you about typos, #)
      (self   "You embarrass $hcname$ $arg$.")
      (target "$mcname$ embarrass$s$ you $arg$.")
      (rest  "$mcname$ embarrass$s$ $hcname$ $arg$.")
    )
  )
)

