// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QTWEBENGINE_PACKAGED_SERVICE_MANIFESTS_H
#define QTWEBENGINE_PACKAGED_SERVICE_MANIFESTS_H

#include <vector>

#include "services/service_manager/public/cpp/manifest.h"

const std::vector<service_manager::Manifest> &GetQtWebEnginePackagedServiceManifests();

#endif  // QTWEBENGINE_PACKAGED_SERVICE_MANIFESTS_H
