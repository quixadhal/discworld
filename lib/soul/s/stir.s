(stir
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments enthusiastically, excitedly, quietly, calmly, carefully, politely, wildly, oddly, 
                        unconvincingly, happily, slightly, up, loudly, lamely)
      (self "You stir $arg$.")
      (rest "$mcname$ stir$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments enthusiastically, excitedly, quietly, calmly, carefully, politely, wildly, oddly, 
                        unconvincingly, happily, slightly, up, loudly, lamely)
      (self   "You stir $arg$ at $hcname$.")
      (target "$mcname$ stir$s$ $arg$ at you.")
      (rest  "$mcname$ stir$s$ $arg$ at $hcname$.")
    )
  )
)

