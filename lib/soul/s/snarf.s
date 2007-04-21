(snarf
  (pattern "<string>")
  (pattern "<indirect:any-living>" )
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You snarf embarrassingly.")
      (rest "$mcname$ $verb$snarfs,snarf$ embarrassingly.")
    )
    (arguments
    (arguments  chocolate cake like Terano, milkshake, brain fluid, 
                orange juice, apple juice, milk, Coca-Cola, Tim Tams,
                Tim Tam explosions, ice water, hot chocolate,
                Smirnoff Ice like a Highlord at a mudmeet, 
                rum and coke, coffee, hot tea, vodka, lemonade, 
                tequila, toothpaste, chips like Revol, #)
      (self "You snarf $arg$ out your nose.")
      (rest "$mcname$ $verb$snarfs,snarf$ $arg$ out $mposs$ nose.")
    )
  )
  (targeted
  (no-arguments
    (self   "You snarf at $hcname$ embarrassingly.")
    (target "$mcname$ $verb$snarfs,snarf$ at you embarrassingly.")
    (rest   "$mcname$ $verb$snarfs,snarf$ at $hcname$ embarrassingly.")
  )
  (arguments
  (arguments  chocolate cake like Terano, milkshake, brain fluid, 
              orange juice, apple juice, milk, Coca-Cola, Tim Tams,
              Tim Tam explosions, ice water, hot chocolate, 
              Smirnoff Ice like a Highlord at a mudmeet, 
              rum and coke, coffee, hot tea, vodka, lemonade, 
              tequila, toothpaste, chips like Revol, #)
    (self   "You snarf $arg$ out your nose at $hcname$." )
    (target "$mcname$ $verb$snarfs,snarf$ $arg$ out $mposs$ nose at you." )
    (rest   "$mcname$ $verb$snarfs,snarf$ $arg$ out $mposs$ nose at $hcname$." )
    )
  )
)
