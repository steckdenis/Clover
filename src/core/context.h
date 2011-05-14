#ifndef CONTEXT_H
#define CONTEXT_H

#include "CL/cl.h"

namespace Coal {

    class Context {
    public:
        Context();
        ~Context();

        bool ref();
        bool deref();

    private:
        cl_uint id;
    };

}

struct _cl_context : public Coal::Context
{
};

#endif
