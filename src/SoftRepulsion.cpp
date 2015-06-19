/* SoftRepulsion.cpp */

#include "SoftRepulsion.h"

// cutoff is set by Simulation, here it is radius_i + radius_j
SoftRepulsion::SoftRepulsion(
	std::string inName,
	std::vector <unsigned int> inAffectedTuple,
	double inRepulsionStrength)
{
	this->name = inName;
	this->type = "SoftRepulsion";
	this->parameters = { inRepulsionStrength };
	// apply the convention that the tuple must be sorted
	if ( inAffectedTuple[0] <= inAffectedTuple[1] ) {
		this->affectedTuple.push_back(inAffectedTuple[0]);
		this->affectedTuple.push_back(inAffectedTuple[1]);
	}
	else {
		this->affectedTuple.push_back(inAffectedTuple[1]);
		this->affectedTuple.push_back(inAffectedTuple[0]);	
		std::cout << "Info: SoftRepulsion affectedTuple order was inverted\n";
	}
}

void SoftRepulsion::calculateForceEnergy(
	std::vector<double>& forceI, //out
	double& energy,
	std::vector<double>& r_ij, //in
	double& rSquared,
	double& radiiSquared)
{
	if ( rSquared > radiiSquared ) {
		forceI = {0.,0.,0.};
		energy = 0.;
		return;
	}
	// E=strength*(r - radii)**2, F=2.*strength*(r - radii)/r * r_ij(vec)
	double a = sqrt(rSquared);
	double c = a - sqrt(radiiSquared);
	energy = this->parameters[0] * c * c;
	a = 2. * this->parameters[0] * c / a;
	forceI[0] = a * r_ij[0];
	forceI[1] = a * r_ij[1];
	forceI[2] = a * r_ij[2];
}
