(care
  (pattern "<string>")
  (pattern "<indirect:any-living>" )
  (pattern "<indirect:any-living> <string>" )
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You care.")
      (rest "$mcname$ care$s$.")
    )
    (arguments
    (arguments frogs, string, free radicals, sheep, old people,
               new people, fluff, pocket watches, world politics,  
               everything, fluoridation, pool cleaners, stuff in a 
               Rywfol like manner, #)
      (self "You care about $arg$.")
      (rest "$mcname$ care$s$ about $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You care for $hcname$.")
      (target "$mcname$ care$s$ for you.")
      (rest   "$mcname$ care$s$ for $hcname$.")
    )
    (arguments
    (arguments frogs, string, free radicals, sheep, old people,
               new people, fluff, pocket watches, world politics, 
               everything, fluoridation, pool cleaners, stuff in a 
               Rywfol like manner, #)
      (self   "You care about $arg$ for $hcname$.")
      (target "$mcname$ care$s$ about $arg$ for you.")
      (rest   "$mcname$ care$s$ about $arg$ for $hcname$.")
    )
  )
)
/* Recoded by Hobbes, 970931 */
