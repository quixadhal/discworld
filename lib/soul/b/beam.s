(beam
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
(arguments brightly, admiringly, strangely, happily, ecstatically, fishily,
           like a train, #)
      (self "You beam $arg$.")
      (rest "$mcname$ beam$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You beam at $hcname$.")
      (target "$mcname$ beam$s$ at you.")
      (rest  "$mcname$ beam$s$ at $hcname$.")
    )
    (arguments
(arguments brightly, admiringly, strangely, happily, ecstatically, fishily,
           like a train, #)
      (self   "You beam at $hcname$ $arg$.")
      (target "$mcname$ beam$s$ at you $arg$.")
      (rest  "$mcname$ beam$s$ at $hcname$ $arg$.")
    )
  )
)

