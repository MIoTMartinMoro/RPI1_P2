/* FICHERO: cliente_udp.c
 * DESCRIPCION: codigo del cliente con sockets datagrama */

#include "common.h"

int main (int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in their_addr;  /* informacion de la direccion del servidor */
    struct sockaddr_in my_addr;     /* informacion de la direccion del cliente */
    char buf[MAXDATASIZE];          /* buffer de recepcion */
    struct idappdata operation;     /* mensaje de operacion enviado */
    struct idappdata *resultado;    /* mensaje de respuesta recibido */
    int numbytes;                   /* numero de bytes recibidos o enviados */
    size_t sin_size;
    int puerto;

    if (argc != 4)
    {
        fprintf (stderr, "uso: cliente sensor hostname puerto\n");
        fprintf (stderr, "sensor: temperatura_ambiente, temperatura_objeto, presion, humedad, luminosidad, sensacion_termica \n");
        exit (1);
    }

    puerto = (uint16_t) strtol(argv[3], NULL, 10);

    /* crea el socket */
    if ((sockfd = socket (AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror ("socket");
        exit (1);
    }

    their_addr.sin_family = AF_INET;  /* Familia: ordenacion de bytes de la maquina */
    their_addr.sin_port =
        htons (puerto);    /* Puerto: ordenacion de bytes de la red */
    their_addr.sin_addr.s_addr =             /* IP: ordenacion de bytes de la red */
        inet_addr (argv[2]);
    memset (&(their_addr.sin_zero), 0, 8);  /* Pone a cero el resto de la estructura */

    my_addr.sin_family = AF_INET;  /* Familia: ordenacion de bytes de la maquina */
    my_addr.sin_port =
        htons (33333);    /* Puerto: ordenacion de bytes de la red */
    my_addr.sin_addr.s_addr = INADDR_ANY;            /* IP: ordenacion de bytes de la red */
        
    bind( sockfd, (void*) &my_addr, sizeof( struct sockaddr_in) );

    memset (buf, '\0', MAXDATASIZE); /* Pone a cero el buffer inicialmente */


    memset (buf, '\0', MAXDATASIZE); /* Pone a cero el buffer inicialmente */

    /* envia mensaje de operacion al servidor */
    if (strcmp(argv[1], "temperatura_ambiente") == 0){
        strcpy(operation.data, "Temperatura ambiente");
        operation.op = AMB_TMP_VALUE;
    } else if (strcmp(argv[1], "Temperatura objeto") == 0){
        strcpy(operation.data, "Esta es Una PRUEBA");
        operation.op = OBJ_TMP_VALUE;
    } else if (strcmp(argv[1], "presion") == 0) {
        strcpy(operation.data, "Presion");
        operation.op = BMP_VALUE;
    } else if (strcmp(argv[1], "humedad") == 0) {
        strcpy(operation.data, "Humedad");
        operation.op = HUM_VALUE; 
    } else if (strcmp(argv[1], "luminosidad") == 0) {
        strcpy(operation.data, "Luminosidad");
        operation.op = OPT_TMP_VALUE;
    } else if (strcmp(argv[1], "sensacion_termica") == 0) {
        strcpy(operation.data, "Sensacion termica");
        operation.op = SEN_TMP_VALUE;
    } else{
        fprintf(stderr, "Ese sensor no esta disponible\n");
        fprintf (stderr, "sensor: temperatura_ambiente, temperatura_objeto, presion, humedad, luminosidad, sensacion_termica \n");
        operation.op = OP_ERROR;   /* op */
    }
    operation.id = 1; /* id */
    operation.len = strlen (operation.data);  /* len */
    if ((numbytes = sendto (sockfd, (char *) &operation,
                            operation.len + ID_HEADER_LEN, 0,
                            (void*)&their_addr,
                            sizeof(struct sockaddr_in))) == -1)
    {
        perror ("send");
        exit (1);
    }
    else
        printf ("(cliente) mensaje enviado al servidor [longitud %d]\n", numbytes);

    printf ("(cliente) operacion solicitada [op 0x%x id %d longitud %d contenido %s]\n",
            operation.op, operation.id, operation.len, operation.data);

    /* espera resultado de la operacion */
    sin_size = sizeof(struct sockaddr_in);
    if ((numbytes = recvfrom (sockfd, buf, MAXDATASIZE, 0 /* flags */,
                              (void*)&their_addr, &sin_size)) == -1)
    {
        perror ("recv");
        exit (1);
    }
    printf ("(cliente) mensaje recibido del servidor [longitud %d]\n", numbytes);


    resultado = (struct idappdata *) &buf;
    /* comprueba el número de bytes recibidos */
    if ((numbytes < ID_HEADER_LEN) || (numbytes != resultado->len + ID_HEADER_LEN))
            printf ("(cliente) unidad de datos recibida de manera incompleta \n");
    else
        if (resultado->id != operation.id)  /* comprueba el identificador del resultado */
            printf ("(cliente) unidad de datos recibida con identificador erroneo \n");
        else
            printf ("(cliente) resultado de la operacion solicitada "
                    "[res 0x%x id %d longitud %d contenido %s]\n",
                    resultado->op, resultado->id, resultado->len, resultado->data);

    /* cierra el socket */
    close (sockfd);

    return 0;
}
