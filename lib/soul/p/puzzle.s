(puzzle
  (pattern "<string>")
  (single
    (no-arguments
      (self "You look puzzled.")
      (rest "$mcname$ look$s$ puzzled.")
    )
    (arguments
      (arguments #)
      (self "You puzzle over $arg$.")
      (rest "$mcname$ puzzle$s$ over $arg$.")
    )
  )
)

