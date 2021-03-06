// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_DOWNLOAD_PUBLIC_CLIENTS_H_
#define COMPONENTS_DOWNLOAD_PUBLIC_CLIENTS_H_

namespace download {

// A list of all clients that are able to make download requests through the
// DownloadService.
// To add a new client, update the metric DownloadService.DownloadClients in
// histograms.xml and make sure to keep this list in sync.  Additions should be
// treated as APPEND ONLY to make sure to keep both UMA metric semantics correct
// but also to make sure the underlying database properly associates each
// download with the right client.
enum class DownloadClient {
  // Represents an uninitialized DownloadClient variable.
  DOWNLOAD_CLIENT_INVALID = 0,

  DOWNLOAD_CLIENT_OFFLINE_PAGE_PREFETCH = 1,

  DOWNLOAD_CLIENT_BOUNDARY = 2,
};

}  // namespace download

#endif  // COMPONENTS_DOWNLOAD_PUBLIC_CLIENTS_H_
