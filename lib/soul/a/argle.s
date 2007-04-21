(argle
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You argle.")
      (rest "$mcname$ argle$s$.")
    )
    (arguments
      (arguments in despair,happily,sadly,annoyingly,loudly,constantly,
      knowingly,grumpily,angrily,#)
      (self "You argle $arg$.")
      (rest "$mcname$ arlges $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You argle at $hcname$.")
      (target "$mcname$ argle$s$ at you.")
      (rest  "$mcname$ argle$s$ at $hcname$.")
    )
    (arguments
      (arguments in despair,happily,sadly,annoyingly,loudly,constantly,
            knowingly,grumpily,angrily,#)
      (self   "You argle $arg$ at $hcname$.")
      (target "$mcname$ argle$s$ $arg$ at you.")
      (rest  "$mcname$ argle$s$ $arg$ at $hcname$.")
    )
  )
)
/*Hobbes*/

