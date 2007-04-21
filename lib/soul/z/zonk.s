(zonk
  (pattern "<indirect:any-living>")
  (single
    (no-arguments
      (self "You go 'Zonke zonke zonke'.")
      (rest "$mcname$ go$s$ 'Zonke zonke zonke'.")
    )
  )
  (targeted
    (no-arguments
      (self "You zonk $hcname$ on the nose.")
      (target "$mcname$ zonk$s$ you on the nose.")
      (rest "$mcname$ zonk$s$ $hcname$ on the nose.")
    )
  )
)
