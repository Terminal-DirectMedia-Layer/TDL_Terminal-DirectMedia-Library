#pragma once

#include <cstddef>
#include <stdexcept>

#include <stdlib.h>

class ArenaAllocator {
public:
    ArenaAllocator(size_t size) : _size(size), _offset(0) {
        _memory = new char[size];
    }

    ~ArenaAllocator() {
        delete[] _memory;
    }

    void * operator new(size_t size) {
        return aligned_alloc(alignof(ArenaAllocator), size);
    }

    void operator delete(void* ptr) {
        free(ptr);
    }

    void* allocate(size_t size) {
        if (_offset + size > _size) {
            throw std::bad_alloc();
        }
        void* ptr = _memory + _offset;
        _offset += size;
        return ptr;
    }

    void deallocate(void* ptr, size_t size) {
        // Only deallocate if the pointer is the most recently allocated block
        if (ptr == _memory + _offset - size) {
            _offset -= size;
        }
    }

    void reset() {
        _offset = 0;
    }

private:
    char* _memory;
    size_t _size;
    size_t _offset;
};