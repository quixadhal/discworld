(yawn
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You yawn.")
      (rest "$mcname$ yawn$s$.")
    )
    (arguments
      (arguments tiredly, boredly, sleepily, happily, brightly, in sympathy,
                 hugely, loudly, #)
      (self "You yawn $arg$.")
      (rest "$mcname$ yawn$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You yawn at $hcname$.")
      (target "$mcname$ yawn$s$ at you.")
      (rest   "$mcname$ yawn$s$ at $hcname$.")
    )
    (arguments
      (arguments tiredly, boredly, sleepily, happily, brightly, in sympathy,
                 hugely, loudly, #)
      (self   "You yawn $arg$ at $hcname$.")
      (target "$mcname$ yawn$s$ $arg$ at you.")
      (rest   "$mcname$ yawn$s$ $arg$ at $hcname$.")
    )
  )
)

