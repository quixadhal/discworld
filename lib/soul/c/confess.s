(confess
  (pattern "[to] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [to] <indirect:any-living>")
  (pattern "[to] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You confess it all.")
      (rest "$mcname$ confesses it all.")
    )
    (arguments
      (arguments  to being a witch,#)
      (self "You confess $arg$.")
      (rest "$mcname$ confesses $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You confess to $hcname$.")
      (target "$mcname$ confesses to you.")
      (rest  "$mcname$ confesses to $hcname$.")
    )
    (arguments
      (arguments  to being a witch,#)
      (self   "You confess $arg$ to $hcname$.")
      (target "$mcname$ confesses $arg$ to you.")
      (rest  "$mcname$ confesses $arg$ to $hcname$.")
    )
  )
)

