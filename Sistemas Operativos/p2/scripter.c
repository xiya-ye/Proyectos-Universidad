#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

/* CONST VARS */
const int max_line = 1024;
#define max_commands 10
#define max_redirections 3 //stdin, stdout, stderr
#define max_args 15

/* VARS TO BE USED FOR THE STUDENTS */
char * argvv[max_args];
char * filev[max_redirections];
int background = 0; 
char *comandos[max_commands];

/**
 * This function splits a char* line into different tokens based on a given character
 * @return Number of tokens 
 */
int tokenizar_linea(char *linea, char *delim, char *tokens[], int max_tokens) {
    /* divide una linea de texto (linea) en partes segun el delimitador (delim) y lo guarda en tokens, y el num max de tokens que puede almacenar es max_tokens */
    int i = 0;
    char *token = strtok(linea, delim); /* separa la cadena */
    while (token != NULL && i < max_tokens - 1) {
        tokens[i++] = token;
        token = strtok(NULL, delim);
    }
    tokens[i] = NULL;
    return i;  /* devuelve el numero de tokens generados */
}

/**
 * This function processes the command line to evaluate if there are redirections. 
 * If any redirection is detected, the destination file is indicated in filev[i] array.
 * filev[0] for STDIN
 * filev[1] for STDOUT
 * filev[2] for STDERR
 */
void procesar_redirecciones(char *args[]) {
    //initialization for every command
    filev[0] = NULL;
    filev[1] = NULL;
    filev[2] = NULL;
    //Store the pointer to the filename if needed.
    //args[i] set to NULL once redirection is processed


    for (int i = 0; args[i] != NULL; i++) 
    {
        
        if (strcmp(args[i], "<") == 0) 
        {
            filev[0] = args[i+1];
            /* eliminamos la redirección y su argumento */
            for (int j = i; args[j] != NULL; j++) 
            {
                args[j] = args[j+2];
	    }
	    i--; /* revisamos de nuevo la posicion de i */
        }
        else if (strcmp(args[i], ">") == 0) 
        {
            filev[1] = args[i+1];
            for (int j = i; args[j] != NULL; j++) 
            {
                args[j] = args[j+2];
            }
            i--;
        }
        else if (strcmp(args[i], "!>") == 0) 
        {
            filev[2] = args[i+1];
            for (int j = i; args[j] != NULL; j++) 
            {
                args[j] = args[j+2];
            }
            i--;
        }
    }
}

/**
 * This function processes the input command line and returns in global variables: 
 * argvv -- command an args as argv 
 * filev -- files for redirections. NULL value means no redirection. 
 * background -- 0 means foreground; 1 background.
 */
int procesar_linea(char *linea) {
    /* procesa una liena de comando (linea), divide la linea en comandos individuales (delimitados por | para pipes) 
    	# si el ultimo comando tiene un & al final, se ejecuta en segundo plano (background=1)
    	# llama a procesar_redirecciones para redirecciones de entrada, salida, error
    	# imprime info sobre el comando, los args, las redirecciones y si es 2º plano*/

    int num_comandos = tokenizar_linea(linea, "|", comandos, max_commands);

    //Check if background is indicated
    if (strchr(comandos[num_comandos - 1], '&')) {
        background = 1;
        char *pos = strchr(comandos[num_comandos - 1], '&'); 
        //remove character 
        *pos = '\0';
    }

    //Finish processing
    for (int i = 0; i < num_comandos; i++) {
        int args_count = tokenizar_linea(comandos[i], "\t\n", argvv, max_args);
        procesar_redirecciones(argvv);
    }

    return num_comandos; /* devuelve numero de comandos (dividido por pipes o 1 si no hay pipes) */
}


void capturaSenal (int s) {
	while ((waitpid(-1, NULL, WNOHANG)) > 0);
}



