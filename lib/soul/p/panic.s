(panic
  (pattern "[at] <indirect:any-living> <string>")
  (pattern "[at] <indirect:any-living>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "<string>")
  (single
    (no-arguments
      (self "You panic.")
      (rest "$mcname$ panic$s$.")
    )
    (arguments
      (arguments happily, sadly, madly, insanely, #)
      (self "You panic $arg$.")
      (rest "$mcname$ panic$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self "You panic at $hcname$.")
      (target "$mcname$ panic$s$ at you.")
      (rest "$mcname$ panic$s$ at $hcname$.")
    )
    (arguments
      (arguments happily, sadly, madly, insanely, #)
      (self "You panic $arg$ at $hcname$.")
      (target "$mcname$ panic$s$ $arg$ at you.")
      (rest "$mcname$ panic$s$ $arg$ at $hcname$.")
    )
  )
)

