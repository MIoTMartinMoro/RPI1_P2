/* FICHERO: read_sensors.c
 * DESCRIPCION: codigo para leer de todos los sensores del sensortag */

#include "common.h"

int main (int argc, char *argv[]){
    float tAmb;
    float tObj;
    float sentT;
    float hum;
    float opt;
    float bmp;

    bmp = read_bmp(BMP_VALUE);
    tAmb = read_bmp(AMB_TMP_VALUE);
    printf("Presión: %fhPas\n", bmp);
    printf("Temperatura ambiente (Sensor presión): %fºC\n\n", tAmb);

    tAmb = read_tmp(AMB_TMP_VALUE);
    tObj = read_tmp(OBJ_TMP_VALUE);
    printf("Temperatura ambiente (Sensor temperatura): %fºC\n", tAmb);
    printf("Temperatura sensor: %fºC\n\n", tObj);

    tAmb = read_hum(AMB_TMP_VALUE);
    sentT = read_hum(SEN_TMP_VALUE);
    hum = read_hum(HUM_VALUE);
    printf("Temperatura ambiente (Sensor humedad): %fºC\n", tAmb);
    printf("Sensación térmica: %fºC\n", sentT);
    printf("Humedad: %f%%\n\n", hum);

    opt = read_opt();
    printf("Luminosidad: %fLux\n", opt);
}