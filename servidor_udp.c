/* FICHERO: servidor_udp.c
 * DESCRIPCION: codigo del servidor con sockets datagrama */

#include "common.h"
#include <time.h>

#define PUERTO_LOCAL PUERTO  /* puerto local en el servidor por el que se reciben los mensajes */

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
        int cont;
        size_t sin_size;
        int puerto;                     /* puerto local en el servidor por el que se reciben los mensajes */


        if (argc != 2)
        {
                fprintf (stderr, "uso: puerto\n");
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
                /* comprueba el número de bytes recibidos */
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
                case AMB_TMP_VALUE: /* temperatura ambiente */
                        resultado.op = OP_RESULTADO; /* op */
                        float tAmb = read_tmp(AMB_TMP_VALUE);
                        gcvt(tAmb, 10, resultado.data);
                        resultado.len = strlen(resultado.data); /* len */
                        break;
                case BMP_VALUE: /* mayusculas */
                        printf("Entra en presion \n");
                        resultado.op = OP_RESULTADO; /* op */
                        float bmp = read_bmp(BMP_VALUE);
                        gcvt(bmp, 10, resultado.data);
                        resultado.len = strlen(resultado.data); /* len */
                        break;
                case HUM_VALUE:
                        resultado.op=OP_RESULTADO;
                        float hum = read_hum(HUM_VALUE);
                        gcvt(hum, 10, resultado.data);
                        resultado.len = strlen(resultado.data); /* len */
                        break;
                case OBJ_TMP_VALUE:
                        resultado.op=OP_RESULTADO;
                        float tObj = read_tmp(OBJ_TMP_VALUE);
                        gcvt(tObj, 10, resultado.data);
                        resultado.len = strlen(resultado.data); /* len */
                        break;
                case OPT_TMP_VALUE:
                        resultado.op=OP_RESULTADO;
                        float opt = read_opt();
                        gcvt(opt, 10, resultado.data);
                        resultado.len = strlen(resultado.data); /* len */
                        break;
                case SEN_TMP_VALUE:
                        resultado.op=OP_RESULTADO;
                        float sentT = read_hum(SEN_TMP_VALUE);
                        gcvt(sentT, 10, resultado.data);
                        resultado.len = strlen(resultado.data); /* len */
                        break;
                default: /* operacion desconocida */
                        resultado.op = OP_ERROR; /* op */
                        strcpy(resultado.data, "Operacion desconocida");  /* data */
                        resultado.len = strlen (resultado.data);  /* len */
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
        }

        /* cierra socket (no se ejecuta nunca) */
        close (sockfd);

        return 0;
}
