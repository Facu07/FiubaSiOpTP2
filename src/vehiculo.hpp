#include "includes.h"

class vehiculo{
public:

  int id;
	const int tiempoDeCruce;
	vehiculo(int id, int peso, int direccion);
	~vehiculo();
	esperarParaCruzar();
	cruzar();
	avisaTerminoDeCruzar();
}
