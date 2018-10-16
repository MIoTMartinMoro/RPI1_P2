#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef unsigned short int uint16_t;  /* Declara nuevo tipo unsigned int de 16 bits */

#define BLE_MAC_CARLOS      "B0:91:22:F7:38:81"  /* Dirección MAC del SensorTag */
#define BLE_MAC_ALDA        "24:71:89:CC:1D:02"  /* Dirección MAC del SensorTag */
#define BLE_MAC             BLE_MAC_CARLOS

#define HANDL_TMP_WRITE   "0x27"  /* Handle para activar el sensor de temperatura */
#define HANDL_TMP_READ    "0x24"  /* Handle para leer del sensor de temperatura */

#define HANDL_OPT_WRITE   "0x47"  /* Handle para activar el sensor de óptica */
#define HANDL_OPT_READ    "0x44"  /* Handle para leer del sensor de óptica */

#define HANDL_HUM_WRITE   "0x2f"  /* Handle para activar el sensor de humedad */
#define HANDL_HUM_READ    "0x2c"  /* Handle para leer del sensor de humedad */