#include "avr_main.h"

AVROculus * global_oculus;
int global_mode;



bool avr_main_loop(bool retryCreate)
{
	global_oculus->runLoop(global_mode);
	return false;
}



avr_main::avr_main(HINSTANCE hinst1)
{
	oculus = new AVROculus();
	global_oculus = oculus;
	hinst = hinst1;
	mode = -1;
}


avr_main::~avr_main()
{
}




void avr_main::setupMultiwavelengthUniverse() {
	oculus->setup(hinst, L"The Multi-Wavelength Universe");
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
		AVRSphere *sphere = new AVRSphere(2.0 - 0.02f*i);
		if (i != 2) {
			sphere->alpha = 0.0f;
		}
		sphere->loadStandardShaderDirectory(base_dir);
		sphere->load(tile_file);
		oculus->objects.push_back(sphere);
	}

	//Rotate to put the galactic centre in front of the viewer.
	glm::mat4 model = glm::mat4
		(
			-1.0, 0.0, 0.0, 0.0,
			0.0, -1.0, 0.0, 0.0,
			0.0, 0.0, -1.0, 0.0,
			0.0, 0.0, 0.0, 1.0
			);

	oculus->fixedTransform = model;



}
typedef struct MapDisplayInfo {
	char * filename;
	int nside;
	float scaling;
} MapDisplayInfo;
#define N_HEALPIX 9

MapDisplayInfo planckMaps[N_HEALPIX] = {
	{ "512_60.00smoothed_LFI_SkyMap_30_256_PR2_full.fits",   512, 1.0e6 },  //1
	{ "512_60.00smoothed_LFI_SkyMap_44_256_PR2_full.fits",   512, 1.0e6 },  //2
	{ "512_60.00smoothed_LFI_SkyMap_70_256_PR2_full.fits",   512, 1.0e6 },  //3
	{ "512_60.00smoothed_HFI_SkyMap_100_2048_PR2_full.fits", 512, 1.0e6 },  //4
	{ "512_60.00smoothed_HFI_SkyMap_143_2048_PR2_full.fits", 512, 1.0e6 },  //5
	{ "512_60.00smoothed_HFI_SkyMap_217_2048_PR2_full.fits", 512, 1.0e6 },  //6
	{ "512_60.00smoothed_HFI_SkyMap_353_2048_PR2_full.fits", 512, 1.0e5 },  //7
	{ "512_60.00smoothed_HFI_SkyMap_545_2048_PR2_full.fits", 512, 500.0 },  //8
	{ "512_60.00smoothed_HFI_SkyMap_857_2048_PR2_full.fits", 512, 200.0 }   //9
};


void avr_main::setupPlanck() {
	oculus->setup(hinst, L"The Microwave Universe with Planck");

	const char * base_dir = "C:\\Users\\Joe Zuntz\\Documents\\GitHubVisualStudio\\astrovr";
	const char * map_dir = "C:\\Users\\Joe Zuntz\\Documents\\Healpix Maps\\Planck";
	double royalSocietyLatitude = 51.505925*(M_PI / 180.0);
	double royalSocietyLongitude = -0.132485*(M_PI / 180.0);


	for (int i = 0; i < N_HEALPIX; i++) {
		MapDisplayInfo mapInfo = planckMaps[i];
		PlanckBrokenColorMap * colorMap = new PlanckBrokenColorMap("C:\\Users\\Joe Zuntz\\Documents\\GitHubVisualStudio\\astrovr\\planck_color_table.txt", mapInfo.scaling);

		char tile_file[1024];
		snprintf(tile_file, 1024, "%s\\%s", map_dir, mapInfo.filename);
		AVRHealpix * healpixMap = new AVRHealpix(mapInfo.nside, 1.0f - i*0.01f, colorMap);
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


}



void avr_main::main(int mode) {
	global_mode = mode;



	if (mode == PLANCK_MODE) {
		setupPlanck();
	}
	else if (mode == MULTIWAVELENGTH_FREE) {
		setupMultiwavelengthUniverse();

	}
	else if (mode == MULTIWAVELENGTH_LONG_TOUR) {
		setupMultiwavelengthUniverse();
	}



	Platform.Run(avr_main_loop);

	ovr_Shutdown();
	OVR::System::Destroy();

}

