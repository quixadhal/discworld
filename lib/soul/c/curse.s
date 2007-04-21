(curse
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You curse.")
      (rest "$mcname$ curses.")
    )
    (arguments
      (arguments loudly,strongly,rudely,unashamedly,#)
      (self "You curse $arg$.")
      (rest "$mcname$ curses $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You curse at $hcname$.")
      (target "$mcname$ curses at you.")
      (rest  "$mcname$ curses at $hcname$.")
    )
    (arguments
      (arguments  loudly,strongly,rudely,unashamedly,#)
      (self   "You curse $arg$ at $hcname$.")
      (target "$mcname$ curses $arg$ at you.")
      (rest  "$mcname$ curses $arg$ at $hcname$.")
    )
  )
)

