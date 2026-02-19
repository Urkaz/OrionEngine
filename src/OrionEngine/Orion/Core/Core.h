#pragma once

#include <memory>

namespace Orion {
	void InitializeCore();
	void ShutdownCore();
}

#ifdef OE_ENABLE_ASSERTS
    #define OE_ASSERT(x, ...)                                                                                          \
        {                                                                                                              \
            if (!(x))                                                                                                  \
            {                                                                                                          \
                OE_CORE_LOG(error, "Assertion Failed: {0}", __VA_ARGS__);                                              \
                __debugbreak();                                                                                        \
            }                                                                                                          \
        }
    #define OE_CORE_ASSERT(x, ...)                                                                                     \
        {                                                                                                              \
            if (!(x))                                                                                                  \
            {                                                                                                          \
                OE_CORE_LOG(error, "Assertion Failed: {0}", __VA_ARGS__);                                              \
                __debugbreak();                                                                                        \
            }                                                                                                          \
        }
#else
    #define OE_ASSERT(x, ...)
    #define OE_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define OE_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Orion
{
    template <typename T>
    using Scope = std::unique_ptr<T>;

    template <typename T>
    using Ref = std::shared_ptr<T>;
} // namespace Orion
