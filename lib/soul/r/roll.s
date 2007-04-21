(roll
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<string> at <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (arguments
      (arguments  around on the floor laughing, 
                  up into a paper to be delivered, 
                  up into a ball, sideways, backwards, 
                  professionally, like a spy through an imaginary door, 
                  fearfully, tightly, slowly, a joint)
      (self "You roll $arg$.")
      (rest "$mcname$ roll$s$ $arg$.")
    )
    (arguments
      (arguments  over and play dead)
      (self "You roll over and play dead.")
      (rest "$mcname$ roll$s$ over and play$s$ dead.")
    )
    (arguments
      (arguments  eyes, own cigarette)
      (self "You roll your $arg$.")
      (rest "$mcname$ roll$s$ $mposs$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You roll over $hcname$.")
      (target "$mcname$ roll$s$ over you.")
      (rest   "$mcname$ roll$s$ over $hcname$.")
    )
    (arguments
      (arguments  under, around)
      (self   "You roll $arg$ $hcname$.")
      (target "$mcname$ roll$s$ $arg$ you.")
      (rest   "$mcname$ roll$s$ $arg$ $hcname$.")
    )
    (arguments
      (arguments  quickly, slowly, painfully, excitedly, lovingly, carefully, 
                  #)
      (self   "You roll $arg$ over $hcname$.")
      (target "$mcname$ roll$s$ $arg$ over you.")
      (rest   "$mcname$ roll$s$ $arg$ over $hcname$.")
    )
    (arguments
      (arguments  eyes, own cigarette)
      (self   "You roll your $arg$ at $hcname$.")
      (target "$mcname$ roll$s$ $mposs$ $arg$ at you.")
      (rest   "$mcname$ roll$s$ $mposs$ $arg$ at $hcname$.")
    )
    (arguments
      (arguments  around on the floor laughing)
      (self    "You roll $arg$ at $hcname$.")
      (target  "$mcname$ roll$s$ $arg$ at you.")
      (rest    "$mcname$ roll$s$ $arg$ at $hcname$.")
    )
  )
)

