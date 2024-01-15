#include "gps.h"

const char GGA_header[] = "$GPGGA";
const char RMC_header[] = "$GPRMC";

int convertStringToInt(char *str, int lenght);

void Decode_GPGGA(char *stringRead, GPGGA_Frame_t *frame){
    // printf("=GPGGA=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    // printf("%s :\n\n",stringRead);

    char hour[11];

    if (sscanf(stringRead, "$GPGGA,%10[^,],%lf,%c,%lf,%c,%d,%d,%lf,%lf,%c,",
        hour, &frame->latitude,
        &frame->latitudeDirection,
        &frame->longitude, &frame->longitudeDirection,
        &frame->typeGpsPositionning, &frame->NbVisionSatelites,
        &frame->hdop,
        &frame->altitude, &frame->altitudeMeasuringUnit))
    {
        int _hours = 0, _minuts = 0;
        double _seconds = 0;

        sscanf(hour, "%2d%2d%lf", &_hours, &_minuts, &_seconds);
        double hour_inSecond = (double)(_hours*3600) + (double)(_minuts*60) + _seconds;
        frame->hour = hour_inSecond;
        // printf("Hour: %s\n",hour);
        // printf("Latitude: %.2f, %c\n",frame->latitude, frame->latitudeDirection);
        // printf("Longitude: %.2f, %c\n", frame->longitude, frame->longitudeDirection);
        // printf("Type GPS: %d\n", frame->typeGpsPositionning);
        // printf("Nombre sat: %d\n", frame->NbVisionSatelites);
        // printf("HDOP: %.2f\n", frame->hdop);
        // printf("Altitude : %.2f, in %c\n", frame->altitude, frame->altitudeMeasuringUnit);


    }else {
        // fprintf(stderr, "Reading GGA error.");
    }
    // printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\n\n");
}

void Decode_GPRMC(char *stringRead, GPRMC_Frame_t* trame){
    // printf("=GPRMC=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    // printf("%s :\n\n",stringRead);

    char hour[11];

    trame->declination = 0;
    trame->declination_direction = 0;

    if(sscanf(stringRead, "$GPRMC,%[^,],%c,%lf,%c,%lf,%c,%lf,%lf,%[^,],%lf,%c",
        hour, &trame->validity,
        &trame->latitude, &trame->latitude_direction,
        &trame->longitude, &trame->longitude_direction,
        &trame->speed_knots, &trame->cap,
        trame->date, 
        &trame->declination, &trame->declination_direction))
    {
        int _hours = 0, _minuts = 0, _seconds = 0;

        sscanf(hour, "%2d%2d%2d", &_hours, &_minuts, &_seconds);
        int hour_inSecond = (_hours*3600) + (_minuts*60) + _seconds;
        trame->hour = hour_inSecond;

        // printf("Hour: %s\n",hour);
        // printf("Validity: %c\n",trame->validity);
        // printf("latitude: %lf,%c\n", latitude, trame->latitude_direction);
        // printf("longitude: %lf,%c\n", longitude, trame->longitude_direction);
        // printf("Speed knots: %.1f\n", trame->speed_knots);
        // printf("cap : %.1f\n", trame->cap);
        // printf("Date : %s\n",trame->date);
        // printf("declination: %lf,%c\n", declination, trame->declination_direction);
    }else {
        // fprintf(stderr, "Reading RMC error.");
    }
    // printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\n\n");
}

int AddGgaToJsonArray(json_object **json_array, GPGGA_Frame_t decoded_frame) {

    struct json_object *jsonObjectTrame = (json_object*)malloc(sizeof(void*));
    
    jsonObjectTrame = json_object_new_object();
    if (jsonObjectTrame == NULL) {
        fprintf(stderr, "Erreur de création de l'objet JSON\n");
        return 1;
    }
    
    json_object_object_add(jsonObjectTrame, "type", json_object_new_string(GGA_header));
    json_object_object_add(jsonObjectTrame, "hour", json_object_new_double(decoded_frame.hour));
    json_object_object_add(jsonObjectTrame, "longitude", json_object_new_double(decoded_frame.longitude));
    json_object_object_add(jsonObjectTrame, "longitudeDirection", json_object_new_string_len(&decoded_frame.longitudeDirection,1));
    json_object_object_add(jsonObjectTrame, "latitude", json_object_new_double(decoded_frame.latitude));
    json_object_object_add(jsonObjectTrame, "latitudeDirection", json_object_new_string_len(&decoded_frame.latitudeDirection,1));
    json_object_object_add(jsonObjectTrame, "typeGpsPositionning", json_object_new_int(decoded_frame.typeGpsPositionning));
    json_object_object_add(jsonObjectTrame, "NbVisionSatelites", json_object_new_int(decoded_frame.NbVisionSatelites));
    json_object_object_add(jsonObjectTrame, "hdop", json_object_new_double(decoded_frame.hdop));
    json_object_object_add(jsonObjectTrame, "altitude", json_object_new_double(decoded_frame.altitude));
    json_object_object_add(jsonObjectTrame, "altitudeMeasuringUnit", json_object_new_string_len(&decoded_frame.altitudeMeasuringUnit,1));
    
    json_object_array_add(*json_array, jsonObjectTrame);

    return 0;
}

int AddRmcToJsonArray(json_object **json_array, GPRMC_Frame_t decoded_frame) {
    struct json_object *jsonObjectTrame = (json_object*)malloc(sizeof(void*));
    jsonObjectTrame = json_object_new_object();
    if (jsonObjectTrame == NULL){
        fprintf(stderr, "Erreur de création de l'objet JSON\n");
        return 1;
    }
    json_object_object_add(jsonObjectTrame, "type", json_object_new_string(RMC_header));
    json_object_object_add(jsonObjectTrame, "hour", json_object_new_int(decoded_frame.hour));
    json_object_object_add(jsonObjectTrame, "validity", json_object_new_string_len(&decoded_frame.validity, 1));
    json_object_object_add(jsonObjectTrame, "latitude", json_object_new_double(decoded_frame.latitude));
    json_object_object_add(jsonObjectTrame, "latitude_direction", json_object_new_string_len(&decoded_frame.latitude_direction, 1));
    json_object_object_add(jsonObjectTrame, "longitude", json_object_new_double(decoded_frame.longitude));
    json_object_object_add(jsonObjectTrame, "longitude_direction", json_object_new_string_len(&decoded_frame.longitude_direction, 1));
    json_object_object_add(jsonObjectTrame, "speed_knots", json_object_new_double(decoded_frame.speed_knots));
    json_object_object_add(jsonObjectTrame, "cap", json_object_new_double(decoded_frame.cap));
    json_object_object_add(jsonObjectTrame, "date", json_object_new_string_len(decoded_frame.date, 6));
    
    if (decoded_frame.declination != 0){
        json_object_object_add(jsonObjectTrame, "declination", json_object_new_double(decoded_frame.declination));
    }
    if (decoded_frame.declination_direction != 0){
        json_object_object_add(jsonObjectTrame, "declination_direction", json_object_new_string_len(&decoded_frame.declination_direction,1));
    }
    
    json_object_array_add(*json_array, jsonObjectTrame);

    return 0;
}