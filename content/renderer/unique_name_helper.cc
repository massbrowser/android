// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/renderer/unique_name_helper.h"

#include <vector>

#include "base/containers/adapters.h"
#include "base/logging.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/string_piece.h"
#include "content/renderer/render_frame_impl.h"
#include "content/renderer/render_frame_proxy.h"
#include "third_party/WebKit/public/web/WebLocalFrame.h"

namespace content {

namespace {

const std::string& UniqueNameForFrame(blink::WebFrame* frame) {
  return frame->IsWebLocalFrame()
             ? RenderFrameImpl::FromWebFrame(frame)->unique_name()
             : RenderFrameProxy::FromWebFrame(frame)->unique_name();
}

bool UniqueNameExists(blink::WebFrame* top, const std::string& candidate) {
  // This method is currently O(N), where N = number of frames in the tree.

  // Before recalculating or checking unique name, we set m_uniqueName
  // to an empty string (so the soon-to-be-removed name does not count
  // as a collision).  This means that uniqueNameExists would return
  // false positives when called with an empty |name|.
  DCHECK(!candidate.empty());

  for (blink::WebFrame* frame = top; frame; frame = frame->TraverseNext()) {
    if (UniqueNameForFrame(frame) == candidate)
      return true;
  }

  return false;
}

std::string GenerateCandidate(blink::WebFrame* parent, blink::WebFrame* child) {
  constexpr char kFramePathPrefix[] = "<!--framePath ";
  constexpr int kFramePathPrefixLength = 14;
  constexpr int kFramePathSuffixLength = 3;

  std::string new_name(kFramePathPrefix);

  // Find the nearest parent that has a frame with a path in it.
  std::vector<blink::WebFrame*> chain;
  for (blink::WebFrame* frame = parent; frame; frame = frame->Parent()) {
    base::StringPiece name = UniqueNameForFrame(frame);
    if (name.starts_with(kFramePathPrefix) && name.ends_with("-->") &&
        kFramePathPrefixLength + kFramePathSuffixLength < name.size()) {
      name.substr(kFramePathPrefixLength,
                  name.size() - kFramePathPrefixLength - kFramePathSuffixLength)
          .AppendToString(&new_name);
      break;
    }
    chain.push_back(frame);
  }

  for (auto* frame : base::Reversed(chain)) {
    new_name += '/';
    new_name += UniqueNameForFrame(frame);
  }

  int child_count = 0;
  for (blink::WebFrame* frame = parent->FirstChild(); frame;
       frame = frame->NextSibling()) {
    ++child_count;
  }

  // When generating a candidate, a null |child| means that this is the
  // candidate name for a child frame that's not yet attached.
  new_name += "/<!--frame";
  new_name += base::IntToString(child_count - (child ? 1 : 0));
  new_name += "-->-->";

  // NOTE: This name might not be unique - see http://crbug.com/588800.
  return new_name;
}

std::string GenerateFramePosition(blink::WebFrame* parent,
                                  blink::WebFrame* child) {
  // This method is currently O(N), where N = number of frames in the tree.

  std::string position_string("<!--framePosition");

  while (parent) {
    int position_in_parent = 0;
    blink::WebFrame* sibling = parent->FirstChild();
    while (sibling != child) {
      sibling = sibling->NextSibling();
      ++position_in_parent;
    }

    position_string += '-';
    position_string += base::IntToString(position_in_parent);

    child = parent;
    parent = parent->Parent();
  }

  // NOTE: The generated string is not guaranteed to be unique, but should
  // have a better chance of being unique than the string generated by
  // GenerateCandidate, because we embed extra information into the string:
  // 1) we walk the full chain of ancestors, all the way to the main frame
  // 2) we use frame-position-within-parent (aka |position_in_parent|)
  //    instead of sibling-count.
  return position_string;
}

std::string AppendUniqueSuffix(blink::WebFrame* top,
                               const std::string& prefix,
                               const std::string& likely_unique_suffix) {
  // This should only be called if the |prefix| isn't unique, as this is
  // otherwise pointless work.
  DCHECK(UniqueNameExists(top, prefix));

  // We want unique name to be stable across page reloads - this is why
  // we use a deterministic |number_of_tries| rather than a random number
  // (a random number would be more likely to avoid a collision, but
  // would change after every page reload).
  int number_of_retries = 0;

  // Keep trying |prefix| + |likely_unique_suffix| + |number_of_tries|
  // concatenations until we get a truly unique name.
  std::string candidate(prefix);
  candidate += likely_unique_suffix;
  candidate += '/';
  while (true) {
    size_t current_length = candidate.size();
    candidate += base::IntToString(number_of_retries++);
    candidate += "-->";
    if (!UniqueNameExists(top, candidate))
      break;
    candidate.resize(current_length);
  }
  return candidate;
}

std::string CalculateNewName(blink::WebFrame* parent,
                             blink::WebFrame* child,
                             const std::string& name) {
  blink::WebFrame* top = parent->Top();
  if (!name.empty() && !UniqueNameExists(top, name) && name != "_blank")
    return name;

  std::string candidate = GenerateCandidate(parent, child);
  if (!UniqueNameExists(top, candidate))
    return candidate;

  std::string likely_unique_suffix = GenerateFramePosition(parent, child);
  return AppendUniqueSuffix(top, candidate, likely_unique_suffix);
}

}  // namespace

UniqueNameHelper::UniqueNameHelper(RenderFrameImpl* render_frame)
    : render_frame_(render_frame) {}

UniqueNameHelper::~UniqueNameHelper() {}

std::string UniqueNameHelper::GenerateNameForNewChildFrame(
    blink::WebFrame* parent,
    const std::string& name) {
  return CalculateNewName(parent, nullptr, name);
}

void UniqueNameHelper::UpdateName(const std::string& name) {
  // The unique name of the main frame is always the empty string.
  if (!GetWebFrame()->Parent())
    return;
  // It's important to clear this before calculating a new name, as the
  // calculation checks for collisions with existing unique names.
  unique_name_.clear();
  unique_name_ = CalculateNewName(GetWebFrame()->Parent(), GetWebFrame(), name);
}

blink::WebLocalFrame* UniqueNameHelper::GetWebFrame() const {
  return render_frame_->GetWebFrame();
}

}  // namespace content
