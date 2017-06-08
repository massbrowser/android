package org.chromium.chrome.browser.init.tasks;

import android.content.Context;
import android.os.AsyncTask;
import org.chromium.chrome.browser.coins.CoinsSingleton;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

/**
 * Created by elvis on 09.01.17.
 */
public class GetUsersCountTask extends AsyncTask<Void, Void, Void> {
    public static final String URL_USER_COUNT = GetCurrencyTask.HOST + "userscount.php";

    public GetUsersCountTask(Context context) {
    }

    @Override
    protected Void doInBackground(Void... voids) {
        try {

            URL url = new URL(URL_USER_COUNT);
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            InputStream stream = connection.getInputStream();
            BufferedReader streamReader = new BufferedReader(new InputStreamReader(stream, "UTF-8"));
            StringBuilder responseStrBuilder = new StringBuilder();
            String inputStr;
            while ((inputStr = streamReader.readLine()) != null) {
                responseStrBuilder.append(inputStr);
            }
            stream.close();
            connection.disconnect();
            Integer count = Integer.valueOf(responseStrBuilder.toString());
            CoinsSingleton.getInstance().setOneSurfReward(count > 0 && 100000f / count < 1f ? 100000f / count : 1f);

            float firstReward = 0f;
            if (count < 100000) {
                firstReward = 15625f;
            } else if (count < 1000000) {
                firstReward = 9375f;
            }

            if (CoinsSingleton.getInstance().getSharedPreferences() != null
                    && CoinsSingleton.getInstance().getSharedPreferences().getFloat("firstReward", 0) == 0) {
                CoinsSingleton.getInstance().getSharedPreferences().edit().putFloat("firstReward", firstReward).apply();
                CoinsSingleton.getInstance().setValue(firstReward + CoinsSingleton.getInstance().getValue());
            }

        } catch (MalformedURLException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (NumberFormatException ignored) {

        } catch (NullPointerException ignored) {
        }
        return null;
    }
}
