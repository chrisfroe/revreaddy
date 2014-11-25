/* revreaddymodule.cpp
 * author: Christoph Froehner
 *
 * This is the Python C-Extension 'revreaddy':
 * reversible reaction diffusion dynamics simulation
 *
 */

#include <Python.h>
//#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>
#include <stdio.h>
#include "revreaddymodule.h"
#include "Random.h"
#include "Particle.h"
#include "Simulation.h"
#include "SingleParticleDiffusion.h"
#include "utils.h"

static PyObject* revreaddy_start(PyObject * self, PyObject * args)
{
	int lengthOfSeq;
	if (!PyArg_ParseTuple(args,"i", &lengthOfSeq) )
	{
		return NULL;
	}
	
	int nd = 1;
	npy_intp dim[1] = {(npy_intp) lengthOfSeq};
	
	double * cSeq;
	PyArrayObject * pySeq;
	
	pySeq = (PyArrayObject*) PyArray_SimpleNew(nd, dim, NPY_DOUBLE);
	cSeq = pyVectorToCArrayPointer(pySeq);
	
	// ----------- Do stuff here ------------ //

	SingleParticleDiffusion * sim = new SingleParticleDiffusion;
	Particle * p1 = new Particle();
	Particle * p2 = new Particle();
	p1->radius = 1.;
	p2->radius = 1.;
	const std::array<double, 3> x0 = {0., 0., 0.};
	sim->addParticle(p1);
	sim->addParticle(p2);
	sim->activeParticles[0].position = x0;
	sim->activeParticles[1].position = x0;
	sim->initialPosition = x0;

	sim->kBoltzmann = 1.;
	sim->maxTime	= lengthOfSeq;
	sim->temperature= 1.;
	sim->timestep	= 0.001;
	sim->isPeriodic = true;
	sim->boxSize = 5.;
	sim->trajectory.resize(sim->maxTime);
	sim->squaredDistances.resize(sim->maxTime);

	sim->run();

	memcpy(cSeq, sim->squaredDistances.data(), sim->maxTime * sizeof(double));

	
	// --------------------------------------//

    return PyArray_Return(pySeq);
}

double * pyVectorToCArrayPointer(PyArrayObject *arrayin)
{
	return (double *) arrayin->data;  // pointer to arrayin data as double
}

static char revreaddy_docs[] = "start( ): Will start the simulation.\n";

static PyMethodDef revreaddy_funcs[] =
{
    {"start", (PyCFunction)revreaddy_start, METH_VARARGS, revreaddy_docs},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initrevreaddy()
{
    PyObject *m = Py_InitModule3("revreaddy", revreaddy_funcs, "reversible reaction diffusion dynamics simulation");
    if (m == NULL)
    	return;
    
    /* Load numpy functionality */
    import_array();
}
