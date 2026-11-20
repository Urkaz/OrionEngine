#pragma once

// Everything almost every engine file needs: Scope/Ref, BIT, logs, asserts and profiling macros.
// Include this instead of the individual headers (like Unreal's CoreMinimal.h).
// The "IWYU pragma: export" comments tell include-what-you-use tools (misc-include-cleaner)
// that including Base.h provides the symbols of these headers.

#include "Orion/Core/Assert.h"         // IWYU pragma: export
#include "Orion/Core/Core.h"           // IWYU pragma: export
#include "Orion/Core/Log.h"            // IWYU pragma: export
#include "Orion/Profiling/Profiling.h" // IWYU pragma: export
