(swing
   (pattern "<indirect:any-living>")
   (pattern "<string> <indirect:any-living>")
   (pattern "<indirect:any-living> <string>")
   (targeted
     (no-arguments
       (self "You swing $hcname$ out onto the dance floor.")
       (target "$mcname$ swing$s$ you out onto the dance floor.")
       (rest "$mcname$ swing$s$ $hcname$ out onto the dance floor.")
     )
     (arguments 
     (arguments spinning, twirling, dipping, whirling, flinging, 
                tripping, hopping, dragging, pulling, pushing, #)
       (self "You swing $hcname$ out onto the dance floor, $arg$ $hobj$ as you dance.")
       (target "$mcname$ swing$s$ you out onto the dance floor, $arg$ you as you dance.")
       (rest "$mcname$ swing$s$ $hcname$ out on the dance floor, $arg$ $hobj$ as they dance.")
      ) 
   )
)

/* Saffra 2-25-00, idea by Archana, fixed 3-29-03 */
