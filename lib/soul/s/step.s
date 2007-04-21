(step
  (pattern "[on] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [on] <indirect:any-living>")
  (pattern "[on] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You do a little step dance.")
      (rest "$mcname$ do$s$ a little step dance.")
    )
    (arguments
      (arguments happily,professionally,in shame,on the head,quickly,
                 in joy,sadly,#)
      (self "You step dance $arg$.")
      (rest "$mcname$ step dance$s$ $arg$.")
    )
)  
  (targeted
   (arguments
    (arguments foot,frog,keyboard,code,head,body,toes,#)
      (self   "You step on $mhcname$ $arg$.")
      (target "$mcname$ step$s$ on your $arg$.")
      (rest  "$mcname$ step$s$ on $mhcname$ $arg$.")
    )
   )
 )  
