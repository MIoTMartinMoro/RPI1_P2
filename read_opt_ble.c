/* FICHERO: read_opt_ble.c
 * DESCRIPCION: codigo para leer temperatura del sensortag */

#include "common.h"

float sensorOpt3001Convert(uint16_t rawData)
{
    uint16_t e, m;
 
    m = rawData & 0x0FFF;
    e = (rawData & 0xF000) >> 12;
 
    /** e on 4 bits stored in a 16 bit unsigned => it can store 2 << (e - 1) with e < 16 */
    e = (e == 0) ? 1 : 2 << (e - 1);
 
    return m * (0.01 * e);
}

int main (int argc, char *argv[])
{
    FILE* fp;
    char* pch;
    char resp[1035];
    char command[200];
    uint16_t rawOpt = 0;
    float data;

    memset(command, '\0', 200);
    memset(resp, '\0', 1035);

    /* Crea el comando que va a ejecutar */
    strcat(command, "/usr/bin/gatttool -b ");
    strcat(command, BLE_MAC);
    strcat(command, " --char-write-req -a ");
    strcat(command, HANDL_OPT_WRITE);
    strcat(command, " -n 01; sleep 1; /usr/bin/gatttool -b ");
    strcat(command, BLE_MAC);
    strcat(command, " --char-read -a ");
    strcat(command, HANDL_OPT_READ);

    printf("%s\n", command);    

    /* Recoge los datos que nos interesa */
    /*do {*/
        fp = popen(command, "r");
        /* Lee la salida de los que escribe el comando */
        while (fgets(resp, sizeof(resp)-1, fp) != NULL) {
        }
        pclose(fp);

        printf("%s\n", resp);

        char* optBytes = strchr(resp, ':') + 2;
        char strRawOpt[5];
        memset(strRawOpt, '\0', 5);

        printf("%s\n", optBytes);

        strRawOpt[0]=optBytes[3];
        strRawOpt[1]=optBytes[4];
        strRawOpt[2]=optBytes[0];
        strRawOpt[3]=optBytes[1];

        printf("%s\n", strRawOpt);

        rawOpt = (uint16_t) strtol(strRawOpt, NULL, 16);

    /*} while(rawOpt == 0);*/

    data = sensorOpt3001Convert(rawOpt);

    printf("%f\n", data);
    return 0;
}