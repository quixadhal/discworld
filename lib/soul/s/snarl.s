(snarl
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments  viciously, nastily, demonically, happily, sadly,
                  unexpectedly, suddenly)
      (self   "You snarl $arg$.")
      (target "$mcname$ snarl$s$ $arg$.")
      (rest  "$mcname$ snarl$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments  viciously, nastily, demonically, happily, sadly,
                  unexpectedly, suddenly)
      (self   "You snarl $arg$ at $hcname$.")
      (target "$mcname$ snarl$s$ $arg$ at you.")
      (rest  "$mcname$ snarl$s$ $arg$ at $hcname$.")
    )
  )
)

