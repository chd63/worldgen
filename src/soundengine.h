#ifndef SoundEngine_H
#define SoundEngine_H
#include <iostream>
#define MINIAUDIO_IMPLEMENTATION
#include "../dependencies/miniaudio.h"


class SoundEngine
{
public:
    ma_result result;
    ma_engine engine;
    const char * location; // should make this a string later
    // for right now 1 sound will be created but 
    // in the future will have sound array
    ma_sound sound;

    SoundEngine()
    {
        ma_result result = ma_engine_init(NULL, &engine);
        if (result != MA_SUCCESS) {
            std::cerr << "Failed to init audio engine" << std::endl;
        }
    }

    // should make one for string instead of char*
    SoundEngine(const char* soundLocation)
    {
        result = ma_engine_init(NULL, &engine);
        if (result != MA_SUCCESS) {
            std::cerr << "Failed to init audio engine" << std::endl;
        }

        location = soundLocation;
        createSound();
    }

    void setSound(const char* soundLocation)
    {
        location = soundLocation;
        createSound();
    }

    void playSound()
    {
        ma_sound_start(&sound);
    }

    ~SoundEngine()
    {
        ma_engine_uninit(&engine);
        ma_sound_uninit(&sound);
    }

private:

    // TODO: add more flag functionality
    void createSound()
    {
        ma_sound_init_from_file(&engine, location, MA_SOUND_FLAG_ASYNC, NULL, NULL, &sound);
        ma_sound_set_volume(&sound, 0.1f);
    }

};

#endif