#pragma once

#include "icPart.h"

namespace sim
{
    class AndGate : public IcPart
    {
    public:
        explicit AndGate(std::string instanceName);

    protected:
        void onReset() override;
        void onTick() override;
    };
} // namespace sim
