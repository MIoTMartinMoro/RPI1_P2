/* FICHERO: read_sensors.c
 * DESCRIPCION: codigo para leer del sensortag */

#include "common.h"
#include "read_opt_ble.h"
#include "read_tmp_ble.h"

int main (int argc, char *argv[])
{
    float tAmb;
    float tObj;
    float dataOpt;
    read_tmp(&tAmb, &tObj);
    dataOpt = read_opt();
    printf("Amb: %f; Obj: %f\n", tAmb, tObj);
    printf("Opt: %f\n", dataOpt);
}