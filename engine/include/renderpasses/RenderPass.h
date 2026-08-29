#pragma once

#include "RenderContext.h"

class RenderPass
{
public:
    virtual ~RenderPass() = default;

    // Called once during renderer initialisation.
    virtual bool init() = 0;

    // Called once per frame.
    virtual void execute(RenderContext& context) = 0;
};