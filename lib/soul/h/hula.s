(hula
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self   "You dance the hula.")
      (rest   "$mcname$ dance$s$ the hula.")
    )
    (arguments
    (arguments with reckless abandon, gracefully, clumsily, slowly,
               seductively, tropically, in a grass skirt, for a good lei, 
               under the stars, with little paper umbrellas, in a coconut bra,
               with a hoop, with a lovely bunch of coconuts, #)
      (self   "You hula $arg$.")
      (rest   "$mcname$ hula$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You dance the hula for $hcname$.")
      (target "$mcname$ dance$s$ the hula for you.")
      (rest   "$mcname$ dance$s$ the hula for $hcname$.")
    )
    (arguments
    (arguments with reckless abandon, gracefully, clumsily, slowly,
               seductively, tropically, in a grass skirt, for a good lei, 
               under the stars, with little paper umbrellas, in a coconut bra,
               with a hoop, with a lovely bunch of coconuts, #)
      (self   "You hula at $hcname$ $arg$.")
      (target "$mcname$ hula$s$ at you $arg$.")
      (rest   "$mcname$ hula$s$ at $hcname$ $arg$.")
    )
  )
)

/* Idea by Shalla */
/* Updated by Saffra 11 Jun 2002 */
