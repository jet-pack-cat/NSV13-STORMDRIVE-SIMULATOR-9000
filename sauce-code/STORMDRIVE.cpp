#include <iostream>
#include <stdbool.h>
#include <windows.h>
#include <cmath>
#include <algorithm>
#include <cstdio>
#include <string>
#include <fstream>

//horrible to read, horribly programmed, design philosophy is "it just works", worst thing ever created

//Gas Interactions replacing with config file, these are unused
#define LOW_ROR 0.5
#define NORMAL_ROR 1
#define HIGH_ROR 1.5
#define HINDER_ROR (-0.5)
#define REALLY_HINDER_ROR (-1)
#define NULL_ROR 0

#define LOW_IPM 0.5
#define MEDIOCRE_IPM 0.85
#define HIGH_IPM 1.25
#define VERY_HIGH_IPM 1.85

#define LOW_COOLING 0.75
#define HIGH_COOLING 2.025
#define VERY_HIGH_COOLING 4

#define LOW_RADIATION 0.75
#define NORMAL_RADIATION 1
#define HIGH_RADIATION 3

#define LOW_REINFORCEMENT 0.75
#define HIGH_REINFORCEMENT 1.25
#define VERY_HIGH_REINFORCEMENT 1.5

#define HIGH_DEG_PROTECTION 0.75

//defines for gas properties
#define MOL 0
#define ROR 1
#define IPM 2
#define COOLING 3
#define RADIATION 4
#define REINFORCEMENT 5
#define DEG_PROTECTION 6

//gas type defines
#define GAS_N2 0
#define GAS_O2 1
#define GAS_CO2 2
#define GAS_PLASMA 3
#define GAS_TRITIUM 4
#define GAS_NITROUS 5
#define GAS_PLUOXIUM 6
#define GAS_HYPERNOB 7
#define GAS_STIMULUM 8
#define GAS_BZ 9
#define GAS_CONSTRICTED_PLASMA 10
#define GAS_H2O 11
#define GAS_NUCLEIUM 12
#define GAS_NITRYL 13


//Hella public variables and functions, is this bad? yes, do I care? no.

//gas storage arrays, lets you modify gas properties, could be an object but why bother.
float air1[7][14] = 
{
	//N2, O2, CO2, PLASMA, TRITIUM, NITROUS, PLUOXIUM, HYPERNOB, STIMULUM, BZ, CONSTRICTED PLASMA, H20, NUCLIUM, NITRYL
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0 }, //MOLCOUNT
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0 }, //ROR
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0 }, //IPM
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0 }, //COOLING
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0 }, //RADIATION
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0 }, //REINFORCEMENT
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0 } //DEG PROTECTION
};
float reaction_chamber_gases[7][14] = {
	//N2, O2, CO2, PLASMA, TRITIUM, NITROUS, PLUOXIUM, HYPERNOB, STIMULUM, BZ, CONSTRICTED PLASMA, H20, NUCLIUM, NITRYL
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0 }, //MOLCOUNT
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0 }, //ROR
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0 }, //IPM
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0 }, //COOLING
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0 }, //RADIATION
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0 }, //REINFORCEMENT
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0 } //DEG PROTECTION
};

float ratio[14] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 }; //ratio math
float molcount = 0; //also ratio math

//more reactor defines just copy and pasted, most arn't even used.
#define REACTOR_STATE_MAINTENANCE 1
#define REACTOR_STATE_IDLE 2
#define REACTOR_STATE_RUNNING 3
#define REACTOR_STATE_MELTDOWN 4
#define REACTOR_STATE_REPAIR 5
#define REACTOR_STATE_REINFORCE 6
#define REACTOR_STATE_REFIT 7

#define WARNING_STATE_NONE 0
#define WARNING_STATE_OVERHEAT 1
#define WARNING_STATE_MELTDOWN 2

#define MAX_CONTROL_RODS 5


//amazing game loop
#define INITALIZE 0
#define RUNNING 1
#define STOP 2
int GAMESTATE = 0;

