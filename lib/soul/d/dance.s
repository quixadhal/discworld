(dance
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You dance the disco duck. Feels silly, doesn't it?")
      (rest "$mcname$ dance$s$ the disco duck.")
    )
    (arguments
    (arguments wildly, gracefully, smoothly, energetically, bouncily, upbeatly, 
               jivily, swingingly, waltzingly, romantically, skillfully, 
               sweepingly, clumsily, professionally, #)
      (self "You dance $arg$.")
      (rest "$mcname$ dance$s$ $arg$.")
    )    
  )
  (targeted
    (no-arguments
      (self   "You sweep $hcname$ across the dance floor.")
      (target "$mcname$ sweep$s$ you across the dance floor.")
      (rest  "$mcname$ sweep$s$ $hcname$ across the dance floor.")
    )
    (arguments
    (arguments wildly, gracefully, smoothly, energetically, bouncily, upbeatly, 
               jivily, swingingly, waltzingly, romantically, skillfully, 
               sweepingly, clumsily, professionally, #)
    (self   "You dance $hcname$ $arg$ around the dance floor.")
      (target "$mcname$ dance$s$ you $arg$ across the dance floor.")
       (rest   "$mcname$ dance$s$ $hcname$ $arg$ around the dance floor.")
    )    
     (arguments
       (arguments the hemp fandango, hemp, fandango)
       (self   "You dance the hemp fandango with $hcname$.")
       (target "$mcname$ dance$s$ the hemp fandango with you.")
       (rest   "$mcname$ dance$s$ the hemp fandango with $hcname$.")
     )
  )
)

