(roar
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You roar.")
      (rest "$mcname$ roars.")
    )
    (arguments
      (arguments loudly,happily,manically,evilly,wickedly,insanely,impishly,
                 cheekily,idiotically,repeatedly,like a drop bear,#)
      (self "You roar $arg$.")
      (rest "$mcname$ roar$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You roar at $hcname$.")
      (target "$mcname$ roar$s$ at you.")
      (rest  "$mcname$ roar$s$ at $hcname$.")
    )
    (arguments
       (arguments loudly,happily,manically,evilly,wickedly,insanely,impishly,
                 cheekily,idiotically,repeatedly,like a drop bear,#)
      (self   "You roar $arg$ at $hcname$.")
      (target "$mcname$ roar$s$ $arg$ at you.")
      (rest  "$mcname$ roar$s$ $arg$ at $hcname$.")
    )
  )	
)
