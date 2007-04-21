(scratch
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (arguments
      (arguments  head, back, leg, arm, nose, ear, finger, bum, 
                  cat behind the ears, eyes, #)
      (self "You scratch your $arg$.")
      (rest "$mcname$ scratch$s$ $mposs$ $arg$.")
    )
    (arguments
      (arguments  chin)
      (self "You scratch your $arg$ and go 'Hmmmm...'.")
      (rest "$mcname$ scratch$s$ $mposs$ $arg$ and goes 'Hmmmm...'.")
    )
  )
  (targeted
    (no-arguments
      (self   "You scratch $hcname$.")
      (target "$mcname$ scratch$s$ you.")
      (rest   "$mcname$ scratch$s$ $hcname$.")
    )
    (arguments
      (arguments  head, back, leg, arm, nose, ear, finger, bum, #)
      (self   "You scratch $mhcname$ $arg$.")
      (target "$mcname$ scratch$s$ your $arg$.")
      (rest   "$mcname$ scratch$s$ $mhcname$ $arg$.")
    )
    (arguments
      (arguments  chin)
      (self   "You scratch your $arg$ and go 'Hmmmm...' at $hcname$.")
      (target "$mcname$ scratch$s$ $mposs$ $arg$ and goes 'Hmmmm...' at you.")
      (rest   "$mcname$ scratch$s$ $mposs$ $arg$ and goes 'Hmmmm...' at $hcname$.")
    )
    (arguments
      (arguments  behind the ears)
      (self   "You scratch behind $mhcname$ ears.")
      (target "$mcname$ scratch$s$ behind your ears.")
      (rest   "$mcname$ scratch$s$ behind $mhcname$ ears.")
    )
  )
)

