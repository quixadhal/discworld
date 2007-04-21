(drumroll
  (pattern "[for] <indirect:any-living>")
  (single
    (no-arguments
      (self "You do a drumroll.  K'tisssh!")
      (rest "$mcname$ do$s$ a drumroll.  K'tisssh!")
    )
  )
  (targeted
    (no-arguments
      (self   "You do a drumroll for $hcname$.  K'tisssh!")
      (target "$mcname$ do$s$ a drumroll for you.  K'tisssh!")
      (rest  "$mcname$ do$s$ a drumroll for $hcname$.  K'tisssh!")
    )
  )
)
