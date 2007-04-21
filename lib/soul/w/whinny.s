(whinny
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You whinny.")
      (rest "$mcname$ whinnies.")
    )
    (arguments
      (arguments like a horse, gently, happily, slowly, stupidly,
                 like a ball of string, in growing pains,
                 back in black, like a stallion, with gusto, excitedly,
                 unhappily, sadly, skittishly, #)
      (self "You whinny $arg$.")
      (rest "$mcname$ whinny$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self "You whinny at $hcname$.")
      (target "$mcname$ whinny$s$ at you.")
      (rest "$mcname$ whinny$s$ at $hcname$.")
    )
    (arguments
      (arguments like a horse, gently, happily, slowly, stupidly,
                 like a ball of string, in growing pains,
                 back in black, like a stallion, with gusto, excitedly,
                 unhappily, sadly, skittishly, #)
      (self "You whinny $arg$ at $hcname$.")
      (target "$mcname$ whinny$s$ $arg$ at you.")
      (rest "$mcname$ whinny$s$ $arg$ at $hcname$.")
    )
  )
)
