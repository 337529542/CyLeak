#include "ml.h"

using namespace std;

bool Tracer::Ready = false;

    Tracer::Tracer():_lockCount(0)
    {
            Ready = true;
    }

    Tracer::~Tracer()
    {
            Ready = false;
            Dump();
    }

    void Tracer::Dump()
    {
            if (_map.size() != 0)
            {
                    cout << _map.size() << " memory leaks detected" << endl;
                    for (iterator itor = _map.begin(); itor != _map.end(); ++itor)
                    {
                            char const* file = itor->second.File();
                            int line = itor->second.Line();
                            cout << file << ", " << line << endl;
                    }
            }
    }

    void Tracer::Add(void *p, const char *file, int line)
    {
            if (_lockCount > 0)
                    return;
            Tracer::Lock lock(*this);
            _map[p] = Entry(file, line);
    }

    void Tracer::Remove(void *p)
    {
            if (_lockCount > 0)
                    return;

            Tracer::Lock lock(*this);

            iterator itor = _map.find(p);
            if (itor != _map.end())
                    _map.erase(itor);
    }

    /*extern*/ Tracer gNewTracer;

    void* operator new (size_t size, const char* file, int line)
    {
            void* p = malloc(size);
            if (Tracer::Ready)
                    gNewTracer.Add(p, file, line);
            return p;
    }

    void operator delete(void* p, const char* file, int line)
    {
            if (Tracer::Ready)
                    gNewTracer.Remove(p);
            free(p);
    }

    void* operator new (size_t size)
    {
            void* p = malloc(size);
            if (Tracer::Ready)
                    gNewTracer.Add(p, "?", 0);
            return p;
    }

    void operator delete(void* p)
    {
            if (Tracer::Ready)
                    gNewTracer.Remove(p);
            free(p);
    }

    void* operator new [](size_t size, const char* file, int line)
    {
            void* p = malloc(size);
            if (Tracer::Ready)
                    gNewTracer.Add(p, file, line);
            return p;
    }

    void operator delete[](void* p, const char* file, int line)
    {
            if (Tracer::Ready)
                    gNewTracer.Remove(p);
            cout <<"delete[](void* p, const char* file, int line)" <<endl;
            free(p);
    }

    void* operator new[] (size_t size)
    {
            void* p = malloc(size);
            if (Tracer::Ready)
                    gNewTracer.Add(p, "?", 0);
            return p;
    }

    void operator delete[](void* p)
    {
            if (Tracer::Ready)
                    gNewTracer.Remove(p);
            cout << "delete[](void* p)"<<endl;
            free(p);
    }
