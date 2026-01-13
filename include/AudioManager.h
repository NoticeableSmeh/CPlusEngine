#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <SDL3/SDL.h>
#include <string>
#include <map>

class AudioManager
{
public:
    // Initialize and shut down audio system
    static bool initialize();
    static void shutdown();
    

    // Load, play and manage sounds
    static bool loadSound(const std::string& name, const std::string& filepath);
    static void unloadSound(const std::string& name);
    
    static void playSound(const std::string& name, bool loop = false);
    static void stopSound(const std::string& name);
    static void stopAllSounds();
    
    static void setSoundVolume(const std::string& name, float volume);
    static void setMasterVolume(float volume);
    
    AudioManager() = delete;
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    
private:
    struct SoundData
    {
        Uint8* wavData;              // Raw audio data
        Uint32 wavDataLen;           // Length of audio data
        SDL_AudioStream* audioStream;// Stream connected to device
        SDL_AudioSpec audioSpec;     // Format/specification for audio
        bool isLooping;              // Whether the sound should loop
    };
    
    static SDL_AudioDeviceID audioDevice;               // Active audio device
    static std::map<std::string, SoundData> sounds;     // Sound cache per name
    static float masterVolume;                          // Volume
    static bool initialized;                            // Init flag
};

#endif
