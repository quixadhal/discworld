.DT
hotfood.c
DW_wibble autodoc help
hotfood.c

.SH Description
.SP 5 5



.EP
.SP 5 5
This is an inherit for hot food which cools down, based on the temperature of the room in which it finds itself and any containers it might be in. 

It performs just as normal food, apart from the following: 

- If eaten while still hot it will cause the consumer to warm up for a period of time. 

- It only decays once it has finished cooling. 

It's cooling level can be checked by query_cool_level(). This starts at a value of twenty times the original weight of the hot food and determines how long it takes to cool down. Larger items cool more slowly. 

The level of cooling is shown in the long and short of the object. These are created using query_short_rotten_adjective and query_long_decay_level, and are based on the percentage by which it has cooled. 

Differant messages can be displayed when the food is eaten hot or cold These can be set using set_hot_eat_message() and set_cold_eat_message() Messages can also be set for eating bites of food where appropriate using setup_cold_eat_piece_messages() and setup_hot_eat_piece_messages() 
.EP
.SP 10 5


Written by Sightblinder


.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/food.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/weather.h and /include/bits_controller.h.
.EP

