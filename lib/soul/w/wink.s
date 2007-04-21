(wink
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You wink.")
      (rest "$mcname$ wink$s$.")
    )
    (arguments
      (arguments suggestively,broadly,slowly,loudly,grumpily,
                 despairingly,coyly,desperately,insolently,curtly,
                 lovingly,coquettishly,knowingly,conspiratorially,
                 patiently,sadistically,warily,seductively,lazily, #)
      (self "You wink $arg$.")
      (rest "$mcname$ wink$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You wink at $hcname$.")
      (target "$mcname$ wink$s$ at you.")
      (rest   "$mcname$ wink$s$ at $hcname$.")
    )
    (arguments
      (arguments suggestively,broadly,slowly,loudly,grumpily,
                 despairingly,coyly,desperately,insolently,curtly,
                 lovingly,coquettishly,knowingly,conspiratorially,
                 patiently,sadistically,warily,seductively,lazily, #)
      (self   "You wink $arg$ at $hcname$.")
      (target "$mcname$ wink$s$ $arg$ at you.")
      (rest   "$mcname$ wink$s$ $arg$ at $hcname$.")
    )
  )
)

