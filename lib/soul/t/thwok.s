(thwok
  (pattern "<indirect:any-living>")
  (single
    (no-arguments
      (self "T H W O K K ! !")
      (rest "$mcname$ 'T H W O K K ! !'s.")
    )
  )
  (targeted
    (no-arguments
      (self   "You thwok $hcname$.")
      (target "You are summarily thwokked by $mcname$.")
      (rest  "$mcname$ thwoks $hcname$.")
    )
  )
)

