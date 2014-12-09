/* Potential.cpp
 * author: Christoph Froehner
 */

#include "Potential.h"

std::array<double, 3> Potential::softcoreForce(std::array<double, 3> r_ij, double rSquared, 
double cutoffSquared, double strength)
{
	if ( rSquared > cutoffSquared ) {
		std::array<double,3> zero = {0.,0.,0.};
		return zero;
	}
	double preFactor = strength * (1. - sqrt(cutoffSquared/rSquared));
	std::array<double, 3> force;
	force[0] = preFactor * r_ij[0];
	force[1] = preFactor * r_ij[1]; 
	force[2] = preFactor * r_ij[2];
	return force;
}
