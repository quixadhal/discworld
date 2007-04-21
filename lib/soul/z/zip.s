(zip
  (pattern "<indirect:any-living>")
  (single
    (no-arguments
      (self "You zip around the room.")
      (rest "$mcname$ zip$s$ around the room.")
    )
  )
  (targeted
    (no-arguments
      (self   "You zip around $hcname$.")
      (target "$mcname$ zip$s$ around you.")
      (rest  "$mcname$ zip$s$ around $hcname$.")
    )
  )
)

