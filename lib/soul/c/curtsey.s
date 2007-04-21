(curtsey
  (pattern "[to|before] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> [before|to] <string>")
  (single
    (no-arguments
      (self "You curtsey.")
      (rest "$mcname$ curtsies.")
    )
    (arguments
      (arguments  gracefully, humbly, quickly, low, slightly, prettily, 	
                  handsomely, surlishly, extravagantly, #)
      (self "You curtsey $arg$.")
      (rest "$mcname$ curtsies $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You curtsey to $hcname$.")
      (target "$mcname$ curtsies before you.")
      (rest  "$mcname$ curtsies to $hcname$.")
    )
    (arguments
      (arguments  gracefully, humbly, quickly, low, slightly, prettily, 	
                  handsomely, surlishly, extravagantly, #)
      (self   "You curtsey $arg$ to $hcname$.")
      (target "$mcname$ curtsies $arg$ before you.")
      (rest  "$mcname$ curtsies $arg$ to $hcname$.")
    )
  )
)

