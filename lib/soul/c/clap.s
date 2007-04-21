(clap
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You clap your hands.")
      (rest "$mcname$ claps $mposs$ hands.")
    )
    (arguments
      (arguments  briefly, loudly, enthusiastically, halfheartedly, 
                  sarcastically, #)
      (self "You clap your hands $arg$.")
      (rest "$mcname$ claps $mposs$ hands $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments  wholeheartedly, loudly, softly, vaguely, briefly, 
                  sarcastically, #)
      (self   "You clap at $hcname$ $arg$.")
      (target "$mcname$ claps at you $arg$.")
      (rest   "$mcname$ claps at $hcname$ $arg$.")
    )
  )
)



