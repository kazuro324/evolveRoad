#include "AudioManager.h"

AudioManager* AudioManager::instance = nullptr;

AudioManager::AudioManager()
{

}

AudioManager::~AudioManager()
{

}

AudioManager& AudioManager::Instance()
{
	if (instance == NULL)
	{
		instance = new AudioManager();
	}

	return *instance;
}
