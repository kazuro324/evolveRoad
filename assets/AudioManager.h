#pragma once
#include <Siv3D.hpp>

class AudioManager
{
	public:
		static AudioManager& Instance();
		AudioManager();
		~AudioManager();
		double volume = 0.2;

	private:
		static AudioManager* instance;
		
		
};
