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
    
    struct json_object *json_array = json_object_new_array();
        
    if (json_array == NULL) {
        fprintf(stderr, "Erreur de création de l'objet JSON\n");
        return 1;
    }

    trameGPGGA_t *pTrameGPGGA = NULL;
    int j = 0;
    while (fgets(trame, sizeof(trame), trameFile)){
        trame[strcspn(trame, "\n")] = 0;

        char nameTrame[7];
        strncpy(nameTrame, trame, 6);
        nameTrame[6] = '\0';

        const char trame_GPGGA[] = "$GPGGA";
        const char trame_GPRMC[] = "$GPRMC";

        if (strcmp(nameTrame, trame_GPGGA) == 0)
        {
            pTrameGPGGA = Decode_GPGGA(trame+6);

            struct json_object *jsonObjectTrame = (json_object*)malloc(8);
            jsonObjectTrame = json_object_new_object();
            
            if (jsonObjectTrame == NULL) {
                fprintf(stderr, "Erreur de création de l'objet JSON\n");
                return 1;
            }
            
            json_object_object_add(jsonObjectTrame, "type", json_object_new_string(trame_GPGGA));
            json_object_object_add(jsonObjectTrame, "time", json_object_new_double(pTrameGPGGA->hour));
            json_object_object_add(jsonObjectTrame, "longitude", json_object_new_double(pTrameGPGGA->longitude));
            json_object_object_add(jsonObjectTrame, "longitudeDirection", json_object_new_string_len(&pTrameGPGGA->longitudeDirection,1));
            json_object_object_add(jsonObjectTrame, "latitude", json_object_new_double(pTrameGPGGA->latitude));
            json_object_object_add(jsonObjectTrame, "latitudeDirection", json_object_new_string_len(&pTrameGPGGA->latitudeDirection,1));
            json_object_object_add(jsonObjectTrame, "typeGpsPositionning", json_object_new_int(pTrameGPGGA->typeGpsPositionning));
            json_object_object_add(jsonObjectTrame, "NbVisionSatelites", json_object_new_int(pTrameGPGGA->NbVisionSatelites));
            json_object_object_add(jsonObjectTrame, "hdop", json_object_new_double(pTrameGPGGA->hdop));
            json_object_object_add(jsonObjectTrame, "altitude", json_object_new_double(pTrameGPGGA->altitude));
            json_object_object_add(jsonObjectTrame, "altitudeMeasuringUnit", json_object_new_string_len(&pTrameGPGGA->altitudeMeasuringUnit,1));
            
            char chaine[20];
            sprintf(chaine, "Chaine %d", j);
            json_object_array_add(json_array, jsonObjectTrame);
            free(pTrameGPGGA);
        }else if (strcmp(nameTrame, trame_GPRMC) == 0){
            Decode_GPRMC(trame);
        }else {
            printf("inconnue");
        }
        j++;
    }
    fprintf(trameJsonFile, "%s\n", json_object_to_json_string_ext(json_array, JSON_C_TO_STRING_PRETTY));
    printf("debug\n");
    fclose(trameFile);
    fclose(trameJsonFile);
    return EXIT_SUCCESS;
}