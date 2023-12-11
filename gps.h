#if !defined(__GPS_H__)
#define __GPS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "json-c/json.h"

typedef struct trameGPGGA_t{
    char type;
    double hour; // en seconde
    double longitude;
    char longitudeDirection;
    double latitude;
    char latitudeDirection;
    int typeGpsPositionning;
    int NbVisionSatelites;
    float hdop;
    double altitude;
    char altitudeMeasuringUnit;
}trameGPGGA_t;

trameGPGGA_t *Decode_GPGGA(char *stringRead);
void Decode_GPRMC(char *stringRead);

#endif // __GPS_H__
