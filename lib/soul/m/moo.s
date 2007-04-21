(moo
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "[at] <indirect:any-living> <string>")
  (pattern "<string> at <indirect:any-living>")
  (single
    (no-arguments
      (self "You moo.")
      (rest "$mcname$ moo$s$.")
    )
    (arguments
      (arguments like a cow,like a bottle of milk,like a tree,happily,
                 annoyingly,like Hobbes,from halfway up a tree,#)
      (self "You moo $arg$.")
      (rest "$mcname$ moo$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You moo at $hcname$.")
      (target "$mcname$ moo$s$ at you.")
      (rest  "$mcname$ moo$s$ at $hcname$.")
    )
    (arguments
      (arguments like a cow,like a bottle of milk,like a tree,
                 happily,annoyingly,like Hobbes,from halfway up a tree,#)
      (self   "You moo $arg$ at $hcname$.")
      (target "$mcname$ moo$s$ $arg$ at you.")
      (rest  "$mcname$ moo$s$ $arg$ at $hcname$.")
    )
  )
)

/* Hobbes, 961204 */
