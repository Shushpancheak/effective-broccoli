#ifndef EFFECTIVE_BROCOLLI_TYPEDEFS
#define EFFECTIVE_BROCOLLI_TYPEDEFS

#ifndef WIN32
#include <cstddef> // for size_t on Linux.
#endif

using TypeID      = size_t;
using EntityID    = size_t;
using ComponentID = size_t;
using SystemID    = size_t;
using EventID     = size_t;

#endif