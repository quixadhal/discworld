(goose
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (arguments
     (arguments playfully, gleefully, like bob the dinosaur, hysterically)
        (self "You goose $hcname$ $arg$.  Ouch, that has got to hurt.")
        (target "$mcname$ $verb$gooses,goose$ you $arg$.")
        (rest "$mcname$ $verb$gooses,goose$ $hcname$ $arg$.  Ouch, that has got to hurt.")
    )
  )
)

/* Dogbolter */
