(hrmph
  (pattern "<string>")
  (pattern "[at] <indirect:any-living>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You hrmph.")
      (rest "$mcname$ hrmph$s$.")
    )
    (arguments
      (arguments  carefully, silently, lengthily, fishily, boringly, snobbishly, 
                  happily, courageously, with wild abandon, snortingly, cutely, 
                  absentmindedly, in disgust, in contempt, in perturbation, 
                  with an evil glint in your eye, #)
      (self "You hrmph $arg$.")
      (rest "$mcname$ hrmph$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You hrmph in $mhcname$ general direction.")
      (target "$mcname$ hrmph$s$ in your general direction.")
      (rest   "$mcname$ hrmph$s$ in $mhcname$ general direction.")
    )
    (arguments
      (arguments  carefully, silently, lengthily, fishily, boringly, snobbishly, 
                  happily, courageously, with wild abandon, snortingly, cutely, 
                  absentmindedly, in disgust, in contempt, in perturbation, 
                  with an evil glint in your eye, #)
      (self   "You hrmph $arg$ at $hcname$.")
      (target "$mcname$ hrmph$s$ $arg$ at you.")
      (rest   "$mcname$ hrmph$s$ $arg at $hcname$.")
    )
  )
)

