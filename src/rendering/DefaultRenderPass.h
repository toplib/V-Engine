#pragma once
#include "RenderPass.h"


namespace Rendering {
    class DefaultRenderPass : RenderPass {
    public:
        DefaultRenderPass();
        ~DefaultRenderPass() override;

        bool prepare() override;
        void apply() override;
    };
}
