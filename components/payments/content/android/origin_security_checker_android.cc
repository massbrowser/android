// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/payments/content/android/origin_security_checker_android.h"

#include "base/android/jni_string.h"
#include "base/android/scoped_java_ref.h"
#include "components/payments/content/origin_security_checker.h"
#include "content/public/browser/web_contents.h"
#include "jni/OriginSecurityChecker_jni.h"

namespace payments {
namespace {

using ::base::android::JavaParamRef;
using ::base::android::ConvertJavaStringToUTF8;

}  // namespace

// static
jboolean IsOriginSecure(JNIEnv* env,
                        const JavaParamRef<jclass>& jcaller,
                        const JavaParamRef<jstring>& jurl) {
  return OriginSecurityChecker::IsOriginSecure(
      GURL(ConvertJavaStringToUTF8(env, jurl)));
}

// static
jboolean IsSchemeCryptographic(JNIEnv* env,
                               const JavaParamRef<jclass>& jcaller,
                               const JavaParamRef<jstring>& jurl) {
  return OriginSecurityChecker::IsSchemeCryptographic(
      GURL(ConvertJavaStringToUTF8(env, jurl)));
}

// static
jboolean IsOriginLocalhostOrFile(JNIEnv* env,
                                 const JavaParamRef<jclass>& jcaller,
                                 const JavaParamRef<jstring>& jurl) {
  return OriginSecurityChecker::IsOriginLocalhostOrFile(
      GURL(ConvertJavaStringToUTF8(env, jurl)));
}

bool RegisterOriginSecurityChecker(JNIEnv* env) {
  return RegisterNativesImpl(env);
}

}  // namespace payments
