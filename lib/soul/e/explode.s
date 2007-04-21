(explode
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [over] <indirect:any-living>")
  (pattern "[over] <indirect:any-living> <string>")
  (single
    (no-arguments
        (self "You explode into thousands of tiny pieces.") 
        (rest "$mcname$ explode$s$ into thousands of tiny pieces.")
        )
      (arguments 
        (arguments messily,into thousands of tiny thetans,over your frog,#)
      (self "You explode $arg$.")
      (rest "$mcname$ explode$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You explode into thousands of tiny pieces over $hcname$.")
      (target "$mcname$ explode$s$ into thousands of tiny pieces over you.")
      (rest  "$mcname$ explode$s$ into thousands of tiny pieces over $hcname$.")
       )
      (arguments 
        (arguments messily,into thousands of tiny thetans,your frog,#)
      (self   "You explode $arg$ over $hcname$.")
      (target "$mcname$ explode$s$ $arg$ over you.")
      (rest   "$mcname$ explode$s$ $arg$ over $hcname$.")
    )
  )
)
