(pose
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You strike a pose.")
      (rest "$mcname$ strike$s$ a pose.")
    )
    (arguments
      (arguments  keenly, hoopily, froggily, slimily, brilliantly, wetly,
                  tiredly, drunkenly, smarmily, smartly, quietly, awkwardly, 
                  loudly, evenly, sarcastically, like a creator, hollowly, 
                  mechanically, in victory, weakly, icily, soggily,
                  overworkedly, like a pretzel, greenly, discreetly, in self 
                  defense, woundedly, like a cat, fuzzily, sharply, like a 
                  parcel, like a desk lamp, electrically, absentmindedly, 
                  hurriedly, #)
      (self "You pose $arg$.")
      (rest "$mcname$ pose$s$ $arg$.")
    )
    (arguments 
      (arguments heroic, modest, superhero, runway, memorable, cute, fussy)
      (self "You strike a $arg$ pose.")
      (rest "$mcname$ strike$s$ a $arg$ pose.")
    )
  )
  (targeted
    (no-arguments
      (self   "You strike a pose at $hcname$.")
      (target "$mcname$ strike$s$ a pose at you.")
      (rest  "$mcname$ strike$s$ a pose at $hcname$.")
    )
    (arguments
      (arguments  keenly, hoopily, froggily, slimily, brilliantly, wetly,
                  tiredly, drunkenly, smarmily, smartly, quietly, awkwardly, 
                  loudly, evenly, sarcastically, like a creator, hollowly, 
                  mechanically, in victory, weakly, icily, soggily,
                  overworkedly, like a pretzel, greenly, discreetly, in self 
                  defense, woundedly, like a cat, fuzzily, sharply, like a 
                  parcel, like a desk lamp, electrically, absentmindedly, 
                  hurriedly, #)
      (self   "You pose $arg$ at $hcname$.")
      (target "$mcname$ pose$s$s $arg$ at you.")
      (rest  "$mcname$ pose$s$ $arg$ at $hcname$.")
    )
    (arguments 
      (arguments heroic, modest, superhero, runway, memorable, cute, fussy)
      (self "You strike a $arg$ pose at $hcname$.")
      (target "$mcname$ pose$s$ $arg$ at you.")
      (rest "$mcname$ strike$s$ a $arg$ pose at $hcname$.")
    )
  )
)

