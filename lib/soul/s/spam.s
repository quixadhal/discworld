(spam
  (pattern "<string>")
  (pattern "<indirect:any-living>" )
  (pattern "<string> <indirect:any-living>" )
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You spam away.")
      (rest "$mcname$ spam$s$ away.")
    )
    (arguments
    (arguments  the room, the boards, the talker, everyone, the creators, 
                mercilessly, annoyingly, endlessly, forever, 
                forever and a day, relentlessly, proudly, unknowingly, 
                torturously, like a newbie, like a creator, #)
      (self "You spam $arg$.")
      (rest "$mcname$ spam$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments  at, like)
      (self   "You spam $arg$ $hcname$.")
      (target "$mcname$ spam$s$ $arg$ you.")
      (rest   "$mcname$ spam$s$ $arg$ $hcname$.")
    )
    (arguments
      (arguments  the room, the boards, the talker, everyone, the creators, 
                  mercilessly, annoyingly, endlessly, forever, 
                  forever and a day, relentlessly, proudly, unknowingly, 
                  torturously, like a newbie, like a creator, specifically, 
                  personally, #)
      (self   "You spam $arg$ at $hcname$.")
      (target "$mcname$ spam$s$ $arg$ at you.")
      (rest   "$mcname$ spam$s$ $arg$ at $hcname$.")
    )
  )
)
