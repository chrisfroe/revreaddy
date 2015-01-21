/* SimulationTest.h
 * 
 * A lot of unit tests of the Simulation class.
 */

#include <cxxtest/TestSuite.h>
// Simulation.h basically includes every other class used.
#include "Simulation.h"
#include "utils.h"

class SimulationTest : public CxxTest::TestSuite
{
	public:
		void test_addParticle_usualOperation(void)
		{
			Simulation * sim = new Simulation();
			std::vector<double> x1 = {1.,2.,3.};
			std::vector<double> x2 = {-3.,-2.,-1.};
			sim->addParticle(x1, "soft", 2., 3.);
			sim->addParticle(x2, "soft", 4., 5.);
			std::vector<double> x3 = {1.,2.,3.};
			std::vector<double> x4 = {-3.,-2.,-1.};
			TS_ASSERT_EQUALS(sim->activeParticles[0].position, x3);
			TS_ASSERT_EQUALS(sim->activeParticles[1].position, x4);
			TS_ASSERT_EQUALS(sim->activeParticles[0].radius, 2.);
			TS_ASSERT_EQUALS(sim->activeParticles[1].radius, 4.);
			TS_ASSERT_EQUALS(sim->activeParticles[0].diffusionConstant, 3.);
			TS_ASSERT_EQUALS(sim->activeParticles[1].diffusionConstant, 5.);
			delete sim;
		}
		
		/* Add a particle with diffusionConstant = 0. outside the box
		 * at {6.,0.,0.}. Boxsize is 10.
		 * After one propagate() it should be at {-4.,0.,0.}
		 */
		void test_propagate_periodicBoundaryCondition(void)
		{
			Simulation * sim       = new Simulation();
			sim->isPeriodic        = true;
			sim->boxsize           = 10.;
			sim->temperature       = 1.;
			sim->kBoltzmann        = 1.;
			std::vector<double> x0 = {6.,0.,0.};
			sim->addParticle(x0, "soft",  1., 0.);
			sim->propagate();
			std::vector<double> x1 = {-4.,0.,0.};
			TS_ASSERT_EQUALS(sim->activeParticles[0].position, x1);
			delete sim;
		}

		/* Place two particles central in the box so that minimum
		 * distance is euclidean distance.
		 * Also place two particles on the edges of the box so that
		 * minimum distance is the distance "through the wall".
		 */
		void test_getMinDistance_minimumImageConvention(void)
		{
			Simulation * sim         = new Simulation();
			sim->isPeriodic          = true;
			sim->boxsize             = 10.;
			std::vector<double> x1  = {1.,1.,1.};
			std::vector<double> x2  = {-1.,-1.,-1.};
			std::vector<double> r12 = {-2.,-2.,-2.}; 
			std::vector<double> r   = {0.,0.,0.};
			getMinDistanceVector(
				r,
				x1,
				x2,
				sim->isPeriodic,
				sim->boxsize);
			TS_ASSERT_EQUALS(r, r12);
			std::vector<double> x3  = {4.,4.,4.};
			std::vector<double> x4  = {-4.,-4.,-4.};
			std::vector<double> r34 = {2.,2.,2.};
			getMinDistanceVector(
				r,
				x3,
				x4,
				sim->isPeriodic,
				sim->boxsize);
			TS_ASSERT_EQUALS(r, r34);
			delete sim;
		}
		
		/* Given 2 particles, check that the calculated force is correctly
		 * assigned to each particle. One particle is at {1,1,1}, the other
		 * at {2,3,4}. The force constant is k=2 and the particles' radii
		 * are 2. so that the cutoff is s=4. The force is
		 * F = k * (1 - s/R) * r_12, where r_12 is the vector pointing from
		 * particle 1 to particle 2. 
		 */
		void test_calculateRepulsionForces_cumulativeForce(void)
		{
			Simulation * sim = new Simulation();
			sim->isPeriodic = true;
			sim->boxsize = 10.;
			sim->repulsionStrength = 2.;
			std::vector<double> x0 = {1.,1.,1.};
			sim->addParticle(x0, "soft", 2., 0.);
			x0 = {2.,3.,4.};
			sim->addParticle(x0, "soft", 2., 0.);
			//printArray(sim->activeParticles[0].cumulativeForce);
			sim->calculateRepulsionForces();
			//printArray(sim->activeParticles[0].cumulativeForce);
			std::vector<double> f = {1.,2.,3.};
			double preFactor = -0.13808993529939517;
			f[0] *= preFactor;
			f[1] *= preFactor;
			f[2] *= preFactor;
			TS_ASSERT_EQUALS(sim->activeParticles[0].cumulativeForce, f);
			f[0] *= -1.;
			f[1] *= -1.;
			f[2] *= -1.;
			TS_ASSERT_EQUALS(sim->activeParticles[1].cumulativeForce, f);
			delete sim;
		}
};
