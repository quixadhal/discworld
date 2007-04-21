(bop
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (pattern "<string> <indirect:any-living>")
  (single
    (no-arguments
      (self "You bop to the beat.")
      (rest "$mcname$ bop$s$ to the beat.")
    )
    (arguments
      (arguments  yourself, myself, me)
      (self "You bop yourself on the head.")
      (rest "$mcname$ bop$s$ $mobj$self on the head.")
    )
  )
  (targeted
    (no-arguments
      (self   "You bop $hcname$ on the head.")
      (target "$mcname$ bop$s$ you on the head.")
      (rest   "$mcname$ bop$s$ $hcname$ on the head.")
    )
    (arguments
      (arguments  absentmindedly, flambuoyantly, accidentally, without reason, 
                  with a stuffed mullet, with a salmon, with very good reason, 
                  only on Sundays, #)
      (self   "You bop $hcname$ on the head $arg$.")
      (target "$mcname$ bop$s$ you on the head $arg$.")
      (rest   "$mcname$ bop$s$ $hcname$ on the head $arg$.")
    )
    (arguments
      (arguments  ear, nose, shoulder, elbow, other ear, forehead, chin)
      (self   "You bop $hcname$ on the $arg$.")
      (target "$mcname$ bop$s$ you on your $arg$.")
      (rest   "$mcname$ bop$s$ $hcname$ on the $arg$.")
    )
  )
)
