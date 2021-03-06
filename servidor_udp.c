/* FICHERO: servidor_udp.c
 * DESCRIPCION: codigo del servidor con sockets datagrama */

#include "common.h"
#include <time.h>

int main (int argc, char* argv[])
{
    int sockfd;
    struct sockaddr_in my_addr;     /* informacion de mi direccion */
    struct sockaddr_in their_addr;  /* informacion de la direccion del cliente */
    char buf[MAXDATASIZE];          /* buffer de recepcion */
    int numbytes;                   /* numero de bytes recibidos o enviados */
    struct idappdata *operation;    /* mensaje de operacion recibido */
    struct idappdata resultado;     /* mensaje de respuesta enviado */
    int error;                      /* indica la existencia de un error */
    size_t sin_size;
    uint16_t puerto;                /* puerto local en el servidor por el que se reciben los mensajes */


    if (argc != 2)
    {
        fprintf (stderr, "uso: servidor puerto\n");
        exit (1);
    }
    puerto = (uint16_t) strtol(argv[1], NULL, 10);

    /* crea el socket */
    if ((sockfd = socket (AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror ("socket");
        exit (1);
    }


    my_addr.sin_family = AF_INET;  /* Familia: ordenacion de bytes de la maquina */
    my_addr.sin_port = htons (puerto);  /* Puerto: ordenacion de bytes de la red */
    my_addr.sin_addr.s_addr = INADDR_ANY;     /* IP: ordenacion de bytes de la red */
    memset (&(my_addr.sin_zero), '\0', 8);  /* Pone a cero el resto de la estructura */

    /* asigna el socket a un puerto local */
    if (bind (sockfd, (void*) &my_addr, sizeof (struct sockaddr_in)) == -1)
    {
        perror ("bind");
        exit (1);
    }

    /* recvfrom() loop... */
    while (1)
    {
        printf ("(servidor) esperando mensajes [puerto local %d]\n",
                ntohs (my_addr.sin_port));

        /* recibe un mensaje de un cliente */
        memset (buf, '\0', MAXDATASIZE); /* Pone a cero el buffer inicialmente */
        sin_size = sizeof(struct sockaddr_in);
        if ((numbytes = recvfrom (sockfd, buf, MAXDATASIZE, 0,
                                  (void*)&their_addr, &sin_size))== -1)
        {
            perror ("recvfrom");
            continue;
        }
        printf("(servidor) mensaje recibido de IP %s puerto %d [longitud %d]\n",
               inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port), numbytes);

        operation = (struct idappdata *) &buf;
        /* comprueba el n�mero de bytes recibidos */
        if ((numbytes < ID_HEADER_LEN) || (numbytes != operation->len + ID_HEADER_LEN))
        {
            printf ("(servidor) unidad de datos recibida de manera incompleta \n");
            continue;
        }
        else
            printf ("(servidor) operacion solicitada [op 0x%x id %d longitud %d contenido %s]\n",
                        operation->op, operation->id, operation->len, operation->data);

        /* realiza operacion solicitada por el cliente */
        error = 0;
        resultado.id = operation->id; /* id */
        switch (operation->op)
        {
        case AMB_TMP_VALUE: ;
            float tAmb = read_tmp(AMB_TMP_VALUE, &resultado.op);
            gcvt(tAmb, 10, resultado.data);
            if (resultado.op == OP_ERROR) strcpy(resultado.data, "Error de lectura");
            resultado.len = strlen(resultado.data);
            break;
        case BMP_VALUE: ;
            float bmp = read_bmp(BMP_VALUE, &resultado.op);
            gcvt(bmp, 10, resultado.data);
            if (resultado.op == OP_ERROR) strcpy(resultado.data, "Error de lectura");
            resultado.len = strlen(resultado.data);
            break;
        case HUM_VALUE: ;
            float hum = read_hum(HUM_VALUE, &resultado.op);
            gcvt(hum, 10, resultado.data);
            if (resultado.op == OP_ERROR) strcpy(resultado.data, "Error de lectura");
            resultado.len = strlen(resultado.data);
            break;
        case OBJ_TMP_VALUE: ;
            float tObj = read_tmp(OBJ_TMP_VALUE, &resultado.op);
            gcvt(tObj, 10, resultado.data);
            if (resultado.op == OP_ERROR) strcpy(resultado.data, "Error de lectura");
            resultado.len = strlen(resultado.data);
            break;
        case OPT_TMP_VALUE: ;
            float opt = read_opt(&resultado.op);
            gcvt(opt, 10, resultado.data);
            if (resultado.op == OP_ERROR) strcpy(resultado.data, "Error de lectura");
            resultado.len = strlen(resultado.data);
            break;
        case SEN_TMP_VALUE: ;
            float sentT = read_hum(SEN_TMP_VALUE, &resultado.op);
            gcvt(sentT, 10, resultado.data);
            if (resultado.op == OP_ERROR) strcpy(resultado.data, "Error de lectura");
            resultado.len = strlen(resultado.data);
            break;
        default: /* operacion desconocida */
            resultado.op = OP_ERROR;
            strcpy(resultado.data, "Operacion desconocida");  /* data */
            resultado.len = strlen (resultado.data);
            error = 1;
            break;
        }

        /* envia resultado de la operacion solicitada por el cliente */
        if ((numbytes = sendto(sockfd, (char *) &resultado,
                               resultado.len + ID_HEADER_LEN, 0,
                               (void*)&their_addr,
                               sizeof(struct sockaddr_in))) == -1)
        {
            perror ("recv");
            continue;
        }
        else
            printf ("(servidor) mensaje enviado al cliente [longitud %d]\n", numbytes);

        printf ("(servidor) resultado de la operacion solicitada "
                "[res 0x%x id %d longitud %d contenido %s]\n",
                resultado.op, resultado.id, resultado.len, resultado.data);

        printf("\n\n");
    }

    /* cierra socket (no se ejecuta nunca) */
    close (sockfd);

    return 0;
}
