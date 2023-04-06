#include <irrKlang.h>
#include "Sound.h"
#pragma comment(lib, "irrKlang.lib")

using namespace irrklang;


Sound::Sound() {
    engine = createIrrKlangDevice();
}


void Sound::play(const char* filepath, bool loop) {
        sound = engine->play2D(filepath, loop, false);
}

void Sound::play(const char* filepath, bool loop, bool wait) {
    if (!wait) {
        engine->stopAllSounds();
        sound = engine->play2D(filepath, loop, false);
    }
}


void Sound::stop() {
    engine->stopAllSounds();
}

void Sound::setVolume(float volume) {
    sound->setVolume(volume);
}

Sound::~Sound() {
    engine->drop();
}

