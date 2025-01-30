### Funny Simulation Software for the Stormdrive 1000
### This is my first time actually coding.
___
Maybe might explode?
who knows.
![goofy](https://github.com/user-attachments/assets/a7f7f0a0-e982-4805-b10c-3b935c4d7305)

___
### OLD PICTURES
___
![image](https://github.com/user-attachments/assets/41c81d90-98b8-459b-9e1b-4e1cc428e855)
![image](https://github.com/user-attachments/assets/0089c9d0-3ecf-417e-8c66-ccf6f30ab74d)
![image](https://github.com/user-attachments/assets/081389b5-bd0f-4f73-b509-2fe25636a44f)
NEW AWESOME TIMING SECTION

___
"help: Displays this menu"

"go: ends initalization starts program"

"stop: Ends Program"

"datawrite: toggles datafile writing (DO NOT USE WHEN SKIPPING LARGE TIMES, WILL FILL UP MEMORY)"

"sync: toggles syncing time"

"clearalarms: clears alarms"

"fueledit: lets you edit fuel ratio"

"rodedit: lets you edit control rods"

"rp: Rod insertion percent"

"fire: shoots level 3 pa blast"

"speed: sim speed"

"close: close command prompt"

"skip: skip forward in time"

"display: toggles display, disabling increases speed"

"displayclear: toggles display clearing, disabling increases speed at the cost of your vision"

"polledit: setup averages polling or reset" 

"entropy: set entropy value"

"numpad 5 to clear alarms" 

"numpad 8 to clear samples" 

"numpad 9 to fire PA" 

"numpad -+ to change speed by 10ms" 

"numpad 7 to toggle syncing" 

"numpad 1 to randomize gas properties" 

"numpad 2 to minimize gas properties"

___
just compile and run the .exe make sure the config.txt is there

![image](https://github.com/user-attachments/assets/24ced471-57ba-44cd-981d-f237120475a1)

^^ should look like this

Dont input commands or numbers incorrectly or you DIE (Not actually, just press ctrl+c to get out of infinite loops (that just closes command prompt))

![image](https://github.com/user-attachments/assets/ec2cc8c4-d200-4ede-a18e-e5214a4fa62d)



### commands and operands can be put on the same line to some degree.

example:
![image](https://github.com/user-attachments/assets/e3e05c38-c533-4b3a-a9a8-5ff12803d271)

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
![image](https://github.com/user-attachments/assets/d6e11583-4da5-44aa-a70d-46e12a85c8fc)

are functionally equivalent.

go + fire does not work (anything that closes the command prompt)

![image](https://github.com/user-attachments/assets/53ddfe78-cb8e-48ec-9b58-ac050e420161)
___
### There is barely any error correction or handling (I am lazy and it was made for myself)
this is my first project ever
Use at the risk of infinite console spam (press ctrl+c to close duh)
___

### All the stormdrive code ripped from you know where

Likes to trigger windows defender, You can just read the source code within 10 mins its tiny.
Ok maybe not anymore... doesn't trigger windows defender anymore for me though!

If you want to simulate really *really* fast, set pollcount to 0, and use skip, or turn off display and display clear and set speed to 0 depending on your purposes. Skip uses unsigned long int, though since every process is 2 seconds divide that by two and you have the max skip amount... not that you will be able to simulate millions of years though. Maximum reasonable time to skip is like 4-11 hours, which is like a millisecond, 130 years takes like 5 minutes maybe... (you arnt playing spessmans that long i fucking hope)
Skip also stops when the reactor is a process tick away from meltdown, usually when the rods deplete or stability effects past 8/12mw destroy you with 90-c temp jumps from two tesla zaps.

### SIM SPEED EFFECTS THE TIME IT TAKES FOR COMMANDS TO TAKE EFFECT!!!!!!

### IF YOU WANT TO SYNC WITH AN IN GAME REACTOR PRESS 9 AS SOON AS THE ACCELERATOR STUFF HITS THE REACTOR!, THIS WILL WORK "GOOD ENOUGH"
___

### COMPILING:
I USE THE G++ compiler with make (so i dont have to type in commands every time)
if you have ever compiled C++ from command line with make its the exact same as anything else
just type "make" (with the cmd directory at the same locaton as the makefile obviously)
it just runs, "g++ -O2 STORMDRIVE.cpp -o STORMDRIVE.exe"

![image](https://github.com/user-attachments/assets/fc68abdb-3058-445b-a42d-b34dfdb34798)

### I DONT KNOW HOW ONLINE COMPILERS WORK, THIS IS A WINDOWS PROGRAM
### YOU ALSO NEED WAY TOO MANY OTHER LIBRARIES
### i just downloaded some c/c++ toolkit i dont really know what exactly goes into linkers and libraries and shit. im sure visual studio works just as well.
Thanks and have fun :)
