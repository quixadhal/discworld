(chiko
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments happily, sadly, joyfully, sagely, knowingly, quickly,
                 loudly, carefully, balefully, excitedly, nervously,
                 brightly, uncertainly, threateningly, grumpily, fluffily,
                 evilly)
      (self "You chiko $arg$.")
      (rest "$mcname$ chikos $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments  happily, sadly, joyfully, sagely, knowingly, quickly,
                  excitedly, nervously, brightly, uncertainly, grumpily,
                  loudly, carefully, balefully, threateningly, fluffily,
                  evilly)
      (self   "You chiko $arg$ at $hcname$.")
      (target "$mcname$ chikos $arg$ at you.")
      (rest  "$mcname$ chikos $arg$ at $hcname$.")
    )
  )
)

