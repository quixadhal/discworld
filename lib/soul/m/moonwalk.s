(moonwalk
  (pattern "[over] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [over] <indirect:any-living>")
  (pattern "[over] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You moonwalk like Michael Jackson, but without the monkey.")
      (rest "$mcname$ moonwalks like Michael Jackson, but without the monkey. ")
    )
  )
  (targeted
    (no-arguments
      (self   "You moonwalk all over $hcname$.")
      (target "$mcname$ moonwalks all over you.")
      (rest  "$mcname$ moonwalks all over $hcname$.")
    )
    (arguments
      (arguments like Michael Jackson, but without the monkey,#)
      (self   "You moonwalk $arg$ over $hcname$.")
      (target "$mcname$ moonwalks $arg$ over you.")
      (rest  "$mcname$ moonwalks $arg$ over $hcname$.")
    )
  )
)

