#include <vector>
#include <cmath>

#include "Config.hpp"

// Disable obnoxious warnings from Eigen headers
#if defined (__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall" 
#pragma GCC diagnostic ignored "-Weffc++" 
#pragma GCC diagnostic ignored "-Wextra"
#include <Eigen/Dense>
#pragma GCC diagnostic pop
#elif (__INTEL_COMPILER)
#pragma warning push
#pragma warning disable "-Wall"
#include <Eigen/Dense>
#pragma warning pop
#endif

#include <boost/filesystem.hpp>

#include "GePolCavity.hpp"
#include "PhysicalConstants.hpp"
#include "Symmetry.hpp"

// Disable obnoxious warnings from Google Test headers
#if defined (__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall" 
#pragma GCC diagnostic ignored "-Weffc++" 
#pragma GCC diagnostic ignored "-Wextra"
#include "gtest/gtest.h"
#pragma GCC diagnostic pop
#endif

namespace fs = boost::filesystem;

class GePolCavityD2hAddTest : public ::testing::Test
{
	protected:
		GePolCavity cavity;
		virtual void SetUp()
		{
  	                Eigen::Vector3d	C1(0.0000000000,  0.0000000000,  1.2578920000); 	
  		        Eigen::Vector3d	C2(0.0000000000,  0.0000000000, -1.2578920000);
  		        Eigen::Vector3d	H1(0.0000000000,  1.7454620000,  2.3427160000);
  		        Eigen::Vector3d	H2(0.0000000000, -1.7454620000,  2.3427160000);
  		        Eigen::Vector3d	H3(0.0000000000,  1.7454620000, -2.3427160000);
  		        Eigen::Vector3d	H4(0.0000000000, -1.7454620000, -2.3427160000);
			std::vector<Sphere> spheres;
			double radiusC = (1.70 * 1.20) / convertBohrToAngstrom;
			double radiusH = (1.20 * 1.20) / convertBohrToAngstrom;
			Sphere sph1(C1, radiusC);
			Sphere sph2(H1, radiusH);
			Sphere sph3(H2, radiusH);
			Sphere sph4(C2, radiusC);
			Sphere sph5(H3, radiusH);
			Sphere sph6(H4, radiusH);
			spheres.push_back(sph1);
			spheres.push_back(sph2);
			spheres.push_back(sph3);
			spheres.push_back(sph4);
			spheres.push_back(sph5);
			spheres.push_back(sph6);
			double area = 0.2 / convertBohr2ToAngstrom2;
			double probeRadius = 1.385 / convertBohrToAngstrom;
			// Addition of spheres is enabled, but will not happen in this particular case
			double minRadius = 0.2 / convertBohrToAngstrom;
			// D2h as generated by Oxy, Oxz, Oyz
			Symmetry pGroup = buildGroup(3, 4, 2, 1);
			cavity = GePolCavity(spheres, area, probeRadius, minRadius, pGroup);
			cavity.saveCavity("c2h4_d2h.npz");
			fs::rename("PEDRA.OUT", "PEDRA.OUT.d2h");
			fs::rename("cavity.off", "cavity.off.d2h");
		}
};

TEST_F(GePolCavityD2hAddTest, size)
{
	int size = 576;
	int actualSize = cavity.size();
	EXPECT_EQ(size, actualSize);
}

TEST_F(GePolCavityD2hAddTest, irreducible_size)
{
	int size = 72;
	int actualSize = cavity.irreducible_size();
	EXPECT_EQ(size, actualSize);
}

TEST_F(GePolCavityD2hAddTest, area)
{
	double area = 281.81993683500656;
 	double actualArea = cavity.elementArea().sum();
	EXPECT_NEAR(area, actualArea, 1.0e-10);
}

TEST_F(GePolCavityD2hAddTest, volume)
{
	double volume = 406.54737252764619;
	Eigen::Matrix3Xd elementCenter = cavity.elementCenter();
	Eigen::Matrix3Xd elementNormal = cavity.elementNormal();
	double actualVolume = 0;
        for ( int i = 0; i < cavity.size(); ++i )
	{
		actualVolume += cavity.elementArea(i) * elementCenter.col(i).dot(elementNormal.col(i));
	}
	actualVolume /= 3;
	EXPECT_NEAR(volume, actualVolume, 1.0e-10);
}