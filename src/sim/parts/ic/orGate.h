#pragma once

#include "../icPart.h"

namespace sim
{
    class OrGate : public IcPart
    {
    public:
        explicit OrGate(std::string instanceName);

    protected:
        void onReset() override;
        void onTick() override;
    };
} // namespace sim