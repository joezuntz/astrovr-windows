#pragma once
#include "stdafx.h"

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
#include "avr_gl.hh"
#include "avr_test.hh"
#include "avr_sphere.hh"
#include "avr_oculus.hh"
#include "avr_healpix.hh"
#include <iostream>
#include "Kernel/OVR_System.h"


class avr_main
{
public:
	avr_main(HINSTANCE hinst);
	~avr_main();
	void main(int mode);
	void setupPlanck();
	void setupMultiwavelengthUniverse();
	AVROculus * oculus;
	int mode;
	HINSTANCE hinst;
};

bool avr_main_loop(bool retryCreate);
