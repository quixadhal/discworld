(bleed
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<indirect:any-living> [at] <string>")
  (pattern "<string> [at] <indirect:any-living>")
  (single
    (no-arguments
      (self "You bleed.")
      (rest "$mcname$ bleed$s$.")
    )
    (arguments 
      (arguments all over, messily, lots, slowly, 
                 in gushes, bitterly, a bit, #)
      (self "You bleed $arg$.")
      (rest "$mcname$ bleed$s$ $arg$.")
    )
    (arguments
      (arguments heart, leg, arm, head, ear, nose, 
                 foot, ankle, toe, hand, finger, neck,
                 throat, stomach, back, mouth, ear, 
                 ears, eye, eyes, wounds, veins,
                 arteries)
      (self "Your $arg$ bleeds.")
      (rest "$mposs$ $arg$ bleeds.")
    )
  )
  (targeted
    (no-arguments
      (self   "You bleed on $hcname$.")
      (target "$mcname$ bleed$s$ on you.")
      (rest   "$mcname$ bleed$s$ on $hcname$.")
    )
    (arguments
      (arguments all over, messily, lots, slowly, 
                 in gushes, bitterly, a bit, #)
      (self   "You bleed $arg$ on $hcname$.")
      (target "$mcname$ bleed$s$ $arg$ on you.")
      (rest   "$mcname$ bleed$s$ $arg$ on $hcname$.")
    )
    (arguments
      (arguments heart)
      (self   "Your heart bleeds for $hcname$.")
      (target "$mcname$'s heart bleed$s$ for you.")
      (rest   "$mcname$'s heart bleed$s$ for $hcname$.")
    )
  )
)

