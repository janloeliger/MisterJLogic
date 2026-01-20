#pragma once

#include "circuit.h"

#include <cstddef>

namespace sim
{
    class Simulator
    {
    public:
        explicit Simulator(Circuit* circuit);

        // Controls
        void reset();
        void step();

        // Settling control
        void setMaxSettleIterations(size_t iterations)
        {
            if (iterations == 0)
            {
                iterations = 1;
            }

            m_maxSettleIterations = iterations;
        }

        size_t maxSettleIterations() const
        {
            return m_maxSettleIterations;
        }

    private:
        Circuit* m_circuit = nullptr;
        size_t m_maxSettleIterations = 8;
    };
} // namespace sim
