#include "oepch.h"
#include "Orion/Core/Layer.h"

namespace Orion
{
    Layer::Layer(std::string name) : m_DebugName(std::move(name)) {}

    Layer::~Layer() = default;
} // namespace Orion
