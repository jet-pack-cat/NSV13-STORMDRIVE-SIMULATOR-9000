#include <iostream>
#include <stdbool.h>
#include <windows.h>
#include <cmath>
#include <algorithm>
#include <cstdio>
#include <string>
//Gas Interactions
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
double air1[13] = { 21,21,0,0,0,0,0,0,0,0,21,0,0 };
double reaction_chamber_gases[13] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 };
double ratio[13] = { 1,1,0,0,0,0,0,0,0,0,1,0,0 };
double molcount = 63;

//Reactor variables

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

#define INITALIZE 0
#define RUNNING 1
#define STOP 2

int GAMESTATE = 0;
//variables
double start_threshold = 20;
double heat = 0;
double target_heat = 200;
double cooling_power = 10;
double cooling_power_modifier = 1;
double control_rod_percent = 8;
double control_rod_integrity = 0;
double control_rod_modifier = 0;
double control_rod_degradation_modifier = 1;
bool control_rod_installation = false;
double control_rods_i[5] =
{
{ 100 }, //integrity
{ 0 },
{ 0 },
{ 0 },
{ 0 },
};
int control_rods_t[5] =
{
{ 1 }, //type
{ 0 },
{ 0 },
{ 0 },
{ 0 },
};
int control_rods_l = 0;
double control_rod_integrity_total = 0;

#define ROD_NONE 0
#define ROD_NORMAL 1
#define ROD_UPGRADE 2
#define ROD_IRRADIATED 3
#define ROD_WEAK 4
#define ROD_PLASMA 5

double heat_gain = 5;
double heat_gaini = 5;
double heat_gain_modifier = 1;
int warning_state = WARNING_STATE_NONE;
double reaction_rate = 0;
double target_reaction_rate = 0;
double delta_reaction_rate = 0;
double reaction_rate_modifier = 1;
double power_loss = 2;
double input_power = 0;
double input_power_modifier = 1;
int state = REACTOR_STATE_IDLE;
double rod_integrity = 100;
double alert_cooldown = 20;
double last_power_produced = 0;
double theoretical_maximum_power = 15000000;
double radiation_modifer = 1;
double reactor_temperature_nominal = 200;
double reactor_temperature_hot = 400;
double reactor_temperature_critical = 650;
double reactor_temperature_meltdown = 800;
double reactor_temperature_modifier = 1;
double reactor_temperature_nominal_i = 200;
double reactor_temperature_hot_i = 400;
double reactor_temperature_critical_i = 650;
double reactor_temperature_meltdown_i = 800;
double reactor_stability = 100;
double base_power = 67500;
double power_produced = 0;
bool reactor_end_times = false;
double nucleium_power_reduction = 0;
double chamber_ror_total = 0;
double chamber_ipm_total = 0;
double chamber_cooling_total = 0;
double chamber_radiation_total = 0;
double chamber_reinforcement_total = 0;
double chamber_degradation_total = 0;
double radiation = 0;
double control_rod_effectiveness_total = 0;
double rod_effectiveness = 0;
double fuel_check = 0;
double total_moles = 0;
void try_start();
void shoot();
void deactivate();
void handle_reaction_rate();
void handle_heat();
void handle_temperature_reinforcement();
void handle_reactor_stability();
void handle_control_rod_efficiency();
void handle_control_rod_integrity();
void initalize();
void process();
void commandio();
void display();
int speed = 1000;
void handle_overload();
int meltdowntimer = 19;
int main()
{

	initalize();

	while (GAMESTATE == RUNNING)
	{
		commandio();
		Sleep(speed);
		display();
		process();
	}
	std::cout << "process halted";
	return 0;
}
void initalize()
{
	std::cout
		<< "Welcome to the Stormdrive Simulator 9000" << std::endl
		<< "This is designed to be an accurate stormdrive simulation." << std::endl
		<< "But without the annoyances of atmospherics or crew" << std::endl
		<< "starts out with a 1:1:1 n2:o2:cnp mix at 8 rod percent by default" << std::endl;
	std::cout << "PRESS ESCAPE TO INPUT COMMANDS, command help for help" << std::endl;
	while (GAMESTATE == INITALIZE)
	{
		commandio();
	}
	handle_control_rod_efficiency();
	return;
}

