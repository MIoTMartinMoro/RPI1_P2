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

int main (int argc, char *argv[])
{
    FILE* fp;
    char* pch;
    char resp[1035];
    char command[2000];
    uint16_t rawTAmb = 0;
    uint16_t rawTObj = 0;
    float tAmb;
    float tObj;

    /* Crea el comando que va a ejecutar */
    strcat(command, "/usr/bin/gatttool -b ");
    strcat(command, BLE_MAC);
    strcat(command, " --char-write-req -a ");
    strcat(command, HANDL_TMP_WRITE);
    strcat(command, " -n 01; /usr/bin/gatttool -b ");
    strcat(command, BLE_MAC);
    strcat(command, " --char-read -a ");
    strcat(command, HANDL_TMP_READ);

    fp = popen(command, "r");

    /* Lee la salida de los que escribe el comando */
    while (fgets(resp, sizeof(resp)-1, fp) != NULL) {
    }
    pclose(fp);

    /* Recoge los datos que nos interesa */
    do {
        char* tempBytes = strchr(resp, ':') + 2;
        printf("%s\n", tempBytes);
        char strRawTAmb[6];
        char strRawTObj[6];
        memcpy(strRawTAmb, &tempBytes[0], 5);
        memcpy(strRawTObj, &tempBytes[6], 5);
        strRawTAmb[5] = '\0';
        strRawTObj[5] = '\0';
        printf("AMb%s\n", strRawTAmb);
        printf("OBJ%s\n", strRawTObj);
        rawTAmb = 3;
        rawTObj = 3;
    } while(rawTAmb == 0 && rawTObj == 0);


    return 0;
    /*sensorTmp007Convert();*/
}