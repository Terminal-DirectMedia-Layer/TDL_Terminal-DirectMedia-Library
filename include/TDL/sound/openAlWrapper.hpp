#include <AL/al.h>
#include <AL/alc.h>
#include <iostream>
#include <string>
#include <cstdint>
#include <type_traits>

/**
 * @brief check if there is an error in the openAL context
 * 
 * @param filename the filename where the error is
 * @param line the line where the error is
 * @return true if there is not an error
 * @return false if there is an error
 */
bool check_al_errors(const std::string& filename, const std::uint_fast32_t line)
{
    ALenum error = alGetError();
    if(error != AL_NO_ERROR)
    {
        std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n" ;
        switch(error)
        {
        case AL_INVALID_NAME:
            std::cerr << "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
            break;
        case AL_INVALID_ENUM:
            std::cerr << "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
            break;
        case AL_INVALID_VALUE:
            std::cerr << "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
            break;
        case AL_INVALID_OPERATION:
            std::cerr << "AL_INVALID_OPERATION: the requested operation is not valid";
            break;
        case AL_OUT_OF_MEMORY:
            std::cerr << "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
            break;
        default:
            std::cerr << "UNKNOWN AL ERROR: " << error;
        }
        std::cerr << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief call an openAL function and check if there is an error
 * 
 * @tparam alFunction OpenAl Function to call
 * @tparam Params Parameters of the function
 * @param filename filename where the function is called
 * @param line line where the function is called
 * @param function function to call
 * @param params parameters of the function
 * @return std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))> the return value of the function
 * @note this template will alwais be calles if the function return a value
 */
template<typename alFunction, typename... Params>
auto alCallImpl(const char* filename,
                const std::uint_fast32_t line,
                alFunction function,
                Params... params)
    ->typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))>
{
    auto ret = function(std::forward<Params>(params)...);
    check_al_errors(filename, line);
    return ret;
}

/**
 * @brief 
 * 
 * @tparam alFunction the OpenAL function to call
 * @tparam Params the parameters of the function
 * @param filename the filename where the function is called
 * @param line the line where the function is called
 * @param function the function to call
 * @param params the parameters of the function
 * @return std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool> true if there is no error
 */
template<typename alFunction, typename... Params>
auto alCallImpl(const char* filename,
                const std::uint_fast32_t line,
                alFunction function,
                Params... params)
    ->typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>
{
    function(std::forward<Params>(params)...);
    return check_al_errors(filename, line);
}

#define alCall(function, ...) alCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)

#define alcCall(function, device, ...) alcCallImpl(__FILE__, __LINE__, function, device, ##__VA_ARGS__)

/**
 * @brief check if there is an error in the openAL context
 * 
 * @param filename Filename where the error is
 * @param line Line where the error is
 * @param device the openAL device
 * @return true if there is no error
 * @return false if there is an error
 */
bool check_alc_errors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device)
{
    ALCenum error = alcGetError(device);
    if(error != ALC_NO_ERROR)
    {
        std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n" ;
        switch(error)
        {
        case ALC_INVALID_VALUE:
            std::cerr << "ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function";
            break;
        case ALC_INVALID_DEVICE:
            std::cerr << "ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function";
            break;
        case ALC_INVALID_CONTEXT:
            std::cerr << "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function";
            break;
        case ALC_INVALID_ENUM:
            std::cerr << "ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function";
            break;
        case ALC_OUT_OF_MEMORY:
            std::cerr << "ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function";
            break;
        default:
            std::cerr << "UNKNOWN ALC ERROR: " << error;
        }
        std::cerr << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief call an openAL function and check if there is an error
 * 
 * @tparam alcFunction OpenAlC Function to call
 * @tparam Params Parameters of the function
 * @param filename filename where the function is called
 * @param line line where the function is called
 * @param function function to call
 * @param device the openALC device
 * @param params parameters of the function
 * @return std::enable_if_t<std::is_same_v<void,decltype(function(params...))>,bool> true if there is no error
 */
template<typename alcFunction, typename... Params>
auto alcCallImpl(const char* filename, 
                 int line, 
                 alcFunction function, 
                 ALCdevice* device, 
                 Params... params)
->typename std::enable_if_t<std::is_same_v<void,decltype(function(params...))>,bool>
{
    function(std::forward<Params>(params)...);
    return check_alc_errors(filename,line,device);
}

/**
 * @brief call an openAL function and check if there is an error
 * 
 * @tparam alcFunction OpenAlC Function to call
 * @tparam ReturnType the return type of the function
 * @tparam Params Parameters of the function
 * @param filename filename where the function is called
 * @param line line where the function is called
 * @param function function to call
 * @param device the openALC device
 * @param params parameters of the function
 * @return std::enable_if_t<!std::is_same_v<void,decltype(function(params...))>,bool> true if there is no error
 */
template<typename alcFunction, typename ReturnType, typename... Params>
auto alcCallImpl(const char* filename,
                 int line,
                 alcFunction function,
                 ReturnType& returnValue,
                 ALCdevice* device, 
                 Params... params)
->typename std::enable_if_t<!std::is_same_v<void,decltype(function(params...))>,bool>
{
    returnValue = function(std::forward<Params>(params)...);
    return check_alc_errors(filename,line,device);
}