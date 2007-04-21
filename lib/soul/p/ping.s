(ping
  (pattern "<indirect:any-living>")
  (targeted
    (no-arguments
      (self   "You ping $hcname$.")
      (target "$mcname$ ping$s$ you.")
      (rest  "$mcname$ ping$s$ $hcname$.")
    )
  )
)

