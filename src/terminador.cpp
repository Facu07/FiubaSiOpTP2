#include "includes.h"


int main(){
  //sirve para la atribucion de los id
  sv_sem mutex_id("mutex_id");
  //sirve para limitar la cantidad de vehiculos en el ferry
  sv_sem cap_ferry("cap_ferry");
  //sirve para evitar el deadlock
  sv_sem orden("orden");
  //sirve para saber que los vehiculos en el ferry van en la misma direccion
  sv_sem ferry("ferry");
  //sirven para la exclusion mutual de las dos direcciones.
  sv_sem primer_mutex ("primera_mutex");
  sv_sem segundo_mutex ("segundo_mutex");
  mutex_id.del();
  cap_ferry.del();
  sem_ferry.del();
  primer_mutex.del();
  segundo_mutex.del();
  //liberar los recursos de la clase ferry
  //ferry.del();
  return 0;
}
