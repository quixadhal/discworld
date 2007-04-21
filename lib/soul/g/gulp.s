(gulp
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You gulp.")
      (rest "$mcname$ gulp$s$.")
    )
    (arguments
      (arguments horrified,nervously,happily,sadly,in astonishment,excetedly,
                 in fright,in terror,astonishment,loudly,in shock,
                 in amazement,carefully,#) 
      (self "You gulp $arg$.")
      (rest "$mcname$ gulp$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You gulp at $hcname$.")
      (target "$mcname$ gulp$s$ at you.")
      (rest  "$mcname$ gulp$s$ at $hcname$.")
    )
    (arguments
      (arguments horrified,nervously,happily,sadly,in astonishment,excetedly,
                 in fright,in terror,astonishment,loudly,in shock,
                 in amazement,carefully,#)  
      (self   "You gulp $arg$ at $hcname$.")
      (target "$mcname$ gulp$s$ $arg$ at you.")
      (rest  "$mcname$ gulp$s$ $arg$ at $hcname$.")
    )
  )
)

