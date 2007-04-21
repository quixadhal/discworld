(chuckle
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You chuckle.")
      (rest "$mcname$ chuckles.")
    )
   (arguments 
     (arguments politely,nastily,demonically,ironically,like a train,
      sarcastically,patiently,slightly,carefully,happily)
      (self "You chuckle $arg$.")
      (rest "$mcname$ chuckles $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You chuckle at $hcname$.")
      (target "$mcname$ chuckles at you.")
      (rest  "$mcname$ chuckles at $hcname$.")      
    )
    (arguments
      (arguments  politely,nastily,demonically,ironically,like a train,
       sarcastically,patiently,slightly,carefully,happily)
      (self   "You chuckle $arg$ at $hcname$.")
      (target "$mcname$ chuckles $arg$ at you.")
      (rest  "$mcname$ chuckles $arg$ at $hcname$.")
     )
   )
 )
