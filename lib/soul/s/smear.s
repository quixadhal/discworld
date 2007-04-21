(smear
  (pattern "<indirect:any-living> in <string>")
  (pattern "<indirect:any-living> <string>")
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (single
    (arguments
      (arguments frogs, cream, butter, maple syrup, chocolate sauce,
                 compliments, a sheet, cheese, crabs, socks,
                 carrots, sharpened carrots, red paint,
                 spam, parakeets, pocket watches,
                 false moustaches, lilies, roses, carnations, honey,
                 ducks, fruit, lemons, green glass balls, spider web,
                 cupcakes, fluffy animals, coffee tables, peanut butter,#)
      (self "You smear yourself in $arg$.")
      (rest "$mcname$ smear$s$ $mobj$self in $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments frogs, cream, butter, maple syrup, chocolate sauce,
                 compliments, a sheet, cheese, crabs, socks, honey,
                 ducks, fruit, lemons, green glass balls, spider web,
                 false moustaches, lilies, roses, carnations,
                 carrots, sharpened carrots, socks, red paint,
                 spam, parakeets, pocket watches,
                 cupcakes, fluffy animals, coffee tables, peanut butter, #)
      (self "You smear $hcname$ in $arg$.")
      (target "$mcname$ smear$s$ you in $arg$.")
      (rest "$mcname$ smear$s$ $hcname$ in $arg$.")
    )
    (arguments
      (arguments up)
      (self "You smear $hcname$ $arg$.")
      (target "$mcname$ smear$s$ you $arg$.")
      (rest "$mcname$ smear$s$ $hcname$ $arg$.")
    )
  )
)
