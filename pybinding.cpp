#include <string>
#include <vector>
#include <boost/python.hpp>
#include <boost/python/numeric.hpp>
#include <boost/python/extract.hpp>
#include "World.h"
#include "Config.h"
#include "Simulation.h"
#include "logging.h"


class ConfigWrap {
public:
	Config * config;
	ConfigWrap() { this->config = new Config();	}
	~ConfigWrap() { delete this->config; }
	double getTimestep() { return this->config->timestep; }
	void setTimestep(double timestep) { this->config->timestep = timestep; }
	double getKT() { return this->config->kT; }
	void setKT(double kT) {this->config->kT = kT; }
	bool getIsPeriodic() { return this->config->isPeriodic; }
	void setIsPeriodic(bool isPeriodic) { this->config->isPeriodic = isPeriodic; }
	double getBoxsize() {return this->config->boxsize; }
	void setBoxsize(double boxsize) { this->config->boxsize = boxsize; }
	void new_Type(const std::string name, const double radius, const double diffusionConstant) {
		this->config->new_Type(name, radius, diffusionConstant);
	}
	unsigned getNumberOfParticleTypes() {
		return this->config->getNumberOfParticleTypes();
	}
	std::string getParticleTypeName(unsigned i) {
		return this->config->getParticleTypeName(i);
	}
	double getParticleTypeRadius(unsigned i) {
		return this->config->getParticleTypeRadius(i);
	}
	double getParticleTypeDiffusionConstant(unsigned i) {
		return this->config->getParticleTypeDiffusionConstant(i);
	}

	void deleteAllGeometries() {
		this->config->deleteAllGeometries();
	}
	void new_Wall(boost::python::numeric::array normal, boost::python::numeric::array point, double strength, boost::python::numeric::array particleTypeIds) {
		std::vector<double> norm = {0.,0.,0.};
		std::vector<double> pnt = {0.,0.,0.};
		std::vector<unsigned> pTypeIds;
		try {
			norm[0] = boost::python::extract<double>(normal[0]);
			norm[1] = boost::python::extract<double>(normal[1]);
			norm[2] = boost::python::extract<double>(normal[2]);
			pnt[0] = boost::python::extract<double>(point[0]);
			pnt[1] = boost::python::extract<double>(point[1]);
			pnt[2] = boost::python::extract<double>(point[2]);
			for (unsigned i=0; i<particleTypeIds.nelements(); ++i) {
				pTypeIds.push_back(boost::python::extract<unsigned>(particleTypeIds[i]));
			}
		} catch (...) {
			LOG_ERROR("Exception in accessing boost::python::numeric::array.")
			LOG_INFO("Wall geometry is not added.")
			return;
		}
		this->config->new_Wall(norm, pnt, strength, pTypeIds);
	}
	void new_DoubleWellZ(double distanceMinima,	double strength, boost::python::numeric::array particleTypeIds) {
		std::vector<unsigned> pTypeIds;
		try {
			for (unsigned i=0; i<particleTypeIds.nelements(); ++i) {
				pTypeIds.push_back(boost::python::extract<unsigned>(particleTypeIds[i]));
			}		
		} catch (...) {
			LOG_ERROR("Exception in accessing boost::python::numeric::array.")
			LOG_INFO("DoubleWellZ geometry is not added.")
			return;
		}
		this->config->new_DoubleWellZ(distanceMinima, strength, pTypeIds);
	}

	void deleteAllInteractions() {
		this->config->deleteAllInteractions();
	}
	void new_SoftRepulsion(std::string name, boost::python::numeric::array affectedTuple, double repulsionStrength) {
		std::vector<unsigned> affTuple = {0,0};
		try {
			affTuple[0] = boost::python::extract<unsigned>(affectedTuple[0]);
			affTuple[1] = boost::python::extract<unsigned>(affectedTuple[1]);
		} catch (...) {
			LOG_ERROR("Exception in accessing boost::python::numeric::array.")
			LOG_INFO("SoftRepulsion interaction is not added.")
			return;
		}
		this->config->new_SoftRepulsion(name, affTuple, repulsionStrength);
	}
	void new_LennardJones(std::string name,	boost::python::numeric::array affectedTuple, double epsilon) {
		std::vector<unsigned> affTuple = {0,0};
		try {
			affTuple[0] = boost::python::extract<unsigned>(affectedTuple[0]);
			affTuple[1] = boost::python::extract<unsigned>(affectedTuple[1]);
		} catch (...) {
			LOG_ERROR("Exception in accessing boost::python::numeric::array.")
			LOG_INFO("LennardJones interaction is not added.")
			return;
		}
		this->config->new_LennardJones(name, affTuple, epsilon);
	}
	unsigned getNumberInteractions() {
		return this->config->getNumberInteractions();
	}
	std::string getInteractionName(unsigned i) {
		return this->config->getInteractionName(i);
	}
	std::string getInteractionType(unsigned i) {
		return this->config->getInteractionType(i);
	}
	boost::python::numeric::array getInteractionAffectedTuple(unsigned i) {
		std::vector<unsigned> affTuple = this->config->getInteractionAffectedTuple(i);
		boost::python::numeric::array affectedTuple = boost::python::numeric::array(
			boost::python::make_tuple(affTuple[0], affTuple[1])
		);
		return affectedTuple;
	}
	// TODO how to construct numeric array of dynamic length
	std::vector<double> getInteractionParameters(unsigned i);
	double getInteractionCutoff(unsigned i);

