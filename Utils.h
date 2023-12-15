#pragma once

struct QueueFamilyIndices {
    // if -1, they do not exist.
    int graphicsFamily = -1;

    // check if queue families exist
    bool isValid() {
        return graphicsFamily >= 0;
    }
};