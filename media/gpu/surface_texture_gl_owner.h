// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_GPU_SURFACE_TEXTURE_GL_OWNER_H_
#define MEDIA_GPU_SURFACE_TEXTURE_GL_OWNER_H_

#include "base/threading/thread_checker.h"
#include "media/gpu/media_gpu_export.h"
#include "ui/gl/android/surface_texture.h"
#include "ui/gl/gl_bindings.h"
#include "ui/gl/gl_context.h"
#include "ui/gl/gl_surface.h"

namespace media {

// Handy subclass of SurfaceTexture which creates and maintains ownership of the
// GL texture also.  This texture is only destroyed with the object; one may
// ReleaseSurfaceTexture without destroying the GL texture.  It must be deleted
// on the same thread on which it is constructed.
class MEDIA_GPU_EXPORT SurfaceTextureGLOwner : public gl::SurfaceTexture {
 public:
  // Must be called with a platform gl context current.  Creates the GL texture
  // using this context, and memorizes it to delete the texture later.
  static scoped_refptr<SurfaceTextureGLOwner> Create();

  // Return the GL texture id that we created.
  GLuint texture_id() const { return texture_id_; }

  gl::GLContext* context() const { return context_.get(); }
  gl::GLSurface* surface() const { return surface_.get(); }

  // We don't support these.  In principle, we could, but they're fairly buggy
  // anyway on many devices.
  void AttachToGLContext() override;
  void DetachFromGLContext() override;

 private:
  SurfaceTextureGLOwner(GLuint texture_id);
  ~SurfaceTextureGLOwner() override;

  // Context and surface that were used to create |texture_id_|.
  scoped_refptr<gl::GLContext> context_;
  scoped_refptr<gl::GLSurface> surface_;

  GLuint texture_id_;

  base::ThreadChecker thread_checker_;
};

}  // namespace media

#endif  // MEDIA_GPU_SURFACE_TEXTURE_GL_OWNER_H_
