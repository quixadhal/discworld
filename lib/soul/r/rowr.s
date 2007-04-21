(rowr
  (pattern "<string>" )
  (pattern "[at] <indirect:any-living>" )
  (pattern "<string> [at] <indirect:any-living>" )
  (pattern "[at] <indirect:any-living> <string>" )
  (single
    (no-arguments
      (self "You rowr." )
      (rest "$mcname$ rowr$s$." )
    )
    (arguments
      (arguments  like Drakkos, like a monkey, like a flying monkey,
                  seductively, flirtatiously, angrily, fiendishly, innocently,
                  devillishly, demonically, angelically, sensuously, playfully,
                  happily, sarcastically, cynically, humorously, loudly,
                  contemplatively, like a lawnmower, like a diesel engine, #)
      (self "You rowr $arg$." )
      (rest "$mcname$ row$rs$ $arg$." )
    )
  )
  (targeted
    (no-arguments
      (self   "You rowr at $hcname$." )
      (target "$mcname$ rowr$s$ at you." )
      (rest   "$mcname$ rowr$s$ at $hcname$." )
    )
    (arguments
      (arguments  like Drakkos, like a monkey, like a flying monkey,
                  seductively, flirtatiously, angrily, fiendishly, innocently,
                  devillishly, demonically, angelically, sensuously, playfully,
                  happily, sarcastically, cynically, humorously, loudly,
                  contemplatively, like a lawnmower, like a diesel engine, #)
      (self   "You rowr $arg$ at $hcname$." )
      (target "$mcname$ rowr$s$ $arg$ at you." )
      (rest   "$mcname$ rowr$s$ $arg$ at $hcname$." )
    )
    (arguments
      (arguments  star, asterisk)
      (self   "You *rowr* at $hcname$." )
      (target "$mcname$ *rowrs* at you." )
      (rest   "$mcname$ *rowrs* at $hcname$." )
    )
  )
)

