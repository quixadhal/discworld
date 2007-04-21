(sfish
  (pattern "[in] <indirect:any-living>")
  (pattern "<string>")
  (pattern "[for|for] <string> [in|in|in] <indirect:any-living> [pockets]")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You fish around in your pockets.")
      (rest "$mcname$ fish$s$ around in $mposs$ pockets.")
    )
    (arguments
      (arguments  spare change, a lighter, #)
      (self "You fish around in your pockets for $arg$.")
      (rest "$mcname$ fish$s$ around in $mposs$ pockets for $arg$.")
    )
    (arguments
      (arguments  backpack, purse, wallet, trunk, cleavage, mind, bank account,  
                  pants, boxer shorts, fishnet stockings, cookie jar, underwear)
      (self "You fish around in your $arg$.")
      (rest "$mcname$ fish$s$ around in $mposs$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You fish around in $mhcname$ pockets.")
      (target "$mcname$ fish$s$ around in your pockets.")
      (rest  "$mcname$ fish$s$ around in $mhcname$ pockets.")
    )
    (arguments
      (arguments  backpack, purse, wallet, trunk, cleavage, mind, bank account,  
                  pants, boxer shorts, fishnet stockings, cookie jar, underwear)
      (self "You fish around in $mhcname$ $arg$.")
      (target "$mcname$ fish$s$ around in your $arg$.")
      (rest "$mcname$ fish$s$ around in $mhcname$ $arg$.")
    )
    (arguments
      (arguments  spare change, a lighter, #)
      (self   "You fish around in $mhcname$ pockets for $arg$.")
      (target "$mcname$ fish$s$ around in your pockets for $arg$.")
      (rest  "$mcname$ fish$s$ around in $mhcname$ pockets for $arg$.")
    )
  )
)
