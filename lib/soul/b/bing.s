(bing
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments happily, sadly, joyfully, sagely, knowingly, quickly,
                 loudly, carefully, balefully, excitedly, nervously,
                 brightly, uncertainly, threateningly, grumpily, fluffily,
                 evilly, drunkenly, sleepily, #)
      (self "You bing $arg$.")
      (rest "$mcname$ bing$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments  happily, sadly, joyfully, sagely, knowingly, quickly,
                  excitedly, nervously, brightly, uncertainly, grumpily,
                  loudly, carefully, balefully, threateningly, fluffily,
                  evilly, drunkenly, sleepily, #)
      (self   "You bing $arg$ at $hcname$.")
      (target "$mcname$ bing$s$ $arg$ at you.")
      (rest  "$mcname$ bing$s$ $arg$ at $hcname$.")
    )
  )
)

