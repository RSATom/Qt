// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "qtwebengine/browser/qtwebengine_renderer_manifest.h"

#include "base/no_destructor.h"
#include "build/build_config.h"
#include "components/spellcheck/spellcheck_buildflags.h"
#include "services/service_manager/public/cpp/manifest_builder.h"

#if BUILDFLAG(ENABLE_SPELLCHECK)
#include "components/spellcheck/common/spellcheck.mojom.h"
#endif

const service_manager::Manifest& GetQtWebEngineRendererManifest()
{
    static base::NoDestructor<service_manager::Manifest> manifest{
        service_manager::ManifestBuilder()
            .WithServiceName("qtwebengine_renderer")
            .WithDisplayName("QtWebEngine Renderer")
#if BUILDFLAG(ENABLE_SPELLCHECK)
            .ExposeCapability("browser",
                              service_manager::Manifest::InterfaceList<spellcheck::mojom::SpellChecker>())
#endif
            .RequireCapability("qtwebengine", "renderer")
            .Build()};
    return *manifest;
}
