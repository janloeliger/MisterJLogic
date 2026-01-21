#pragma once

#include "../part.h"

namespace sim
{
    class NotGate : public Part
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
