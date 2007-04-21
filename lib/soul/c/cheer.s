(cheer
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments enthusiastically, excitedly, quietly, calmly, carefully, 
                 politely, wildly, oddly, unconvincingly, happily, slightly, 
                 up, loudly, lamely, drunkenly, #)
      (self "You cheer $arg$.")
      (rest "$mcname$ cheer$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments enthusiastically, excitedly, quietly, calmly, carefully, 
                 politely, wildly, oddly, unconvincingly, happily, slightly, 
                 up, loudly, lamely, drunkenly, #)
      (self   "You cheer $arg$ at $hcname$.")
      (target "$mcname$ cheer$s$ $arg$ at you.")
      (rest  "$mcname$ cheer$s$ $arg$ at $hcname$.")
    )
  )
)
