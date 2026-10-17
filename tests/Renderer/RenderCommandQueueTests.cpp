#include "Orion/Renderer/RenderCommandQueue.h"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <cstring>
#include <iterator>
#include <new>
#include <span>
#include <utility>
#include <vector>

namespace
{
    // A payload that records an id in a shared vector when its command runs
    struct Probe
    {
        std::vector<int>* Order;
        int Id;
    };

    void RecordId(void* payload)
    {
        auto* probe = static_cast<Probe*>(payload);
        probe->Order->push_back(probe->Id);
    }

    void DoNothing(void* /*payload*/) {}

    // Queues a command that records "id" when executed. Returns false if the queue is full.
    bool Push(Orion::RenderCommandQueue& queue, std::vector<int>& order, int id)
    {
        void* payload = queue.Allocate(&RecordId, sizeof(Probe));
        if (payload == nullptr)
            return false;
        new (payload) Probe{.Order = &order, .Id = id}; // the queue only reserves memory: we construct the payload
        return true;
    }

    // Executes whatever is still pending when the scope ends. Without it, a failed REQUIRE unwinds the stack,
    // the queue is destroyed with pending commands and its OE_CORE_ASSERT kills the test process.
    struct DrainOnExit
    {
        Orion::RenderCommandQueue& Queue;
        ~DrainOnExit()
        {
            Queue.Execute();
        }
    };
} // namespace

TEST_CASE("Renderer/CommandQueue/RenderCommandQueue: commands run in the order they were queued",
          "[Renderer][CommandQueue]")
{
    Orion::RenderCommandQueue queue(1024);
    DrainOnExit drain{queue};
    std::vector<int> order;

    REQUIRE(Push(queue, order, 1));
    REQUIRE(Push(queue, order, 2));
    REQUIRE(Push(queue, order, 3));

    CHECK(queue.GetCommandCount() == 3);
    CHECK(queue.GetUsedBytes() > 0);

    queue.Execute();

    CHECK(order == std::vector<int>{1, 2, 3});
    CHECK(queue.GetCommandCount() == 0);
    CHECK(queue.GetUsedBytes() == 0);
}

TEST_CASE("Renderer/CommandQueue/RenderCommandQueue: an empty queue is a no-op and the queue can be reused",
          "[Renderer][CommandQueue]")
{
    Orion::RenderCommandQueue queue(1024);
    DrainOnExit drain{queue};
    std::vector<int> order;

    queue.Execute(); // Nothing queued

    CHECK(order.empty());

    REQUIRE(Push(queue, order, 1));
    REQUIRE(Push(queue, order, 2));

    queue.Execute();

    REQUIRE(Push(queue, order, 3));

    queue.Execute();

    CHECK(order == std::vector<int>{1, 2, 3});
}

TEST_CASE("Renderer/CommandQueue/RenderCommandQueue: every payload is aligned and payloads never overlap",
          "[Renderer][CommandQueue]")
{
    Orion::RenderCommandQueue queue(1024);
    DrainOnExit drain{queue};

    const uint32_t sizes[] = {1, 3, 17, 64, 5};
    std::vector<std::pair<unsigned char*, uint32_t>> payloads;

    for (uint32_t i = 0; i < std::size(sizes); ++i)
    {
        void* payload = queue.Allocate(&DoNothing, sizes[i]);
        REQUIRE(payload != nullptr);
        CHECK(reinterpret_cast<std::uintptr_t>(payload) % Orion::RenderCommandQueue::PayloadAlignment == 0);

        memset(payload, static_cast<int>(i + 1), sizes[i]);
        payloads.emplace_back(static_cast<unsigned char*>(payload), sizes[i]);
    }

    // Check if all payloads have their stored values and overlaps never ocur
    for (size_t i = 0; i < payloads.size(); ++i)
    {
        const std::span<const unsigned char> bytes(payloads[i].first, payloads[i].second);
        for (const unsigned char byte : bytes)
            CHECK(byte == static_cast<unsigned char>(i + 1));
    }

    queue.Execute(); // would jump to garbage if a header had been overwritten
}

TEST_CASE("Renderer/CommandQueue/RenderCommandQueue: a full queue refuses new commands without corrupting itself",
          "[Renderer][CommandQueue]")
{
    SECTION("fills up, refuses, then recovers")
    {
        Orion::RenderCommandQueue queue(256);
        DrainOnExit drain{queue};
        std::vector<int> order;

        uint32_t accepted = 0;
        while (Push(queue, order, static_cast<int>(accepted)))
            ++accepted;

        const uint32_t usedBytes = queue.GetUsedBytes();

        CHECK(accepted > 0);
        CHECK(queue.GetCommandCount() == accepted);
        CHECK(usedBytes <= 256);

        // Check if it accepts another command and changes
        CHECK(queue.Allocate(&RecordId, sizeof(Probe)) == nullptr);
        CHECK(queue.GetCommandCount() == accepted);
        CHECK(queue.GetUsedBytes() == usedBytes);

        queue.Execute();
        CHECK(order.size() == accepted);
        CHECK(Push(queue, order, 99));
    }

    SECTION("a single payload bigger than the capacity is refused")
    {
        Orion::RenderCommandQueue queue(256);
        CHECK(queue.Allocate(&DoNothing, 1024) == nullptr);
        CHECK(queue.GetCommandCount() == 0);
    }
}

namespace
{
    struct Spawner
    {
        Orion::RenderCommandQueue* Queue;
        std::vector<int>* Order;
    };

    void SpawnChild(void* payload)
    {
        auto* spawner = static_cast<Spawner*>(payload);
        spawner->Order->push_back(1);

        void* child = spawner->Queue->Allocate(&RecordId, sizeof(Probe));
        if (child != nullptr)
            new (child) Probe{.Order = spawner->Order, .Id = 2};
    }
} // namespace

TEST_CASE("Renderer/CommandQueue/RenderCommandQueue: commands queued while executing run in the same pass, at the end",
          "[Renderer][CommandQueue]")
{
    Orion::RenderCommandQueue queue(1024);
    DrainOnExit drain{queue};
    std::vector<int> order;

    void* spawnerPayload = queue.Allocate(&SpawnChild, sizeof(Spawner));
    REQUIRE(spawnerPayload != nullptr);
    new (spawnerPayload) Spawner{.Queue = &queue, .Order = &order};
    REQUIRE(Push(queue, order, 3));

    queue.Execute();

    CHECK(order == std::vector<int>{1, 3, 2}); // the child goes after the command that was already queued
    CHECK(queue.GetCommandCount() == 0);
}

namespace
{

    struct ExecutingObserver
    {
        Orion::RenderCommandQueue* Queue;
        bool* ObservedExecuting;
    };

    void ObseveExecuting(void* payload)
    {
        auto* observer              = static_cast<ExecutingObserver*>(payload);
        *observer->ObservedExecuting = observer->Queue->IsExecuting();
    }

} // namespace

TEST_CASE("Renderer/CommandQueue/RenderCommandQueue: IsExecuting is true only while Execute runs",
          "[Renderer][CommandQueue]")
{
    Orion::RenderCommandQueue queue(1024);
    DrainOnExit drain{queue};

    void* payload = queue.Allocate(ObseveExecuting, sizeof(ExecutingObserver));
    REQUIRE(payload != nullptr);

    bool observedExecuting = false;
    new (payload) ExecutingObserver{.Queue = &queue, .ObservedExecuting = &observedExecuting};

    CHECK_FALSE(queue.IsExecuting());
    queue.Execute();
    CHECK_FALSE(queue.IsExecuting());
    CHECK(observedExecuting);
}
