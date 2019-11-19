#include "vehiculo.h

vehiculo::vehiculo(int id, int peso, int direccion){
	this->id = id;
  this->peso = peso;
	this->direccion = direccion;
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
	//acciones del vehiculo
	esperarParaCruzar();
	cruzar();
	avisaTerminoDeCruzar();

	}
sv_sem getSemaforoDireccion(int direccion){
	if (direccion == 0){
		return primer_mutex;
	}else{
		return segundo_mutex;
	}
}
vehiculo::esperarParaCruzar(){
	orden.wait();
	getSemaforoDireccion(this->direccion).wait()
	vehiculos_cruzando[this->direccion]++
	if (vehiculos_cruzando[this->direccion] == 1)
		//soy el primer vehiculo
		ferry.wait();
	getSemaforoDireccion(this->direccion).post();
	orden.post();
	cap_ferry.wait();

}

vehiculo::cruzar(){
	delay(tiempoDeCruce);
}

vehiculo::avisaTerminoDeCruzar(){
	getSemaforoDireccion(this->direccion).wait();
	cap_ferry.post();
	vehiculos_cruzando[this->direccion]--;
	if (vehiculos_cruzando[this->direccion] == 0){
		ferry.post();
	}
	getSemaforoDireccion(this->direccion).post();
}
