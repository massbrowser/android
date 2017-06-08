package org.chromium.chrome.browser.coins;

import org.chromium.chrome.R;

/**
 * Created by elvis on 28.11.16.
 */
public enum CoinType {
    MASS_COIN(R.drawable.masscoin_selector, "#"),
    BIT_COIN(R.drawable.bitcoin_selector, "#.####"),
    DOLLAR(R.drawable.dollar_selector, "#.####");


    private int iconRes;
    private String format;
    CoinType(int res, String s) {
        iconRes = res;
        format = s;
    }

    public int getIconRes() {
        return iconRes;
    }

    public String getFormat() {
        return format;
    }

}
