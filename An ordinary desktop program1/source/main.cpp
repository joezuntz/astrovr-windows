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
		AVRSphere *sphere = new AVRSphere(10.0);
		if (i != 0) {
			sphere->alpha = 0.0f;
		}
		sphere->loadStandardShaderDirectory(base_dir);
		sphere->load(tile_file);
		oculus->objects.push_back(sphere);
	}

}
typedef struct MapDisplayInfo {
	char * filename;
	int nside;
	float vmin;
	float vmax;
} MapDisplayInfo;
#define N_HEALPIX 9

MapDisplayInfo planckMaps[N_HEALPIX] = {
	{"512_60.00smoothed_LFI_SkyMap_30_256_PR2_full.fits",   512, -0.0002f, 0.0002f},
	{"512_60.00smoothed_LFI_SkyMap_44_256_PR2_full.fits",   512, -0.0003f, 0.0003f},
	{"512_60.00smoothed_LFI_SkyMap_70_256_PR2_full.fits",   512, -0.0006f, 0.0006f},
	{"512_60.00smoothed_HFI_SkyMap_100_2048_PR2_full.fits", 512, -0.001f, 0.001f},
	{"512_60.00smoothed_HFI_SkyMap_143_2048_PR2_full.fits", 512, -0.001f, 0.001f},
	{"512_60.00smoothed_HFI_SkyMap_217_2048_PR2_full.fits", 512, -0.001f, 0.001f},
	{"512_60.00smoothed_HFI_SkyMap_353_2048_PR2_full.fits", 512, -0.001f, 0.01f},
	{"512_60.00smoothed_HFI_SkyMap_545_2048_PR2_full.fits", 512, -0.001f, 5.0f},
	{"512_60.00smoothed_HFI_SkyMap_857_2048_PR2_full.fits", 512, -0.001f, 10.0f}
};


void setupPlanck() {
	const char * base_dir = "C:\\Users\\Joe Zuntz\\Documents\\GitHubVisualStudio\\astrovr";
	const char * map_dir = "C:\\Users\\Joe Zuntz\\Documents\\Healpix Maps\\Planck";

	for (int i = 0; i < N_HEALPIX; i++) {
		MapDisplayInfo mapInfo = planckMaps[i];
		JetColorMap * colorMap = new JetColorMap(mapInfo.vmin, mapInfo.vmax, false);


		char tile_file[1024];
		snprintf(tile_file, 1024, "%s\\%s", map_dir, mapInfo.filename);
		AVRHealpix * healpixMap = new AVRHealpix(mapInfo.nside, 1.0f, colorMap);
		if (i != 0) {
			healpixMap->setAlpha(0.0f);
		}
		healpixMap->loadStandardShaderDirectory(base_dir);
		healpixMap->load(tile_file);
		oculus->objects.push_back(healpixMap);
	}

}



int main1(HINSTANCE hinst) {

	OVR::System::Init();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
	oculus->setup(hinst);


	//setupMultiwavelengthUniverse();
	setupMultiwavelengthUniverse();


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

		MessageBox(0, ws.c_str(), L"Go on…", MB_ICONINFORMATION);
		return 1;
	}
	catch (const char *e) {
		std::string s(e);
		std::wstring ws(s.size() * 2, L' '); // Overestimate number of code points.
		ws.resize(std::mbstowcs(&ws[0], s.c_str(), s.size())); // Shrink to fit.
		MessageBox(0, ws.c_str(), L"Go on…", MB_ICONINFORMATION);
		return 1;

	}

}
