#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

int resolverTarea(char* tarea, float tiempo, int dificultad);
int resolver(int probExito);

int main() {
  srand(time(NULL)); // se necesita para generar randoms diferentes
  char tarea[100], c, buffer[100];
  int dificultad, contador=0, instancia=1, p[2], readbytes, resuelto;
  float tiempo;
  pipe(p);
  printf("Ingrese la tarea a realizar\n");
  scanf("%[^\n]s", tarea);
  printf("La tarea es: %s\n", tarea);
  printf("Desea ingresar la dificultad? (s/n):");
  scanf(" %c", &c);
  if (c == 's'){
    printf("Ingrese la dificultad de la solicitud:\n");
    scanf(" %d", &dificultad);
  } else {
    dificultad = rand() % 101;
  }
  tiempo = ((float)rand()/(float)(RAND_MAX)*5.0);
  printf("The difficulty is %d\n", dificultad);
  printf("The estimated time is %f\n", tiempo);
  if (dificultad > 85){
    if (fork()){
      contador++;
      close(p[1]); // cerrar lado de escritura del pipe
      readbytes = read(p[0], buffer, sizeof(buffer)); // lee la tarea
      close(p[0]);
      printf("Hi I'm Mr Meeseeks! Look at Meeeee. (%d, %d, %d, %d)\n", getpid(), getppid(), contador, instancia);
      printf("Resolving request (%s)\n", buffer);
      sleep(tiempo);
      printf("The request has been solved... It has been a pleasure to assist you, BYE!\n");
    } else {
      close(p[0]); //cerrar lado de lectura del pipe
      write(p[1], tarea, sizeof(tarea)); // escritura de la tarea
      close(p[1]);
    }
    wait(NULL);
  } else if (dificultad > 45 && dificultad <= 85){
    resolverTarea(tarea, tiempo, dificultad);
  } else {

  }
  return 0;
}

int resolverTarea(char* tarea, float tiempo, int dificultad){
  char buffer[100];
  int p[2], readbytes, contador=0, instancia=1;
  pipe(p);
  int p1,p2;
  while (contador < 2){
    p1 = fork();
    contador++;
    if (p1 != 0){
      close(p[0]); //cerrar lado de lectura del pipe
      write(p[1], tarea, sizeof(tarea)); // escritura de la tarea
      close(p[1]);
      p2 = fork();
      if (p2 == 0){
        close(p[1]); // cerrar lado de escritura del pipe
        readbytes = read(p[0], buffer, sizeof(buffer)); // lee la tarea
        close(p[0]);
        printf("Hi I'm Mr Meeseeks! Look at Meeeee. (%d, %d, %d, %d)\n", getpid(), getppid(), contador, instancia+1);
        printf("Resolving request (%s)\n", buffer);
        sleep(tiempo);
      }
    }
    if (p1 != 0 && p2 != 0)
      break;
    if (p1 == 0){
      close(p[1]); // cerrar lado de escritura del pipe
      readbytes = read(p[0], buffer, sizeof(buffer)); // lee la tarea
      close(p[0]);
      printf("Hi I'm Mr Meeseeks! Look at Meeeee. (%d, %d, %d, %d)\n", getpid(), getppid(), contador, instancia);
      printf("Resolving request (%s)\n", buffer);
      sleep(tiempo);
    }
  }
  waitpid(p1, NULL, 0);
  waitpid(p2, NULL, 0);
  return 0;
}

int resolver(int probExito){
  int prob = rand() % 101;
  if (prob <= probExito)
    return 1;
  return 0;
}
