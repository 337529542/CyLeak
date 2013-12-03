#ifndef __MEMORY_LEAK_H__
#define __MEMORY_LEAK_H__
#include <stdlib.h>
#include <iostream>
#include <map>
#include <string>
#include <string.h>
#include <vector>

class Tracer
{
private:
    class Entry
    {
    public:
        Entry (char const * file, int line)
            : _file (file), _line (line)
        {}
        Entry ()
            : _file (0), _line (0)
        {}
        char const * File () const { return _file; }
        int Line () const { return _line; }
    private:
        char const * _file;
        int _line;
    };

    class Lock
    {
    public:
        Lock (Tracer & tracer)
            : _tracer (tracer)
        {
            _tracer.lock ();
        }
        ~Lock ()
        {
            _tracer.unlock ();
        }
    private:
        Tracer & _tracer;
    };



public:
    Tracer ();
    ~Tracer ();
    void Add (void * p, char const * file, int line);
    void Remove (void * p);
    void Dump ();

    static bool Ready;

private:
    void lock () { _lockCount++; }
    void unlock () { _lockCount--; }

private:
    typedef std::map<void *, Entry>::iterator iterator;
    std::map<void *, Entry> _map;
    int _lockCount;
};

extern Tracer gNewTracer;

#endif


