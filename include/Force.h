/* Force.h
 * author: Christoph Froehner
 * 
 * Handler for calculation of forces.
 */

#ifndef __FORCE_H_INCLUDED__
#define __FORCE_H_INCLUDED__
#include <vector>
#include <math.h>
#include <cmath>
#include <string>

class Force
{
	public:
		std::vector<double> repulsionForce(
			std::vector<double> r_ij,
			double rSquared,
			double radiiSquared,
			double strength,
			std::string typeI,
			std::string typeJ);
		std::vector<double> softcoreForce(
			std::vector<double> r_ij,
			double rSquared, 
			double radiiSquared,
			double strength);
		std::vector<double> LJ1206Force(
			std::vector<double> r_ij,
			double rSquared,
			double sigmaSquared,
			double strength);
		double repulsionEnergy(
			double rSquared,
			double radiiSquared,
			double strength,
			std::string typeI,
			std::string typeJ);
		double softcoreEnergy(
			double rSquared,
			double radiiSquared,
			double strength);
		double LJ1206Energy(
			double rSquared,
			double sigmaSquared,
			double strength);
};
#endif // __FORCE_H_INCLUDED__