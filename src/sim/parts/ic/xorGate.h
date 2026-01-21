#pragma once

#include "icPart.h"

namespace sim
{
    class XorGate : public IcPart
    {
    public:
        explicit XorGate(std::string instanceName);

    protected:
        void onReset() override;
        void onTick() override;
    };
} // namespace sim
