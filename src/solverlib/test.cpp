#include <string>
#include <iostream>
#include <fstream>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

#include "Getkw.h"
#include "GreensFunction.h"
#include "Vacuum.h"
#include "UniformDielectric.h"
#include "MetalSphere.h"
#include "GreensFunctionSum.h"
#include "Cavity.h"
#include "GePolCavity.h"
#include "PCMSolver.h"

int main(int argc, char** argv){

	const char *infile = 0;
	if (argc == 1) {
		infile = "STDIN";
	} else if (argc == 2) {
		infile = argv[1];
	} else {
		cout << "Invalid nr. of arguments" << endl;
		exit(1);
	}
	
	Getkw Input = Getkw(infile, false, true);

	int printl = Input.getInt("PRINTL");
	double area = Input.getDbl("Cavity.Area");

    GePolCavity cavity(Input);
	cavity.makeCavity();
    
	Vector3d p1(0.0, 10.1, 0.0);
    Vector3d p2(0.0, 10.2, 0.0);
    Vector3d ps(0.0,  0.0, 0.0);
    
    MetalSphere metal(20.0, 0.0, 10000.0, ps, 10.0);
    UniformDielectric water(10000.0);
    Vacuum vacuum;

    PCMSolver waterSolver(vacuum, water);
    waterSolver.buildPCMMatrix(cavity);
    VectorXd potential(cavity.size());
    VectorXd charges(cavity.size());
    potential.setConstant(1.0);
    const MatrixXd &matrix = waterSolver.getPCMMatrix();
    charges = matrix * potential;
    cout << " CHARGES " << charges.sum() << endl;
}