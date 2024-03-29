// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Contains holistic tests of the bindings infrastructure

#include "chrome/browser/extensions/api/permissions/permissions_api.h"
#include "chrome/browser/extensions/extension_apitest.h"
#include "chrome/browser/net/url_request_mock_util.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/common/chrome_switches.h"
#include "chrome/test/base/ui_test_utils.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/test/browser_test_utils.h"
#include "extensions/browser/extension_host.h"
#include "extensions/browser/process_manager.h"
#include "extensions/test/extension_test_message_listener.h"
#include "extensions/test/result_catcher.h"
#include "net/test/embedded_test_server/embedded_test_server.h"

namespace extensions {
namespace {

class ExtensionBindingsApiTest : public ExtensionApiTest {
 public:
  void SetUpOnMainThread() override {
    ExtensionApiTest::SetUpOnMainThread();
    content::BrowserThread::PostTask(
        content::BrowserThread::IO, FROM_HERE,
        base::BindOnce(&chrome_browser_net::SetUrlRequestMocksEnabled, true));
  }
};

IN_PROC_BROWSER_TEST_F(ExtensionBindingsApiTest,
                       UnavailableBindingsNeverRegistered) {
  // Test will request the 'storage' permission.
  PermissionsRequestFunction::SetIgnoreUserGestureForTests(true);
  ASSERT_TRUE(RunExtensionTest(
      "bindings/unavailable_bindings_never_registered")) << message_;
}

IN_PROC_BROWSER_TEST_F(ExtensionBindingsApiTest,
                       ExceptionInHandlerShouldNotCrash) {
  ASSERT_TRUE(RunExtensionSubtest(
      "bindings/exception_in_handler_should_not_crash",
      "page.html")) << message_;
}

// Tests that an error raised during an async function still fires
// the callback, but sets chrome.runtime.lastError.
// FIXME should be in ExtensionBindingsApiTest.
IN_PROC_BROWSER_TEST_F(ExtensionBrowserTest, LastError) {
  ASSERT_TRUE(LoadExtension(
      test_data_dir_.AppendASCII("browsertest").AppendASCII("last_error")));

  // Get the ExtensionHost that is hosting our background page.
  extensions::ProcessManager* manager =
      extensions::ProcessManager::Get(browser()->profile());
  extensions::ExtensionHost* host = FindHostWithPath(manager, "/bg.html", 1);

  bool result = false;
  ASSERT_TRUE(content::ExecuteScriptAndExtractBool(
      host->render_view_host(), "testLastError()", &result));
  EXPECT_TRUE(result);
}

// Regression test that we don't delete our own bindings with about:blank
// iframes.
IN_PROC_BROWSER_TEST_F(ExtensionBindingsApiTest, AboutBlankIframe) {
  ResultCatcher catcher;
  ExtensionTestMessageListener listener("load", true);

  ASSERT_TRUE(LoadExtension(test_data_dir_.AppendASCII("bindings")
                                          .AppendASCII("about_blank_iframe")));

  ASSERT_TRUE(listener.WaitUntilSatisfied());

  const Extension* extension = LoadExtension(
        test_data_dir_.AppendASCII("bindings")
                      .AppendASCII("internal_apis_not_on_chrome_object"));
  ASSERT_TRUE(extension);
  listener.Reply(extension->id());

  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(ExtensionBindingsApiTest,
                       InternalAPIsNotOnChromeObject) {
  ASSERT_TRUE(RunExtensionSubtest(
      "bindings/internal_apis_not_on_chrome_object",
      "page.html")) << message_;
}

// Tests that we don't override events when bindings are re-injected.
// Regression test for http://crbug.com/269149.
// Regression test for http://crbug.com/436593.
IN_PROC_BROWSER_TEST_F(ExtensionBindingsApiTest, EventOverriding) {
  ASSERT_TRUE(RunExtensionTest("bindings/event_overriding")) << message_;
}

// Tests the effectiveness of the 'nocompile' feature file property.
// Regression test for http://crbug.com/356133.
IN_PROC_BROWSER_TEST_F(ExtensionBindingsApiTest, Nocompile) {
  ASSERT_TRUE(RunExtensionSubtest("bindings/nocompile", "page.html"))
      << message_;
}

IN_PROC_BROWSER_TEST_F(ExtensionBindingsApiTest, ApiEnums) {
  ASSERT_TRUE(RunExtensionTest("bindings/api_enums")) << message_;
};

// Regression test for http://crbug.com/504011 - proper access checks on
// getModuleSystem().
IN_PROC_BROWSER_TEST_F(ExtensionBindingsApiTest, ModuleSystem) {
  ASSERT_TRUE(RunExtensionTest("bindings/module_system")) << message_;
}

IN_PROC_BROWSER_TEST_F(ExtensionBindingsApiTest, NoExportOverriding) {
  ASSERT_TRUE(embedded_test_server()->Start());

  // We need to create runtime bindings in the web page. An extension that's
  // externally connectable will do that for us.
  ASSERT_TRUE(LoadExtension(
      test_data_dir_.AppendASCII("bindings")
                    .AppendASCII("externally_connectable_everywhere")));

  ui_test_utils::NavigateToURL(
      browser(),
      embedded_test_server()->GetURL(
          "/extensions/api_test/bindings/override_exports.html"));

  // See chrome/test/data/extensions/api_test/bindings/override_exports.html.
  std::string result;
  EXPECT_TRUE(content::ExecuteScriptAndExtractString(
      browser()->tab_strip_model()->GetActiveWebContents(),
      "window.domAutomationController.send("
          "document.getElementById('status').textContent.trim());",
      &result));
  EXPECT_EQ("success", result);
}

IN_PROC_BROWSER_TEST_F(ExtensionBindingsApiTest, NoGinDefineOverriding) {
  ASSERT_TRUE(embedded_test_server()->Start());

  // We need to create runtime bindings in the web page. An extension that's
  // externally connectable will do that for us.
  ASSERT_TRUE(LoadExtension(
      test_data_dir_.AppendASCII("bindings")
                    .AppendASCII("externally_connectable_everywhere")));

  ui_test_utils::NavigateToURL(
      browser(),
      embedded_test_server()->GetURL(
          "/extensions/api_test/bindings/override_gin_define.html"));
  ASSERT_FALSE(
      browser()->tab_strip_model()->GetActiveWebContents()->IsCrashed());

  // See chrome/test/data/extensions/api_test/bindings/override_gin_define.html.
  std::string result;
  EXPECT_TRUE(content::ExecuteScriptAndExtractString(
      browser()->tab_strip_model()->GetActiveWebContents(),
      "window.domAutomationController.send("
          "document.getElementById('status').textContent.trim());",
      &result));
  EXPECT_EQ("success", result);
}

IN_PROC_BROWSER_TEST_F(ExtensionBindingsApiTest, HandlerFunctionTypeChecking) {
  ASSERT_TRUE(embedded_test_server()->Start());
  ui_test_utils::NavigateToURL(
      browser(),
      embedded_test_server()->GetURL(
          "/extensions/api_test/bindings/handler_function_type_checking.html"));
  content::WebContents* web_contents =
      browser()->tab_strip_model()->GetActiveWebContents();
  EXPECT_FALSE(web_contents->IsCrashed());
  // See handler_function_type_checking.html.
  std::string result;
  EXPECT_TRUE(content::ExecuteScriptAndExtractString(
      web_contents,
      "window.domAutomationController.send("
          "document.getElementById('status').textContent.trim());",
      &result));
  EXPECT_EQ("success", result);
}

IN_PROC_BROWSER_TEST_F(ExtensionBindingsApiTest,
                       MoreNativeFunctionInterceptionTests) {
  ASSERT_TRUE(embedded_test_server()->Start());

  // We need to create runtime bindings in the web page. An extension that's
  // externally connectable will do that for us.
  ASSERT_TRUE(
      LoadExtension(test_data_dir_.AppendASCII("bindings")
                        .AppendASCII("externally_connectable_everywhere")));

  ui_test_utils::NavigateToURL(
      browser(),
      embedded_test_server()->GetURL(
          "/extensions/api_test/bindings/function_interceptions.html"));
  content::WebContents* web_contents =
      browser()->tab_strip_model()->GetActiveWebContents();
  EXPECT_FALSE(web_contents->IsCrashed());
  // See function_interceptions.html.
  std::string result;
  EXPECT_TRUE(content::ExecuteScriptAndExtractString(
      web_contents, "window.domAutomationController.send(window.testStatus);",
      &result));
  EXPECT_EQ("success", result);
}

class FramesExtensionBindingsApiTest : public ExtensionBindingsApiTest {
 public:
  void SetUpCommandLine(base::CommandLine* command_line) override {
    ExtensionBindingsApiTest::SetUpCommandLine(command_line);
    command_line->AppendSwitch(switches::kDisablePopupBlocking);
  }
};

// This tests that web pages with iframes or child windows pointing at
// chrome-extenison:// urls, both web_accessible and nonexistent pages, don't
// get improper extensions bindings injected while they briefly still point at
// about:blank and are still scriptable by their parent.
//
// The general idea is to load up 2 extensions, one which listens for external
// messages ("receiver") and one which we'll try first faking messages from in
// the web page's iframe, as well as actually send a message from later
// ("sender").
IN_PROC_BROWSER_TEST_F(FramesExtensionBindingsApiTest, FramesBeforeNavigation) {
  // Load the sender and receiver extensions, and make sure they are ready.
  ExtensionTestMessageListener sender_ready("sender_ready", true);
  const Extension* sender = LoadExtension(
      test_data_dir_.AppendASCII("bindings").AppendASCII("message_sender"));
  ASSERT_NE(nullptr, sender);
  ASSERT_TRUE(sender_ready.WaitUntilSatisfied());

  ExtensionTestMessageListener receiver_ready("receiver_ready", false);
  const Extension* receiver =
      LoadExtension(test_data_dir_.AppendASCII("bindings")
                        .AppendASCII("external_message_listener"));
  ASSERT_NE(nullptr, receiver);
  ASSERT_TRUE(receiver_ready.WaitUntilSatisfied());

  // Load the web page which tries to impersonate the sender extension via
  // scripting iframes/child windows before they finish navigating to pages
  // within the sender extension.
  ASSERT_TRUE(embedded_test_server()->Start());
  ui_test_utils::NavigateToURL(
      browser(),
      embedded_test_server()->GetURL(
          "/extensions/api_test/bindings/frames_before_navigation.html"));

  bool page_success = false;
  ASSERT_TRUE(content::ExecuteScriptAndExtractBool(
      browser()->tab_strip_model()->GetWebContentsAt(0), "getResult()",
      &page_success));
  EXPECT_TRUE(page_success);

  // Reply to |sender|, causing it to send a message over to |receiver|, and
  // then ask |receiver| for the total message count. It should be 1 since
  // |receiver| should not have received any impersonated messages.
  sender_ready.Reply(receiver->id());
  int message_count = 0;
  ASSERT_TRUE(content::ExecuteScriptAndExtractInt(
      ProcessManager::Get(profile())
          ->GetBackgroundHostForExtension(receiver->id())
          ->host_contents(),
      "getMessageCountAfterReceivingRealSenderMessage()", &message_count));
  EXPECT_EQ(1, message_count);
}

IN_PROC_BROWSER_TEST_F(ExtensionBindingsApiTest, TestFreezingChrome) {
  ASSERT_TRUE(embedded_test_server()->Start());
  ui_test_utils::NavigateToURL(
      browser(), embedded_test_server()->GetURL(
                     "/extensions/api_test/bindings/freeze.html"));
  content::WebContents* web_contents =
      browser()->tab_strip_model()->GetActiveWebContents();
  ASSERT_FALSE(web_contents->IsCrashed());
}

}  // namespace
}  // namespace extensions
