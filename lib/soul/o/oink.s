(oink
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You oink.")
      (rest "$mcname$ oink$s$.")
    )
    (arguments
      (arguments happily,like a pig,manically, evilly, wickedly, insanely, 
                 mischievously, devilishly, impishly, cheekily, idiotically, 
                 tentatively, wryly, sceptically,#)
      (self "You oink $arg$.")
      (rest "$mcname$ oink$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You oink at $hcname$.")
      (target "$mcname$ oink$s$ at you.")
      (rest  "$mcname$ oink$s$ at $hcname$.")
    )
    (arguments
      (arguments happily,like a pig,manically, evilly, wickedly, insanely, 
                 mischievously, devilishly, impishly, cheekily, idiotically, 
                 tentatively, wryly, sceptically,#)
      (self   "You oink $arg$ at $hcname$.")
      (target "$mcname$ oink$s$ $arg$ at you.")
      (rest  "$mcname$ oink$s$ $arg$ at $hcname$.")
    )
  )
)
/* Hobbes, 961216 */
