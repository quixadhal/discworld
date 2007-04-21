(moan
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<string> at <indirect:any-living>")
  (single
    (no-arguments
      (self "You moan.")
      (rest "$mcname$ moan$s$.")
    )
    (arguments
      (arguments gently, deeply, sensually, agonisingly, in terror, loudly, 
                 excitedly, painfully, #)
      (self "You moan $arg$.")
      (rest "$mcname$ moan$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self "You moan at $hcname$.")
      (target "$mcname$ moan$s$ at you.")
      (rest "$mcname$ moan$s$ at $hcname$.")
    )
    (arguments
      (arguments gently, deeply, sensually, agonisingly, in terror, loudly, 
                 excitedly, painfully, #)
      (self "You moan $arg$ at $hcname$.")
      (target "$mcname$ moan$s$ $arg$ at you.")
      (rest "$mcname$ moan$s$ $arg$ at $hcname$.")
    )
  )
)