int main (int argc, char *argv[]) {

	/* declaramos variables */
	char primera_linea[] = "## Script de SSOO\n";
    	char lineas_leidos[2048];
    	int fd, leidos, i=0;
    	int num_comandos;
    	int pid, status, pd[2];
    	int p10; /* var para pipes */
    	int fd_ent, fd_sal, fd_err; /* var para ficheros de redirecciones */
    	
    	struct sigaction sa;
    	sa.sa_handler = capturaSenal;
    	sa.sa_flags = 0;
    	sigemptyset (&(sa.sa_mask));
    	sigaction (SIGCHLD, &sa, NULL); 	
	
    	if (argc != 2) 
   	{
		printf("Usage: %s <ruta_fichero>\n", argv[0]);
    		exit(-1);
  	}
  	
	/* COMPROBAR QUE FICHERO EMPIEZA POR "## Script de SSOO" */
	/* abrimos el fichero de comandos */
    	fd = open(argv[1], O_RDONLY, 0644);
    	if (fd < 0) 
    	{
    		perror ("open:");
    		exit(-1);
    	}
  
    
    	/* leemos la primera linea */
    	leidos = read(fd, lineas_leidos, strlen(primera_linea));
    	lineas_leidos[leidos] = '\0';
    	
    	/* error al leer fichero */
    	if (leidos < 0) 
    	{
    		perror ("read:");
    		close(fd);
    		exit(-1);
    	}
    

	/* comparamos si la primera linea coincide */
   	if (strcmp(primera_linea, lineas_leidos) != 0) 
   	{
    		printf("ERROR: fichero debe empezar por ## Script de SSOO\n");
    		close(fd);
    		exit(-1);
    	}
	
	
	/* LEEMOS LOS COMANDOS */
	/* vamos leyendo linea por linea */
	while ((leidos = read(fd, &lineas_leidos[i], 1)) > 0) /* leemos por caracter */
	{
		
		
		
		if (lineas_leidos[i] == '\n')
		{
			lineas_leidos[i] = '\0';
			i = 0;
			
			/* comprobamos si la linea leida es una linea en blanco */
			if (strlen(lineas_leidos) == 0) {
				printf("linea en blanco\n");
				close(fd);
				exit(-1);
			}
			
			
			
			/* procesamos la linea */
			num_comandos = procesar_linea (lineas_leidos);
			
			if (num_comandos == 1)
			{
				/* comandos simples */
				pid = fork();
				switch (pid)
				{
					case -1:	/* error */
						perror("fork: ");
						close(fd);
		            			exit(-1);
		            			
					case 0:	/* hijo: ejecuta comando */
						/* comprobamos si hay redirecciones */
                    	 			tokenizar_linea(comandos[0], " \t\n", argvv, max_args);
						procesar_redirecciones(argvv);
                    	 			
                        			if (filev[0] != NULL) 
                        			{    /* comando con redireccion de entrada */
		                    			fd_ent = open(filev[0], O_RDONLY, 0644);
		                    			if (fd_ent < 0) 
		                    			{
		                        			perror("open redireccion entrada:");
		                        			exit(-1);
		                    			}
		                    			close(STDIN_FILENO);
		                    			dup(fd_ent);
		                    			close(fd_ent);
                        			}
                        			if (filev[1] != NULL) 
                        			{    /* comando con redireccion de salida */
                            				fd_sal = open(filev[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                            				if (fd_sal < 0) 
                            				{
                                				perror("open redireccion salida:");
                                				exit(-1);
                            				}
                            				close(STDOUT_FILENO);
							dup(fd_sal);
							close(fd_sal);
                        			}
                        			if (filev[2] != NULL) 
                        			{    /* comando con redireccion de salida */
                            				fd_err = open(filev[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                            				if (fd_err < 0) 
                            				{
                                			perror("open redireccion errores:");
                                			exit(-1);
                            				}
                            				close(STDERR_FILENO);
                            				dup(fd_err);
                            				close(fd_err);
                        			}
                        			
                        			/* ejecutamos el comando */
                        			execvp(argvv[0], argvv);
                        			perror("execvp:");
                        			close(fd);
                        			exit(-1);
                        			
					default:	/* padre */
						if (background == 0) 
                        			{
                    	     				waitpid(pid, &status, 0);  
                    				}
                    				else {
                    					printf("%d", pid);
                    				}
				}
			}
			
			if (num_comandos > 1) 
			{/* linea con pipes */
				for (int comando=0; comando < num_comandos; comando++)
				{	
					/* creamos pipe cuando no es el último comando */
					if (comando != (num_comandos-1)) 
					{
						if (pipe(pd) < 0) 
						{
							perror("pipe: ");
							close(fd);
							exit(-1);
						}
				       }
				       pid = fork();
				       switch (pid) 
				       {
				       	case -1:	/* error */
				       		perror("fork:");
				       		close(fd);
                                    			exit(-1);
                                    			
				       	case 0:	/* hijo: ejecuta comandos */
				       		tokenizar_linea(comandos[comando], " \t\n", argvv, max_args);
							procesar_redirecciones(argvv);
							
							/* comando con redireccion de entrada cuando es el primero comando */
							if (filev[0] != NULL && comando == 0) {
					    			fd_ent = open(filev[0], O_RDONLY, 0644);
					    			if (fd_ent < 0) 
					    			{
					        			perror("open redireccion entrada:");
					        			exit(-1);
					    			}
					    			close(STDIN_FILENO);
					    			dup(fd_ent);
					    			close(fd_ent);
							}
							
							/* comando con redireccion de salida cuando es el último comando */
				        		if (filev[1] != NULL && comando == (num_comandos-1)) {
				            			fd_sal = open(filev[1], O_WRONLY | O_CREAT |O_TRUNC, 0644);
				            			if (fd_sal < 0) 
				    				{
				        				perror("open redireccion salida:");
				        				exit(-1);
				    				}
				    				close(STDOUT_FILENO);
								dup(fd_sal);
								close(fd_sal);
							}
							
							/* comando con redireccion de errores para todos los comandos */
							if (filev[2] != NULL) {    
				    				fd_err = open(filev[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
				    				if (fd_err < 0) 
				    				{
									perror("open redireccion errores:");
									exit(-1);
				    				}
				    				close(STDERR_FILENO);
				    				dup(fd_err);
				    				close(fd_err);
							}
							
							/* redirecciones + limpieza */
							if (comando != 0) {    		/* entrada */
								close(STDIN_FILENO);
								dup(p10);
								close(p10);
				                    	}
				                    	if (comando != (num_comandos-1)) {	/* salida */
				                        	close(STDOUT_FILENO);
				                        	dup(pd[1]);
				                        	close(pd[0]);
				                        	close(pd[1]);
				                    	}
							
							/* inicializamos los datos */
							filev[0] = NULL;
				                    	filev[1] = NULL;
				                    	filev[2] = NULL;
				                    	
				                    	/* ejecutamos el comando */
				                    	execvp(argvv[0], argvv);
				                    	perror("execvp: ");
				                    	close(fd);
				                    	exit(-1);
							
				       	default:	/* padre */
				       		if (comando != (num_comandos-1))
				       		{ /* salida */
				       			close(pd[1]);
				       			p10 = pd[0];
				       		}
				       		else {
				       			close(pd[0]);
				       		}
				       		
				       		if (background == 0) 
				       		{
				       			waitpid(pid, &status, 0);
				       		}
				       		else {
				       			printf("%d", pid);
				       		}
				       }
				}
			}
			background = 0;
		}
		else{i++;}
	}
	close(fd);
	return 0;	
}



