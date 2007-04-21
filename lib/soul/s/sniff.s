(sniff
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (pattern "<string> <indirect:any-living>")
  (single
    (no-arguments
      (self "You sniff.")
      (rest "$mcname$ $verb$sniffs,sniff$.")
    )
    (arguments
      (arguments  dejectedly, noisily, silently, loudly, in disdain, 
                  lustily, with a bad cold, expressively, suddenly, 
                  haughtily, #)
      (self "You sniff $arg$.")
      (rest "$mcname$ $verb$sniffs,sniff$ $arg$.")
    )
    (arguments
      (arguments  hair, nose, ear, eyebrow, leg, foot, arm, back, 
                  chest, thigh, clothes, cheek, fingers, toe, 
                  celery, carrot, pants, spicy brain)
      (self "You sniff your $arg$.")
      (rest "$mcname$ $verb$sniffs,sniff$ $mposs$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self "You sniff at $hcname$.")
      (target "$mcname$ $verb$sniffs,sniff$ at you.")
      (rest "$mcname$ $verb$sniffs,sniff$ at $hcname$.")
    )
    (arguments
      (arguments  dejectedly, noisily, silently, loudly, in disdain, 
                  lustily, with a bad cold, expressively, suddenly, 
                  haughtily, #)
      (self   "You sniff $arg$ at $hcname$.")
      (target "$mcname$ $verb$sniffs,sniff$ $arg$ at you.")
      (rest   "$mcname$ $verb$sniffs,sniff$ $arg$ at $hcname$.")
    )
    (arguments
      (arguments  hair, nose, ear, eyebrow, leg, foot, arm, back,
                  chest, thigh, clothes, cheek, fingers, toe, 
                  celery, carrot, pants, spicy brain)
      (self   "You sniff $mhcname$ $arg$.")
      (target "$mcname$ $verb$sniffs,sniff$ your $arg$.")
      (rest   "$mcname$ $verb$sniffs,sniff$ $mhcname$ $arg$.")
    )
  )
)
