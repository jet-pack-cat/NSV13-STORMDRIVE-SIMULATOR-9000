Funny Simulation Software for the Stormdrive 1000
Maybe might explode?
who knows.

just compile and run the .exe make sure the config.txt is there
Dont input commands or numbers incorrectly or you DIE

This is my first time actually coding.

commands and operands can be put on the same line to some degree.
example:

fueledit

GAS_O2

14

GAS_N2

15

GAS_PLASMA 42

mols

100

view

save

close



AND:



fueledit GAS_O2 14 GAS_N2 15 GAS_PLASMA 42 mols 100 view save close

are functionally equivalent.

go + fire does not work (anything that closes the command prompt)

There is barely any error correction or handling (I am lazy and it was made for myself)


All the stormdrive code ripped from you know where


Likes to trigger windows defender, You can just read the source code within 10 mins its tiny.
Ok maybe not anymore... doesn't trigger windows defender anymore for me though!


If you want to simulate really *really* fast, set pollcount to 0, and use skip, or turn off display and display clear and set speed to 0 depending on your purposes. Skip uses unsigned long int, though since every process is 2 seconds divide that by two and you have the max skip amount... not that you will be able to simulate millions of years though. Maximum reasonable time to skip is like 4 hours, which is like a millisecond, 130 years takes like 5 minutes maybe... (you arnt playing spessmans that long i fucking hope)
Skip also stops when the reactor is a process tick away from meltdown, usually when the rods deplete or stability effects past 8/12mw destroy you with 90-c temp jumps from two tesla zaps.

Thanks and have fun :)
