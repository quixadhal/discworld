t(woo
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
   (no-arguments
    (self   "You woo.")
    (rest  "$mcname$ woo$s$.")
      )
    (arguments
      (arguments
          vaguely,carefully,slowly,dangerously,quietly,silently,sarcastically,
          falsely,manically,evilly,wickedly,insanely,
         mischievously,devilishly,impishly,cheekily,idiotically,
         tentatively,wryly,sceptically,happily,backwards,sideways,#)
      (self "You woo $arg$.")
      (rest "$mcname$ woo$s$ $arg$.")
    )
   (arguments
       (arguments pleasure,amazement,#)
       (self "You woo in $arg$.")
       (rest "$mcname$ woo$s$ in $arg$.")
  )
   )
  (targeted
    (no-arguments
      (self   "You woo at $hcname$.")
      (target "$mcname$ woo$s$ at you.")
      (rest  "$mcname$ woo$s$ at $hcname$.")
    )
    (arguments
      (arguments
          vaguely,carefully,slowly,dangerously,quietly,silently,sarcastically,
          falsely,manically,evilly,wickedly,insanely,
         mischievously,devilishly,impishly,cheekily,idiotically,
         tentatively,wryly,sceptically,happily,backwards,sideways,#)
      (self   "You woo $arg$ at $hcname$.")
      (target "$mcname$ woo$s$ $arg$ at you.")
      (rest  "$mcname$ woo$s$ $arg$ at $hcname$.")
    )
     (arguments
       (arguments pleasure,amazement,#)    
       (self "You woo in $arg$ at $hcname$.")   
       (target "$mcname$ woo$s$ in $arg$ at you.")  
       (rest "$mcname$ woo$s$ in $arg$ at $hcname$.")
 )
  )
)

