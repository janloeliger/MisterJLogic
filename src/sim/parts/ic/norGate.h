#pragma once

#include "../icPart.h"

namespace sim
{
    class NorGate : public IcPart
    {
    public:
        explicit NorGate(std::string instanceName);

    protected:
        void onReset() override;
        void onTick() override;
    };
} // namespace sim
