#include <windows.h>

#include <objidl.h>
#include <algorithm>
#pragma warning(disable : 4458)

namespace Gdiplus
{
	using std::min;
	using std::max;
}

#include <gdiplus.h>

#define AVR_INCLUDE_OCULUS

#include "avr_gl.hh"
#include "avr_test.hh"
#include "avr_sphere.hh"
#include "avr_oculus.hh"
#include "avr_healpix.hh"
#include <iostream>
#include "Kernel/OVR_System.h"

//#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "cfitsio.lib")
//#pragma comment (lib, "glew32s.lib")
#pragma comment (lib, "opengl32")
#pragma comment (lib, "glu32")
#pragma comment (lib, "gdiplus.lib")
#pragma comment (lib, "gdi32.lib")
#pragma comment (lib, "Shell32.lib")
#pragma comment (lib, "Oleaut32.lib")
#pragma comment (lib, "HP2.lib")

/*
auto main() -> int
{

	try {
		return main2();
	}
	catch (char *e) {
		std::string s(e);
		std::wstring ws(s.size() * 2, L' '); // Overestimate number of code points.
		ws.resize(std::mbstowcs(&ws[0], s.c_str(), s.size())); // Shrink to fit.

		MessageBox(0, ws.c_str(), L"Go on…", MB_ICONINFORMATION);

	}
	catch (const char *e) {
		std::string s(e);
		std::wstring ws(s.size() * 2, L' '); // Overestimate number of code points.
		ws.resize(std::mbstowcs(&ws[0], s.c_str(), s.size())); // Shrink to fit.

		MessageBox(0, ws.c_str(), L"Go on…", MB_ICONINFORMATION);

	}
}
*/

AVROculus * oculus;

static bool MainLoop(bool retryCreate)
{
	oculus->runLoop();
	return false;
}


void setupMultiwavelengthUniverse() {
	const char * base_dir = "C:\\Users\\Joe Zuntz\\Documents\\GitHubVisualStudio\\astrovr";
	const char * tile_dir = "C:\\Users\\Joe Zuntz\\Documents\\TileImages";
#define N_SPHERE 8
	const char * tile_sets[N_SPHERE] = {
		"fermi_3yr",
		"RASS",
		"DSS",
		"H-alpha",
		"WISE",
		"IRAS",
		"Planck",
		"Radio_vsmoothed"
	};

	for (int i = 0; i < N_SPHERE; i++) {
		char tile_file[1024];
		snprintf(tile_file, 1024, "%s\\%s-tiles.png", tile_dir, tile_sets[i]);
		AVRSphere *sphere = new AVRSphere(2.0-0.02f*i);
		if (i != 0) {
			sphere->alpha = 0.0f;
		}
		sphere->loadStandardShaderDirectory(base_dir);
		sphere->load(tile_file);
		oculus->objects.push_back(sphere);
	}

	//Rotate to put the galactic centre in front of the viewer.
	glm::mat4 model;
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 reflectX = glm::mat4
	(
	-1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
	);
	model = reflectX*model;
	oculus->fixedTransform = model;
	


}
typedef struct MapDisplayInfo {
	char * filename;
	int nside;
	float scaling;
} MapDisplayInfo;
#define N_HEALPIX 9

MapDisplayInfo planckMaps[N_HEALPIX] = {
	{"512_60.00smoothed_LFI_SkyMap_30_256_PR2_full.fits",   512, 1.0e6},  //1
	{"512_60.00smoothed_LFI_SkyMap_44_256_PR2_full.fits",   512, 1.0e6},  //2
	{"512_60.00smoothed_LFI_SkyMap_70_256_PR2_full.fits",   512, 1.0e6},  //3
	{"512_60.00smoothed_HFI_SkyMap_100_2048_PR2_full.fits", 512, 1.0e6},  //4
	{"512_60.00smoothed_HFI_SkyMap_143_2048_PR2_full.fits", 512, 1.0e6},  //5
	{"512_60.00smoothed_HFI_SkyMap_217_2048_PR2_full.fits", 512, 1.0e6},  //6
	{"512_60.00smoothed_HFI_SkyMap_353_2048_PR2_full.fits", 512, 1.0e5},  //7
	{"512_60.00smoothed_HFI_SkyMap_545_2048_PR2_full.fits", 512, 500.0},  //8
	{"512_60.00smoothed_HFI_SkyMap_857_2048_PR2_full.fits", 512, 200.0}   //9
};


void setupPlanck() {
	const char * base_dir = "C:\\Users\\Joe Zuntz\\Documents\\GitHubVisualStudio\\astrovr";
	const char * map_dir = "C:\\Users\\Joe Zuntz\\Documents\\Healpix Maps\\Planck";
	double royalSocietyLatitude = 51.505925*(M_PI/180.0);
	double royalSocietyLongitude = -0.132485*(M_PI / 180.0);


	for (int i = 0; i < N_HEALPIX; i++) {
		MapDisplayInfo mapInfo = planckMaps[i];
		PlanckBrokenColorMap * colorMap = new PlanckBrokenColorMap("C:\\Users\\Joe Zuntz\\Documents\\GitHubVisualStudio\\astrovr\\planck_color_table.txt", mapInfo.scaling);

		char tile_file[1024];
		snprintf(tile_file, 1024, "%s\\%s", map_dir, mapInfo.filename);
		AVRHealpix * healpixMap = new AVRHealpix(mapInfo.nside, 1.0f-i*0.01f, colorMap);
		healpixMap->latitude = royalSocietyLatitude;
		healpixMap->longitude = royalSocietyLongitude;
		if (i < 2) {
			healpixMap->setAlpha(0.5f);
		}
		else {
			healpixMap->setAlpha(0.0f);

		}
		healpixMap->loadStandardShaderDirectory(base_dir);
		healpixMap->load(tile_file);
		oculus->objects.push_back(healpixMap);
	}

	//At this point galactic north is in front of the viewer.
	//Rotate to put the plane there
	//glm::mat4 model;
	//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//oculus->fixedTransform = model;

}



int main1(HINSTANCE hinst) {

	OVR::System::Init();


	// Initialize GDI+.
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	//Initialize Oculus
	ovrResult result = ovr_Initialize(nullptr);
	if (!OVR_SUCCESS(result)) {
		throw "Failed to init";
	}

	oculus = new AVROculus();
	
	
	
//	oculus->setup(hinst, L"The Multi-Wavelength Universe");
//	setupMultiwavelengthUniverse();
	
	oculus->setup(hinst, L"The Microwave Universe with Planck");
	setupPlanck();


	Platform.Run(MainLoop);

	ovr_Shutdown();
	OVR::System::Destroy();

	return(0);
}


int WINAPI WinMain(HINSTANCE hinst, HINSTANCE, LPSTR, int)
{
	try {
		return main1(hinst);
	}
	catch (char *e) {
		std::string s(e);
		std::wstring ws(s.size() * 2, L' '); // Overestimate number of code points.
		ws.resize(std::mbstowcs(&ws[0], s.c_str(), s.size())); // Shrink to fit.

		MessageBox(0, ws.c_str(), L"Error thrown in program…", MB_ICONINFORMATION);
		return 1;
	}
	
	catch (const char *e) {
		std::string s(e);
		std::wstring ws(s.size() * 2, L' '); // Overestimate number of code points.
		ws.resize(std::mbstowcs(&ws[0], s.c_str(), s.size())); // Shrink to fit.
		MessageBox(0, ws.c_str(), L"Error thrown in program…", MB_ICONINFORMATION);
		return 1;

	}
	
}
