/*
 * Copyright (C) 2010 Google Inc. All rights reserved.
 * Copyright (C) 2011 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "core/html/forms/HiddenInputType.h"

#include "core/HTMLNames.h"
#include "core/InputTypeNames.h"
#include "core/html/FormData.h"
#include "core/html/HTMLInputElement.h"
#include "core/html/forms/FormController.h"

namespace blink {

using namespace HTMLNames;

InputType* HiddenInputType::Create(HTMLInputElement& element) {
  return new HiddenInputType(element);
}

DEFINE_TRACE(HiddenInputType) {
  InputTypeView::Trace(visitor);
  InputType::Trace(visitor);
}

InputTypeView* HiddenInputType::CreateView() {
  return this;
}

const AtomicString& HiddenInputType::FormControlType() const {
  return InputTypeNames::hidden;
}

FormControlState HiddenInputType::SaveFormControlState() const {
  // valueAttributeWasUpdatedAfterParsing() never be true for form
  // controls create by createElement() or cloneNode(). It's ok for
  // now because we restore values only to form controls created by
  // parsing.
  return GetElement().ValueAttributeWasUpdatedAfterParsing()
             ? FormControlState(GetElement().value())
             : FormControlState();
}

void HiddenInputType::RestoreFormControlState(const FormControlState& state) {
  GetElement().setAttribute(valueAttr, AtomicString(state[0]));
}

bool HiddenInputType::SupportsValidation() const {
  return false;
}

LayoutObject* HiddenInputType::CreateLayoutObject(const ComputedStyle&) const {
  NOTREACHED();
  return nullptr;
}

void HiddenInputType::AccessKeyAction(bool) {}

bool HiddenInputType::LayoutObjectIsNeeded() {
  return false;
}

InputType::ValueMode HiddenInputType::GetValueMode() const {
  return ValueMode::kDefault;
}

void HiddenInputType::SetValue(const String& sanitized_value,
                               bool,
                               TextFieldEventBehavior,
                               TextControlSetValueSelection) {
  GetElement().setAttribute(valueAttr, AtomicString(sanitized_value));
}

void HiddenInputType::AppendToFormData(FormData& form_data) const {
  if (DeprecatedEqualIgnoringCase(GetElement().GetName(), "_charset_")) {
    form_data.append(GetElement().GetName(),
                     String(form_data.Encoding().GetName()));
    return;
  }
  InputType::AppendToFormData(form_data);
}

bool HiddenInputType::ShouldRespectHeightAndWidthAttributes() {
  return true;
}

}  // namespace blink