//lots of variables
float start_threshold = 20;
float heat = 0;
float target_heat = 200;
float cooling_power = 10;
float cooling_power_modifier = 1;
float control_rod_percent = 8;
float control_rod_integrity = 0;
float control_rod_modifier = 0;
float control_rod_degradation_modifier = 1;
bool control_rod_installation = false;
float control_rods_i[5] =
{
{ 0 }, //integrity
{ 0 },
{ 0 },
{ 0 },
{ 0 },
};
int control_rods_t[5] =
{
{ 0 }, //type
{ 0 },
{ 0 },
{ 0 },
{ 0 },
};
int control_rods_l = 0;
float control_rod_integrity_total = 0;
//rod type defines
#define ROD_NONE 0
#define ROD_NORMAL 1
#define ROD_UPGRADE 2
#define ROD_IRRADIATED 3
#define ROD_WEAK 4
#define ROD_PLASMA 5
//more variables
float heat_gain = 5;
float heat_gaini = 5;
float heat_gain_modifier = 1;
int warning_state = WARNING_STATE_NONE;
float reaction_rate = 0;
float target_reaction_rate = 0;
float delta_reaction_rate = 0;
float reaction_rate_modifier = 1;
float power_loss = 2;
float input_power = 0;
float input_power_modifier = 1;
int state = REACTOR_STATE_IDLE;
float rod_integrity = 100;
float alert_cooldown = 20;
float last_power_produced = 0;
float theoretical_maximum_power = 15000000;
float radiation_modifer = 1;
float reactor_temperature_nominal = 200;
float reactor_temperature_hot = 400;
float reactor_temperature_critical = 650;
float reactor_temperature_meltdown = 800;
float reactor_temperature_modifier = 1;
float reactor_temperature_nominal_i = 200;
float reactor_temperature_hot_i = 400;
float reactor_temperature_critical_i = 650;
float reactor_temperature_meltdown_i = 800;
float reactor_stability = 100;
float base_power = 67500;
float power_produced = 0;
bool reactor_end_times = false;
float nucleium_power_reduction = 0;
float chamber_ror_total = 0;
float chamber_ipm_total = 0;
float chamber_cooling_total = 0;
float chamber_radiation_total = 0;
float chamber_reinforcement_total = 0;
float chamber_degradation_total = 0;
float radiation = 0;
float control_rod_effectiveness_total = 0;
float rod_effectiveness = 0;
float fuel_check = 0;
float total_moles = 0;
//more variables and functions :D
void try_start();
void shoot();
void deactivate();
void handle_reaction_rate();
void handle_heat();
void handle_temperature_reinforcement();
void handle_reactor_stability();
void handle_control_rod_efficiency();
void handle_control_rod_integrity();
int initalize();
void process();
void commandio();
void display();
bool can_cool();
void handle_overload();
int speed = 2000;
void handle_overload();
int meltdowntimer = 19;
void handle_meltdown();
float temp_avg = 0;
float temp_peak = 0;
float temperature_delta_min = 0;
float temperature_delta_max = -80085;
float cooling_peak = 0;
float heating_peak = 0;
float temps[200] = {}; //list of temperatures :)
float rod_last = 100;
float roddep_last = 0;
float dep_avg = 0;
float dep_peak = 0;
float rod_lifetime = -1;
float rod_lifetime_max = -1;
float roddeps[200] = {}; //ROD DEPLETION STATS :)
float power_avg = 0;
float power_peak = 0;
float powers[200] = {0}; //power
unsigned long int uptime = 0;
unsigned long int upseconds = 0;
unsigned long int uphours = 0;
unsigned long int upminutes = 0;
int loop = 0;
int skip = 0;
unsigned long int skipcount = 0;
unsigned long int skipmax = 0;
int polling_count = 200;
int polls = 0;
int total_polls = 0;
void getavg();
int display_enable = 1;
int display_clear = 1;
int dopoll = 1;
int main()
{

	if (initalize())
	{
		std::cerr << "INITALIZATION FAILED!" << std::endl;
	}

	while (GAMESTATE == RUNNING)
	{
		if (skip == 0)
		{
			if (display_clear == 1)
			{
				std::system("cls");
			}
			process();
			if (display_enable == 1)
			{
				display();
			}
			else
			{
				if (dopoll)
				{
					getavg();
				}
				if((heat >= reactor_temperature_meltdown))
				{
					display_enable = 1;
					display_clear = 1;
					std::cout <<"MELTDOWN NEXT PROCESS!!!"<< std::endl << std::endl;
					loop = 1;
					display();
				}
				std::cout <<"UP TIME: " << uptime << "s" << std::endl 
				<<"UP TIME Hrs:Mins:S: " << uphours << ":" << upminutes << ":" << upseconds << std::endl 
				<< "integrity: " << control_rod_integrity << "%" << std::endl;
			}
			commandio();
			if (speed > 0)
			{
				Sleep(speed);
			}
		}
		else
		{
			process();
			if(dopoll)
			{
				getavg();
			}
			if (GetAsyncKeyState(VK_ESCAPE))
			{
				std::cout << (skipmax - skipcount) << std::endl;
			}
			if((skipcount >= skipmax) || (heat >= reactor_temperature_meltdown) || (state == REACTOR_STATE_IDLE)) //:)
			{
				display_enable = 1;
				display_clear = 1;
				skipmax = 0;
				skipcount = 0;
				skip = 0;
				if((heat >= reactor_temperature_meltdown))
				{
					std::cout <<"MELTDOWN NEXT PROCESS!!!"<< std::endl;
					loop = 1;
					display();
					commandio();
				}
			}
			skipcount++;
		}
	}
	std::cout << "process halted" << std::endl;
	system("pause");
	return 0;
}
int initalize()
{
	//GET CONFIG

	std::ifstream sdconfig;
	sdconfig.open("sdconfig.txt");

	//error checking

	if (sdconfig.fail())
	{
		std::cerr << "NO CONFIG FILE!" << std::endl;
		sdconfig.close();
		GAMESTATE = STOP;
		return 1;
	}
	int count = 0;
	float number = 0;
	while (!sdconfig.eof()) //this should be a function but I dont care it runs once
	{
		sdconfig >> number;
		count++;
	}
	sdconfig.clear();
	sdconfig.seekg(0);
	if (count != 109)
	{
		std::cerr << "CONFIG CORRUPT! " << count << std::endl;
		sdconfig.close();
		GAMESTATE = STOP;
	}
	else
	{
		//file exists
		std::cout << "READING CONFIG FILE" << std::endl;

		sdconfig >> molcount; //molcount

		for (int i = 0; i < 14; i++) //Reading ratio numbers
		{
			sdconfig >> ratio[i];
		}

		for (int i = 1; i < 7; i++) //Which property are we reading (we dont need to read mols)
		{
			for (int k = 0; k < 14; k++) //Which gas type
			{
				sdconfig >> air1[i][k];
				reaction_chamber_gases[i][k] = air1[i][k];
			}
		}

		float t = 0;
		for (int i = 0; i < 14; i++) //calculate gasses
		{
			t += ratio[i];
		}
		t = molcount / t;
		for (int i = 0; i < 14; i++)
		{
			air1[MOL][i] = ratio[i] * (t);
		}

		for (int i = 0; i < 5; i++) //rod integrity
		{
			sdconfig >> control_rods_i[i];
		}

		for (int i = 0; i < 5; i++) //rod type
		{
			sdconfig >> control_rods_t[i];
		}


		//read off configured settings

		std::cout << molcount << " MOLCOUNT" << std::endl;

		for (int i = 0; i < 14; i++) //Reading ratio numbers
		{
			std::cout << ratio[i] << " ";
		}

		std::cout << "RATIO" << std::endl;

		for (int i = 0; i < 7; i++)
		{
			for (int k = 0; k < 14; k++)
			{
				std::cout << air1[i][k] << " ";
			}
			switch (i)
			{
			case 0:
				std::cout << "MOLS";
				break;
			case 1:
				std::cout << "ROR";
				break;
			case 2:
				std::cout << "IPM";
				break;
			case 3:
				std::cout << "COOLING";
				break;
			case 4:
				std::cout << "RADIATION";
				break;
			case 5:
				std::cout << "REINFORCEMENT";
				break;
			case 6:
				std::cout << "DEG_PROTECTION";
				break;
			}
			std::cout << std::endl;
		}

		for (int i = 0; i < 5; i++)
		{
			std::cout << std::endl << i << " ROD TYPE: ";
			switch (control_rods_t[i])
			{
			case 0:
				std::cout << "ROD_NONE";
				break;
			case 1:
				std::cout << "ROD_NORMAL";
				break;
			case 2:
				std::cout << "ROD_UPGRADE";
				break;
			case 3:
				std::cout << "ROD_IRRADIATED";
				break;
			case 4:
				std::cout << "ROD_WEAK";
				break;
			case 5:
				std::cout << "ROD_PLASMA";
				break;
			}
			std::cout << std::endl << "INTEGRITY: " << control_rods_i[i] << std::endl;
		}
	}
	sdconfig.close();
	std::cout << "Config success!" << std::endl;
	system("pause");
	std::system("cls");
	std::cout
		<< "Welcome to the Stormdrive Simulator 9000" << std::endl
		<< "This is designed to be an accurate stormdrive simulation." << std::endl
		<< "But without the annoyances of atmospherics or crew" << std::endl;
	std::cout << "PRESS ESCAPE TO INPUT COMMANDS, command help for help" << std::endl;

	while (GAMESTATE == INITALIZE)
	{
		commandio();
	}
	handle_control_rod_efficiency();
	handle_control_rod_integrity();
	return 0;
}

