#ifndef RESERVATION_STATIONS_C_INCLUDED
#define RESERVATION_STATIONS_C_INCLUDED

#include "definitions.h"

Res_Station * ResStations;
static int last_given_resStation = 0;

void initializeResStations()
{
    for(int i=0; i<num_of_reservation_stations; i++)
    {
        ResStations[i].isFree = true;
        ResStations[i].isWaiting = false;
        ResStations[i].isIssuedInTheSameCycle = false;
    }

}

Res_Station* get_free_res_station()
{
    static int i = 0;
    for(; i<=num_of_reservation_stations; i++)
    {
        if(i == num_of_reservation_stations)
            i = 0;

        if(ResStations[i].isFree == true)
        {
            ResStations[i].isFree = false;
            return &ResStations[i];
        }
    }

    //return NULL;
}


bool there_are_free_res_stations()
{
    for(int i=0; i<num_of_reservation_stations; i++)
        if(ResStations[i].isFree == true)
            return true;
    return false;
}

bool res_station_is_executing()
{
    for(int i=0; i<num_of_reservation_stations; i++)
        if(ResStations[i].isFree == false)
            return true;
    return false;
}

void print_res_station()
{
    for(int i =0; i<num_of_reservation_stations; i++)
        if(ResStations[i].isFree == false)
            fprintf(stderr, "Print the ResStations[i] values.");
}


void update_res_stations()
{
    //For all reservation stations
    for(int i=0; i<num_of_reservation_stations; i++)
    {
        //If it is free then go to the next reservation station
        if(ResStations[i].isFree == true || ResStations[i].isIssuedInTheSameCycle == true)
            continue;
        //If reservation station is not free and not waiting
        ResStations[i].cycles_remaining--;
        if(ResStations[i].cycles_remaining <= 0)
        {
            if(there_are_free_broadcasts())
            {
                //TODO Here start the problems
                broadcast_result(&ResStations[i].cmd.instr, execute_ASMinstr(&ResStations[i].cmd));
                ResStations[i].isFree = true;
                //TODO WTF?
                free_functional_unit(ResStations[i].cmd);
                free_registers(ResStations[i].cmd);
            }
            else
            {
                ResStations[i].stalls_before_wb++;
                ResStations[i].printInfo->stalls_wb++;
            }
        }
    }

    for(int i=0; i<num_of_reservation_stations; i++)
    {
        if(ResStations[i].isWaiting == false || ResStations[i].isIssuedInTheSameCycle == true)
            continue;
        if(there_are_free_registers(ResStations[i].cmd) && there_are_free_functional_units(ResStations[i].cmd))
        {
            get_functional_unit(ResStations[i].cmd);
            acquire_registers(ResStations[i].cmd);
            ResStations[i].isWaiting = true;
        }
        else
        {
            ResStations[i].stalls_before_ex++;
            ResStations[i].printInfo->stalls_ex++;
        }
    }

    for(int i=0; i<num_of_reservation_stations; i++)
    {
        if(ResStations[i].isIssuedInTheSameCycle == true)
            ResStations[i].isIssuedInTheSameCycle = false;
    }
}

#endif // RESERVATION_STATIONS_C_INCLUDED
