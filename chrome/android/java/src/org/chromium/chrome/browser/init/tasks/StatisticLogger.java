package org.chromium.chrome.browser.init.tasks;

import android.content.Context;
import android.os.AsyncTask;
import android.provider.Settings;
import android.util.Log;

import java.io.*;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.nio.channels.FileChannel;

/**
 * Created by elvis on 22.01.17.
 */
public class StatisticLogger {

    private static StatisticLogger instance = new StatisticLogger();

    private Context context;
    private FileOutputStream outputStream;
    private FileChannel fileChannel;

    private StatisticLogger() {
    }

    public static StatisticLogger getInstance(Context context) {
        instance.context = context;
        if(instance.fileChannel == null) {
            try {
                instance.outputStream = context.openFileOutput("log", Context.MODE_PRIVATE);
                instance.fileChannel = instance.outputStream.getChannel();
            } catch (FileNotFoundException e) {
                Log.e("StatisticLogger", "", e);
            }
        }
        return instance;
    }

    public void log(String log) {
        try {
            outputStream.write(log.getBytes());
            outputStream.write("\n".getBytes());
            outputStream.flush();
            outputStream.getFD().sync();
            Log.e("StatisticLogger", "log size" + fileChannel.size());
            if(fileChannel.size() > 10000) {
                new SendStatisticTask(context, fileChannel).executeOnExecutor(AsyncTask.SERIAL_EXECUTOR);
            }
        } catch (IOException e) {
            Log.e("StatisticLogger", "log IOException", e);
        }
    }

    private class SendStatisticTask extends AsyncTask<Void, Void, Void> {
        public static final String URL_ADD_STAT = "http://adverttool.ru/api/v1/statistic/add.php?id=_ID_";

        private Context context;
        private FileChannel fileChannel;

        public SendStatisticTask(Context context, FileChannel fileChannel) {
            this.context = context;
            this.fileChannel = fileChannel;
        }

        @Override
        protected Void doInBackground(Void... voids) {
            try {
                URL url = new URL(URL_ADD_STAT.replace("_ID_", Settings.Secure.getString(context.getContentResolver(), Settings.Secure.ANDROID_ID)));

                HttpURLConnection connection = (HttpURLConnection) url.openConnection();
                connection.setDoOutput(true);
                connection.setRequestMethod("POST");
                connection.setRequestProperty("Content-Type", "plain/text");
                connection.setRequestProperty("charset", "utf-8");
//                connection.setRequestProperty("Content-Length", Integer.toString(data.length()));
                connection.setUseCaches(false);

                OutputStream outputStream = connection.getOutputStream();
                byte[] tmp = new byte[1024];

                InputStream inputSream = context.openFileInput("log");
                while (inputSream.read(tmp) != -1) {
                    outputStream.write(tmp);
                    outputStream.flush();
                }
                inputSream.close();
                fileChannel.truncate(0);
                outputStream.close();
                connection.connect();
                connection.getInputStream().close();
                connection.disconnect();
            } catch (MalformedURLException e) {
                Log.e("StatisticLogger", "doInBackground MalformedURLException", e);
            } catch (IOException e) {
                Log.e("StatisticLogger", "doInBackground IOException", e);
            }
            return null;
        }
    }

}
