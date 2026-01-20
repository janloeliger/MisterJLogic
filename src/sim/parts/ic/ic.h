#pragma once

#include "../../part.h"

namespace sim
{
    class AndGate : public Part
    {
    public:
        explicit AndGate(std::string instanceName);

    protected:
        void onReset() override;
        void onTick() override;

    private:
    };
} // namespace sim
