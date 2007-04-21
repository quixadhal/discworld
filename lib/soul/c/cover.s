(cover
  (pattern "<indirect:any-living> in <string>")
  (pattern "<indirect:any-living> <string>")
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (single
    (arguments
      (arguments frogs, cream, butter, maple syrup,
                 compliments, a sheet, cheese, crabs, socks,
                 carrots, sharpened carrots, red paint,
                 spam, parakeets, pocket watches,
                 false moustaches, lilies, roses, carnations, honey,
                 ducks, fruit, lemons, green glass balls, spider web,
                 cupcakes, fluff, fluffy animals, coffee tables,#)
      (self "You cover yourself in $arg$.")
      (rest "$mcname$ covers $mobj$self in $arg$.")
    )
    (arguments
      (arguments up)
      (self "You cover yourself $arg$.")
      (rest "$mcname$ covers $mobj$self $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments frogs, cream, butter, maple syrup,
                 compliments, a sheet, cheese, crabs, socks, honey,
                 ducks, fruit, lemons, green glass balls, spider web,
                 false moustaches, lilies, roses, carnations,
                 carrots, sharpened carrots, socks, red paint,
                 spam, parakeets, pocket watches,
                 cupcakes, fluff, fluffy animals, coffee tables,#)
      (self "You cover $hcname$ in $arg$.")
      (target "$mcname$ covers you in $arg$.")
      (rest "$mcname$ covers $hcname$ in $arg$.")
    )
    (arguments
      (arguments up)
      (self "You cover $hcname$ $arg$.")
      (target "$mcname$ covers you $arg$.")
      (rest "$mcname$ covers $hcname$ $arg$.")
    )
  )
)
