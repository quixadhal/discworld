.DT
meltable.c
DW_wibble autodoc help
meltable.c

.SH Description
.SP 5 5



.EP
.SP 5 5
This is an inheritable for frozen food which melts, based on the temperature of the room environment in which it finds itself. 

It performs just as normal food, apart from the following: 

- It cannot be cured or dried 

- It does not decay 

- It cannot be forced to behave as a liquid 

- When applied or eaten, it will cause a brief decrease in temperature of the user. 

It will melt into a liquid, which can be set using set_melt_result(). This is default at water, and can be queried using query_melt_result(). 

It's melt level can be checked by query_melt_level(). This starts at a value of ten times the original weight of the meltable food. 

It's rate of melting adjusts itself regularly, based on the temperature. This can be checked with query_melt_rate(), and set temporarily with set_melt_rate. 

The level of melting is shown in the long and short of the object. These are created using query_short_rotten_adjective and query_long_decay_level, and are based on the percentage of object left. 
.EP
.SP 10 5


Written by Carmine


.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/food.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/weather.h and /include/bits_controller.h.
.EP

