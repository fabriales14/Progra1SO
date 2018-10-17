#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

void crearMrMeeseek();

int main() {
  srand(time(NULL)); // se necesita para generar randoms diferentes
  char tarea[100];
  int dificultad;
  float tiempo;
  char c;
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
    tiempo = ((float)rand()/(float)(RAND_MAX)*5.0);
  }
  printf("%d\n", dificultad);
  printf("%f\n", tiempo);
  crearMrMeeseek();
  wait(NULL);
  return 0;
}

void crearMrMeeseek(){
  if (fork() == 0){
    printf("Hi I'm Mr Meeseeks! Look at Meeeee. (%d, %d, %d, %d)\n", getpid(), getppid(), 1, 1);
  }
}
