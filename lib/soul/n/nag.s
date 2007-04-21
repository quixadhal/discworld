(nag
  (pattern "[at] <indirect:any-living>")
  (pattern "[about] <string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> [about] <string>")

  (targeted
    (no-arguments
      (self   "You nag at $hcname$.")
      (target "$mcname$ nag$s$ at you.")
      (rest   "$mcname$ nag$s$ at $hcname$.")
    )
    (arguments
      (arguments  creators, add_command, everything, nothing, unfinished code, players, #)
      (self   "You nag at $hcname$ about $arg$.")
      (target "$mcname$ nag$s$ at you about $arg$.")
      (rest   "$mcname$ nag$s$ at $hcname$ about $arg$.")
    )
  )
)

