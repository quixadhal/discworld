(womble
  (pattern "<string>")
  (pattern "[around] <indirect:any-living>")
  (pattern "<string> [around] <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You womble around.")
      (rest "$mcname$ womble$s$ around.")
    )
    (arguments
      (arguments happily, peacefully, brightly, sadly, greenly, backwards,
                 orinocoly, wellingtonly, bungoly, tomskly, with your frog,
                 commonly, twice, once, with a hey nonny nonny,
                 doing a dance to the great frog in the sky, in a cloud of 
                 chicken tikka, #)
      (self "You womble around $arg$.")
      (rest "$mcname$ womble$s$ around $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments happily, peacefully, brightly, sadly, greenly, backwards,
                 orinocoly, wellingtonly, bungoly, tomskly, with your frog,
                 commonly, twice, once, with a hey nonny nonny,
                 doing a dance to the great frog in the sky, in a cloud of 
                 chicken tikka, #)
      (self   "You womble around $hcname$ $arg$.")
      (target "$mcname$ womble$s$ around you $arg$.")
      (rest   "$mcname$ womble$s$ around $hcname$ $arg$.")
    )
  )
)
