(tremble
  (pattern "<string>")
  (pattern "<indirect:any-living>" )
  (pattern "<indirect:any-living> <string>" )
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You tremble.")
      (rest "$mcname$ tremble$s$.")
    )
    (arguments
      (arguments in fear, nervously, in pain, slightly, violently,
                 insistently, carefully, in a pink fit, passionately,
                 sensuously, slowly, quickly, jerkily, uncontrollably,
                 in excitement, in pleasure,
                 in anticip.......pation, #)
      (self "You tremble $arg$.")
      (rest "$mcname$ tremble$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self "You tremble at $hcname$.")
      (target "$mcname$ tremble$s$ at you.")
      (rest "$mcname$ tremble$s$ at $hcname$.")
    )
  (arguments
    (arguments in fear, nervously, in pain, slightly, violently,
               insistently, carefully, in a pink fit, passionately,
               sensuously, slowly, quickly, jerkily, uncontrollably,  
               in excitement, in pleasure,
               in anticip.......pation, #)    
      (self "You tremble at $hcname$ $arg$." )
      (target "$mcname$ tremble$s$ at you $arg$." )
      (rest "$mcname$ tremble$s$ at $hcname$ $arg$." )
     )
  )
)