void commandio()
{
	std::string command;


	if (GetAsyncKeyState(VK_ESCAPE))
	{
		int loop = 1;
		while (loop == 1)
		{
			std::cout << "COMMAND: ";
			std::cin >> command;
			if (command == "help")
			{
				std::cout << std::endl << "help: Displays this menu"
					<< std::endl << "go: ends initalization"
					<< std::endl << "stop: Ends Program"
					<< std::endl << "------------------"
					<< std::endl << "fueledit: lets you edit fuel ratio"
					<< std::endl << "rodedit: lets you edit control rods"
					<< std::endl << "rp: Rod insertion percent"
					<< std::endl << "fire: shoots level 3 pa blast"
					<< std::endl << "speed: sim speed"
					<< std::endl << "close: close command prompt" << std::endl;
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
				std::cout << std::endl << "speed in ms (default 1000): ";
				std::cin >> speed;
				std::cout << std::endl;
				loop = 0;
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
						double t = 0;
						for (int i = 0; i < 13; i++)
						{
							t += ratio[i];
						}
						t = molcount / t;
						for (int i = 0; i < 13; i++)
						{
							air1[i] = ratio[i] * (t);
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
						double t = 0;
						for (int i = 0; i < 13; i++)
						{
							t += ratio[i];
						}
						t = molcount / t;
						for (int i = 0; i < 13; i++)
						{
							air1[i] = ratio[i] * (t);
						}
						for (int i = 0; i < 13; i++)
						{
							switch (i)
							{
							case 0:
								std::cout << "GAS_N2: " << ratio[i] << " mols " << air1[i] << std::endl;
								break;
							case 1:
								std::cout << "GAS_O2: " << ratio[i] << " mols " << air1[i] << std::endl;
								break;
							case 2:
								std::cout << "GAS_CO2: " << ratio[i] << " mols " << air1[i] << std::endl;
								break;
							case 3:
								std::cout << "GAS_PLASMA: " << ratio[i] << " mols " << air1[i] << std::endl;
								break;
							case 4:
								std::cout << "GAS_TRITIUM: " << ratio[i] << " mols " << air1[i] << std::endl;
								break;
							case 5:
								std::cout << "GAS_NITROUS: " << ratio[i] << " mols " << air1[i] << std::endl;
								break;
							case 6:
								std::cout << "GAS_PLUOXIUM: " << ratio[i] << " mols " << air1[i] << std::endl;
								break;
							case 7:
								std::cout << "GAS_HYPERNOB: " << ratio[i] << " mols " << air1[i] << std::endl;
								break;
							case 8:
								std::cout << "GAS_STIMULUM: " << ratio[i] << " mols " << air1[i] << std::endl;
								break;
							case 9:
								std::cout << "GAS_BZ: " << ratio[i] << " mols " << air1[i] << std::endl;
								break;
							case 10:
								std::cout << "GAS_CONSTRICTED_PLASMA: " << ratio[i] << " mols " << air1[i]<< std::endl;
								break;
							case 11:
								std::cout << "GAS_H2O: " << ratio[i] << " mols " << air1[i] << std::endl;
								break;
							case 12:
								std::cout << "GAS_NUCLEIUM: " << ratio[i] << " mols " << air1[i] << std::endl;
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
	fuel_check = (
		(air1[GAS_PLASMA] * LOW_ROR) +
		(air1[GAS_CONSTRICTED_PLASMA] * NORMAL_ROR) +
		(air1[GAS_TRITIUM] * HIGH_ROR) +
		(air1[GAS_CO2] * HINDER_ROR) +
		(air1[GAS_H2O] * HINDER_ROR) +
		(air1[GAS_HYPERNOB] * REALLY_HINDER_ROR)
		);
	if (fuel_check >= start_threshold && heat >= start_threshold)
	{
		reactor_stability = 100;
		heat = start_threshold + 10;
		state = REACTOR_STATE_RUNNING;
		if (reaction_rate <= 0)
		{
			reaction_rate = 5;
		}
	}
	else {
	}
	return;
}

void process()
{
	if (state >= REACTOR_STATE_REPAIR)
	{
		return;
	}
	
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

	nucleium_power_reduction = 0;
	total_moles = 0;
	for (int i = 0; i < 13; i++)
	{
		total_moles += air1[i];
	}

	double fuel_check = ((air1[GAS_PLASMA] + air1[GAS_CONSTRICTED_PLASMA] + air1[GAS_TRITIUM]) / total_moles) * 100;
	if (total_moles >= reaction_rate && fuel_check >= 12.5)
	{
		for (int i = 0; i < 13; i++)
		{
			reaction_chamber_gases[i] = (air1[i]/total_moles) * reaction_rate;
		}
		chamber_ror_total =
			(
				reaction_chamber_gases[GAS_PLASMA] * LOW_ROR +
				reaction_chamber_gases[GAS_CONSTRICTED_PLASMA] * NORMAL_ROR +
				reaction_chamber_gases[GAS_TRITIUM] * HIGH_ROR +
				reaction_chamber_gases[GAS_N2] * HINDER_ROR +
				reaction_chamber_gases[GAS_H2O] * HINDER_ROR +
				reaction_chamber_gases[GAS_HYPERNOB] * REALLY_HINDER_ROR
			);
		reaction_rate_modifier = chamber_ror_total / reaction_rate;

		chamber_ipm_total = reaction_rate +
			(
				reaction_chamber_gases[GAS_TRITIUM] * HIGH_IPM +
				reaction_chamber_gases[GAS_O2] * HIGH_IPM +
				reaction_chamber_gases[GAS_PLUOXIUM] * HIGH_IPM +
				reaction_chamber_gases[GAS_STIMULUM] * VERY_HIGH_IPM -
				reaction_chamber_gases[GAS_PLASMA] * MEDIOCRE_IPM -
				reaction_chamber_gases[GAS_CO2] * LOW_IPM -
				reaction_chamber_gases[GAS_HYPERNOB] * LOW_IPM
			);
		input_power_modifier = chamber_ipm_total / reaction_rate;

		chamber_cooling_total = reaction_rate +
			(
				reaction_chamber_gases[GAS_HYPERNOB] * VERY_HIGH_COOLING +
				reaction_chamber_gases[GAS_N2] * HIGH_COOLING +
				reaction_chamber_gases[GAS_CO2] * HIGH_COOLING -
				reaction_chamber_gases[GAS_TRITIUM] * LOW_COOLING -
				reaction_chamber_gases[GAS_NUCLEIUM] * LOW_COOLING -
				reaction_chamber_gases[GAS_STIMULUM] * LOW_COOLING
			);
		cooling_power_modifier = chamber_cooling_total / reaction_rate;

		chamber_radiation_total = reaction_rate +
			(
				reaction_chamber_gases[GAS_PLUOXIUM] * HIGH_RADIATION +
				reaction_chamber_gases[GAS_TRITIUM] * HIGH_RADIATION -
				reaction_chamber_gases[GAS_BZ] * LOW_RADIATION 
			);
		radiation_modifer = chamber_radiation_total / reaction_rate;

		chamber_reinforcement_total = reaction_rate +
			(
				reaction_chamber_gases[GAS_PLUOXIUM] * VERY_HIGH_REINFORCEMENT +
				reaction_chamber_gases[GAS_TRITIUM] * HIGH_REINFORCEMENT +
				reaction_chamber_gases[GAS_NITROUS] * HIGH_REINFORCEMENT -
				reaction_chamber_gases[GAS_NUCLEIUM] * LOW_REINFORCEMENT -
				reaction_chamber_gases[GAS_STIMULUM] * LOW_REINFORCEMENT -
				reaction_chamber_gases[GAS_BZ] * LOW_REINFORCEMENT
			);
		reactor_temperature_modifier = chamber_reinforcement_total / reaction_rate;

		chamber_degradation_total = reaction_rate +
			(
				reaction_chamber_gases[GAS_PLASMA] * HIGH_DEG_PROTECTION +
				reaction_chamber_gases[GAS_NITROUS] * HIGH_DEG_PROTECTION +
				reaction_chamber_gases[GAS_HYPERNOB] * HIGH_DEG_PROTECTION +
				reaction_chamber_gases[GAS_PLUOXIUM] * HIGH_DEG_PROTECTION
			);
		control_rod_degradation_modifier = chamber_degradation_total / reaction_rate;

		nucleium_power_reduction = reaction_chamber_gases[GAS_NUCLEIUM] * 1000;

		heat_gain = heat_gaini + reaction_rate;
		for (int i = 0; i < 13; i++)
		{
			reaction_chamber_gases[i] = 0;
		}

		if (total_moles > ((reaction_rate * 12) + 20))
		{
			std::cout << "Losing stability! Too many input mols! Maximum: " << ((reaction_rate * 12) + 20) << std::endl;
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
			std::cout << "Losing stability! Fuel ratio below 25%!" << std::endl;
			reactor_stability -= 0.01;
		}
	}
	else
	{
		std::cout << "Losing stability! Out of fuel!" << std::endl;
		reactor_stability--;
		heat_gain = -5;
	}

	input_power = (std::pow((heat / 150), 3) * input_power_modifier);
	power_produced = base_power;
	last_power_produced = std::max<double>(0, (power_produced * input_power) - nucleium_power_reduction);
	handle_reaction_rate();
	handle_heat();
	handle_temperature_reinforcement();
	handle_reactor_stability();
	radiation = heat * radiation_modifer;

	if (last_power_produced > 3000000)
	{
		std::cout << "Power Output High!" << std::endl;
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
			std::cout << "Reactor control rods have failed!" << std::endl;
		}
		return false;
	}
	if (control_rod_integrity <= 35 && warning_state <= WARNING_STATE_NONE)
	{
		if (state == REACTOR_STATE_RUNNING)
		{
			std::cout << "Reactor control rods failing!" << std::endl;
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
	control_rod_modifier = control_rod_effectiveness_total / control_rods_l;
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
	target_heat = (-1) + (std::pow(2, (0.1 * ((100 - control_rod_percent) * control_rod_modifier))));
	if (heat > target_heat + ((cooling_power * cooling_power_modifier) - heat_gain))
	{
		if (can_cool())
		{
			if (control_rod_percent > 0)
			{
				heat -= cooling_power * cooling_power_modifier;
			}
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
	double delta_rt_nominal = (reactor_temperature_nominal_i * reactor_temperature_modifier) - reactor_temperature_nominal;
	reactor_temperature_nominal += delta_rt_nominal / 2;
	double delta_rt_hot= (reactor_temperature_hot_i * reactor_temperature_modifier) - reactor_temperature_hot;
	reactor_temperature_hot += delta_rt_hot / 2;
	double delta_rt_critical = (reactor_temperature_critical_i * reactor_temperature_modifier) - reactor_temperature_critical;
	reactor_temperature_critical += delta_rt_critical / 2;
	double delta_rt_meltdown = (reactor_temperature_meltdown_i * reactor_temperature_modifier) - reactor_temperature_meltdown;
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
		std::cout << "Gravitational Pulse Detected!" << std::endl;
		//gravity haha
	}

	if (reactor_stability < 75)
	{
		if (rand() % 101 < ((100 - reactor_stability) / 4))
		{
			if (rand() % 101 < 50)
			{
				std::cout << "Heat Destabilizing!" << std::endl;
				heat += reaction_rate * (rand() % 3 + 5);
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
		std::cout << "ANOMALY WARNING!" << std::endl;
		if (rand() % 101 < 1)
		{
			std::cout << "ANOMALY DETECTED!" << std::endl;
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
		std::cout << "Reactor hunger bar at half!" << std::endl;
	}
	if (reactor_stability <= 25)
	{
		std::cout << "REACTOR NEEDS FLESH!!!" << std::endl;
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
	std::system("cls");
	if (warning_state >= WARNING_STATE_OVERHEAT)
	{
		if (heat <= reactor_temperature_critical)
		{
			warning_state = 0;
			std::cout << "Nuclear Meltdown Averted" << std::endl << std::endl;
			reactor_end_times = false;
			meltdowntimer = 19;
		}
	}
	if (heat >= reactor_temperature_critical)
	{
		warning_state = WARNING_STATE_OVERHEAT;
		std::cout << "NUCLEAR MELTDOWN IMMINENT!!!" << std::endl << std::endl; 
	}
	if (heat >= reactor_temperature_meltdown)
	{
		if (warning_state < WARNING_STATE_MELTDOWN)
		{
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
			std::cout << "ERROR IN MODULE FISSREAC0 AT ADDRESS 0x12DF. CONTROL RODS HAVE FAILED.IMMEDIATE INTERVENTION REQUIRED." << std::endl;
			reactor_end_times = true;
			meltdowntimer--;
			std::cout << "MELTDOWN T-: " << meltdowntimer << std::endl;
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
		return;
	}
	std::cout << std::endl
		<< "STORMDRIVE 1000 DISPLAY:" << std::endl
		<< "" << std::endl
		<< "Temperature: " << heat << "C" << std::endl
		<< "Temperature Nominal: " << reactor_temperature_nominal << "C" << std::endl
		<< "Temperature Hot: " << reactor_temperature_hot << "C" << std::endl
		<< "Temperature Critical: " << reactor_temperature_critical << "C" << std::endl
		<< "Temperature Meltdown:" << reactor_temperature_meltdown << "C" << std::endl
		<< "Power Output: " << last_power_produced << "W " << last_power_produced / 1000000 << "MW" << std::endl
		<< "Rod Insertion: " << control_rod_percent << "%" << std::endl
		<< "Rod Depletion (avg): " << control_rod_integrity << "%" << std::endl
		<< "Reaction Rate: " << reaction_rate << "mol/s" << std::endl
		<< "Stability: " << reactor_stability << "%" << std::endl
		<< "Fuel Mols: " << total_moles << std::endl
		<< "Radiation: " << radiation << std::endl;
	return;
}

void handle_overload()
{
	if (last_power_produced >= 3000000 && last_power_produced <= 8000000)
	{
		std::cout << "Annoying light flicker!" << std::endl;
	}
	if (last_power_produced >= 8000000 && last_power_produced <= 12000000)
	{
		std::cout << "Annoying light flicker!" << std::endl;
		std::cout << "Annoying light burnout!" << std::endl;
		std::cout << "Telsa Zap Warning!" << std::endl;
		std::cout << "Stability Decreasing Due to Power!" << std::endl;
		if (rand() % 101 < 0.02) {
			reactor_stability -= std::rand() % 5 + 5;
		}
	}
	if (last_power_produced >= 12000000)
	{
		std::cout << "Annoying light flicker!" << std::endl;
		std::cout << "Annoying light burnout!" << std::endl;
		std::cout << "Telsa Zap Warning!" << std::endl;
		std::cout << "Stability Decreasing Due to Power!" << std::endl;
		if (rand() % 101 < 0.2) {
			reactor_stability -= std::rand() % 10 + 10;
		}
	}
	return;
}