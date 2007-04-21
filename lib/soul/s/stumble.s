(stumble
  (pattern "<string>")
  (single
    (no-arguments
      (self "You stumble around the room.")
      (rest "$mcname$ stumble$s$ around the room.")
    )
    (arguments
      (arguments #)
      (self "You stumble $arg$.")
      (rest "$mcname$ stumble$s$ $arg$.")
    )
  )
)

