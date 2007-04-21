(snore
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You snore.")
      (rest "$mcname$ snore$s$.")
    )
    (arguments
     (arguments loudly, sadly, joyfully, sagely, knowingly, quickly, happily, carefully,
     balefully, excitedly, nervously, brightly, uncertainly, threateningly, grumpily,
     fluffily, absentmindedly, quickly,#)
      (self "You snore $arg$.")
      (rest "$mcname$ snore$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You snore at $hcname$.")
      (target "$mcname$ snore$s$ at you.")
      (rest  "$mcname$ snore$s$ at $hcname$.")
    )
    (arguments
     (arguments loudly, sadly, joyfully, sagely, knowingly, quickly, happily, carefully,
     balefully, excitedly, nervously, brightly, uncertainly, threateningly, grumpily,
     fluffily, absentmindedly, quickly,#)
      (self   "You snore $arg$ at $hcname$.")
      (target "$mcname$ snore$s$ $arg$ at you.")
      (rest  "$mcname$ snore$s$ $arg$ at $hcname$.")
    )
  )
)

