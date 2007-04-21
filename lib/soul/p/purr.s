(purr
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
   (arguments 
     (arguments  contentedly,demonically,loudly,happily,sensuously,
      like a cute kitten, #)
      (self "You purr $arg$.")
      (rest "$mcname$ purr$s$ $arg$.")
    )
  )
  (targeted     
    (arguments
      (arguments contentedly,demonically,loudly,happily,sensuously,
       like a cute kitten, #)
      (self   "You purr $arg$ at $hcname$.")
      (target "$mcname$ purr$s$ $arg$ at you.")
      (rest  "$mcname$ purr$s$ $arg$ at $hcname$.")
     )
   )
 )

