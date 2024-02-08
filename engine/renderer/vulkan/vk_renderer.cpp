#include "vk_renderer.h"

namespace Engine::Vulkan {

    VkRenderer::VkRenderer(const char* application_name, Window& window, bool validation_layers_enabled) {

        // init instance
        opt_instance_.emplace(application_name, "onion engine", window, validation_layers_enabled);
        Instance& instance = opt_instance_.value();

        opt_surface_.emplace(
                instance, window
                );
        Surface& surface = opt_surface_.value();

        std::vector<PhysicalDevice> physical_devices = DeviceUtils::GetPhysicalDevices(instance);

        assert(!physical_devices.empty());
        spdlog::debug("Using device {}.", physical_devices[0].name());
        opt_device_.emplace(
                instance, surface, physical_devices[0]
                );
        Device& device = opt_device_.value();

        opt_swapchain_.emplace(
                device, instance, surface, window
                );
        Swapchain& swapchain = opt_swapchain_.value();

        opt_graphics_pipeline_.emplace(
                /// may havae to remove
                device, swapchain
                );
        GraphicsPipeline& graphics_pipeline = opt_graphics_pipeline_.value();
        swapchain.CreateFramebuffers(graphics_pipeline.render_pass());

        opt_cmd_recorder_.emplace(
                device, swapchain, graphics_pipeline
                );
        CommandBufferRecorder& cmd_recorder = opt_cmd_recorder_.value();

        instance_ = &instance;
        device_ = &device;
        swapchain_ = &swapchain;
        graphics_pipeline_ = &graphics_pipeline;
        cmd_recorder_ = &cmd_recorder;

        spdlog::info("Finished renderer initialization.");
    }

    VkRenderer::~VkRenderer() {
        spdlog::info("Shutting down renderer...");
        vkDeviceWaitIdle(device_->vk_device());
    }

    void VkRenderer::Resize(u16 w, u16 h) {

    }

    bool VkRenderer::BeginFrame(f32 delta_time) {
        return true;
    }

    bool VkRenderer::EndFrame(f32 delta_time) {
        return true;
    }

    void VkRenderer::DrawFrame() {
        spdlog::info("start draw frame fn");

        // wait until previous frame is finished
        VkFence inflight_fence = swapchain_->vk_inflight_fence();
        vkWaitForFences(device_->vk_device(), 1, &inflight_fence, VK_TRUE, 2000000000);
        vkResetFences(device_->vk_device(), 1, &inflight_fence);

        cmd_recorder_->ResetCommandBuffers();
        uint32_t image_idx = swapchain_->AcquireNextImageIdx();
        cmd_recorder_->RecordCommandBuffers(image_idx);

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {swapchain_->vk_image_available_sempahore()};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = waitSemaphores;
        submit_info.pWaitDstStageMask = waitStages;

        submit_info.commandBufferCount = 1;
        VkCommandBuffer cmd_buffer = cmd_recorder_->vk_cmd_buffer();
        submit_info.pCommandBuffers = &cmd_buffer;

        VkSemaphore signal_semaphores[] = {swapchain_->vk_render_finished_semaphore()};
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = signal_semaphores;

        Utils::ExpectBadResult(
                "Failed to submit cmd buffer to queue",
                vkQueueSubmit(device_->graphics_queue_handle, 1, &submit_info, swapchain_->vk_inflight_fence())
                );

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;

        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;

        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkPresentInfoKHR present_info{};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = signal_semaphores;

        VkSwapchainKHR swapchains[] = {swapchain_->vk_swapchain()};
        present_info.swapchainCount = 1;
        present_info.pSwapchains = swapchains;
        present_info.pImageIndices = &image_idx;

        present_info.pResults = nullptr;

        vkQueuePresentKHR(device_->present_queue_handle, &present_info);

//        VkSemaphoreWaitInfo wait_info{};
//        wait_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO;
//        wait_info.semaphoreCount = 1;
//        VkSemaphore wait_on = swapchain_->vk_render_finished_semaphore();
//        wait_info.pSemaphores = &wait_on;
//
//        VkResult res = vkWaitSemaphores(device_->vk_device(), &wait_info, 2000000000);
//        Utils::ExpectBadResult("something", res);
        spdlog::info("end draw frame fn");
    }

}
