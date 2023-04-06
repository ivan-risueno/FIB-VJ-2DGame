#ifndef SOUND_H
#define SOUND_H

#include <irrKlang.h>
#include <string>
using namespace std;
using namespace irrklang;

class Sound {
private:
    ISoundEngine* engine;
    ISound* sound;
public:
    Sound();
    void play(const char* filepath, bool loop);
    void play(const char* filepath, bool loop, bool wait);
    void stop();
    void setVolume(float volume);
    ~Sound();
};

#endif // _SOUND_INCLUDE
