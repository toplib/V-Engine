#pragma once

namespace Rendering {
    class RenderPass {
    public:
        RenderPass();
        virtual ~RenderPass();

        virtual bool prepare();
        virtual void apply();
    };
}