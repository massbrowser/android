package org.chromium.chrome.browser.init.adblocktask;

import android.content.Context;
import android.os.AsyncTask;
import org.chromium.chrome.browser.init.ADBlockUtils;

/**
 * Created by elvis on 23.01.17.
 */
public class DownloadWhiteListTask extends AsyncTask<Void, Void, Long> {
    private static final String WHITELIST_URL = "http://adverttool.ru/whitelist";
    public static final String WHITE_LIST_PART = "WhiteList.part";
    public static final String WHITE_LIST_DAT = "WhiteList.dat";

    private Context mContext;

    public DownloadWhiteListTask(Context mContext) {
        this.mContext = mContext;
    }

    @Override
    protected Long doInBackground(Void... voids) {
        ADBlockUtils.readData(mContext,
                WHITE_LIST_PART,
                WHITELIST_URL,
                ADBlockUtils.ETAG_PREPEND_ADBLOCK, "0",
                WHITE_LIST_DAT, true);
        ADBlockUtils.CreateDownloadedFile(mContext, WHITE_LIST_PART,
                "0", WHITE_LIST_DAT);
        return null;
    }
}
