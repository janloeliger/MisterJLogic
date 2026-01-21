#pragma once

#include "icPart.h"

namespace sim
{
    class NandGate : public IcPart
    {
    public:
        explicit NandGate(std::string instanceName);

    protected:
        void onReset() override;
        void onTick() override;
    };
} // namespace sim
