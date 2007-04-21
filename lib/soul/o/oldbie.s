(oldbie
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (arguments
      (arguments  better, faster, cheaper to advance, more full of NPCs, #)
      (self "You begin to ramble about the ol' days and how it was much $arg$.")
      (rest "$mcname$ begin$s$ to rant about the ol' days and how it was much $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments  bitter, old, ancient, ancestor like, wrinkled, #)
      (self   "You turn $hcname$ into an oldbie, $hpronoun$ looks utterly and completely $arg$.")
      (target "$mcname$ turn$s$ you into an oldbie, you look completely and utterly $arg$.")
      (rest   "$mcname$ turn$s$ $hcname$ into an oldbie, $hpronoun$ looks completely and utterly $arg$.")
    )
  )
)

