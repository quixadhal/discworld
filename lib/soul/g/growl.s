(growl
  (pattern "<string>")
  (pattern "[at] <indirect:any-living>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments  menacingly, nastily, loudly, playfully, happily, 
                  affectionately, #)
      (self "You growl $arg$.")
      (rest "$mcname$ growl$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments  menacingly, nastily, loudly, playfully, happily, 
                  affectionately, #)
      (self   "You growl $arg$ at $hcname$.")
      (target "$mcname$ growl$s$ $arg$ at you.")
      (rest   "$mcname$ growl$s$ $arg$ at $hcname$.")
    )
  )
)

