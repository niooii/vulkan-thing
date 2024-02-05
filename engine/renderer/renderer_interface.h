#ifndef VULKAN_STUFF_RENDERER_INTERFACE_H
#define VULKAN_STUFF_RENDERER_INTERFACE_H

#include "defines.h"

namespace Engine {

    class RendererInterface {
    public:
        virtual ~RendererInterface() = default;

        virtual void Resize(u16 w, u16 h) = 0;

        // If successful, returns true
        virtual bool BeginFrame(f32 delta_time) = 0;

        // If successful, return true
        virtual bool EndFrame(f32 delta_time) = 0;
    };

}


#endif
