#ifndef EFFECTIVE_BROCOLLI_TYPEDEFS
#define EFFECTIVE_BROCOLLI_TYPEDEFS

#ifndef WIN32
#include <cstddef> // for size_t on Linux.
#endif

#include <cstdint>

using TypeID      = uint32_t;
using EntityID    = TypeID;
using ComponentID = TypeID;
using SystemID    = TypeID;
using EventID     = TypeID;

#endif