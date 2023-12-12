#include "gps.h"

int convertStringToInt(char *str, int lenght);

trameGPGGA_t *Decode_GPGGA(char *stringRead){
    printf("Je décode un GPGGA\n");
    int lenght = strlen(stringRead);
    trameGPGGA_t *pTrame = (trameGPGGA_t*)malloc(sizeof(trameGPGGA_t));
    char sepaparator[] = ",";

    // Time
    char *data = strtok(stringRead, sepaparator);
    int hours, minutes;
    double seconds;
    sscanf(data, "%2d%2d%lf",&hours,&minutes,&seconds);
    pTrame->hour = (double)hours*3600 + (double)minutes*60 + seconds;

    // Lattitude
    data = strtok(NULL, sepaparator);
    int latDegree;
    double  latMinuts;
    sscanf(data, "%2d%lf", &latDegree, &latMinuts);
    pTrame->latitude = latDegree + (latMinuts/60);

    // Latitude dir
    data = strtok(NULL, sepaparator);
    pTrame->latitudeDirection = *data;

    // longitude
    data = strtok(NULL, sepaparator);
    int longDegree;
    double  longMinuts;
    sscanf(data, "%3d%lf", &longDegree, &longMinuts);
    pTrame->longitude = longDegree + (longMinuts/60);

    // longitude dir
    data = strtok(NULL, sepaparator);
    pTrame->longitudeDirection = *data;

    // Type of GPS
    data = strtok(NULL, sepaparator);
    sscanf(data, "%d", &pTrame->typeGpsPositionning);
    
    // Number of satelite in vision
    data = strtok(NULL, sepaparator);
    sscanf(data, "%d", &pTrame->NbVisionSatelites);

    // HDOP
    data = strtok(NULL, sepaparator);
    sscanf(data, "%f",&pTrame->hdop);
    
    // Altitude
    data = strtok(NULL, sepaparator);
    sscanf(data, "%lf", &pTrame->altitude);

    // Altitude dir
    data = strtok(NULL, sepaparator);
    pTrame->altitudeMeasuringUnit = *data;
    
    // EOF
    
    return pTrame;
}

void Decode_GPRMC(char *stringRead){
    printf("Je décode un GPRMC\n");
}