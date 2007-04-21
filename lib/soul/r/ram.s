(ram
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self  "You ram the nearest turtle.")
      (rest "$mcname$ ram$s$ the nearest turtle.")
    )
    (arguments
      (arguments wall, door, sheep, #)
      (self  "You ram into the $arg$.")
      (rest "$mcname$ ram$s$ into the $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self     "You ram into $hcname$.")
      (target "$mcname$ ram$s$ into you.")
      (rest    "$mcname$ ram$s$ into $hcname$.")
    )
    (arguments
      (arguments head, back, body, #)
      (self     "You ram into $hposs$ $arg$.")
      (target "$mcname$ rams into your $arg$.")
      (rest    "$mcname$ ram$s$ into $hposs$ $arg$.")
    )
  )
)