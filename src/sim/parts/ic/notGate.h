#pragma once

#include "Icpart.h"

namespace sim
{
    class NotGate : public IcPart
    {
    public:
        explicit NotGate(std::string instanceName);

    protected:
        void onReset() override;
        void onTick() override;

    private:
        LogicLevel invert(LogicLevel in) const;
    };
} // namespace sim