
#ifndef TDL_TERMINAL_DIRECTMEDIA_LIBRARY_SOUNDBUFFER_HPP
    #define TDL_TERMINAL_DIRECTMEDIA_LIBRARY_SOUNDBUFFER_HPP

    #include <AL/al.h>
    #include <AL/alc.h>
    #include <string>
    #include <vector>
    #include <thread>
    #include <condition_variable>
    #include <mutex>
    #include <atomic>

    namespace tdl {
        /**
         * @class Sound
         * @brief Sound class
         * class for sound management
         */
        class Sound {
            public:
            /**
             * @brief Construct a new Sound object
             * 
             * @param device the device to use
             */
                Sound(const std::string &device = "default");

            /**
             * @brief Destroy the Sound object
             * 
             */
                ~Sound();

            /**
             * @brief Get the available devices object
             * 
             * @param devicesVec The vector to store the devices
             * @param device the device to use
             * @return true if the devices are available
             * @return false if the devices are not available
             */
                bool get_available_devices(std::vector<std::string>& devicesVec, ALCdevice* device);

            /**
             * @brief load a audio file
             * 
             * @param filename the filename of the file to load
             */
                void loadFile(const std::string& filename);

            /**
             * @brief play the sound
             * 
             */
                void play();

            /**
             * @brief stop the sound
             * 
             */
                void pause();

            /**
             * @brief resume the sound
             * 
             */
                void resume();

            private:
            /**
             * @brief lauch the thread for the sound
             * 
             */
                void playImpl();

                ALCdevice* _device; /* !< the device to use */
                ALCcontext* _context; /* !< the context to use */
                std::vector<ALuint> _buffers; /* !< the buffers */
                std::thread _playThread; /* !< the thread for the sound */
                std::atomic<bool> _isPaused{false}; /* !< the pause state */
                std::atomic<bool> _isChanged{false}; /* !< the change state */
                bool _isPlaying; /* !< the playing state */
        };
    }

#endif //TDL_TERMINAL_DIRECTMEDIA_LIBRARY_SOUNDBUFFER_HPP
