#ifndef RESERVATION_STATIONS_H_INCLUDED
#define RESERVATION_STATIONS_H_INCLUDED

#include "definitions.h"


typedef struct {
    ASMinstr      *cmd;
    uint_fast32_t cycle_issued;
    uint_fast8_t  stalls_ex;
    uint_fast8_t  stalls_wb;
} printinfo;

typedef struct {
    ASMinstr cmd;
    int cycles_remaining;
    int issuedOnCycle;
    bool isFree;
    bool isWaiting;
    bool isIssuedInTheSameCycle;
    int stalls_before_ex;
    int stalls_before_wb;
    printinfo* printInfo;
} Res_Station;

extern Res_Station * ResStations;

void update_res_stations();
void initializeResStations();
Res_Station* get_free_res_station();
bool there_are_free_res_stations();

#endif // RESERVATION_STATIONS_H_INCLUDED
