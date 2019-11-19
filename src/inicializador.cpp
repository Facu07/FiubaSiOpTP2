
#include "includes.h"

void thread_func(int id)
{
  srand (time(NULL));
  int dir = rand() % 2;
  if( dir == 0)
  dir = -1;
  int peso = rand() % 10;
  sv_sem mutex_id("mutex_id");
  mutex_id.wait();
  id++;
  Vehiculo vehiculo(id, peso, dir);
  mutex_id.post();
}

//argv[1] es la cantidad de vehiculos que queremos crear.
int main(int argc, char * argv[])
{
  //imprimo los numeros de padron
  cout << 93644 << "Santiago Aso" << endl;
  cout << 12345 << "Pablo Alejandro Prieto" << endl;
  cout << 96029 << "Facundo Ivan Walter"  << endl;
  cout << 12345 << "Adrian Gustavo Bonino" << endl;
  //recursos compartidos
  //cantidad de vehiculos en cada direccion
  int * vehiculos_cruzando[2] = {0,0};
  sv_shm scz0("VehiculosCruzando_0");
  vehiculos_cruzando[0] = static_cast <int*>(scz0.map(sizeof(int)));
  sv_shm scz1("VehiculosCruzando_1");
  vehiculos_cruzando[1] = static_cast <int*>(scz1.map(sizeof(int)));
  //sirve para la atribucion de los id
  sv_sem mutex_id("mutex_id",1);
  //sirve para limitar la cantidad de vehiculos en el ferry
  sv_sem cap_ferry("cap_ferry",5);
  //sirve para evitar el deadlock
  sv_sem orden("orden",1);
  //sirve para saber que los vehiculos en el ferry van en la misma direccion
  sv_sem sem_ferry("ferry", 1);
  //sirven para la exclusion mutual de las dos direcciones.
  sv_sem primer_mutex ("primera_mutex", 1);
  sv_sem segundo_mutex ("segundo_mutex", 1);
  int end = stoi(argv[1], nullptr, 10);
  int id = 0;
  thread *threads[end];
  int i = 0;
  for (i = 0; i < end; i++)
  {
    mutex_id.wait();
    threads[i] = new thread(thread_func, id);
    mutex_id.post();
  }
  return 0;
}
