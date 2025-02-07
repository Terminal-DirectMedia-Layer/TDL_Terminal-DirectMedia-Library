
#ifndef DYNLIBLOADER_HPP
    #define DYNLIBLOADER_HPP

#include <string>
#include <dlfcn.h>
#include <iostream>

class DynLibLoader {
    public:
        DynLibLoader(const std::string &path) {
            handle = dlopen(path.c_str(), RTLD_LAZY);
            if (!handle) {
                std::cerr << "Error: " << dlerror() << std::endl;
            }
        }

        ~DynLibLoader() {
            dlclose(handle);
        }

        template <typename T>
        T getFunction(const std::string &name) {
            void *function = dlsym(handle, name.c_str());
            if (!function) {
                std::cerr << "Error: " << dlerror() << std::endl;
            }
            return reinterpret_cast<T>(function);
        }

    private:
        void *handle;
};

#endif //DYNLIBLOADER_HPP
