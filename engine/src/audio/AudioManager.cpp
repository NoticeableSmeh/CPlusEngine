#include "AudioManager.h"
#include <iostream>

SDL_AudioDeviceID AudioManager::audioDevice = 0;
std::map<std::string, AudioManager::SoundData> AudioManager::sounds;
float AudioManager::masterVolume = 1.0f;
bool AudioManager::initialized = false;

bool AudioManager::initialize()
{
    // Avoid double initialization
    if (initialized)
        return false;
    
    // Open default playback device
    audioDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (audioDevice == 0)
    {
        SDL_Log("Couldn't open audio device: %s", SDL_GetError());
        return false;
    }
    
    initialized = true;
    return true;
}

void AudioManager::shutdown()
{
    // Clear streams and close device
    if (!initialized)
        return;
    
    stopAllSounds();
    
    for (auto& pair : sounds)
    {
        if (pair.second.audioStream)
        {
            SDL_DestroyAudioStream(pair.second.audioStream);
        }
        SDL_free(pair.second.wavData);
    }
    sounds.clear();
    
    if (audioDevice != 0)
    {
        SDL_CloseAudioDevice(audioDevice);
        audioDevice = 0;
    }
    
    initialized = false;
}

bool AudioManager::loadSound(const std::string& name, const std::string& filepath)
{
    // Load WAV and create audio stream
    if (!initialized)
    {
        SDL_Log("AudioManager not initialized!");
        return false;
    }
    
    if (sounds.find(name) != sounds.end())
    {
        return true;
    }
    
    SoundData soundData;
    soundData.isLooping = false;
    
    if (!SDL_LoadWAV(filepath.c_str(), &soundData.audioSpec, 
                     &soundData.wavData, &soundData.wavDataLen))
    {
        SDL_Log("Couldn't load .wav file '%s': %s", filepath.c_str(), SDL_GetError());
        return false;
    }
    
    soundData.audioStream = SDL_CreateAudioStream(&soundData.audioSpec, NULL);
    if (!soundData.audioStream)
    {
        SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
        SDL_free(soundData.wavData);
        return false;
    }
    
    if (!SDL_BindAudioStream(audioDevice, soundData.audioStream))
    {
        SDL_Log("Failed to bind audio stream: %s", SDL_GetError());
        SDL_DestroyAudioStream(soundData.audioStream);
        SDL_free(soundData.wavData);
        return false;
    }
    
    // Cache sound for playback by name
    sounds[name] = soundData;
    return true;
}

void AudioManager::unloadSound(const std::string& name)
{
    auto it = sounds.find(name);
    if (it != sounds.end())
    {
        // Free stream and wav data
        if (it->second.audioStream)
        {
            SDL_DestroyAudioStream(it->second.audioStream);
        }
        SDL_free(it->second.wavData);
        sounds.erase(it);
    }
}

void AudioManager::playSound(const std::string& name, bool loop)
{
    // Start sound playback
    if (!initialized)
        return;
    
    auto it = sounds.find(name);
    if (it == sounds.end())
    {
        SDL_Log("Sound '%s' not loaded!", name.c_str());
        return;
    }
    
    SoundData& sound = it->second;
    sound.isLooping = loop;
    
    // Send audio data to audio stream
    SDL_PutAudioStreamData(sound.audioStream, sound.wavData, sound.wavDataLen);
}

void AudioManager::stopSound(const std::string& name)
{
    auto it = sounds.find(name);
    if (it != sounds.end())
    {
        // Clear stream for individual sound
        SDL_ClearAudioStream(it->second.audioStream);
        it->second.isLooping = false;
    }
}

void AudioManager::stopAllSounds()
{
    for (auto& pair : sounds)
    {
        // Clear all streams and disable loop
        SDL_ClearAudioStream(pair.second.audioStream);
        pair.second.isLooping = false;
    }
}

void AudioManager::setSoundVolume(const std::string& name, float volume)
{
    auto it = sounds.find(name);
    if (it != sounds.end())
    {
        // Clamp value to min/max range
        volume = std::max(0.0f, std::min(1.0f, volume));
        SDL_SetAudioStreamGain(it->second.audioStream, volume * masterVolume);
    }
}

void AudioManager::setMasterVolume(float volume)
{
    // Clamp value to min/max range
    masterVolume = std::max(0.0f, std::min(1.0f, volume));
    
    for (auto& pair : sounds)
    {
        SDL_SetAudioStreamGain(pair.second.audioStream, masterVolume);
    }
}
