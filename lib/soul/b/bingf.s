(bingf
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments sarcastic, sad, joyful, sage, knowing, quick,  
                 loud, careful, baleful, excited, nervous, bright, 
                 uncertain)
      (self "You bing in a $arg$ but formatted way.")
      (rest "$mcname$ bing$s$ in a $arg$ but formatted way.")
    )
  )
  (targeted
    (arguments
      (arguments sarcastic, sad, joyful, sage, knowing, quick,  
                 loud, careful, baleful, excited, nervous, bright, 
                 uncertain)
      (self   "You bing in a $arg$ but formatted way at $hcname$.")
      (target "$mcname$ bing$s$ in a $arg$ but formatted way at you.")
      (rest  "$mcname$ bing$s$ in a $arg$ but formatted way at $hcname$.")
    )
  )
)
