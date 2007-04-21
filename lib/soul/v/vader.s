(vader
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<string>")  
  (single
    (no-arguments
      (self   "You squeeze your fingers together and say: \"I find your lack of faith disturbing.\"")    
      (rest   "$mcname$ squeeze$s$ $mposs$ fingers together and say$s$: \"I find your lack of faith disturbing.\"")
    )
    (arguments
      (arguments I find your lack of faith disturbing., You have failed me for the last time Admiral, #)
      (self   "You squeeze your fingers together and say: \"$arg$\"")    
      (rest   "$mcname$ squeeze$s$ $mposs$ fingers together and say$s$: \"$arg$\"")
    )    
  )
  (targeted
    (no-arguments
      (self   "You squeeze your fingers together and say to $hcname$: \"I find your lack of faith disturbing.\"")    
      (rest   "$mcname$ squeeze$s$ $mposs$ fingers together and say$s$ to $hcname$: \"I find your lack of faith disturbing.\"")    
      (target   "$mcname$ squeeze$s$ $mposs$ fingers together and say$s$ to $hcname$: \"I find your lack of faith disturbing.\"")          
    )
    (arguments
      (arguments I find your lack of faith disturbing., You have failed me for the last time Admiral, #)
      (self   "You squeeze your fingers together and say to $hcname$: \"$arg$\"")    
      (rest   "$mcname$ squeeze$s$ $mposs$ fingers together and say$s$ to $hcname$: \"$arg$\"")    
      (target   "$mcname$ squeeze$s$ $mposs$ fingers together and say$s$ to $hcname$: \"$arg$\"")          
    )
  )
)
