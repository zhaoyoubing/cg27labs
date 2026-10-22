#pragma once

#include "RenderContext.h"

class RenderPass
{
public:
    virtual ~RenderPass() = default;

    // Called once during renderer initialisation.
    virtual bool init(RenderContext& context) { return true; };

    // Called once per frame.
    virtual void execute(RenderContext& context) = 0;
};