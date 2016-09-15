package com.ford.interns.parkit.parkitandroid;

import android.app.IntentService;
import android.content.Intent;
import android.content.Context;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.SocketTimeoutException;
import java.net.URL;
import java.net.URLConnection;
import java.util.Scanner;

/**
 * An {@link IntentService} subclass for handling asynchronous task requests in
 * a service on a separate handler thread.
 * <p/>
 * TODO: Customize class - update intent actions, extra parameters and static
 * helper methods.
 */
public class JSONIntentService extends IntentService {
    private static String TAG = JSONIntentService.class.getName();
    private String apiPort = "3000";
    private String apiEndpoint = "/api/spotInfo";
    private String apiUrl = "http://52.41.133.252:"+apiPort+apiEndpoint;

    // IntentService can perform, e.g. ACTION_FETCH_NEW_ITEMS
    private static final String ACTION_FOO = "com.ford.interns.parkit.parkitandroid.action.FOO";

    public JSONIntentService() {
        super("JSONIntentService");
    }

    /**
     * Starts this service to perform action Foo with the given parameters. If
     * the service is already performing a task this action will be queued.
     *
     * @see IntentService
     */
    public static void startActionFoo(Context context) {
        Intent intent = new Intent(context, JSONIntentService.class);
        intent.setAction(ACTION_FOO);
        context.startService(intent);
    }

    @Override
    protected void onHandleIntent(Intent intent) {
        if (intent != null) {
            final String action = intent.getAction();
            if (ACTION_FOO.equals(action)) {
                JSONArray jsonResult = handleActionFoo();
                Log.d(TAG, "Got JSON! ---- "+jsonResult);
                if(jsonResult != null) {
                    Intent localIntent =
                            new Intent("com.ford.interns.parkit.parkitandroid.SPOT_DATA")
                                    // Puts the status into the Intent
                                    .putExtra("the_data", jsonResult.toString());
                    // Broadcasts the Intent to receivers in this app.
                    LocalBroadcastManager.getInstance(this).sendBroadcast(localIntent);
                }
            }
        }
    }

    /**
     * Handle action Foo in the provided background thread with the provided
     * parameters.
     */
    private JSONArray handleActionFoo() {
            HttpURLConnection urlConnection = null;
            try {
                // create connection
                URL urlToRequest = new URL(apiUrl);
                urlConnection = (HttpURLConnection)
                        urlToRequest.openConnection();
                urlConnection.setConnectTimeout(5000);
                urlConnection.setReadTimeout(5000);

                // handle issues
                int statusCode = urlConnection.getResponseCode();
                if (statusCode == HttpURLConnection.HTTP_UNAUTHORIZED) {
                    // handle unauthorized (if service requires user login)
                    Log.d(TAG, "Error "+statusCode);
                } else if (statusCode != HttpURLConnection.HTTP_OK) {
                    // handle any other errors, like 404, 500,..
                    Log.d(TAG, "Error "+statusCode);
                }

                // create JSON array from content
                InputStream in = new BufferedInputStream(
                        urlConnection.getInputStream());
                return new JSONArray(new Scanner(in).useDelimiter("\\A").next());

            } catch (MalformedURLException e) {
                e.printStackTrace();
                // URL is invalid
            } catch (SocketTimeoutException e) {
                e.printStackTrace();
                // data retrieval or connection timed out
            } catch (IOException e) {
                e.printStackTrace();
                // could not read response body
                // (could not create input stream)
            } catch (JSONException e) {
                e.printStackTrace();
                // response body is no valid JSON string
            } finally {
                if (urlConnection != null) {
                    urlConnection.disconnect();
                }
            }

            return null;
        }
}
