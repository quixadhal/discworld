(fonz
  (pattern "<indirect:any-living>")
  (single
    (no-arguments
      (self "You stick both thumbs in the air and say: Ayyyyyyyy...")
      (rest "$mcname$ stick$s$ both thumbs in the air and say$s$: Ayyyyyyyy...")
    )
  )
  (targeted
    (no-arguments
      (self   "You tug on your jacket, and point at $hcname$ with both hands.")
      (target "$mcname$ tug$s$ on $mposs$ jacket, and point$s$ at you with both hands.")
      (rest  "$mcname$ tug$s$ on $mposs$ jacket, and point$s$ at $hcname$ with both hands.")
    )
  )
)

/* Implemented by Hobbes 960920. Ideas from Grampa and Presto. */

