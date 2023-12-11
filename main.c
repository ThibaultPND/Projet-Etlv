#include "gps.h"

int main(int argc, char *argv[]) {
    system("chcp 65001");
    FILE *trameFile = fopen("res/trames.txt", "r");
    
    if (trameFile == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier txt :");
        exit(EXIT_FAILURE);
    }
    
    FILE *trameJsonFile = fopen("res/trame.json","w");
    if (trameJsonFile == NULL){
        perror("Erreur lors de l'ouverture du fichier json :");
        fclose(trameFile);
        exit(EXIT_FAILURE);
    }
    char trame[128];
    struct 

    trameGPGGA_t *pTrameGPGGA = NULL;
    while (fgets(trame, sizeof(trame), trameFile)){
        trame[strcspn(trame, "\n")] = 0;
        //printf("data : {%s}\n",trame);

        char nameTrame[7];
        strncpy(nameTrame, trame, 6);
        nameTrame[6] = '\0';


        const char trame_GPGGA[] = "$GPGGA";
        const char trame_GPRMC[] = "$GPRMC";

        if (strcmp(nameTrame, trame_GPGGA) == 0)
        {
            pTrameGPGGA = Decode_GPGGA(trame+6);
            struct json_object *jsonObject = json_object_new_object();
            if (jsonObject == NULL) {
                fprintf(stderr, "Erreur de création de l'objet JSON\n");
                return 1;
            }
            json_object_object_add(jsonObject, "type", json_object_new_string(trame_GPGGA));
            json_object_object_add(jsonObject, "time", json_object_new_double(pTrameGPGGA->hour));
            json_object_object_add(jsonObject, "longitude", json_object_new_double(pTrameGPGGA->longitude));
            json_object_object_add(jsonObject, "longitudeDirection", json_object_new_double(pTrameGPGGA->longitudeDirection));
            json_object_object_add(jsonObject, "latitude", json_object_new_double(pTrameGPGGA->latitude));
            json_object_object_add(jsonObject, "latitudeDirection", json_object_new_double(pTrameGPGGA->latitudeDirection));
            json_object_object_add(jsonObject, "typeGpsPositionning", json_object_new_double(pTrameGPGGA->typeGpsPositionning));
            json_object_object_add(jsonObject, "NbVisionSatelites", json_object_new_double(pTrameGPGGA->NbVisionSatelites));
            json_object_object_add(jsonObject, "hdop", json_object_new_double(pTrameGPGGA->hdop));
            json_object_object_add(jsonObject, "altitude", json_object_new_double(pTrameGPGGA->altitude));
            json_object_object_add(jsonObject, "altitudeMeasuringUnit", json_object_new_double(pTrameGPGGA->altitudeMeasuringUnit));
            fprintf(trameJsonFile, "%s\n", json_object_to_json_string_ext(jsonObject, JSON_C_TO_STRING_PRETTY));
            json_object_put(jsonObject);
            free(pTrameGPGGA);
        }else if (strcmp(nameTrame, trame_GPRMC) == 0){
            Decode_GPRMC(trame);
        }else {
            printf("inconnue");
        }
        
    }
    fclose(trameFile);
    fclose(trameJsonFile);
    return EXIT_SUCCESS;
}