(flutter
  (pattern "<indirect:any-living>")
  (single
    (no-arguments
      (self "You flutter your eyelashes.")
      (rest "$mcname$ flutter$s$ $mposs$ eyelashes.")
    )
  )
  (targeted
    (no-arguments
      (self   "You flutter your eyelashes at $hcname$.")
      (target "$mcname$ flutter$s$ $mposs$ eyelashes at you.")
      (rest  "$mcname$ flutter$s$ $mposs$ eyelashes at $hcname$.")
    )
  )
)

