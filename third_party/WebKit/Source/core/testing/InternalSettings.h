/*
 * Copyright (C) 2012 Google Inc. All rights reserved.
 * Copyright (C) 2013 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef InternalSettings_h
#define InternalSettings_h

#include "core/editing/EditingBehaviorTypes.h"
#include "core/page/Page.h"
#include "core/testing/InternalSettingsGenerated.h"
#include "platform/geometry/IntSize.h"
#include "platform/graphics/ImageAnimationPolicy.h"
#include "platform/heap/Handle.h"
#include "platform/wtf/Allocator.h"
#include "platform/wtf/text/WTFString.h"
#include "public/platform/WebDisplayMode.h"

namespace blink {

class ExceptionState;
class Page;
class Settings;

class InternalSettings final : public InternalSettingsGenerated,
                               public Supplement<Page> {
  USING_GARBAGE_COLLECTED_MIXIN(InternalSettings);
  DEFINE_WRAPPERTYPEINFO();

 public:
  class Backup {
    DISALLOW_NEW();

   public:
    explicit Backup(Settings*);
    void RestoreTo(Settings*);

    bool original_csp_;
    bool original_css_sticky_position_enabled_;
    bool original_overlay_scrollbars_enabled_;
    EditingBehaviorType original_editing_behavior_;
    bool original_text_autosizing_enabled_;
    IntSize original_text_autosizing_window_size_override_;
    float original_accessibility_font_scale_factor_;
    String original_media_type_override_;
    WebDisplayMode original_display_mode_override_;
    bool original_mock_scrollbars_enabled_;
    bool original_mock_gesture_tap_highlights_enabled_;
    bool lang_attribute_aware_form_control_ui_enabled_;
    bool images_enabled_;
    String default_video_poster_url_;
    ImageAnimationPolicy original_image_animation_policy_;
    bool original_scroll_top_left_interop_enabled_;
    bool original_compositor_worker_enabled_;
  };

  static InternalSettings* Create(Page& page) {
    return new InternalSettings(page);
  }
  static InternalSettings* From(Page&);

  ~InternalSettings() override;
  void ResetToConsistentState();

  void setStandardFontFamily(const AtomicString& family,
                             const String& script,
                             ExceptionState&);
  void setSerifFontFamily(const AtomicString& family,
                          const String& script,
                          ExceptionState&);
  void setSansSerifFontFamily(const AtomicString& family,
                              const String& script,
                              ExceptionState&);
  void setFixedFontFamily(const AtomicString& family,
                          const String& script,
                          ExceptionState&);
  void setCursiveFontFamily(const AtomicString& family,
                            const String& script,
                            ExceptionState&);
  void setFantasyFontFamily(const AtomicString& family,
                            const String& script,
                            ExceptionState&);
  void setPictographFontFamily(const AtomicString& family,
                               const String& script,
                               ExceptionState&);

  void setDefaultVideoPosterURL(const String& url, ExceptionState&);
  void setEditingBehavior(const String&, ExceptionState&);
  void setImagesEnabled(bool, ExceptionState&);
  void setMediaTypeOverride(const String& media_type, ExceptionState&);
  void setDisplayModeOverride(const String& display_mode, ExceptionState&);
  void setMockScrollbarsEnabled(bool, ExceptionState&);
  void setHideScrollbars(bool, ExceptionState&);
  void setMockGestureTapHighlightsEnabled(bool, ExceptionState&);
  void setTextAutosizingEnabled(bool, ExceptionState&);
  void setTextTrackKindUserPreference(const String& preference,
                                      ExceptionState&);
  void setAccessibilityFontScaleFactor(float font_scale_factor,
                                       ExceptionState&);
  void setTextAutosizingWindowSizeOverride(int width,
                                           int height,
                                           ExceptionState&);
  void setViewportEnabled(bool, ExceptionState&);
  void setViewportMetaEnabled(bool, ExceptionState&);
  void setViewportStyle(const String& preference, ExceptionState&);
  void setCompositorWorkerEnabled(bool, ExceptionState&);
  void setPresentationReceiver(bool, ExceptionState&);

  // FIXME: The following are RuntimeEnabledFeatures and likely
  // cannot be changed after process start. These setters should
  // be removed or moved onto internals.runtimeFlags:
  void setCSSStickyPositionEnabled(bool);
  void setLangAttributeAwareFormControlUIEnabled(bool);
  void setOverlayScrollbarsEnabled(bool);
  void setExperimentalContentSecurityPolicyFeaturesEnabled(bool);
  void setImageAnimationPolicy(const String&, ExceptionState&);
  void setScrollTopLeftInteropEnabled(bool);

  DECLARE_VIRTUAL_TRACE();

  void setAvailablePointerTypes(const String&, ExceptionState&);
  void setPrimaryPointerType(const String&, ExceptionState&);
  void setAvailableHoverTypes(const String&, ExceptionState&);
  void setPrimaryHoverType(const String&, ExceptionState&);
  void SetDnsPrefetchLogging(bool, ExceptionState&);
  void SetPreloadLogging(bool, ExceptionState&);

 private:
  explicit InternalSettings(Page&);

  Settings* GetSettings() const;
  Page* GetPage() const { return GetSupplementable(); }
  static const char* SupplementName();

  Backup backup_;
};

}  // namespace blink

#endif  // InternalSettings_h
