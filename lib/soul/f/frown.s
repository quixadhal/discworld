(frown
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "What's the problem?")
      (rest "$mcname$ frown$s$.")
    )
    (arguments
      (arguments sadly, thoughtfully, meaningfully, dangerously, cruelly,
                 sternly, disapprovingly, sarcastically, worriedly, 
                 in concentration, in thought, in anger, in sadness, #)
      (self "You frown $arg$.")
      (rest "$mcname$ frown$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You frown at $hcname$.")
      (target "$mcname$ frown$s$ at you.")
      (rest  "$mcname$ frown$s$ at $hcname$.")
    )
    (arguments
      (arguments sadly, thoughtfully, meaningfully, dangerously, cruelly,
                 sternly, disapprovingly, sarcastically, worriedly, 
                 in concentration, in thought, in anger, in sadness, #)
      (self   "You frown $arg$ at $hcname$.")
      (target "$mcname$ frown$s$ $arg$ at you.")
      (rest   "$mcname$ frown$s$ $arg$ at $hcname$.")
    )
  )
)
