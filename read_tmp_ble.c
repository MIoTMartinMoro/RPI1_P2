/* FICHERO: read_tmp_ble.c
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

void read_tmp (float* tAmb, float* tObj)
{
    FILE* fp;
    char* pch;
    char resp[1035];
    char command[200];
    uint16_t rawTAmb = 0;
    uint16_t rawTObj = 0;

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
        char strRawTAmb[5];
        char strRawTObj[5];
        memset(strRawTAmb, '\0', 5);
        memset(strRawTObj, '\0', 5);

        printf("%s\n", tempBytes);

        strRawTAmb[0]=tempBytes[9];
        strRawTAmb[1]=tempBytes[10];
        strRawTAmb[2]=tempBytes[6];
        strRawTAmb[3]=tempBytes[7];

        strRawTObj[0]=tempBytes[3];
        strRawTObj[1]=tempBytes[4];
        strRawTObj[2]=tempBytes[0];
        strRawTObj[3]=tempBytes[1];

        rawTAmb = (uint16_t) strtol(strRawTAmb, NULL, 16);
        rawTObj = (uint16_t) strtol(strRawTObj, NULL, 16);
        printf("strAmb %s\n", strRawTAmb);
        printf("%x\n", rawTAmb);
        printf("strObj %s\n", strRawTObj);
        printf("%x\n", rawTObj);
    } while(rawTAmb == 0 && rawTObj == 0);

    sensorTmp007Convert(rawTAmb, rawTObj, tAmb, tObj);

    printf("Amb Conv Bien: %f\n",tAmb);
    printf("Obj Conv Bien: %f\n",tObj);
}