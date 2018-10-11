/* FICHERO: read_ble.c
 * DESCRIPCION: codigo para leer temperatura del sensortag */

#include "common.h"

void sensorTmp007Convert(uint16_t rawAmbTemp, uint16_t rawObjTemp, float* tAmb, float* tObj)
{
    const float SCALE_LSB = 0.03125;
    float t;
    int it;
    it = (int)((rawObjTemp) >> 2);
    t = ((float)(it)) * SCALE_LSB;
    *tObj = t;
    it = (int)((rawAmbTemp) >> 2);
    t = (float)it;
    *tAmb = t * SCALE_LSB;
}

void fahrenheit_to_celsius(float* t_fah)
{
    *t_fah=(*t_fah-32)*5/9;
}

int main (int argc, char *argv[])
{
    FILE* fp;
    char* pch;
    char resp[1035];
    char command[200];
    uint16_t rawTAmb = 0;
    uint16_t rawTObj = 0;
    float tAmb;
    float tObj;

    memset(command, '\0', 200);

    /* Crea el comando que va a ejecutar */
    strcat(command, "/usr/bin/gatttool -b ");
    strcat(command, BLE_MAC);
    strcat(command, " --char-write-req -a ");
    strcat(command, HANDL_TMP_WRITE);
    strcat(command, " -n 01; /usr/bin/gatttool -b ");
    strcat(command, BLE_MAC);
    strcat(command, " --char-read -a ");
    strcat(command, HANDL_TMP_READ);

    printf("%s\n", command);

    

    /* Recoge los datos que nos interesa */
    do {
        fp = popen(command, "r");
        /* Lee la salida de los que escribe el comando */
        while (fgets(resp, sizeof(resp)-1, fp) != NULL) {
        }
        pclose(fp);

        char* tempBytes = strchr(resp, ':') + 2;
        printf("%s\n", tempBytes);
        char strRawTAmb[6];
        char strRawTObj[6];
        memcpy(strRawTAmb, &tempBytes[0], 5);
        memcpy(strRawTObj, &tempBytes[6], 5);
        strRawTAmb[5] = '\0';
        strRawTObj[5] = '\0';
        memmove(&strRawTAmb[2], &strRawTAmb[3], strlen(strRawTAmb)-2);
        memmove(&strRawTObj[2], &strRawTObj[3], strlen(strRawTObj)-2);
        printf("AMb %s\n", strRawTAmb);
        printf("OBJ %s\n", strRawTObj);
        rawTAmb = (uint16_t) strtol(strRawTAmb, NULL, 16);
        rawTObj = (uint16_t) strtol(strRawTObj, NULL, 16);
        printf("%d\n", rawTAmb);
        printf("%d\n", rawTObj);
    } while(rawTAmb == 0 && rawTObj == 0);

    sensorTmp007Convert(rawTAmb, rawTObj, &tAmb, &tObj);

    printf("Amb Bien: %f\n",tAmb);
    printf("Obj Bien: %f\n",tObj);

    fahrenheit_to_celsius(&tAmb);
    fahrenheit_to_celsius(&tObj);

    printf("Amb Conv Bien: %f\n",tAmb);
    printf("Obj Conv Bien: %f\n",tObj);


    return 0;
    
}