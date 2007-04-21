(whicker
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You whicker.")
      (rest "$mcname$ whickers.")
    )
    (arguments
      (arguments  like a horse, gently, happily, slowly, stupidly, 
                  like a ball of string, back in black, like a stallion, 
                  with gusto, excitedly, unhappily, sadly, skitishly, #)
      (self "You whicker $arg$.")
      (rest "$mcname$ whicker$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You whicker at $hcname$.")
      (target "$mcname$ whicker$s$ at you.")
      (rest   "$mcname$ whicker$s$ at $hcname$.")
    )
    (arguments
      (arguments  like a horse, gently, happily, slowly, stupidly, 
                  like a ball of string, back in black, like a stallion, 
                  with gusto, excitedly, unhappily, sadly, skitishly, #)
      (self   "You whicker $arg$ at $hcname$.")
      (target "$mcname$ whicker$s$ $arg$ at you.")
      (rest   "$mcname$ whicker$s$ $arg$ at $hcname$.")
    )
  )
)

