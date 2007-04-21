(bimble
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You bimble.")
      (rest "$mcname$ bimble$s$.")
    )
    (arguments
    (arguments excitedly, fearfully, happily, wildly, #)
    (self "You bimble $arg$.")
    (rest "$mcname$ bimble$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self "You bimble around $hcname$.")
      (target "$mcname$ bimble$s$ around you.")
      (rest  "$mcname$ bimble$s$ around $hcname$.")
    )
    (arguments
    (arguments excitedly, fearfully, happily, wildly, #)
    (self "You bimble around $hcname$ $arg$.")
    (rest "$mcname$ bimble$s$ around $hcname$ $arg$.")
    )
  )
)

