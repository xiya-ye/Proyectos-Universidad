#ifndef LINES_H
#define LINES_H

int readLine(int fd, void *buffer, int n);
int sendMessage(int socket, char *buffer, int len);
int writeLine(int socket, char *buffer);
int read_int(int socket, int *number);
int write_int(int socket, int number);

#endif