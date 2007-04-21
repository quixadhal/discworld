(hls5   
  (pattern "<indirect:any-living>")   
  (pattern "<string> <indirect:any-living>")   
  (pattern "<indirect:any-living> <string>")   
  (single     
    (no-arguments       
      (self "You high, low, and side five yourself all at once.  You feel confused.")                
      (rest "$mcname$ high, low, and side five$s$ $mobj$self all at once.")     
    )
  )
  (targeted
    (no-arguments       
      (self   "You high, low, and side five $hcname$ all at once.")       
      (target "$mcname$ high, low, and side five$s$ you all at once.  You feel confused.")       
      (rest  "$mcname$ high, low, and side five$s$ $hcname$ all at once.")     
    )     
  (arguments     
  (arguments happily, annoyingly, courteously, hysterically,
             absentmindedly, confusingly, rapidly, deliberately, 
             slowly, forcefully, violently, #)     
    (self "You high, low, and side five $hcname$ $arg$.")      
    (target "$mcname$ high, low, and side five$s$ you $arg$.  You feel confused.")    
    (rest "$mcname$ high, low, and side five$s$ $hcname$ $arg$.")    
   ) 
  )
)

/* Saffra for Dek Aug 12 2001 */