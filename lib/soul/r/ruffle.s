(ruffle
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You ruffle your hair.")
      (rest "$mcname$ ruffle$s$ $mposs$ hair.")
    )
    (arguments
    (arguments fronds, ruffles, pleats, crinolines, moustache, ridges,
               nerves, feathers, skirts, fur, wool, #)
      (self "You ruffle your $arg$.")
      (rest "$mcname$ ruffle$s$ $mposs$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You ruffle $mhcname$ hair.")
      (target "$mcname$ ruffle$s$ your hair.")
      (rest   "$mcname$ ruffle$s$ $mhcname$ hair.")
    )
    (arguments
      (arguments fronds, ruffles, pleats, crinolines, moustache, ridges,
                 nerves, feathers, skirts, fur, wool, #)
      (self   "You ruffle $mhcname$ $arg$.")
      (target "$mcname$ ruffle$s$ your $arg$.")
      (rest   "$mcname$ ruffle$s$ $mhcname$ $arg$.")
    )
  )
)
