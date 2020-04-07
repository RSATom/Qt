// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "qtwebengine/browser/qtwebengine_content_renderer_overlay_manifest.h"

#include "base/no_destructor.h"
#include "build/build_config.h"
#include "extensions/buildflags/buildflags.h"
#include "services/service_manager/public/cpp/manifest_builder.h"

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "extensions/common/mojom/guest_view.mojom.h"
#endif

const service_manager::Manifest &GetQtWebEngineContentRendererOverlayManifest()
{
    static base::NoDestructor<service_manager::Manifest> manifest {
        service_manager::ManifestBuilder()
#if BUILDFLAG(ENABLE_EXTENSIONS)
            .ExposeInterfaceFilterCapability_Deprecated(
                "navigation:frame", "browser",
                service_manager::Manifest::InterfaceList<
                    extensions::mojom::MimeHandlerViewContainerManager>())
#endif
            .Build()
    };
    return *manifest;
}
