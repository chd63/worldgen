#include <iostream>
#include "../dependencies/miniaudio.c"

int main() {
    ma_result result;
    ma_engine engine;
    ma_engine_config engineConfig;

    // if you need to allocate on the heap
    // ma_engine* pEngine = malloc(sizeof(*pEngine));

    result = ma_engine_init(NULL, &engine);

    if (result != MA_SUCCESS) {
        return result; 
    }

    // very simple way of playing a sound
    // ma_engine_play_sound(&engine, "../sounds/puppet.mp3", NULL);
    // printf("Press Enter to quit ..\n");
    // getchar();

    ma_sound sound;
    ma_sound sound2;
    //MA_SOUND_FLAG_NO_PITCH -> optimization if not needed for sound
    //MA_SOUND_FLAG_NO_SPATIALIZATION -> optimization if not needed for sound
    // can do this dynamicaly 
    // Dynamically disable or enable spatialization post-initialization:
    //ma_sound_set_spatialization_enabled(&sound, isSpatializationEnabled);
    
    // MA_SOUND_FLAG_DECODE
    // MA_SOUND_FLAG_ASYNC

    // if we want to stream the song to the system we can use MA_SOUND_FLAG_STREAM
    // 2 seconds worth of audio will be stored in memory
    result = ma_sound_init_from_file(&engine, "../sounds/puppet.mp3", MA_SOUND_FLAG_DECODE, NULL, NULL, &sound);
    if(result != MA_SUCCESS){
        return result;
    }

    ma_sound_init_from_file(&engine, "../sounds/aba.mp3", MA_SOUND_FLAG_DECODE, NULL, NULL, &sound2);
    if(result != MA_SUCCESS){
        return result;
    }

    //ma_sound_set_positioning(&sound, ma_positioning_relative);
    ma_sound_set_position(&sound, 0, 0, 0);
    ma_sound_set_position(&sound2, 0, 0, 50);
    //ma_sound_set_direction(&sound, 0, 1, 0);
    ma_sound_set_cone(&sound, 1/2, 1/2, 1);
    ma_sound_set_cone(&sound2, 1/2, 1/2, 1);

    ma_sound_set_start_time_in_pcm_frames(&sound, ma_engine_get_time_in_pcm_frames(&engine) + (ma_engine_get_sample_rate(&engine) * 5));
    ma_sound_start(&sound);
    //ma_sound_start(&sound2);
    printf("Press Enter to quit ..\n");
    getchar();

    //std::cout << ma_engine_get_time_in_pcm_frames(&engine);
  
    ma_sound_uninit(&sound);
    ma_sound_uninit(&sound2);
    ma_engine_uninit(&engine);

    // I mean with this I have enough to create three diffrent sounds
    // and I can try to jumpscare

    return 0;
}