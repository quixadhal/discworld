(weep
  (pattern "[for] <indirect:any-living>")
  (pattern "<string>")
  (single
    (arguments
      (arguments unashamedly,sadly,uncontrollably)
      (self "You weep $arg$.")
      (rest "$mcname$ weep$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You weep with $hcname$.")
      (target "$mcname$ weep$s$ with you.")
      (rest   "$mcname$ weep$s$ with $hcname$.")
    )
  )
)

