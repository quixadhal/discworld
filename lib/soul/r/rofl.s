(rofl
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<string> at <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You roll around on the floor laughing.")
      (rest "$mcname$ roll$s$ around on the floor laughing.")
    )
    (arguments
      (arguments  frogs, fate, the irony, the mysteries of life, that awful
                  punne, the antics of the creators, #)
      (self "You roll around on the floor laughing at $arg$.")
      (rest "$mcname$ roll$s$ around on the floor laughing at $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You roll around on the floor laughing at $hcname$.")
      (target "$mcname$ roll$s$ around on the floor laughing at you.")
      (rest   "$mcname$ roll$s$ around on the floor laughing at $hcname$.")
    )
    (arguments
      (arguments  frogs, fate, the irony, the mysteries of life, that awful
                  punne, the antics of the creators, #)
      (self   "You roll around on the floor laughing about $arg$ at $hcname$.")
      (target "$mcname$ roll$s$ around on the floor laughing about $arg$ at you.")
      (rest   "$mcname$ roll$s$ around on the floor laughing about $arg$ at $hcname$.")
    )
  )
)
/* Saffra 4-29-01 */
