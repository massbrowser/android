// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/android/voice_search_tab_helper.h"

#include "base/command_line.h"
#include "components/google/core/browser/google_util.h"
#include "content/public/browser/render_view_host.h"
#include "content/public/browser/web_contents.h"
#include "content/public/common/web_preferences.h"
#include "media/base/media_switches.h"

DEFINE_WEB_CONTENTS_USER_DATA_KEY(VoiceSearchTabHelper);

// TODO(715588): this class shouldn't be playing with the user gesture
// requirements like this.
VoiceSearchTabHelper::VoiceSearchTabHelper(content::WebContents* contents)
    : content::WebContentsObserver(contents) {
  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  gesture_requirement_for_playback_disabled_ =
      command_line->HasSwitch(switches::kIgnoreAutoplayRestrictionsForTests) ||
      command_line->GetSwitchValueASCII(switches::kAutoplayPolicy) !=
          switches::autoplay::kNoUserGestureRequiredPolicy;
}

VoiceSearchTabHelper::~VoiceSearchTabHelper() {
}

void VoiceSearchTabHelper::NavigationEntryCommitted(
    const content::LoadCommittedDetails& load_details) {
  // In the case where media autoplay has been disabled by default (e.g. in
  // performance media tests) do not update it based on navigation changes.
  if (gesture_requirement_for_playback_disabled_)
    return;

  content::RenderViewHost* host = web_contents()->GetRenderViewHost();
  content::WebPreferences prefs = host->GetWebkitPreferences();

  bool gesture_required =
      !google_util::IsGoogleSearchUrl(web_contents()->GetLastCommittedURL());

  if (gesture_required != prefs.user_gesture_required_for_media_playback) {
    // TODO(chrishtr): this is wrong. user_gesture_required_for_media_playback
    // will be reset the next time a preference changes.
    prefs.user_gesture_required_for_media_playback = gesture_required;
    host->UpdateWebkitPreferences(prefs);
  }
}
