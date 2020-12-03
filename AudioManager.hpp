#pragma once

#include "Load.hpp"
#include "Sound.hpp"

#include <queue>


struct AudioManager {
	// Types of audio clips to play
	enum class AudioClip {
		BACKGROUND, ERROR, FOOTSTEP, FOOTSTEP_SHORT, ROLL_BARREL, DRAG_STONE, UNDO
	};

	// Static variables
	static Load< std::vector<Sound::Sample> > Samples;
  static std::queue<AudioClip> clips_to_play;
	static std::shared_ptr<Sound::PlayingSample> backgroundSamplePlayer;

	// Methods
	static const Sound::Sample &get_sample(AudioClip clip);
	static void ensure_background_is_playing();
};
