(boggle
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (single
    (arguments
      (arguments the concept,#)
      (self "You boggle at $arg$.")
      (rest "$mcname$ boggle$s$ at $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You boggle at $hcname$.")
      (target "$mcname$ boggle$s$ at you.")
      (rest  "$mcname$ boggle$s$ at $hcname$.")
    )
  )
)

