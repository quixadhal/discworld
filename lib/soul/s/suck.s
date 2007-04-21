(suck
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (arguments
      (arguments thumb,pen,#)
      (self "You suck your $arg$.")
      (rest "$mcname$ suck$s$ $mposs$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments  ear, nose, leg, arm, finger, apples, strawberries,
                  frog, straw, cow, coffee table, car, tent,
                  house, lawnmower, thumb, toe, knee, thigh, lolly, #)
      (self   "You suck on $mhcname$ $arg$.")
      (target "$mcname$ suck$s$ on your $arg$.")
      (rest   "$mcname$ suck$s$ on $mhcname$ $arg$.")
    )
    (arguments
      (arguments  up to)
      (self   "You suck $arg$ $hcname$.")
      (target "$mcname$ suck$s$ $arg$ you.")
      (rest   "$mcname$ suck$s$ $arg$ $hcname$.")
    )
  )
)

