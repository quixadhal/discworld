(choke
  (pattern "<indirect:any-living>")
  (single
    (no-arguments
      (self "You choke.")
      (rest "$mcname$ chokes.")
    )
  )
  (targeted
    (no-arguments
      (self   "You choke $hcname$.")
      (target "$mcname$ chokes you.")
      (rest  "$mcname$ chokes $hcname$.")
      
    )
  )
)

