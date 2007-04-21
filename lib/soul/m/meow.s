(meow
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You meow.")
      (rest "$mcname$ meow$s$.")
    )
    (arguments
      (arguments like a cat,like a cute kitten,like a cabbage,happily,insanely,mischievously,affectionately,faintly,like Hobbes,#)
      (self "You meow $arg$.")
      (rest "$mcname$ meow$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You meow at $hcname$.")
      (target "$mcname$ meow$s$ at you.")
      (rest  "$mcname$ meow$s$ at $hcname$.")
    )
    (arguments
      (arguments like a cat,like a cute kitten,like a cabbage,happily,insanely,mischievously,affectionately,faintly,like Hobbes,#)
      (self   "You meow $arg$ at $hcname$.")
      (target "$mcname$ meow$s$ $arg$ at you.")
      (rest  "$mcname$ meow$s$ $arg$ at $hcname$.")
    )
  )
)

