/* FICHERO: read_sensors.c
 * DESCRIPCION: codigo para leer del sensortag */

#include "common.h"
#include "read_tmp_ble.h"

int main (int argc, char *argv[])
{
    float tAmb2;
    float tObj2;
    read_tmp(&tAmb2, &tObj2);
    printf("Amb: %f; Obj: %f\n", tAmb2, tObj2);
}