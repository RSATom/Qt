// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "storage/browser/quota/quota_features.h"

namespace storage {

namespace features {

#if defined(OS_CHROMEOS)
// Chrome OS is given a larger fraction, as web content is the considered
// the primary use of the platform. Chrome OS itself maintains free space by
// starting to evict data (old profiles) when less than 1GB remains,
// stopping eviction once 2GB is free.
// Prior to M66 this was 1/3, same as other platforms.
const double kTemporaryPoolSizeRatioThirds = 2.0 / 3.0;  // 66%
#else
const double kTemporaryPoolSizeRatioThirds = 1.0 / 3.0;  // 33%
#endif

const base::Feature kQuotaExpandPoolSize{"QuotaExpandPoolSize",
                                         base::FEATURE_DISABLED_BY_DEFAULT};

const base::FeatureParam<double> kExperimentalPoolSizeRatio{
    &kQuotaExpandPoolSize, "PoolSizeRatio", kTemporaryPoolSizeRatioThirds};

const base::FeatureParam<double> kPerHostRatio{&kQuotaExpandPoolSize,
                                               "PerHostRatio", 0.2};
}  // namespace features
}  // namespace storage
