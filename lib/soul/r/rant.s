(rant
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You rant.")
      (rest "$mcname$ rants.")
    )
    (arguments
      (arguments like Tannah,continuously,pointlessly,like a rabid dog,loudly,#)
      (self "You rant $arg$.")
      (rest "$mcname$ rant$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You rant at $hcname$.")
      (target "$mcname$ rant$s$ at you.")
      (rest  "$mcname$ rant$s$ at $hcname$.")
    )
    (arguments
      (arguments like Tannah,continuously,pointlessly,like a rabid dog,loudly,#)
      (self   "You rant $arg$ at $hcname$.")
      (target "$mcname$ rant$s$ $arg$ at you.")
      (rest  "$mcname$ rant$s$ $arg$ at $hcname$.")
    )
  )
)

