// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_WEBKIT_SOURCE_PLATFORM_SCHEDULER_CHILD_WEB_TASK_RUNNER_IMPL_H_
#define THIRD_PARTY_WEBKIT_SOURCE_PLATFORM_SCHEDULER_CHILD_WEB_TASK_RUNNER_IMPL_H_

#include <memory>

#include "base/callback.h"
#include "base/macros.h"
#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "platform/PlatformExport.h"
#include "platform/WebTaskRunner.h"

namespace blink {
namespace scheduler {
class TaskQueue;

class PLATFORM_EXPORT WebTaskRunnerImpl : public WebTaskRunner {
 public:
  static RefPtr<WebTaskRunnerImpl> Create(scoped_refptr<TaskQueue> task_queue);

  // WebTaskRunner implementation:
  void PostDelayedTask(const WebTraceLocation&,
                       base::OnceClosure,
                       double delay_ms) override;
  bool RunsTasksOnCurrentThread() override;
  double VirtualTimeSeconds() const override;
  double MonotonicallyIncreasingVirtualTimeSeconds() const override;
  base::SingleThreadTaskRunner* ToSingleThreadTaskRunner() override;

  TaskQueue* GetTaskQueue() const { return task_queue_.get(); }

 private:
  explicit WebTaskRunnerImpl(scoped_refptr<TaskQueue> task_queue);
  ~WebTaskRunnerImpl() override;

  base::TimeTicks Now() const;

  scoped_refptr<TaskQueue> task_queue_;

  DISALLOW_COPY_AND_ASSIGN(WebTaskRunnerImpl);
};

}  // namespace scheduler
}  // namespace blink

#endif  // THIRD_PARTY_WEBKIT_SOURCE_PLATFORM_SCHEDULER_CHILD_WEB_TASK_RUNNER_IMPL_H_
