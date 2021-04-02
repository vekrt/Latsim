#include <iostream>
#include <fstream>
#include <cmath>
#include <omp.h>

#include "glad.h"
#include "GLFW/glfw3.h"
#include "shader.h"
#include "texture.h"
#include "renderer.h"
#include "r2.h"

#include "integrator_mh.h"
#include "integrator_wolff.h"
#include "square_lattice.h"
#include "triangular_lattice.h"
#include "honeycomb_lattice.h"
#include "ising.h"
#include "potts.h"
#include "config_file.h"
#include "config_file.tpp"
#include "context.h"


int main(int argc, char** argv) {
	Config_file conf(argv[1]);
	for (int i=1; i<argc; ++i) {
		std::cout << argv[i] << std::endl;
		conf.insert(split_on(argv[i], '='));
	}

	conf.print();

	const size_t h = conf.get<size_t>("h");
	const size_t w = h;
	const double J = conf.get<double>("J");

	const double beta = conf.get<double>("beta");

	const std::string lattice_type = conf.get<std::string>("lattice");
	const std::string model_type = conf.get<std::string>("model");
	const std::string integrator_type = conf.get<std::string>("integrator");

	Lattice* lattice = nullptr;
	if (lattice_type == "honeycomb")
		lattice = new Honeycomb_lattice(h, w, J);
	else if (lattice_type == "square")
		lattice = new Square_lattice(h, w, J);
	else if (lattice_type == "triangular")
		lattice = new Triangular_lattice(h, w, J);
	else
		throw std::invalid_argument("unknown lattice type");

	Model* model = nullptr;
	if (model_type == "ising") {
		lattice -> random_init({-1, 1});
		model = new Ising(*lattice, beta, true);
	}
	else if (model_type == "potts") {
		std::vector<int> qs;
		for (size_t i=0; i<conf.get<size_t>("q"); ++i)
			qs.push_back(i+1);
		lattice -> random_init(qs);
		model = new Potts(*lattice, beta, false, conf.get<size_t>("q"));
	}
	else
		throw std::invalid_argument("unknown model type");

	model -> compute_energy();
	model -> compute_M();

	Integrator* integrator = nullptr;
	if (integrator_type == "mh") {
		integrator  = new Integrator_mh();
	}
	else if (integrator_type == "wolff") {
		if (model_type == "ising")
			integrator  = new Integrator_wolff(1-exp(-2*J*beta));
		else
			integrator  = new Integrator_wolff(1-exp(-J*beta));
	}
	else
		throw std::invalid_argument("unknown integrator type");

	std::random_device rd;
	integrator -> set_random_generator(rd, *lattice);

	//size_t kk = 0;
	//while (kk < 200000) {
	//	integrator -> step(*model, *lattice);
	//	++kk;
	//	if (kk % 1000 == 0)
	//		std::cout << kk << std::endl;
	//}

	Context context;
	context.init(lattice, model);
	context.launch(integrator, model, lattice);
	context.terminate();
	
	return 0;
}
