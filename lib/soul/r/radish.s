(radish
  (pattern "<indirect:any-living>" )
  (pattern "<indirect:any-living> <string>" )
  (targeted
    (no-arguments
      (self   "You radish $hcname$.")
      (target "$mcname$ radish$s$ you.")
      (rest   "$mcname$ radish$s$ $hcname$.")
    )
    (arguments
      (arguments  redly, slowly, quickly, carefully, with a womble, 
                  in a shower, in a swimming pool, on a coffee table, 
                  expertly, awkwardly, absentmindedly, 
                  viciously, with ropes and chains, with nutella, 
                  using fruit tingles, with maple syrup and waffles, 
                  under the lilacs in the spring time, #)
      (self   "You radish $hcname$ $arg$.")
      (target "$mcname$ radish$s$ you $arg$.")
      (rest   "$mcname$ radish$s$ $hcname$ $arg$.")
    )
  )
)


