
#include "TDL/sound/Sound.hpp"
#include <alsa/asoundlib.h>
#include <sndfile.h>
#include <vector>
#include <vorbis/vorbisfile.h>
#include <TDL/sound/openAlWrapper.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <condition_variable>
#include <mutex>

namespace tdl {
    Sound::Sound(const std::string &device)
    {
        ALCdevice* _device = alcOpenDevice(nullptr);
        if(!_device)
        {
            std::cerr << "Error: Could not open OpenAL device" << std::endl;
            return;
        }

        ALCcontext* _context;
        if(!alcCall(alcCreateContext, _context, _device, _device, nullptr) || !_context)
        {
            std::cerr << "ERROR: Could not create audio context" << std::endl;
        }

        alcMakeContextCurrent(_context);
        _isPlaying = false;

    }

    Sound::~Sound()
    {
        _playThread.join();
        alcMakeContextCurrent(_context);
        alcDestroyContext(_context);
        alcCloseDevice(_device);
    }

    bool Sound::get_available_devices(std::vector<std::string>& devicesVec, ALCdevice* device)
    {
        const ALCchar* devices;
        if(!alcCall(alcGetString, devices, device, nullptr, ALC_DEVICE_SPECIFIER))
            return false;

        const char* ptr = devices;

        devicesVec.clear();

        do
        {
            devicesVec.push_back(std::string(ptr));
            ptr += devicesVec.back().size() + 1;
        }
        while(*(ptr + 1) != '\0');

        return true;
    }

    void Sound::loadFile(const std::string& filename)
    {
        SF_INFO info;
        SNDFILE* file = sf_open(filename.c_str(), SFM_READ, &info);
        if(file == nullptr)
        {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return;
        }

        std::vector<short> buffer(info.frames * info.channels);
        sf_read_short(file, buffer.data(), buffer.size());
        sf_close(file);

        ALuint bufferID;
        alCall(alGenBuffers, 1, &bufferID);
        alCall(alBufferData, bufferID, info.channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, buffer.data(), buffer.size() * sizeof(short), info.samplerate);

        _buffers.push_back(bufferID);
    }


    void Sound::play()
    {
        _playThread = std::thread(&Sound::playImpl, this);
    }

    void Sound::pause() {
        std::cerr << "pause" << std::endl;
        _isPaused.store(true);
        _isChanged.store(true);
    }

    void Sound::resume() {
        std::cerr << "resume" << std::endl;
        _isPaused.store(false);
        _isChanged.store(true);
    }

    void Sound::playImpl()
    {
        if (!_isPlaying) {
            ALuint sourceID;
            alCall(alGenSources, 1, &sourceID);

            alCall(alSourcei, sourceID, AL_BUFFER, _buffers[0]);
            alCall(alSourcePlay, sourceID);

            ALint source_state;
            do
            {
                alCall(alGetSourcei, sourceID, AL_SOURCE_STATE, &source_state);
                if (!_isChanged.load()) {
                    continue;
                }
                std::cerr <<" paused: " << _isPaused.load() << std::endl;
                if (!_isPaused.load()) {
                    std::cerr << "paused" << std::endl;
                    alCall(alSourcePause, sourceID);
                } else {
                    std::cerr << "playing" << std::endl;
                    alCall(alSourcePlay, sourceID);
                }
                _isChanged.store(false);
            }
            while(source_state != AL_STOPPED);

            alCall(alDeleteSources, 1, &sourceID);
            _isPlaying = true;
        }
    }
}