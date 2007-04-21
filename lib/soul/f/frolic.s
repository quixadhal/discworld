(frolic
  (pattern "[around] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [around] <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You frolic all around.")
      (rest "$mcname$ frolic$s$ all around.")
    )
    (arguments
      (arguments happily, peacefully, brightly, sadly, greenly, backwards,
                 orinocoly, with your frog, mischievously, loudly, gleefully, 
                 commonly, twice, once, #)
      (self "You frolic all around $arg$.")
      (rest "$mcname$ frolic$s$ all around $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments happily, peacefully, brightly, sadly, greenly, backwards,
                 orinocoly, with your frog, mischievously, loudly, gleefully, 
                 commonly, twice, once, #)
      (self   "You frolic all around $hcname$ $arg$.")
      (target "$mcname$ frolic$s$ all around you $arg$.")
      (rest   "$mcname$ frolic$s$ all around $hcname$ $arg$.")
    )
  )
)

/* Implemented by Hobbes, suggested by Zaika 961204 */
