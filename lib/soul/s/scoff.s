(scoff
  (pattern "<string>")
  (single
    (no-arguments
      (self "You scoff.")
      (rest "$mcname$ scoff$s$.")
    )
    (arguments
      (arguments loudly,sarcastically,sweetly,sardonically)
      (self "You scoff $arg$.")
      (rest "$mcname$ scoff$s$ $arg$.")
    )
  )
)

