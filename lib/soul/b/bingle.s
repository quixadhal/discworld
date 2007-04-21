(bingle
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments happily,sadly,joyfully,sagely,knowingly,quickly,loudly,carefully,balefully,excitedly,
       nervously,tiredly,brightly,uncertainly,
       threateningly,grumpily,like a crazed taxi driver on cocaine, and bangles,#)
      (self "You bingle $arg$.")
      (rest "$mcname$ bingle$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments happily,sadly,joyfully,sagely,knowingly,quickly,excitedly,nervously,
               brightly,tiredly,uncertainly,grumpily,loudly,carefully,balefully,
               threateningly,like a crazed taxi driver on cocaine, and bangles,#)
      (self   "You bingle $arg$ at $hcname$.")
      (target "$mcname$ bingle$s$ $arg$ at you.")
      (rest  "$mcname$ bingle$s$ $arg$ at $hcname$.")
    )
  )
)

