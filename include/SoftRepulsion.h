/* SoftRepulsion.h 
 * 
 * Implement the repulsive harmonic force as a child
 * of the ParticleInteraction class. */

#ifndef __SOFTREPULSION_H_INCLUDED__
#define __SOFTREPULSION_H_INCLUDED__
#include <math.h>
#include <cmath>
#include "ParticleInteraction.h"

class SoftRepulsion : public ParticleInteraction
{
	public:
		SoftRepulsion(
			std::string inName,
			std::vector<unsigned int> inAffectedTuple,
			double inRepulsionStrength);
		~SoftRepulsion();
		
		void calculateForceEnergy(
			std::vector<double>& forceI, //out
			double& energy,
			std::vector<double>& r_ij, //in
			double& rSquared,
			double& radiiSquared);
		double calculateEnergy(
			double rSquared, // in
			double radiiSquared); //in
};

#endif // __SOFTREPULSION_H_INCLUDED__
