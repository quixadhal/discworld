(furbert
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (arguments
      (arguments  on the tummy, on the arm,on the foot, on the forehead, 
                  on the hand, on the leg, all over the body)
      (self   "You furbert $hcname$ $arg$.")
      (target "$mcname$ furbert$s$ you $arg$.")
      (rest   "$mcname$ furbert$s$ $hcname$ $arg$.")
    )
    (arguments
      (arguments  tummy, arm, foot, forehead, hand, leg, body, #)
      (self   "You furbert $mhcname$ $arg$.")
      (target "$mcname$ furbert$s$ your $arg$.")
      (rest   "$mcname$ furbert$s$ $mhcname$ $arg$.")
    )
  )
)

