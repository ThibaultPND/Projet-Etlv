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

    while (fgets(trame, sizeof(trame), trameFile)){
        trame[strcspn(trame, "\n")] = '\0';

        if (strncmp(trame, GGA_header, 6) == 0){
            GPGGA_Frame_t decoded_frame;
            Decode_GPGGA(trame, &decoded_frame);

            if (AddGgaToJsonArray(&json_array, decoded_frame))
                fprintf(stderr, "Erreur d'ajout dans la liste json.");
            
        }else if (strncmp(trame, RMC_header, 6) == 0){
            GPRMC_Frame_t decoded_frame;
            Decode_GPRMC(trame, &decoded_frame);

            if (AddRmcToJsonArray(&json_array, decoded_frame))
            {
                fprintf(stderr, "Erreur d'ajout dans la list json.");
            }
            
        }else {
            printf("inconnue");
        }
    }

    fprintf(trameJsonFile, "%s\n", json_object_to_json_string_ext(json_array, JSON_C_TO_STRING_PRETTY));
    fclose(trameFile);
    fclose(trameJsonFile);

    printf("\n...Fin du programme\n");
    return EXIT_SUCCESS;
}