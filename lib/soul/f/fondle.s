(fondle
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You fondle $hcname$.")
      (target "$mcname$ fondle$s$ you.")
      (rest   "$mcname$ fondle$s$ $hcname$.")
    )
    (arguments
      (arguments  hand, leg, knee, foot, finger, ear, hair,
                  soft curvaceous pillows, tree trunk, strawberries,
                  eyebrows, nose, cucumber, melons, coconuts, #)
      (self   "You fondle $mhcname$ $arg$.")
      (target "$mcname$ fondle$s$ your $arg$.")
      (rest   "$mcname$ fondle$s$ $mhcname$ $arg$.")
    )
    (arguments
      (arguments  affectionately, kindly, magnanimously, out of duty, 
                  for a dare, for a friend, as a bribe, for a job, softly, 
                  gently, melodramatically, timidly, for the first time, 
                  again, twice, in case of emergency, accurately, 
                  inaccurately, with an imp-guided targeting system, 
                  with your toes, like a goanna, on Octedays only, 
                  for luck)
      (self   "You fondle $hcname$ $arg$.")
      (target "$mcname$ fondle$s$ you $arg$.")
      (rest   "$mcname$ fondle$s$ $hcname$ $arg$.")
    )
  )
)

