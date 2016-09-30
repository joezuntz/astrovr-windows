#pragma warning(disable : 4458)


#include "avr_main.h"


// Libraries we link to.
#pragma comment (lib, "cfitsio.lib")
#pragma comment (lib, "opengl32")
#pragma comment (lib, "glu32")
#pragma comment (lib, "gdiplus.lib")
#pragma comment (lib, "gdi32.lib")
#pragma comment (lib, "Shell32.lib")
#pragma comment (lib, "Oleaut32.lib")
#pragma comment (lib, "HP2.lib")


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


	// THIS IS THE BIT WHERE YOU CHOOSE THE MODE.
	// PICK ONE OF THESE!

	// Or figure out how to open a little window to let the user choose

	avr_main * main_manager = new avr_main(hinst);
	main_manager->main(MULTIWAVELENGTH_LONG_TOUR);
//	main_manager->main(MULTIWAVELENGTH_FREE);
//	main_manager->main(PLANCK_MODE);

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
