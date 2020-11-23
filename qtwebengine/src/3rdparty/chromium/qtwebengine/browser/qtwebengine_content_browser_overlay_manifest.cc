// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "qtwebengine/browser/qtwebengine_content_browser_overlay_manifest.h"

#include "base/no_destructor.h"
#include "build/build_config.h"
#include "extensions/buildflags/buildflags.h"
#include "services/service_manager/public/cpp/manifest_builder.h"

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "extensions/common/api/mime_handler.mojom.h"
#include "extensions/common/mojom/keep_alive.mojom.h"
#endif

const service_manager::Manifest &GetQtWebEngineContentBrowserOverlayManifest()
{
    static base::NoDestructor<service_manager::Manifest> manifest {
        service_manager::ManifestBuilder()
#if BUILDFLAG(ENABLE_EXTENSIONS)
            .ExposeInterfaceFilterCapability_Deprecated(
                "navigation:frame", "renderer",
                service_manager::Manifest::InterfaceList<
                    extensions::KeepAlive,
                    extensions::mime_handler::BeforeUnloadControl,
                    extensions::mime_handler::MimeHandlerService
                >())
#endif
            .Build()
    };
    return *manifest;
}