void commandio()
{
	std::string command;


	if (GetAsyncKeyState(VK_ESCAPE) || (loop == 1))
	{
		loop = 1;
		while (loop == 1)
		{
			std::cout << "COMMAND: ";
			std::cin >> command;
			if (command == "help")
			{
				std::cout << std::endl << "help: Displays this menu"
					<< std::endl << "go: ends initalization starts program"
					<< std::endl << "stop: Ends Program"
					<< std::endl << "------------------"
					<< std::endl << "fueledit: lets you edit fuel ratio"
					<< std::endl << "rodedit: lets you edit control rods"
					<< std::endl << "rp: Rod insertion percent"
					<< std::endl << "fire: shoots level 3 pa blast"
					<< std::endl << "speed: sim speed"
					<< std::endl << "close: close command prompt"
					<< std::endl << "skip: skip forward in time"
					<< std::endl << "display: toggles display, disabling increases speed"
					<< std::endl << "displayclear: toggles display clearing, disabling increases speed at the cost of your vision"
					<< std::endl << "polledit: setup averages polling or reset" << std::endl;
			}
			if (command == "display")
			{
				std::cout << std::endl << "display toggled" << std::endl;
				display_enable = !display_enable;
				loop = 0;
				return;
			}
			if (command == "displayclear")
			{
				std::cout << std::endl << "display clear toggled" << std::endl;
				display_clear = !display_clear;
				loop = 0;
				return;
			}
			if (command == "close")
			{
				std::cout << std::endl << "closed" << std::endl;
				loop = 0;
				return;
			}
			if (command == "stop")
			{
				loop = 0;
				GAMESTATE = STOP;
				return;
			}
			if (command == "go")
			{
				std::cout << std::endl << "starting" << std::endl;
				loop = 0;
				GAMESTATE = RUNNING;
				return;
			}
			if (command == "fire")
			{
				shoot();
				std::cout << std::endl << "pew!" << std::endl;
				loop = 0;
			}
			if (command == "rp")
			{
				std::cout << std::endl << "Rod Percent: ";
				std::cin >> control_rod_percent;
				std::cout << std::endl;
				loop = 0;
			}
			if (command == "speed")
			{
				std::cout << std::endl << "speed in ms default 2000: ";
				std::cin >> speed;
				std::cout << std::endl;
				loop = 0;
			}
			if (command == "skip")
			{
				std::cout << std::endl << "How many process ticks to skip (each is two seconds): ";
				std::cin >> skipmax;
				display_enable = 0;
				display_clear = 0;
				skipcount = 0;
				skip = 1;
				std::cout << std::endl;
				loop = 0;
				std::system("cls");
				std::cout << std::endl << "press esc to see remaining processes: " << std::endl;
			}
			if (command == "rodedit")
			{
				bool rodedit = 1;
				while (rodedit == 1)
				{
					std::cout << std::endl << "WELCOME TO ROD EDIT"
						<< std::endl << "TYPE ROD NUMBER 1-5, save, or view" << std::endl << "ROD: ";

					std::cin >> command;
					if (command == "save")
					{
						std::cout << "saved" << std::endl;
						handle_control_rod_efficiency();
						handle_control_rod_integrity();
						rodedit = 0;
					}
					if (command == "view")
					{
						for (int i = 0; i < 5; i++)
						{
							std::cout << std::endl << i <<" ROD TYPE: ";
							switch (control_rods_t[i])
							{
							case 0:
								std::cout << "ROD_NONE";
								break;
							case 1:
								std::cout << "ROD_NORMAL";
								break;
							case 2:
								std::cout << "ROD_UPGRADE";
								break;
							case 3:
								std::cout << "ROD_IRRADIATED";
								break;
							case 4:
								std::cout << "ROD_WEAK";
								break;
							case 5:
								std::cout << "ROD_PLASMA";
								break;
							}
							std::cout << std::endl << "INTEGRITY: " << control_rods_i[i] << std::endl;
						}
					}
					if (command == "1" || command == "2" || command == "3" || command == "4" || command == "5")
					{
						int r = std::stoi(command) - 1;
						std::cout << std::endl << "Select Rod Type: ROD_NONE, ROD_NORMAL, ROD_IRRADIATED, ROD_WEAK, ROD_UPGRADE, ROD_PLASMA" << std::endl;
						std::cin >> command;
						if (command == "ROD_NONE")
						{
							control_rods_t[r] = ROD_NONE;
							control_rods_i[r] = 0;
						}
						if (command == "ROD_NORMAL")
						{
							control_rods_t[r] = ROD_NORMAL;
							control_rods_i[r] = 100;
						}
						if (command == "ROD_UPGRADE")
						{
							control_rods_t[r] = ROD_UPGRADE;
							control_rods_i[r] = 200;
						}
						if (command == "ROD_IRRADIATED")
						{
							control_rods_t[r] = ROD_IRRADIATED;
							control_rods_i[r] = 0;
						}
						if (command == "ROD_WEAK")
						{
							control_rods_t[r] = ROD_WEAK;
							control_rods_i[r] = 80;
						}
						if (command == "ROD_PLASMA")
						{
							control_rods_t[r] = ROD_PLASMA;
							control_rods_i[r] = 100;
						}
					}
				}
			}
			if (command == "fueledit")
			{
				bool fueledit = 1;
				while (fueledit == 1)
				{
					std::cout << std::endl << "WELCOME TO FUEL EDIT"
						<< std::endl << "Type gas name to edit gas ratio (format GAS_[NAME]), mols for total mol count, Type save to continue, Type view to view current ratio" << std::endl;
					std::cin >> command;
					if (command == "save")
					{
						float t = 0;
						for (int i = 0; i < 14; i++)
						{
							t += ratio[i];
						}
						t = molcount / t;
						for (int i = 0; i < 14; i++)
						{
							air1[MOL][i] = ratio[i] * (t);
						}
						std::cout << "saved" << std::endl;
						fueledit = 0;
					}
					if (command == "mols")
					{
						std::cout << "Mol count: ";
						std::cin >> molcount;
					}
					if (command == "view")
					{
						float t = 0;
						for (int i = 0; i < 14; i++)
						{
							t += ratio[i];
						}
						t = molcount / t;
						for (int i = 0; i < 14; i++)
						{
							air1[MOL][i] = ratio[i] * (t);
						}
						for (int i = 0; i < 14; i++)
						{
							switch (i)
							{
							case 0:
								std::cout << "GAS_N2: " << ratio[i] << " mols " << air1[MOL][i] << std::endl;
								break;
							case 1:
								std::cout << "GAS_O2: " << ratio[i] << " mols " << air1[MOL][i] << std::endl;
								break;
							case 2:
								std::cout << "GAS_CO2: " << ratio[i] << " mols " << air1[MOL][i] << std::endl;
								break;
							case 3:
								std::cout << "GAS_PLASMA: " << ratio[i] << " mols " << air1[MOL][i] << std::endl;
								break;
							case 4:
								std::cout << "GAS_TRITIUM: " << ratio[i] << " mols " << air1[MOL][i] << std::endl;
								break;
							case 5:
								std::cout << "GAS_NITROUS: " << ratio[i] << " mols " << air1[MOL][i] << std::endl;
								break;
							case 6:
								std::cout << "GAS_PLUOXIUM: " << ratio[i] << " mols " << air1[MOL][i] << std::endl;
								break;
							case 7:
								std::cout << "GAS_HYPERNOB: " << ratio[i] << " mols " << air1[MOL][i] << std::endl;
								break;
							case 8:
								std::cout << "GAS_STIMULUM: " << ratio[i] << " mols " << air1[MOL][i] << std::endl;
								break;
							case 9:
								std::cout << "GAS_BZ: " << ratio[i] << " mols " << air1[MOL][i] << std::endl;
								break;
							case 10:
								std::cout << "GAS_CONSTRICTED_PLASMA: " << ratio[i] << " mols " << air1[MOL][i] << std::endl;
								break;
							case 11:
								std::cout << "GAS_H2O: " << ratio[i] << " mols " << air1[MOL][i] << std::endl;
								break;
							case 12:
								std::cout << "GAS_NUCLEIUM: " << ratio[i] << " mols " << air1[MOL][i] << std::endl;
								break;
							case 13:
								std::cout << "GAS_NITRYL: " << ratio[i] << " mols " << air1[MOL][i] << std::endl;
								break;
							}
						}
						std::cout << "Total Mols: " << molcount << std::endl;
					}
					if (command == "GAS_N2")
					{
						std::cout << std::endl << "Ratio Number: ";
						std::cin >> ratio[GAS_N2];
					}
					if (command == "GAS_O2")
					{
						std::cout << std::endl << "Ratio Number: ";
						std::cin >> ratio[GAS_O2];
					}
					if (command == "GAS_CO2")
					{
						std::cout << std::endl << "Ratio Number: ";
						std::cin >> ratio[GAS_CO2];
					}
					if (command == "GAS_PLASMA")
					{
						std::cout << std::endl << "Ratio Number: ";
						std::cin >> ratio[GAS_PLASMA];
					}
					if (command == "GAS_TRITIUM")
					{
						std::cout << std::endl << "Ratio Number: ";
						std::cin >> ratio[GAS_TRITIUM];
					}
					if (command == "GAS_NITROUS")
					{
						std::cout << std::endl << "Ratio Number: ";
						std::cin >> ratio[GAS_NITROUS];
					}
					if (command == "GAS_PLUOXIUM")
					{
						std::cout << std::endl << "Ratio Number: ";
						std::cin >> ratio[GAS_PLUOXIUM];
					}
					if (command == "GAS_HYPERNOB")
					{
						std::cout << std::endl << "Ratio Number: ";
						std::cin >> ratio[GAS_HYPERNOB];
					}
					if (command == "GAS_STIMULUM")
					{
						std::cout << std::endl << "Ratio Number: ";
						std::cin >> ratio[GAS_STIMULUM];
					}
					if (command == "GAS_BZ")
					{
						std::cout << std::endl << "Ratio Number: ";
						std::cin >> ratio[GAS_BZ];
					}
					if (command == "GAS_CONSTRICTED_PLASMA")
					{
						std::cout << std::endl << "Ratio Number: ";
						std::cin >> ratio[GAS_CONSTRICTED_PLASMA];
					}
					if (command == "GAS_H2O")
					{
						std::cout << std::endl << "Ratio Number: ";
						std::cin >> ratio[GAS_H2O];
					}
					if (command == "GAS_NUCLEIUM")
					{
						std::cout << std::endl << "Ratio Number: ";
						std::cin >> ratio[GAS_NUCLEIUM];
					}
					if (command == "GAS_NITRYL")
					{
						std::cout << std::endl << "Ratio Number: ";
						std::cin >> ratio[GAS_NITRYL];
					}
				}
			}
			if (command == "polledit")
			{
				bool polledit = 1;
				while (polledit == 1)
				{
					std::cout << std::endl << "Welcome to polledit, here you can edit how average polling works" << std::endl
						<< "Type pollcount, to edit how many samples you want to poll (MAX 200), type save to save and reset the samples" << std::endl;
					std::cin >> command;
					if (command == "pollcount")
					{
						std::cout << std::endl << "sample count: ";
						std::cin >> command;
						int a = 0;
						try 
						{
							a = std::stoi(command);
						}
						catch(std::exception &err)
						{
							a = 0;
						}
						if (a <= 200 && a >= 0)
						{
							if (a == 0)
							{
								dopoll = 0;
							}
							else
							{
								dopoll = 1;
							}
							polling_count = a;
							polls = 0;
							total_polls = 0;
							for (int i = 0; i < polling_count; i++)
							{
								roddeps[i] = 0;
								temps[i] = 0;
								powers[i] = 0;
							}
						}
						else
						{
							std::cout << "INVALID ENTRY >:(" << std::endl;
						}
					}
					if (command == "view")
					{
						for (int i = 0; i < polling_count; i++)
						{
							std::cout << "sample: " << i + 1 << std::endl;
							std::cout << "heat: " << temps[i] << std::endl;
							std::cout << "power: " << powers[i] << std::endl;
							std::cout << "depletion %: " << roddeps[i] << std::endl;
						}
						std::cout << "last sample # " << (polls + 1) << std::endl;
					}
					if (command == "save")
					{
						rod_lifetime = -1;
						rod_lifetime_max = -1;
						roddep_last = 0;
						rod_last = control_rod_integrity;
						temperature_delta_min = 0;
						temperature_delta_max = -80085;
						cooling_peak = 0;
						heating_peak = 0;
						polls = 0;
						total_polls = 0;
						for (int i = 0; i < polling_count; i++)
						{
							roddeps[i] = 0;
							temps[i] = 0;
							powers[i] = 0;
						}
						polledit = 0;
						std::cout << "saved and reset" << std::endl;
					}
				}
			}
		}
	}
}

