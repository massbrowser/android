// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/**
 * BackgroundComponents.
 * @constructor
 * @struct
 */
function BackgroundComponents() {
  /**
   * @type {!Object<string>}
   */
  this.stringData;

  /**
   * @type {!VolumeManager}
   */
  this.volumeManager;
}

/**
 * File Manager. This definition is required by
 * ui/file_manager/file_manager/foreground/js/ui/file_manager_dialog_base.js.
 *
 * @constructor
 * @struct
 */
function FileManager() {}

/**
 * A global flag which indicates whether it is in tests or not. This is set in
 * gallery/js/background.js.
 *
 * @type {boolean}
 */
Window.prototype.IN_TEST;

/**
 * @type {string}
 */
Entry.prototype.cachedUrl;
