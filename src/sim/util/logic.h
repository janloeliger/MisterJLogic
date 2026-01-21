#pragma once

#include "../parts/part.h"   // for sim::LogicLevel

namespace sim
{
    // Converts a LogicLevel to a readable C-string.
    // Returned pointer is valid for the lifetime of the program.
    const char* logicLevelToStr(LogicLevel v) noexcept;
} // namespace sim
