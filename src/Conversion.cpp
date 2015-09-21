/* Conversion.cpp */

#include "Conversion.h"

Conversion::Conversion(
	std::string inName,
	std::vector<unsigned int> inForwardTypes,
	std::vector<unsigned int> inBackwardTypes,
	double inForwardRate,
	double inBackwardRate)
{
	this->name = inName;
	this->forwardTypes = inForwardTypes;
	this->backwardTypes = inBackwardTypes;
	this->forwardRate = inForwardRate;
	this->backwardRate = inBackwardRate;
}

double performForward(
	std::vector<unsigned long int> particleIndices,
	Simulation* simulation)
{
	return 1.;
}

double performBackward(
	std::vector<unsigned long int> particleIndices,
	Simulation* simulation)
{
	return 1.;
}