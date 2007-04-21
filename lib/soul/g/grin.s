(grin
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You grin.")
      (rest "$mcname$ grin$s$.")
    )
    (arguments
      (arguments manically, evilly, wickedly, insanely, mischievously, 
                 devilishly, impishly, cheekily, idiotically, tentatively, wryly, 
                 skeptically, #)
      (self "You grin $arg$.")
      (rest "$mcname$ grin$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You grin at $hcname$.")
      (target "$mcname$ grin$s$ at you.")
      (rest  "$mcname$ grin$s$ at $hcname$.")
    )
    (arguments
      (arguments manically, evilly, wickedly, insanely, mischievously, 
                 devilishly, impishly, cheekily, idiotically, tentatively, wryly, 
                 skeptically, #)
      (self   "You grin $arg$ at $hcname$.")
      (target "$mcname$ grin$s$ $arg$ at you.")
      (rest  "$mcname$ grin$s$ $arg$ at $hcname$.")
    )
  )
)

