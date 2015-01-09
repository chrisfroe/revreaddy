/* main.cpp
 *
 * This is a pure C++ test environment for
 * the revreaddy python/cython extension.
 *
 */

#include "Simulation.h"
#include "utils.h"
#include "Trajectory.h"
#include "RadialDistribution.h"

int main()
{
	Simulation * sim = new Simulation();

	sim->kBoltzmann = 1.;
	sim->maxTime	= 10;
	sim->temperature= 1.;
	sim->timestep	= 0.0001;
	sim->isPeriodic = true;
	sim->boxsize = 10.;
	sim->repulsionStrength = 1.;

	std::vector<double> x0 = {0., 0., 0.};
	for (double i=0; i<9; i++)
		for (double j=0; j<9; j++)
			for (double k=0; k<9; k++) {
				x0[0] = -0.49 * sim->boxsize + i;
				x0[1] = -0.49 * sim->boxsize + j;
				x0[2] = -0.49 * sim->boxsize + k;
				sim->addParticle(x0, "lj", 0.5, 1.);
			}
	Trajectory * traj = new Trajectory();
	sim->observables.push_back(traj);

	sim->run();

	std::vector<double> ranges;
	for (double i=0; i<101; i++) {
		ranges.push_back(0.1*i);
	}
	RadialDistribution * rad = new RadialDistribution( ranges );
	rad->isPeriodic = sim->isPeriodic;
	rad->boxsize = sim->boxsize;

	sim->observables.push_back(rad);
	sim->maxTime = 1;

	sim->run();

	traj->writeBufferToFile();
	rad->writeBufferToFile();
	return 0;
}
