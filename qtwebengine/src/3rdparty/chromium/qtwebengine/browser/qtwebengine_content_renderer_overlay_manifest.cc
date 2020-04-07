// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "qtwebengine/browser/qtwebengine_content_renderer_overlay_manifest.h"

#include "base/no_destructor.h"
#include "build/build_config.h"
#include "services/service_manager/public/cpp/manifest_builder.h"

const service_manager::Manifest &GetQtWebEngineContentRendererOverlayManifest()
{
    static base::NoDestructor<service_manager::Manifest> manifest {
        service_manager::ManifestBuilder()
            .Build()
    };
    return *manifest;
}
