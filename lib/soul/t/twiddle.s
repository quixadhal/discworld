(twiddle
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (arguments
      (arguments thumbs, fingers, hair, nose, bits, #)
      (self "You twiddle your $arg$.")
      (rest "$mcname$ twiddle$s$ $mposs$ $arg$.")
    )
  )
  (targeted 
    (arguments
      (arguments thumbs, fingers, hair, nose, bits, #)
        (self   "You twiddle $mhcname$ $arg$.")
        (target "$mcname$ twiddle$s$ your $arg$.")
        (rest   "$mcname$ twiddle$s$ $mhcname$ $arg$.")
    )
  )
)