	void configureReactions();
	void deleteAllReactions();
	void new_Conversion(std::string name, unsigned forwardType, unsigned backwardType, double forwardRate, double backwardRate);	
	void new_Fusion(std::string name, unsigned forwardTypeA, unsigned forwardTypeB,	unsigned backwardTypeC,	double forwardRate,	double backwardRate, double reactionDistance);
	void configureFusion(unsigned reactionIndex, std::vector<unsigned> interactionsIndices,	double inversePartition, double maxDistr, double radiiSum, double reactionRadiiSum,	double meanDistr, double inverseTemperature, double radiusA, double radiusB);
	unsigned getNumberReactions();
	std::string getReactionName(unsigned i);
	std::string getReactionType(unsigned i);
	std::vector<unsigned> getReactionForwardTypes(unsigned i);
	std::vector<unsigned> getReactionBackwardTypes(unsigned i);
	double getReactionForwardRate(unsigned i);
	double getReactionBackwardRate(unsigned i);
};

class WorldWrap {
public:
	World * world;
	WorldWrap() {
		LOG_TRACE("Enter WorldWrap Constructor.")
		this->world = new World();
		LOG_TRACE("Leave WorldWrap Constructor.")
	}

	~WorldWrap() {
		LOG_TRACE("Enter WorldWrap Destructor.")
		delete this->world;
		LOG_TRACE("Leave WorldWrap Destructor.")
	}

	void addParticle(boost::python::numeric::array initPos, unsigned particleTypeId) {
		std::vector<double> pos = {0., 0., 0.};
		try {
			pos[0] = boost::python::extract<double>(initPos[0]);
			pos[1] = boost::python::extract<double>(initPos[1]);
			pos[2] = boost::python::extract<double>(initPos[2]);
		} catch (...) {
			LOG_ERROR("Exception in accessing boost::python::numeric::array.")
			LOG_INFO("Particle is not added.")
			return;
		}
		world->addParticle(pos, particleTypeId);
	}

	boost::python::numeric::array getPosition(unsigned long index) {
		std::vector<double> pos = world->getPosition(index);
		boost::python::numeric::array arr = boost::python::numeric::array(
			boost::python::make_tuple(pos[0], pos[1], pos[2])
		);
		return arr;
	}
};

class SimulationWrap {
public:
	Simulation * simulation;
	SimulationWrap(WorldWrap * worldWrap, ConfigWrap * configWrap, std::string whichImpl) {
		LOG_TRACE("Enter SimulationWrap Constructor.")
		this->simulation = new Simulation(worldWrap->world, configWrap->config, whichImpl);
		LOG_TRACE("Leave SimulationWrap Constructor.")
	}
	~SimulationWrap() {
		LOG_TRACE("Enter SimulationWrap Destructor.")
		delete simulation;
		LOG_TRACE("Leave SimulationWrap Destructor.")
	}
};

using namespace boost::python;

BOOST_PYTHON_MODULE(revreaddyPy) {
	numeric::array::set_module_and_type("numpy", "ndarray");
	class_<WorldWrap>("World", init<>())
		.def("addParticle", &WorldWrap::addParticle)
		.def("getPosition", &WorldWrap::getPosition);
	class_<ConfigWrap>("Config", init<>())
		.def("getTimestep", &ConfigWrap::getTimestep)
		.def("setTimestep", &ConfigWrap::setTimestep)
		.def("getKT", &ConfigWrap::getKT)
		.def("setKT", &ConfigWrap::setKT)
		.def("getIsPeriodic", &ConfigWrap::getIsPeriodic)
		.def("setIsPeriodic", &ConfigWrap::setIsPeriodic)
		.def("getBoxsize", &ConfigWrap::getBoxsize)
		.def("setBoxsize", &ConfigWrap::setBoxsize)
		.def("new_Type", &ConfigWrap::new_Type)
		.def("getNumberOfParticleTypes", &ConfigWrap::getNumberOfParticleTypes)
		.def("getParticleTypeName", &ConfigWrap::getParticleTypeName)
		.def("getParticleTypeRadius", &ConfigWrap::getParticleTypeRadius)
		.def("getParticleTypeDiffusionConstant", &ConfigWrap::getParticleTypeDiffusionConstant)
		.def("deleteAllGeometries", &ConfigWrap::deleteAllGeometries)
		.def("new_Wall", &ConfigWrap::new_Wall)
		.def("new_DoubleWellZ", &ConfigWrap::new_DoubleWellZ)
		.def("new_SoftRepulsion", &ConfigWrap::new_SoftRepulsion);
	class_<SimulationWrap>("Simulation", init<WorldWrap*, ConfigWrap*, std::string>());
};