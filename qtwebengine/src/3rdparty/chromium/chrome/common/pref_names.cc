// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/common/pref_names.h"

namespace prefs {
const char kAcceptLanguages[] = "intl.accept_languages";

// Integer that holds the value of the next persistent notification ID to be
// used.
const char kNotificationNextPersistentId[] = "persistent_notifications.next_id";

// Boolean that indicates whether chrome://accessibility should show the
// internal accessibility tree.
const char kShowInternalAccessibilityTree[] =
    "accessibility.show_internal_accessibility_tree";

// Additional features for image labels for accessibility.
const char kAccessibilityImageLabelsEnabled[] =
    "settings.a11y.enable_accessibility_image_labels";

// Whether the opt-in dialog for image labels has been accepted yet. The opt-in
// need not be shown every time if it has already been accepted once.
const char kAccessibilityImageLabelsOptInAccepted[] =
    "settings.a11y.enable_accessibility_image_labels_opt_in_accepted";

}  // namespace prefs
