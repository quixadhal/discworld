(flatter
  (pattern "<string>")
  (pattern "<indirect:any-living>" )
  (pattern "<indirect:any-living> <string>")
  (pattern "<string> <indirect:any-living>")
  (single
    (no-arguments
      (self "You flatter yourself.")
      (rest "$mcname$ flatter$s$ $mobj$self.")
    )
    (arguments
      (arguments   #)
      (self "You flatter $arg$.")
      (rest "$mcname$ flatter$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You flatter $hcname$.")
      (target "$mcname$ flatter$s$ you.")
      (rest   "$mcname$ flatter$s$ $hcname$.")
    )
    (arguments
      (arguments  sincerely, insincerely, excessively, slickly, cleverly, lots,          
                  like a professional, like a sycophant, wooingly, #)
      (self   "You flatter $hcname$ $arg$." )
      (target "$mcname$ flatter$s$ you $arg$." )
      (rest   "$mcname$ flatter$s$ $hcname$ $arg$." )
    )
  )
)

/* Saffra 2-29-00, idea by Gruper. */
