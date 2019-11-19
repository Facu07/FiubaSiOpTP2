#include "includes.h"

int main(){
  int * vehiculos_cruzando[2] = {0,0};
  sv_shm scz0("VehiculosCruzando_0");
  vehiculos_cruzando[0] = static_cast <int*>(scz0.map(sizeof(int)));
  sv_shm scz1("VehiculosCruzando_1");
  vehiculos_cruzando[1] = static_cast <int*>(scz1.map(sizeof(int)));
  //sirve para limitar la cantidad de vehiculos en el ferry
  sv_sem cap_ferry("cap_ferry",5);
  //sirve para evitar el deadlock
  sv_sem orden("orden",1);
  //sirve para saber que los vehiculos en el ferry van en la misma direccion
  sv_sem sem_ferry("ferry", 1);
  //sirven para la exclusion mutual de las dos direcciones.
  sv_sem primer_mutex ("primera_mutex", 1);
  sv_sem segundo_mutex ("segundo_mutex", 1);
  primer_mutex.post();
  segundo_mutex.post();
  sem_ferry.post();
  orden.post();
  int n = cap_ferry.getsem();
  for(int i = 0; i < n; i++ ){
    cap_ferry.post;
  }
  return 0;
}
