#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include "lines.h"

 int readLine ( int fd, void *buffer, int n )
 {
     int numRead;  /* num of bytes fetched by last read() */
     int totRead;   /* total bytes read so far */
     char *buf;
     char ch;

     if (n <= 0 || buffer == NULL) {
         errno = EINVAL;
         return -1;
     }

     buf = buffer;
     totRead = 0;
     while (1)
     {
         numRead = read(fd, &ch, 1);     /* read a byte */

         if (numRead == -1) {
              if (errno == EINTR)        /* interrupted -> restart read() */
                  continue;
         else return -1;                 /* some other error */
         } else if (numRead == 0) {      /* EOF */
                 if (totRead == 0)       /* no bytes read; return 0 */
                      return 0;
                 else break;
         } else {                        /* numRead must be 1 if we get here*/
                 if (ch == '\n') break;
                 if (ch == '\0') break;
                 if (totRead < n - 1) {   /* discard > (n-1) bytes */
                      totRead++;
                     *buf++ = ch;
                 }
         }
     }

     *buf = '\0';
     return totRead;
 }


int sendMessage ( int socket, char * buffer, int len )
{
    int r;
    int l = len;

    do
    {
         r = write(socket, buffer, l);
         if (r < 0) {
             return (-1);   /* fail */
         }
         l = l - r;
         buffer = buffer + r;

    } while ((l>0) && (r>=0));

    return 0;
}

int writeLine ( int socket, char * buffer )
{
    return sendMessage(socket, buffer, strlen(buffer)+1);
}




int read_int ( int socket, int *number )
{
   char buffer[1024];
   char *endptr ;

   readLine(socket, buffer, 1024) ;
   (*number) = strtol(buffer, &endptr, 10) ; // n = atoi(buffer) ;
   if (endptr[0] != '\0') {
       printf("Error: %s no es un número en base %d\n", buffer, 10) ;
       return -1 ;
   }

   return 0 ;
}

int write_int ( int socket, int number )
{
    char buffer[1024];

    sprintf(buffer, "%d", number);
    sendMessage(socket, buffer, strlen(buffer)+1);
    return 0 ;
}