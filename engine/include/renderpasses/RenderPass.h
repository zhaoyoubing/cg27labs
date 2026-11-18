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

    // for future support of window resizing
    // void onResize(int w, int h);

protected:
    bool bInited = false;
};