(die
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments  suddenly, dramatically, of boredom, of hunger, 
                  of anticipation, pathetically, gracefully, senselessly, 
                  stupidly, tragically, slowly, painfully, quickly
                  an excruciating death, in no specific direction, #)
      (self "You die $arg$.")
      (rest "$mcname$ $verb$dies,die$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You die in $mhcname$ general direction.")
      (target "$mcname$ $verb$dies,die$ in your general direction.")
      (rest   "$mcname$ $verb$dies,die$ in $mhcname$ general direction.")
    )
    (arguments
      (arguments  arms)
      (self   "You die in $mhcname$ arms.")
      (target "$mcname$ $verb$dies,die$ in your arms.")
      (rest   "$mcname$ $verb$dies,die$ in $mhcname$ arms.")
    )
    (arguments
      (arguments  suddenly, dramatically, of boredom, of hunger, 
                  of anticipation, pathetically, gracefully, senselessly, 
                  stupidly, tragically, slowly, painfully, quickly
                  an excruciating death, #)
      (self   "You die $arg$ in $mhcname$ general direction.")
      (target "$mcname$ $verb$dies,die$ $arg$ in your general direction.")
      (rest   "$mcname$ $verb$dies,die$ $arg$ in $mhcname$ general direction.")
    )
  )
)

