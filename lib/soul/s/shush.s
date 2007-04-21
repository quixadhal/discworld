(shush
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You shush.")
      (rest "$mcname$ shush$s$.")
    )
    (arguments 
    (arguments  quietly, quickly, carefully, threateningly, nervously, 
                for using such language, #)
      (self   "You make a shushing sound $arg$.")
      (rest  "$mcname$ make$s$ a shushing sound $arg$.")
    )  
  )
  (targeted
    (no-arguments
      (self   "You shush $hcname$.")
      (target "$mcname$ shush$s$ you.")
      (rest  "$mcname$ shush$s$ $hcname$.")
    )
    (arguments
    (arguments  quietly, quickly, carefully, threateningly, nervously, 
                for using such language, #)
      (self   "You shush $hcname$ $arg$.")
      (target "$mcname$ shush$s$ you $arg$.")
      (rest  "$mcname$ shush$s$ $hcname$ $arg$.")
    )
  )
)
