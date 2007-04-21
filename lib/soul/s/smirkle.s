(smirkle
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You smirkle.")
      (rest "$mcname$ smirkle$s$.")
    )
    (arguments
      (arguments  happily, sadly, like Truilkan, sardonically, strangely, #)
      (self "You smirkle $arg$.")
      (rest "$mcname$ smirkle$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You smirkle at $hcname$.")
      (target "$mcname$ smirkle$s$ at you.")
      (rest   "$mcname$ smirkle$s$ at $hcname$.")
    )
    (arguments
      (arguments  happily, sadly, like Truilkan, sardonically, strangely, #)
      (self   "You smirkle $arg$ at $hcname$.")
      (target "$mcname$ smirkle$s$ $arg$ at you.")
      (rest   "$mcname$ smirkle$s$ $arg$ at $hcname$.")
    )
  )
)

