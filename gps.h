#if !defined(__GPS_H__)
#define __GPS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "json-c/json.h"

typedef struct GPGGA_Frame_t{
    char type;

    // Time in seconds
    double hour;

    // Latitude and direction
    double latitude;
    char latitudeDirection;
    
    // Longitude and direction
    double longitude;
    char longitudeDirection;

    // Type of GPS positioning
    int typeGpsPositionning;
    
    // Number of satelit in the area.
    int NbVisionSatelites;
    
    // Horizontal dilution of precision
    double hdop;
    
    // Altitude and direction
    double altitude;
    char altitudeMeasuringUnit;
}GPGGA_Frame_t;

typedef struct GPRMC_Frame_t{
    char type;

    // Time in seconds.
    int hour;

    // (A = OK,  V = Warning).
    char validity;
    
    // Latitude and direction.
    double latitude;
    char latitude_direction;
    
    // Longitude and direction.
    double longitude;
    char longitude_direction;
    
    // Speed in knots.
    double speed_knots;

    // Cap (true).
    double cap;

    // Date (jjmmyy).
    char date[7];

    // Magnetic declination and direction.
    double declination;
    char declination_direction;

}GPRMC_Frame_t;

void Decode_GPGGA(char *stringRead, GPGGA_Frame_t *frame);
void Decode_GPRMC(char *stringRead, GPRMC_Frame_t *frame);

int AddGgaToJsonArray(json_object **json_array, GPGGA_Frame_t decoded_frame);
int AddRmcToJsonArray(json_object **json_array, GPRMC_Frame_t decoded_frame);

extern const char GGA_header[];
extern const char RMC_header[];
#endif // __GPS_H__
