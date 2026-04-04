#pragma once

namespace Rendering {
    class RenderPass {
        RenderPass();
        virtual ~RenderPass();

        virtual bool prepare();
        virtual void apply();
    };
}