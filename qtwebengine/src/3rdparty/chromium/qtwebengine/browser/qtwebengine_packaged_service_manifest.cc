// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "qtwebengine/browser/qtwebengine_packaged_service_manifest.h"

#include "base/no_destructor.h"
#include "build/build_config.h"
#include "components/spellcheck/spellcheck_buildflags.h"
#include "services/proxy_resolver/proxy_resolver_manifest.h"
#include "services/service_manager/public/cpp/manifest_builder.h"

#if BUILDFLAG(ENABLE_SPELLCHECK)
#include "components/spellcheck/common/spellcheck.mojom.h"
#endif

namespace {

const service_manager::Manifest &GetQtWebEngineManifest()
{
    static base::NoDestructor<service_manager::Manifest> manifest {
        service_manager::ManifestBuilder()
            .WithServiceName("qtwebengine")
            .WithDisplayName("QtWebEngine")
            .WithOptions(service_manager::ManifestOptionsBuilder()
                             .WithInstanceSharingPolicy(service_manager::Manifest::InstanceSharingPolicy::kSharedAcrossGroups)
                             .CanConnectToInstancesWithAnyId(true)
                             .CanRegisterOtherServiceInstances(true)
                             .Build())
#if BUILDFLAG(ENABLE_SPELLCHECK)
            .ExposeCapability("renderer",
                              service_manager::Manifest::InterfaceList<spellcheck::mojom::SpellCheckHost>())
#endif
        .RequireCapability("qtwebengine_renderer", "browser")
        .Build()
  };
  return *manifest;
}

}  // namespace

const std::vector<service_manager::Manifest> &GetQtWebEnginePackagedServiceManifests()
{
    static base::NoDestructor<std::vector<service_manager::Manifest>> manifests{{
        GetQtWebEngineManifest(),
        proxy_resolver::GetManifest(),
    }};
    return *manifests;
}
