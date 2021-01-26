// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "tools/gn/frameworks_utils.h"

#include "tools/gn/filesystem_utils.h"

namespace {

// Name of the extension of frameworks.
const char kFrameworkExtension[] = "framework";

}  // anonymous namespace

base::StringPiece GetFrameworkName(const std::string& file) {
  if (FindFilenameOffset(file) != 0)
    return base::StringPiece();

  base::StringPiece extension = FindExtension(&file);
  if (extension != kFrameworkExtension)
    return base::StringPiece();

  return FindFilenameNoExtension(&file);
}
