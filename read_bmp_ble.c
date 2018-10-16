/* FICHERO: read_bmp_ble.c
 * DESCRIPCION: código para leer presión del sensortag */

#include "common.h"

float sensorBmp280Convert(uint32_t rawValue)
{
    return rawValue / 100.0f;
}

float read_bmp (uint8_t value)
{
    FILE* fp;
    char* pch;
    char resp[1035];
    char command[200];
    uint32_t rawPres = 0;
    uint32_t rawTemp = 0;
    float data;

    /* Crea el comando que va a ejecutar */
    memset(command, '\0', 200);
    strcat(command, "/usr/bin/gatttool -b ");
    strcat(command, BLE_MAC);
    strcat(command, " --char-write-req -a ");
    strcat(command, HANDL_BMP_WRITE);
    strcat(command, " -n 01; sleep 1; /usr/bin/gatttool -b ");
    strcat(command, BLE_MAC);
    strcat(command, " --char-read -a ");
    strcat(command, HANDL_BMP_READ);

    /* Recoge los datos que nos interesa */
    do {
        fp = popen(command, "r");
        /* Lee la salida de los que escribe el comando */
        while (fgets(resp, sizeof(resp)-1, fp) != NULL) {
        }
        pclose(fp);

        /* Parsea la salida para quedarse con los bytes que nos interesan (Está en formato Little Endian) */
        char* optBytes = strchr(resp, ':') + 2;
        char strRawPres[7];
        char strRawTemp[7];

        memset(strRawPres, '\0', 7);
        strRawPres[0] = optBytes[15];
        strRawPres[1] = optBytes[16];
        strRawPres[2] = optBytes[12];
        strRawPres[3] = optBytes[13];
        strRawPres[4] = optBytes[9];
        strRawPres[5] = optBytes[10];

        memset(strRawTemp, '\0', 7);
        strRawTemp[0] = optBytes[6];
        strRawTemp[1] = optBytes[7];
        strRawTemp[2] = optBytes[3];
        strRawTemp[3] = optBytes[4];
        strRawTemp[4] = optBytes[0];
        strRawTemp[5] = optBytes[1];

        /* Pasa de string a unsigned int de 32 bits */
        rawPres = (uint32_t) strtol(strRawPres, NULL, 16);
        rawTemp = (uint32_t) strtol(strRawTemp, NULL, 16);

    } while(rawTemp == 0 && rawPres == 0);

    /* Convierte los datos y pasa el que se le pida */
    if (value == BMP_VALUE) {
        data = sensorBmp280Convert(rawPres);
    } else if (value == AMB_TMP_VALUE) {
        data = sensorBmp280Convert(rawTemp);
    }

    return data;  
}