void shoot()
{
	heat += 50; //level 3
	try_start();
	return;
}
void try_start()
{
	if (state == REACTOR_STATE_RUNNING || state == REACTOR_STATE_MELTDOWN || state == REACTOR_STATE_MAINTENANCE)
	{
		return;
	}
	fuel_check = 0;
	for (int i = 1; i < 14; i++) //does not take n2 into ror for whatever reason :)
	{
		fuel_check += ((air1[ROR][i]) * (air1[MOL][i]));
	}
	std::cout << "Fuel Check: " << fuel_check << std::endl ;
	std::cout << "Start: " << start_threshold << std::endl;
	if (fuel_check >= start_threshold && heat >= start_threshold)
	{
		reactor_stability = 100;
		heat = start_threshold + 10;
		state = REACTOR_STATE_RUNNING;
		uptime = 0;
		if (reaction_rate <= 0)
		{
			reaction_rate = 5;
		}
	}
	else 
	{
		std::cout << "Failed to start " << fuel_check << std::endl;
	}
	return;
}

void process()
{
	if(display_enable == 1)
	{
		std::cout << "Alarms: " << std::endl;
	}
	handle_meltdown(); 
	if (state == REACTOR_STATE_MELTDOWN)
	{
		radiation = 1000 * radiation_modifer;
		return;
	}

	if (state != REACTOR_STATE_RUNNING || heat <= start_threshold)
	{
		deactivate();
		return;
	}
	uptime += 2; //process happens about every 2 real life seconds
	if(display_enable) // division and modulus very expensive holy shit
	{
		upseconds = ((uptime % 60) % 60);
		upminutes = ((uptime - upseconds)/60) % 60;
		uphours = (((uptime - upseconds) / 60) - upminutes) / 60;
	}
	nucleium_power_reduction = 0;
	total_moles = 0;
	for (int i = 0; i < 13; i++)
	{
		total_moles += air1[MOL][i];
	}

	float fuel_check = ((air1[MOL][GAS_PLASMA] + air1[MOL][GAS_CONSTRICTED_PLASMA] + air1[MOL][GAS_TRITIUM]) / total_moles) * 100;
	if (total_moles >= reaction_rate && fuel_check >= 12.5)
	{
		for (int i = 0; i < 13; i++)
		{
			reaction_chamber_gases[MOL][i] = (air1[MOL][i] / total_moles) * reaction_rate;
		}
		chamber_ror_total = 0;
		for (int i = 0; i < 14; i++)
		{
			chamber_ror_total += ((reaction_chamber_gases[ROR][i]) * (reaction_chamber_gases[MOL][i]));
		}
		reaction_rate_modifier = chamber_ror_total / reaction_rate;
		chamber_ipm_total = 0;
		for (int i = 0; i < 14; i++)
		{
			chamber_ipm_total += ((reaction_chamber_gases[IPM][i]) * (reaction_chamber_gases[MOL][i]));
		}
		chamber_ipm_total += reaction_rate;
		input_power_modifier = chamber_ipm_total / reaction_rate;
		chamber_cooling_total = 0;
		for (int i = 0; i < 14; i++)
		{
			chamber_cooling_total += ((reaction_chamber_gases[COOLING][i]) * (reaction_chamber_gases[MOL][i]));
		}
		chamber_cooling_total += reaction_rate;
		cooling_power_modifier = chamber_cooling_total / reaction_rate;
		chamber_radiation_total = 0;
		for (int i = 0; i < 14; i++)
		{
			chamber_radiation_total += ((reaction_chamber_gases[RADIATION][i]) * (reaction_chamber_gases[MOL][i]));
		}
		chamber_radiation_total += reaction_rate;
		radiation_modifer = chamber_radiation_total / reaction_rate;
		chamber_reinforcement_total = 0;
		for (int i = 0; i < 14; i++)
		{
			chamber_reinforcement_total += ((reaction_chamber_gases[REINFORCEMENT][i]) * (reaction_chamber_gases[MOL][i]));
		}
		chamber_reinforcement_total += reaction_rate;
		reactor_temperature_modifier = chamber_reinforcement_total / reaction_rate;
		chamber_degradation_total = 0;
		for (int i = 0; i < 14; i++)
		{
			chamber_degradation_total += ((reaction_chamber_gases[DEG_PROTECTION][i]) * (reaction_chamber_gases[MOL][i]));
		}
		chamber_degradation_total += reaction_rate;
		control_rod_degradation_modifier = chamber_degradation_total / reaction_rate;

		nucleium_power_reduction = reaction_chamber_gases[MOL][GAS_NUCLEIUM] * 1000;

		heat_gain = heat_gaini + reaction_rate;
		for (int i = 0; i < 13; i++)
		{
			reaction_chamber_gases[MOL][i] = 0;
		}

		if (total_moles > ((reaction_rate * 12) + 20))
		{
			if(display_enable == 1)
			{
				std::cout << "Losing stability! Too many input mols! Maximum: " << ((reaction_rate * 12) + 20) << std::endl;
			}
			reactor_stability -= 0.51;
		}

		if (fuel_check >= 25)
		{
			if (reactor_stability < 100)
			{
				reactor_stability += 0.5;
			}
		}
		else
		{
			if(display_enable == 1)
			{
				std::cout << "Losing stability! Fuel ratio below 25%!" << std::endl;
			}
			reactor_stability -= 0.01;
		}
	}
	else
	{
		if(display_enable == 1)
		{
			if(display_enable == 1)
			{
			std::cout << "Losing stability! Out of fuel!" << std::endl;
			}
		}
		reactor_stability--;
		heat_gain = -5;
	}

	input_power = (std::pow((heat / 150), 3) * input_power_modifier);
	power_produced = base_power;
	last_power_produced = std::max<float>(0, (power_produced * input_power) - nucleium_power_reduction);
	handle_reaction_rate();
	handle_heat();
	can_cool(); //function update icon calls this too, rods decay functionally twice per process :)
	handle_temperature_reinforcement();
	handle_reactor_stability();
	radiation = heat * radiation_modifer;

	if (last_power_produced > 3000000)
	{
		if(display_enable == 1)
		{
			if(display_enable == 1)
			{
			std::cout << "Power Output High!" << std::endl;
			}
		}
		handle_overload();
		//get a grounding rod
	}
	
	return;
}

