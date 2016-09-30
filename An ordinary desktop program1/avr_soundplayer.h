#pragma once
#include "stdafx.h"
#include <string>

class AVRSoundPlayer
{
public:
	AVRSoundPlayer(std::wstring file_name);
	~AVRSoundPlayer();
	void play();
	std::wstring filename;
};

