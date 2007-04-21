(scalm
  (pattern "<indirect:any-living> [down]")
  (single
    (no-arguments
      (self "You calm down.")
      (rest "$mcname$ calms down.")
    )
  )
  (targeted
    (no-arguments
      (self   "You calm $hcname$ down.")
      (target "$mcname$ calms you down.")
      (rest  "$mcname$ calms down $hcname$.")
    )
  )
)