bool can_cool()
{
	for (int i = 0; i < 5; i++)
	{
		if (control_rods_t[i] != ROD_NONE) //dont touch rods that dont exist
		{
			if (control_rods_i[i] <= 0 && control_rods_t[i] != ROD_IRRADIATED)
			{
				control_rods_t[i] = ROD_IRRADIATED;
				handle_control_rod_efficiency();
			}
			if ((std::rand() % 101) < (80 * control_rod_degradation_modifier))
			{
				control_rods_i[i] -= ((input_power / 75000) * control_rod_degradation_modifier) * control_rod_percent;
			}
		}
	}
	handle_control_rod_integrity();
	if (control_rod_integrity < 0)
	{
		control_rod_integrity = 0;
		if (state == REACTOR_STATE_RUNNING)
		{
			if(display_enable == 1)
			{
				std::cout << "Reactor control rods have failed! ";
			}
		}
		return false;
	}
	if (control_rod_integrity <= 35 && warning_state <= WARNING_STATE_NONE)
	{
		if (state == REACTOR_STATE_RUNNING)
		{
			if(display_enable == 1)
			{
				std::cout << "Reactor control rods failing! "; // yes this displays twice because this function is called twice... :)
			}
		}
	}
	if (control_rod_integrity > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void handle_control_rod_efficiency()
{
	control_rod_effectiveness_total = 0;
	for (int i = 0; i < 5; i++)
	{
		switch (control_rods_t[i])
		{
		case ROD_NONE:
			control_rod_effectiveness_total += 0;
			break;
		case ROD_NORMAL:
			control_rod_effectiveness_total += 1;
			break;
		case ROD_IRRADIATED:
			control_rod_effectiveness_total += 0;
			break;
		case ROD_WEAK:
			control_rod_effectiveness_total += 0.8;
			break;
		case ROD_UPGRADE:
			control_rod_effectiveness_total += 1;
			break;
		case ROD_PLASMA: 
			control_rod_effectiveness_total += (-0.5);
			break;
		}
	}
	control_rods_l = 0;
	for (int i = 0; i < 5; i++)
	{
		if (control_rods_t[i] != ROD_NONE)
		{
			control_rods_l++;
		}
	}
	if (control_rods_l > 0) //mimics runtime error lmao
	{
	control_rod_modifier = control_rod_effectiveness_total / control_rods_l;
	}
	return;
}

void handle_control_rod_integrity()
{
	control_rods_l = 0;
	for (int i = 0; i < 5; i++)
	{
		if (control_rods_t[i] != ROD_NONE)
		{
			control_rods_l++;
		}
	}
	if (control_rods_l > 0)
	{
		control_rod_integrity_total = 0;
		for (int i = 0; i < 5; i++)
		{
			if (control_rods_t != ROD_NONE)
			{
				control_rod_integrity_total += control_rods_i[i];
			}
		}
		control_rod_integrity = control_rod_integrity_total / control_rods_l;
	}
	else
	{
		control_rod_integrity = 0;
	}
	return;
}

void handle_heat()
{
	heat += heat_gain;
	if(display_enable == 1)
	{
		std::cout << "Are we cooling?: ";
	}
	target_heat = (-1) + (std::pow(2, (0.1 * ((100 - control_rod_percent) * control_rod_modifier))));
	if (heat > target_heat + ((cooling_power * cooling_power_modifier) - heat_gain))
	{
		if (can_cool())
		{
			if (control_rod_percent > 0)
			{
				heat -= cooling_power * cooling_power_modifier;
				if (heat_gain >= cooling_power * cooling_power_modifier)
				{
					if(display_enable == 1)
					{
						std::cout << "Cooling lower than heat gain!" << std::endl;
					}
				}
				else 
				{
					if(display_enable == 1)
					{
						std::cout << "Cooling" << std::endl;
					}
				}
			}
			else
			{
				if(display_enable == 1)
				{
				std::cout << "Not cooling (Rod Percent)"  << std::endl;
				}
			}
		}
		else
		{
			if(display_enable == 1)
			{
				std::cout << "Not cooling (Rod Integrity)" <<  std::endl;
			}
		}
	}
	else
	{
		if(display_enable == 1)
		{
			std::cout << "Not cooling (Heat Below Target + Power)" << std::endl;
		}
	}
	return;
}

void handle_reaction_rate()
{
	target_reaction_rate = (0.5 + (0.001 * (std::pow(((100 - control_rod_percent) * control_rod_modifier),2))) * reaction_rate_modifier) + 0.00001 * (std::pow(heat, 2));
	delta_reaction_rate = target_reaction_rate - reaction_rate;
	reaction_rate += delta_reaction_rate / 2;
	return;
}

void handle_temperature_reinforcement()
{
	float delta_rt_nominal = (reactor_temperature_nominal_i * reactor_temperature_modifier) - reactor_temperature_nominal;
	reactor_temperature_nominal += delta_rt_nominal / 2;
	float delta_rt_hot= (reactor_temperature_hot_i * reactor_temperature_modifier) - reactor_temperature_hot;
	reactor_temperature_hot += delta_rt_hot / 2;
	float delta_rt_critical = (reactor_temperature_critical_i * reactor_temperature_modifier) - reactor_temperature_critical;
	reactor_temperature_critical += delta_rt_critical / 2;
	float delta_rt_meltdown = (reactor_temperature_meltdown_i * reactor_temperature_modifier) - reactor_temperature_meltdown;
	reactor_temperature_meltdown += delta_rt_meltdown / 2;
	return;
}

void handle_reactor_stability()
{
	if (reactor_stability > 100)
	{
		reactor_stability = 100;
	}
	if (reactor_stability < 0)
	{
		reactor_stability = 0;
	}

	if (rand() % 101 < ((100 - reactor_stability) / 3))
	{
		if(display_enable == 1)
		{
			std::cout << "Gravitational Pulse Detected!" << std::endl;
		}
		//gravity haha
	}

	if (reactor_stability < 75)
	{
		if (rand() % 101 < ((100 - reactor_stability) / 4))
		{
			if (rand() % 101 < 50)
			{
				float ee = 0;
				std::cout << "Heat Destabilizing!" << std::endl;
				ee = reaction_rate * (rand() % 3 + 5);
				heat += ee;
				std::cout << ee << "C" << std::endl;
			}
			else 
			{
				std::cout << "Reaction Rate Destabilizing!" << std::endl;
				reaction_rate += reaction_rate / (rand() % 2 + 3);
			}
		}
	}

	if (reactor_stability < 15)
	{
		if(display_enable == 1)
		{
			std::cout << "ANOMALY WARNING!" << std::endl;
		}
		if (rand() % 101 < 1)
		{
			if(display_enable == 1)
			{
				std::cout << "ANOMALY DETECTED!" << std::endl;
			}
			//john anomaly
		}
		reactor_stability += 15;
	}

	if (reactor_stability <= 1)
	{
		std::cout << "heat rising 1C/s due to low stability!" << std::endl;
		heat += 1;
	}
	if (reactor_stability <= 15 && reactor_stability > 1)
	{
		std::cout << "heat rising 0.5C/s due to low stability!" << std::endl;
		heat += 0.5;
	}
	if (reactor_stability <= 30 && reactor_stability > 15)
	{
		std::cout << "heat rising 0.1C/s due to low stability!" << std::endl;
		heat += 0.1;
	}
	if (reactor_stability <=  75 && reactor_stability > 30)
	{
		std::cout << "heat rising 0.01C/s due to low stability!" << std::endl;
		heat += 0.01;
	}
	if (reactor_stability <= 50 && reactor_stability > 325)
	{
		if(display_enable == 1)
		{
			std::cout << "Reactor hunger bar at half!" << std::endl;
		}
	}
	if (reactor_stability <= 25)
	{
		if(display_enable == 1)
		{
			std::cout << "REACTOR NEEDS FLESH!!!" << std::endl;
		}
	}
	return;
}

void deactivate()
{
	if (state != REACTOR_STATE_IDLE)
	{
		heat = 0;
	}
	reaction_rate = 0;
	reactor_stability = 0;
	state = REACTOR_STATE_IDLE;
	return;
}

void display()
{
	//this is where all the reactor stats should be displayed :)
	if(dopoll)
	{
		getavg();
	}
	if(display_enable == 1)
	{
		std::cout << std::endl
			<< "STORMDRIVE 1000 STATS:" << std::endl
			<< std::endl
			<< "UP TIME: " << uptime << "s" << std::endl
			<<"UP TIME Hrs:Mins:S: " << uphours << ":" << upminutes << ":" << upseconds << std::endl 
			<< std::endl
			<< "Temperature: " << heat << "C" << std::endl
			<< "Temperature mean avg (horribly done): " << temp_avg << "C" << std::endl
			<< "Temperature Nominal: " << reactor_temperature_nominal << "C" << std::endl
			<< "Temperature Hot: " << reactor_temperature_hot << "C" << std::endl
			<< "Temperature Critical: " << reactor_temperature_critical << "C" << std::endl
			<< "Temperature Meltdown:" << reactor_temperature_meltdown << "C" << std::endl
			<< "Temperature peak: " << temp_peak << "C" << std::endl
			<< "Meltdown delta peak: " << (temp_peak - reactor_temperature_meltdown) << "C" << std::endl
			<< "Critical delta peak: " << (temp_peak - reactor_temperature_critical) << "C" << std::endl
			<< std::endl
			<< "Target Heat: " << target_heat << "C" << std::endl
			<< "Target Heat (plus some math (both of these are sorta silly): " << (target_heat + ((cooling_power * cooling_power_modifier) - heat_gain)) << "C" << std::endl
			<< "Heat Gain: " << heat_gain << "C" << std::endl 
			<< "Cooling: " << (cooling_power * cooling_power_modifier) << "C" << std::endl
			<< "Temperature delta: " << heat_gain - (cooling_power * cooling_power_modifier) << "C" << std::endl
			<< "Temperature delta Max, Min: " << temperature_delta_max << "C" << " " << temperature_delta_min << "C" << std::endl
			<< "Heat Gain Peak: " << heating_peak << "C" << std::endl
			<< "Cooling Peak: " << cooling_peak << "C" << std::endl
			<< std::endl
			<< "Power Output: " << last_power_produced << "W " << last_power_produced / 1000000 << "MW" << std::endl
			<< "Power Output mean avg (horribly done): " << power_avg << "W " << power_avg / 1000000 << "MW" << std::endl
			<< "Power Output peak: " << power_peak << "W " << power_peak / 1000000 << "MW" << std::endl
			<< std::endl
			<< "Rod Insertion: " << control_rod_percent << "%" << std::endl
			<< "Rod Integrity (avg between all rods): " << control_rod_integrity << "%" << std::endl
			<< "Rod depletion per second (avg (still bad)): " << dep_avg << "%" << std::endl
			<< "Hypothetical Total Rod Lifetime: " << rod_lifetime_max << "s" << std::endl
			<< "Hypothetical Current Rod Lifetime: " << rod_lifetime << "s" << std::endl
			<< std::endl
			<< "Reaction Rate: " << reaction_rate << "mol/s" << std::endl
			<< std::endl
			<< "Stability: " << reactor_stability << "%" << std::endl
			<< std::endl
			<< "Fuel Mols: " << total_moles << std::endl
			<< std::endl
			<< "Radiation: " << radiation << std::endl
			<< std::endl
			<< "Control Rod Modifier: " << control_rod_modifier << std::endl
			<< "Input Power Modifier: " << input_power_modifier << std::endl
			<< "Cooling Power Modifier: " << cooling_power_modifier << std::endl
			<< "Reaction Rate Modifer: " << reaction_rate_modifier << std::endl
			<< "Radiation Modifier: " << radiation_modifer << std::endl
			<< "Reactor Temperature Modifier: " << reactor_temperature_modifier << std::endl
			<< "Control Rod Modifier: " << control_rod_modifier << std::endl
			<< "Control Rod Degradation Modifer: " << control_rod_degradation_modifier << std::endl
			<< "Nucleium Power Reduction: " << nucleium_power_reduction << std::endl
			<< std::endl;
	}
	return;
}

void handle_overload()
{
	if (last_power_produced >= 3000000 && last_power_produced <= 8000000)
	{
		if(display_enable == 1)
		{
			std::cout << "Annoying light flicker!" << std::endl;
		}
	}
	if (last_power_produced >= 8000000 && last_power_produced <= 12000000)
	{
		if(display_enable == 1)
		{
			std::cout << "Annoying light flicker!" << std::endl;
			std::cout << "Annoying light burnout!" << std::endl;
			std::cout << "Telsa Zap Warning!" << std::endl;
			std::cout << "Stability Decreasing Due to Power!" << std::endl;
		}
		if (rand() % 101 < 0.02) {
			reactor_stability -= std::rand() % 5 + 5;
			std::cout << "Stability FUCKED! (tesla zap)" << std::endl;
		}
	}
	if (last_power_produced >= 12000000)
	{
		if(display_enable == 1)
		{
			std::cout << "Annoying light flicker!" << std::endl;
			std::cout << "Annoying light burnout!" << std::endl;
			std::cout << "Telsa Zap Warning!" << std::endl;
			std::cout << "Stability Decreasing Due to Power!" << std::endl;
		}
		if (rand() % 101 < 0.2) {
			reactor_stability -= std::rand() % 10 + 10;
			std::cout << "Stability FUCKED! (tesla zap)" << std::endl;
		}
	}
	return;
}

void handle_meltdown()
{
	if (warning_state >= WARNING_STATE_OVERHEAT)
	{
		if (heat <= reactor_temperature_critical)
		{
			warning_state = 0;
			if(display_enable == 1)
			{
				std::cout << "Nuclear Meltdown Averted" << std::endl << std::endl;
			}
			reactor_end_times = false;
			meltdowntimer = 19;
		}
	}
	if (heat >= reactor_temperature_critical)
	{
		warning_state = WARNING_STATE_OVERHEAT;
		if(display_enable == 1)
		{
			std::cout << "NUCLEAR MELTDOWN IMMINENT!!!" << std::endl << std::endl;
		}
	}
	if (heat >= reactor_temperature_meltdown)
	{
		if (warning_state < WARNING_STATE_MELTDOWN)
		{
			std::cout << "LAST ROD INTEGRITY BEFORE MELTDOWN: " << control_rod_integrity << "%" << std::endl;
			for (int i = 0; i < 5; i++)
			{
				if (control_rods_t[i] != ROD_NONE) //dont touch rods that dont exist
				{
					if (control_rods_t[i] != ROD_IRRADIATED)
					{
						control_rods_i[i] = 0;
						control_rods_t[i] = ROD_IRRADIATED;
					}
					handle_control_rod_efficiency();
				}
			}
			std::cout << "ERROR IN MODULE FISSREAC0 AT ADDRESS 0x12DF. CONTROL RODS HAVE FAILED.IMMEDIATE INTERVENTION REQUIRED." << "\a" << std::endl;
			reactor_end_times = true;
			meltdowntimer--;
			std::cout << "MELTDOWN T-: " << meltdowntimer << std::endl << std::endl;
			if (meltdowntimer == 0)
			{
				if (heat >= reactor_temperature_meltdown)
				{
					state = REACTOR_STATE_MELTDOWN;
					reactor_end_times = false;
				}
			}
		}
	}
	if (state == REACTOR_STATE_MELTDOWN)
	{
		std::cout << "YOU DIED THE END!!!" << std::endl;
		GAMESTATE = STOP;
		return;
	}
}

void getavg()
{
	if (state != REACTOR_STATE_RUNNING)
	{
		return;
	}
	roddeps[polls] = (control_rod_integrity - rod_last);
	rod_last = control_rod_integrity;
	roddep_last = roddeps[polls];
	temps[polls] = heat; //poll for power and heat
	powers[polls] = last_power_produced;
	if (polls == polling_count - 1) //reset index so we DONT FUCKING INDEX OUT OF BOUNDS
	{
		polls = 0;
	}
	else
	{
		polls++; //add to how many times we polled already 
	}
	if (total_polls != polling_count) // some nice math for very high poll counts
	{
		total_polls = polls;
	}
	float sumtemp = 0;
	float sumpower = 0;
	float sumdeps = 0;
	float polldif = polling_count - total_polls;
	if (polldif == polling_count)
	{
		polldif = polling_count - 1; //NO DIVIDING BY ZERO!!!!! >:(
	}
	for (int i = 0; i < polling_count - polldif; i++) //math time
	{
		sumdeps += roddeps[i];
		sumtemp += temps[i];
		sumpower += powers[i];
	}
	dep_avg = sumdeps / (polling_count - polldif);
	dep_avg = dep_avg/2; //each process is two seconds so divide by two :)
	temp_avg = sumtemp / (polling_count - polldif);
	power_avg = sumpower / (polling_count - polldif);
	if((-dep_avg) > 0)
	{
		rod_lifetime_max = (100/(-dep_avg));
	}
	else
	{
		rod_lifetime_max = -1;
	}
	if((-dep_avg) > 0)
	{
		rod_lifetime = (control_rod_integrity/(-dep_avg));
	}
	else
	{
		rod_lifetime = -1;
	}
	if(temp_peak < heat)
	{
		temp_peak = heat;
	}
	if(power_peak < last_power_produced)
	{
		power_peak = last_power_produced;
	}
	if(heating_peak < heat_gain)
	{
		heating_peak = heat_gain;
	}
	if(cooling_peak <=(cooling_power * cooling_power_modifier))
	{
		cooling_peak = (cooling_power * cooling_power_modifier);
	}
	if((temperature_delta_max < (heat_gain - (cooling_power * cooling_power_modifier))))
	{
		temperature_delta_max = (heat_gain - (cooling_power * cooling_power_modifier));
	}
	if(temperature_delta_min > (heat_gain - (cooling_power * cooling_power_modifier)))
	{
		temperature_delta_min = (heat_gain - (cooling_power * cooling_power_modifier));
	}
	return;
}
