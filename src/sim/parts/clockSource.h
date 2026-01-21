#pragma once

#include "part.h"

namespace sim
{
    class ClockSource : public Part
    {
    public:
        explicit ClockSource(std::string instanceName, size_t cycleSpeed);
        void changeCycleSpeed(size_t cycleSpeed);

    protected:
        void onReset() override;
        void onTick() override;

    private:
        size_t m_cycleSpeed = 0; // After how many ticks a cycle is triggered
        size_t m_accumulatorCycles = 0; // how many cicles have happened
        bool m_clockHigh = false;
    };
} // namespace sim
