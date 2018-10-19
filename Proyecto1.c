#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

void crearMrMeeseek();

int main() {
  srand(time(NULL)); // se necesita para generar randoms diferentes
  char tarea[100];
  int dificultad;
  int contador = 0;
  int instancia = 1;
  float tiempo;
  char c;
  int p[2], readbytes;
  char buffer[100];
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
  printf("%d\n", dificultad);
  printf("%f\n", tiempo);
  if (dificultad > 85){
    if (fork()){
      contador++;
      close(p[1]); // cerrar lado de escritura del pipe
      readbytes = read(p[0], buffer, sizeof(buffer)); // lee la tarea
      printf("%s\n", buffer);
      close(p[0]);
      printf("Hi I'm Mr Meeseeks! Look at Meeeee. (%d, %d, %d, %d)\n", getpid(), getppid(), contador, instancia);
      printf("Resolving problem... (%d, %d, %d, %d)\n", getpid(), getppid(), contador, instancia);
      sleep(tiempo);
    } else {
      close(p[0]); //cerrar lado de lectura del pipe
      write(p[1], tarea, sizeof(tarea)); // escritura de la tarea
      close(p[1]);
    }
  }
  wait(NULL);
  return 0;
}

void crearMrMeeseek(){
  if (fork() == 0){
    printf("Hi I'm Mr Meeseeks! Look at Meeeee. (%d, %d, %d, %d)\n", getpid(), getppid(), 1, 1);
  }
}
