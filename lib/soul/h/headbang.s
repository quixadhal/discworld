(headbang
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments  to Music With Rocks In, wildly, with blatant disregard for
                  safety, till dawn, like it was just invented, 
                  like it was going out of style, violently, like a lady, 
                  like Ralph Womblefluff, primally, gently, #)
      (self "You headbang $arg$.")
      (rest "$mcname$ headbang$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments  to Music With Rocks In, wildly, with blatant disregard for
                  safety, till dawn, like it was just invented, 
                  like it was going out of style, violently, like a lady, 
                  like Ralph Womblefluff, primally, gently, #)
      (self   "You headbang $arg$ at $hcname$.")
      (target "$mcname$ headbang$s$ $arg$ at you.")
      (rest  "$mcname$ headbang$s$ $arg$ at $hcname$.")
    )
  )
)
/* Saffra, for Fevvling, Feb 8 2002 */

