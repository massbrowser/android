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
        SharedPreferences sharedPreferences = context.getSharedPreferences("referral",Context.MODE_PRIVATE);
        sharedPreferences.edit().putString("referral", intent.getStringExtra("referral")).apply();
    }
}
