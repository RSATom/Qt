// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_FRAMEWORKS_UTILS_H_
#define TOOLS_GN_FRAMEWORKS_UTILS_H_

#include <string>

#include "base/strings/string_piece.h"

// Returns the name of the framework from a file name. This returns an empty
// string_view if the name is incorrect (does not ends in ".framework", ...).
base::StringPiece GetFrameworkName(const std::string& file);

#endif  // TOOLS_GN_FRAMEWORKS_UTILS_H_
