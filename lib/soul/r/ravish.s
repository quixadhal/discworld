(ravish
  (pattern "<indirect:any-living>" )
  (pattern "<indirect:any-living> <string>" )
  (targeted
  (no-arguments
    (self "You ravish $hcname$.")
    (target "$mcname$ ravish$s$ you.")
    (rest "$mcname$ ravish$s$ $hcname$.")
  )
  (arguments
    (arguments  slowly, quickly, carefully, with a womble, in a shower, 
                in a swimming pool, on a coffee table, expertly,
                awkwardly, absentmindedly, 
                viciously, with ropes and chains, with nutella, 
                using fruit tingles, with maple syrup and waffles, 
                under the lilacs in the spring time, #)
    (self "You ravish $hcname$ $arg$." )
    (target "$mcname$ ravish$s$ you $arg$." )
    (rest "$mcname$ ravish$s$ $hcname$ $arg$." )
    )
  )
)
