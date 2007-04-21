(slook
  (pattern "<string>" )
  (pattern "<indirect:any-living>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You look around.")
      (rest "$mcname$ look$s$ around.")
    )
    (arguments
    (arguments  away, elsewhere, guilty, innocent, thoughtful, #)
      (self "You look $arg$.")
      (rest "$mcname$ look$s$ $arg$.")
    )
  )
  (targeted
    (arguments
    (arguments  over, guiltily, innocently, thoughtfully, #)
    (self   "You look $arg$ at $hcname$.")
    (target "$mcname$ look$s$ $arg$ at you.")
    (rest   "$mcname$ look$s$ $arg$ at $hcname$.")
    )
  )
)

/* Saffra 3-30-00    */
/* Modified 10-11-00 */

