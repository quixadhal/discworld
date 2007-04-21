(jedi
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<string>")  
  (single
    (no-arguments
      (self   "You do wavy hand movements and say: \"These are not the imps you are looking for.\"")    
      (rest   "$mcname$ do$s$ wavy hand movements and says: \"There are not the imps you are looking for.\"")
    )
    (arguments
      (arguments These are not the imps you are looking for., You have no need to see my papers., #)
      (self   "You do wavy hand movements and say: \"$arg$\"")    
      (rest   "$mcname$ do$s$ wavy hand movements and says: \"$arg$\"")
    )    
  )
  (targeted
    (no-arguments
      (self   "You do wavy hand movements and say to $hcname$: \"These are not the imps you are looking for.\"")    
      (rest   "$mcname$ do$s$ wavy hand movements and says to $hcname$: \"These are not the imps you are looking for.\"")    
      (target   "$mcname$ do$s$ wavy hand movements and says to you: \"These are not the imps you are looking for.\"")          
    )
    (arguments
      (arguments These are not the imps you are looking for., You have no need to see my papers., #)
      (self   "You do wavy hand movements and say to $hcname$: \"$arg$\"")    
      (rest   "$mcname$ do$s$ wavy hand movements and says to $hcname$: \"$arg$\"")    
      (target   "$mcname$ do$s$ wavy hand movements and says to you: \"$arg$\"")          
    )
  )
)
