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
    char path[1035];
    char* command;
    uint16_t rawTAmb;
    uint16_t rawTObj;
    float tAmb;
    float tObj;

    /* Crea el comando que va a ejecutar */
    strcat(command, "/usr/bin/gatttool -b ");
    strcat(command, BLE_MAC);
    strcat(command, " --char-write-req -a ");
    strcat(command, HANDL_WRITE);
    strcat(command, " -n 01; /usr/bin/gatttool -b ");
    strcat(command, BLE_MAC);
    strcat(command, " --char-read -a ");
    strcat(command, HANDL_READ);

    fp = popen(command, "r");

    /* Lee la salida de los que escribe el comando */
    while (fgets(path, sizeof(path)-1, fp) != NULL) {
        printf("%s\n", path);
    }
    pclose(fp);
    return 0;
    /*sensorTmp007Convert();*/
}