### Funny Simulation Software for the Stormdrive 1000
___
Maybe might explode?
who knows.

![image](https://github.com/user-attachments/assets/41c81d90-98b8-459b-9e1b-4e1cc428e855)
![image](https://github.com/user-attachments/assets/0089c9d0-3ecf-417e-8c66-ccf6f30ab74d)
![image](https://github.com/user-attachments/assets/081389b5-bd0f-4f73-b509-2fe25636a44f)
NEW AWESOME TIMING SECTION

___
### PRESS ESC KEY TO OPEN THE COMMAND PROMPT! 

### NUMPAD 9 TO FIRE PARTICLE ACCELERATOR

### NUMPAD 5 TO CLEAR ALARMS

### NUMPAD 8 TO RESET SAMPLES

### NUMPAD +- TO CHANGE SPEED BY 10ms (useful for syncing or something)

"help: Displays this menu" (displays help menu)

### "go: ends initalization starts program" (displays the display and enables reactor processing! NEEDED TO START)

"stop: Ends Program" (Halts the program)

"fueledit: lets you edit fuel ratio" (Fuel edit mode, edit the input gasses and view them too!)

"rodedit: lets you edit control rods" (View individual rod type and integrities, and insert (or remove) rods)

"rp: Rod insertion percent" (self explainatory)

"fire: shoots level 3 pa blast" (basically turning the PA on for one blast at level 3)

"speed: sim speed" (Simulation speed, literally the time it takes between process ticks (and commands))

### "close: close command prompt" (doesn't work within other menus)

"clearalarms: clears alarms"

"skip: skip forward in time" (skips that amount of process ticks, every process tick is 2 seconds real time)

"display: toggles display, disabling increases speed" (says what it is)

"displayclear: toggles display clearing, disabling increases speed at the cost of your vision" (says what it is)

"polledit: setup averages polling or reset" (how many samples we will take before looping, and also views samples, resets taken samples on save)
___
just compile and run the .exe make sure the config.txt is there

![image](https://github.com/user-attachments/assets/24ced471-57ba-44cd-981d-f237120475a1)

^^ should look like this

Dont input commands or numbers incorrectly or you DIE (Not actually, just press ctrl+c to get out of infinite loops, file system is READ ONLY)

![image](https://github.com/user-attachments/assets/ec2cc8c4-d200-4ede-a18e-e5214a4fa62d)

This is my first time actually coding.

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
Use at the risk of infinite console spam (press ctrl+c to close duh)
___

### All the stormdrive code ripped from you know where


Likes to trigger windows defender, You can just read the source code within 10 mins its tiny.
Ok maybe not anymore... doesn't trigger windows defender anymore for me though!


If you want to simulate really *really* fast, set pollcount to 0, and use skip, or turn off display and display clear and set speed to 0 depending on your purposes. Skip uses unsigned long int, though since every process is 2 seconds divide that by two and you have the max skip amount... not that you will be able to simulate millions of years though. Maximum reasonable time to skip is like 4-11 hours, which is like a millisecond, 130 years takes like 5 minutes maybe... (you arnt playing spessmans that long i fucking hope)
Skip also stops when the reactor is a process tick away from meltdown, usually when the rods deplete or stability effects past 8/12mw destroy you with 90-c temp jumps from two tesla zaps.

### SIM SPEED EFFECTS THE TIME IT TAKES FOR COMMANDS TO TAKE EFFECT!!!!!!
Skip may seem weirdly slow at small values, a sim speed of 2000ms (default) will have any command take 2000ms to take effect dispite how fast it may be.

EXCEPT NOT NOW HAHA

### IF YOU WANT TO SYNC WITH AN IN GAME REACTOR PRESS 9 AS SOON AS THE ACCELERATOR STUFF HITS THE REACTOR!, THIS WILL WORK "GOOD ENOUGH"
___
### COMPILING:
I USE THE G++ compiler with make (so i dont have to type in commands every time)
if you have ever compiled C++ from command line with make its the exact same as anything else
just type "make" (with the cmd directory at the same locaton as the makefile obviously)
it just runs, "g++ -O2 STORMDRIVE.cpp -o STORMDRIVE.exe"

![image](https://github.com/user-attachments/assets/fc68abdb-3058-445b-a42d-b34dfdb34798)

### I DONT KNOW HOW ONLINE COMPILERS WORK, YOU NEED THE windows.h LIBRARY, THIS IS A WINDOWS PROGRAM
### YOU ALSO NEED WAY TOO MANY OTHER LIBRARIES
Thanks and have fun :)
