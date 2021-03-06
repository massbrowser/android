// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_CLIENT_IOS_DOMAIN_CLIENT_SESSION_DETAILS_H_
#define REMOTING_CLIENT_IOS_DOMAIN_CLIENT_SESSION_DETAILS_H_

#import <Foundation/Foundation.h>

@class HostInfo;

// Session states that map to |remoting::protocol::ConnectionToHost::State| with
// the added state of PinPrompt. This can be used to track the state of the
// session for the duration of the connection.
typedef NS_ENUM(NSInteger, SessionState) {
  SessionInitializing,
  SessionConnecting,
  SessionPinPrompt,
  SessionAuthenticated,
  SessionConnected,
  SessionFailed,
  SessionClosed,
};

// The current state of a session and data needed for session context.
@interface ClientSessionDetails : NSObject

// This is the object that describes the host this session is centered around.
@property(nonatomic) HostInfo* hostInfo;
// The current state of the session.
@property(nonatomic, assign) SessionState state;

@end

#endif  //  REMOTING_CLIENT_IOS_DOMAIN_CLIENT_SESSION_DETAILS_H_
