// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_VIZ_COMMON_RESOURCES_RESOURCE_FORMAT_UTILS_VULKAN_H_
#define COMPONENTS_VIZ_COMMON_RESOURCES_RESOURCE_FORMAT_UTILS_VULKAN_H_

#include "components/viz/common/resources/resource_format_utils.h"

#if BUILDFLAG(ENABLE_VULKAN)
#include "third_party/vulkan/include/vulkan/vulkan.h"
#endif

namespace viz {

#if BUILDFLAG(ENABLE_VULKAN)
VIZ_RESOURCE_FORMAT_EXPORT bool HasVkFormat(ResourceFormat format);
VIZ_RESOURCE_FORMAT_EXPORT VkFormat ToVkFormat(ResourceFormat format);
#endif

}  // namespace viz

#endif  // COMPONENTS_VIZ_COMMON_RESOURCES_RESOURCE_FORMAT_UTILS_VULKAN_H_
