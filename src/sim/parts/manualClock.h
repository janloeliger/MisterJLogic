#pragma once

#include "../part.h"

namespace sim
{
    class ManualClock : public Part
    {
    public:
        // Pin:
        // 0 = OUT (Output)
        explicit ManualClock(std::string instanceName);

        // Manual control
        void setHigh();
        void setLow();

        // Creates a pulse:
        // Step 1: OUT goes High
        // Step 2: OUT goes Low
        void pulse();

    protected:
        void onReset() override;
        void onTick() override;

    private:
        void updateOutput();

        bool m_high = false;

        // If true, we are in the middle of a pulse and need to go low next tick
        bool m_pulsePendingLow = false;
    };
} // namespace sim
