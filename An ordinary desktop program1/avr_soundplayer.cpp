#include "stdafx.h"

#ifndef JOE_NONSENSE
#error argh
#endif

#include "avr_soundplayer.h"

#include <algorithm>

namespace Gdiplus
{
	using std::min;
	using std::max;
}



#include <windows.h>
#include <Mmsystem.h>

AVRSoundPlayer::AVRSoundPlayer(std::wstring file_name)
{
	filename = file_name;
}


AVRSoundPlayer::~AVRSoundPlayer()
{
}

void AVRSoundPlayer::play() {
	const wchar_t * filename_cstr = filename.c_str();
	PlaySound(filename_cstr, NULL, SND_FILENAME | SND_ASYNC);
//	PlaySound(TEXT("C:\\Users\\Joe Zuntz\\Music\\oculucfulltourwithmusicmono.wav"), NULL, SND_FILENAME | SND_ASYNC);


}