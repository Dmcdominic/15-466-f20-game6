#include "AudioManager.hpp"

#include "data_path.hpp"

#include <iostream>



// Audio loading.
// Make sure to update the enum class AudioClip in AudioManager.hpp when you add/change this.
Load< std::vector<Sound::Sample> > AudioManager::Samples(LoadTagDefault, []() -> std::vector<Sound::Sample> const* {
	return new std::vector<Sound::Sample>(
		{
			Sound::Sample(data_path("audio/Deep Relaxation.wav")),
			Sound::Sample(data_path("audio/Error1.wav")),
			//Sound::Sample(data_path("audio/footstep-grass.wav")),
			//Sound::Sample(data_path("audio/footstep-gravel.wav")),
			Sound::Sample(data_path("audio/footstep-stone.wav")),
			Sound::Sample(data_path("audio/footstep-stone-short.wav")),
			Sound::Sample(data_path("audio/rolling-metal-barrel.wav")),
			Sound::Sample(data_path("audio/drag-stone.wav")),
			Sound::Sample(data_path("audio/swosh.wav")),
			Sound::Sample(data_path("audio/toxic-area.wav")),
			Sound::Sample(data_path("audio/quack.wav")),
			Sound::Sample(data_path("audio/Toggle.wav")),
			Sound::Sample(data_path("audio/Select.wav")),
			Sound::Sample(data_path("audio/wind-gust.wav")),
			Sound::Sample(data_path("audio/Victory.wav")),
		});
});


// Static variable declarations
std::queue<AudioManager::AudioClip> AudioManager::clips_to_play = std::queue<AudioManager::AudioClip>();
std::shared_ptr<Sound::PlayingSample> AudioManager::backgroundSamplePlayer = nullptr;


// Returns the sample of a certain AudioClip
const Sound::Sample &AudioManager::get_sample(AudioClip clip) {
	size_t index = static_cast<size_t>(clip);
	if (index >= Samples->size()) throw std::runtime_error("Trying to get an audio sample outside of the loaded sample size");
	return AudioManager::Samples->at(index);
}


// If background music isn't playing set, start it (and set it to loop)
void AudioManager::ensure_background_is_playing() {
	if (backgroundSamplePlayer != nullptr) return;
	backgroundSamplePlayer = Sound::play_3D(AudioManager::get_sample(AudioManager::AudioClip::BACKGROUND), 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));
	backgroundSamplePlayer->loop = true;
}
