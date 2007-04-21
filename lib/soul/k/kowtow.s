(kowtow
  (pattern "[to] <indirect:any-living>")
  (pattern "[to] <indirect:any-living> <string>")
  (pattern "<string> [to] <indirect:any-living>")
  (pattern "<string>")
  (single
    (no-arguments
      (self "You kowtow.")
      (rest "$mcname$ kowtow$s$.")
    )
    (arguments
      (arguments awkwardly, grudgingly, slavishly, like someone with something to hide)
      (self "You kowtow $arg$.")
      (rest "$mcname$ kowtow$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You kowtow to $hcname$.")
      (target "$mcname$ kowtow$s$ to you.")
      (rest  "$mcname$ kowtow$s$ to $hcname$.")
    )
    (arguments
      (arguments awkwardly, grudgingly, slavishly, like someone with something to hide)    
      (self   "You kowtow $arg$ to $hcname$.")
      (target "$mcname$ kowtow$s$ $arg$ to you.")
      (rest  "$mcname$ kowtow$s$ $arg$ to $hcname$.")
    )
  )
)
