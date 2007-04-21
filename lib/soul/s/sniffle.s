(sniffle
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You sniffle.")
      (rest "$mcname$ sniffle$s$.")
    )
    (arguments
      (arguments dejectedly, noisily, silently, loudly, in disdain,
                 in a rousing chorus of 'A wizards staff has a knob on the end', 
                 happily, sadly, dramatically, #)
      (self "You sniffle $arg$.")
      (rest "$mcname$ sniffle$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You sniffle at $hcname$.")
      (target "$mcname$ sniffle$s$ at you.")
      (rest   "$mcname$ sniffle$s$ at $hcname$.")
    )
    (arguments
      (arguments dejectedly, noisily, silently, loudly, in disdain,
                 in a rousing chorus of 'A wizards staff has a knob on the end', 
                 happily, sadly, dramatically, #)
      (self   "You sniffle $arg$ at $hcname$.")
      (target "$mcname$ sniffle$s$ $arg$ at you.")
      (rest   "$mcname$ sniffle$s$ $arg$ at $hcname$.")
    )
  )
)

