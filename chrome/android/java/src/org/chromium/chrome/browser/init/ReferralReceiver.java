package org.chromium.chrome.browser.init;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;

/**
 * Created by elvis on 20.01.17.
 */
public class ReferralReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        SharedPreferences sharedPreferences = context.getSharedPreferences("referrer", Context.MODE_PRIVATE);
        sharedPreferences.edit().putString("referrer", intent.getStringExtra("referrer")).apply();
    }
}
