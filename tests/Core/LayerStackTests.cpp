#include "Orion/Core/LayerStack.h"

#include <catch2/catch_test_macros.hpp>

#include <string>
#include <vector>

namespace
{
    using Journal = std::vector<std::string>;

    // A layer that writes "detach <name>" and "~<name>" in a shared journal
    class SpyLayer : public Orion::Layer
    {
    public:
        SpyLayer(std::string name, Journal& journal) : Layer(std::move(name)), m_Journal(journal) {}

        ~SpyLayer() override
        {
            m_Journal.push_back("~" + m_DebugName);
        }

        void OnDetach() override
        {
            m_Journal.push_back("detach " + m_DebugName);
        }

    private:
        Journal& m_Journal;
    };

    Journal NamesOf(const Orion::LayerStack& stack)
    {
        Journal names;
        for (const Orion::Layer* layer : stack)
            names.push_back(layer->GetName());
        return names;
    }
} // namespace

TEST_CASE("Core/LayerStack/LayerStack: layers are kept before overlays, in push order", "[Core][LayerStack]")
{
    Journal journal;
    Orion::LayerStack stack;

    stack.PushLayer(new SpyLayer("L1", journal));
    stack.PushLayer(new SpyLayer("L2", journal));
    stack.PushOverlay(new SpyLayer("O1", journal));
    stack.PushLayer(new SpyLayer("L3", journal));

    CHECK(NamesOf(stack) == Journal{"L1", "L2", "L3", "O1"});
}

TEST_CASE("Core/LayerStack/LayerStack: reverse iteration visits overlays first", "[Core][LayerStack]")
{
    Journal journal;
    Orion::LayerStack stack;

    stack.PushLayer(new SpyLayer("L1", journal));
    stack.PushLayer(new SpyLayer("L2", journal));
    stack.PushOverlay(new SpyLayer("O1", journal));
    stack.PushLayer(new SpyLayer("L3", journal));

    Journal reversed;
    for (auto it = stack.rbegin(); it != stack.rend(); ++it)
        reversed.push_back((*it)->GetName());

    CHECK(reversed == Journal{"O1", "L3", "L2", "L1"});
}

TEST_CASE(
    "Core/LayerStack/LayerStack: popping a layer detaches it, keeps ownership with the caller and keeps the boundary",
    "[Core][LayerStack]")
{
    Journal journal;
    Orion::LayerStack stack;

    auto* l1 = new SpyLayer("L1", journal);
    auto* l2 = new SpyLayer("L2", journal);
    auto* l3 = new SpyLayer("L3", journal);
    auto* o1 = new SpyLayer("O1", journal);

    stack.PushLayer(l1);
    stack.PushLayer(l2);
    stack.PushOverlay(o1);
    stack.PopLayer(l1);

    CHECK(journal == Journal{"detach L1"});
    CHECK(NamesOf(stack) == Journal{"L2", "O1"});

    stack.PushLayer(l3);
    CHECK(NamesOf(stack) == Journal{"L2", "L3", "O1"});

    // A popped layer belongs to the caller again: it must be deleted by hand
    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
    delete l1;
    CHECK(journal == Journal{"detach L1", "~L1"});
}

TEST_CASE("Core/LayerStack/LayerStack: popping something that is not there (or is of the other kind) does nothing",
          "[Core][LayerStack]")
{
    Journal journal;
    Orion::LayerStack stack;

    auto* layer   = new SpyLayer("L1", journal);
    auto* overlay = new SpyLayer("O1", journal);

    stack.PushLayer(layer);
    stack.PushOverlay(overlay);

    SECTION("a layer that was never pushed")
    {
        auto* stranger = new SpyLayer("X", journal);
        stack.PopLayer(stranger);
        stack.PopOverlay(stranger);
        // A popped layer belongs to the caller again: it must be deleted by hand
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        delete stranger;
        CHECK(journal == Journal{"~X"}); // only the delete we did ourselves
    }

    SECTION("an overlay popped as a layer")
    {
        stack.PopLayer(overlay);
        CHECK(journal.empty());
    }

    SECTION("a layer popped as an overlay")
    {
        stack.PopOverlay(layer);
        CHECK(journal.empty());
    }

    CHECK(NamesOf(stack) == Journal{"L1", "O1"});
}

TEST_CASE("Core/LayerStack/LayerStack: the destructor detaches and deletes every layer in order", "[Core][LayerStack]")
{
    Journal journal;
    {
        Orion::LayerStack stack;
        stack.PushLayer(new SpyLayer("A", journal));
        stack.PushOverlay(new SpyLayer("B", journal));
    }
    CHECK(journal == Journal{"detach A", "~A", "detach B", "~B"});
}
