(hiss
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You hiss.")
      (rest "$mcname$ hiss$s$.")
    )
    (arguments
      (arguments angrily,cynically,sarcastically,happily,nastily,like a snake,
                 like a cat,like a woman scorned,menacingly,#)
      (self "You hiss $arg$.")
      (rest "$mcname$ hiss$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You hiss at $hcname$.")
      (target "$mcname$ hiss$s$ at you.")
      (rest  "$mcname$ hiss$s$ at $hcname$.")
    )
    (arguments
      (arguments  angrily,cynically,sarcastically,happily,nastily,like a snake,
                  like a cat,like a woman scorned,menacingly,#)
      (self   "You hiss $arg$ at $hcname$.")
      (target "$mcname$ hiss$s$ $arg$ at you.")
      (rest  "$mcname$ hiss$s$ $arg$ at $hcname$.")
    )
  )
)

