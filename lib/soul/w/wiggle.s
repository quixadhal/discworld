(wiggle
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<indirect:any-living> [at] <string>")
  (pattern "<string> [at] <indirect:any-living>")
  (single
    (no-arguments
      (self "You wiggle your bottom.")
      (rest "$mcname$ wiggle$s$ $mposs$ bottom.")
    )
    (arguments
      (arguments eyebrows, nose, ear, toe, foot, arm, leg, coffee table,
                  shower hose, green shoes,#)
      (self "You wiggle your $arg$.")
      (rest "$mcname$ wiggle$s$ $mposs$ $arg$.")
    )
    (arguments
      (arguments happily,enticingly,suggestively,vigorously,arrogantly,
                 nonchalantly,quickly,tunelessly)
      (self "You wiggle your bottom $arg$.")
      (rest "$mcname$ wiggle$s$ $mposs$ bottom $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments bottom, eyebrows, nose, ear, toe, foot, arm, leg, coffee table,
                  shower hose, green shoes, three-legged table,
                  tush, behind, cute backside, butt, oranges,
                  fingers, thumbs, chest, foot, tongue, face,
                  gloves,#)
      (self "You wiggle your $arg$ at $hcname$.")
      (target "$mcname$ wiggle$s$ $mposs$ $arg$ at you.")
      (rest "$mcname$ wiggle$s$ $mposs$ $arg$ at $hcname$.")
    )
    (arguments
      (arguments happily, enticingly, suggestively, vigorously, arrogantly,
                 tunelessly, nonchalantly, quickly, bizarrely, with some celery,
                 expressively, enthusiastically, creamily)
      (self   "You wiggle your bottom $arg$ at $hcname$.")
      (target "$mcname$ wiggle$s$ $mposs$ bottom $arg$ at you.")
      (rest   "$mcname$ wiggle$s$ $mposs$ bottom $arg$ at $hcname$.")
    )
  )
)

