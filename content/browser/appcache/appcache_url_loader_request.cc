// Copyright (c) 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/browser/appcache/appcache_url_loader_request.h"
#include "net/url_request/url_request.h"

namespace content {

// static
AppCacheURLLoaderRequest* AppCacheURLLoaderRequest::Create(
    const ResourceRequest& request) {
  return new AppCacheURLLoaderRequest(request);
}

const GURL& AppCacheURLLoaderRequest::GetURL() const {
  return request_.url;
}

const std::string& AppCacheURLLoaderRequest::GetMethod() const {
  return request_.method;
}

const GURL& AppCacheURLLoaderRequest::GetFirstPartyForCookies() const {
  return request_.first_party_for_cookies;
}

const GURL AppCacheURLLoaderRequest::GetReferrer() const {
  return request_.referrer;
}

bool AppCacheURLLoaderRequest::IsSuccess() const {
  return false;
}

bool AppCacheURLLoaderRequest::IsCancelled() const {
  return false;
}

bool AppCacheURLLoaderRequest::IsError() const {
  return false;
}

int AppCacheURLLoaderRequest::GetResponseCode() const {
  return 0;
}

std::string AppCacheURLLoaderRequest::GetResponseHeaderByName(
    const std::string& name) const {
  return std::string();
}

ResourceRequest* AppCacheURLLoaderRequest::GetResourceRequest() {
  return &request_;
}

AppCacheURLLoaderRequest::AppCacheURLLoaderRequest(
    const ResourceRequest& request)
    : request_(request) {}

AppCacheURLLoaderRequest::~AppCacheURLLoaderRequest() {}

}  // namespace content
