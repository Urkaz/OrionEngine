#include "Orion/Core/Log.h"

#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>

namespace
{
    // Runs once per test process, before the first test
    class OrionTestListener : public Catch::EventListenerBase
    {
    public:
        using Catch::EventListenerBase::EventListenerBase;

        void testRunStarting(Catch::TestRunInfo const& /*testRunInfo*/) override
        {
#ifdef OE_ENABLE_LOGS
            Orion::Log::Init();
#endif
        }
    };
} // namespace

CATCH_REGISTER_LISTENER(OrionTestListener)
