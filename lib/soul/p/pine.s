(pine
  (pattern "<string>")
  (pattern "[at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (pattern "<string> <indirect:any-living>")
  (single
    (no-arguments
      (self "You pine away.")
      (rest "$mcname$ pine$s$ away.")
    )
    (arguments
      (arguments dramatically, melodramatically, romantically, swooningly, 
                 sarcastically, longingly, lovingly, not at all, #)
      (self "You pine $arg$.")
      (rest "$mcname$ pine$s$ $arg$.")
    )
   )
  (targeted
    (no-arguments
      (self   "You pine at $hcname$.")
      (target "$mcname$ pine$s$ at you.")
      (rest  "$mcname$ pine$s$ at $hcname$.")
    )
    (arguments
      (arguments dramatically, melodramatically, romantically, swooningly, 
                 sarcastically, longingly, lovingly, not at all, #)
      (self   "You pine $arg$ at $hcname$.")
      (target "$mcname$ pine$s$ $arg$ at you.")
      (rest  "$mcname$ pine$s$ $arg$ at $hcname$.")
    )
    (arguments
      (arguments for)
      (self   "You pine for $hcname$.")
      (target "$mcname$ pine$s$ for you.")
      (rest  "$mcname$ pine$s$ for $hcname$.")
    )  
  )
)
