(quirk
  (pattern "<indirect:any-living>")
  (single
    (no-arguments
      (self "You quirk one eyebrow.")
      (rest "$mcname$ quirk$s$ one eyebrow.")
    )
  )
  (targeted
    (no-arguments
      (self "You quirk one eyebrow at $hcname$.")
      (target "$mcname$ quirk$s$ one eyebrow at you.")
      (rest "$mcname$ quirk$s$ one eyebrow at $hcname$.")
    )
  )
)

