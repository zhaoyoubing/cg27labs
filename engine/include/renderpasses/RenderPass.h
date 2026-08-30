#pragma once

#include "RenderContext.h"

class RenderPass
{
public:
    virtual ~RenderPass() = default;

    // Called once during renderer initialisation.
    bool init() { };

    // Called once per frame.
    virtual void execute(RenderContext& context) = 0;
};