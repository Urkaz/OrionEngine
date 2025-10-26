#include "oepch.h"
#include "Layer.h"

namespace OrionEngine
{
    Layer::Layer(std::string name) : m_DebugName(std::move(name)) {}

    Layer::~Layer() = default;
} // namespace OrionEngine
