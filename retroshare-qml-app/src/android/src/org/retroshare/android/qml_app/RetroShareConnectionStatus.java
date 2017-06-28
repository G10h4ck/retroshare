package org.retroshare.android.qml_app;

/**
 * Created by Angesoc on 24/06/17.
 */

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;

import android.net.Uri;

import android.util.Log;


public class RetroShareConnectionStatus  extends BroadcastReceiver
{


        public RetroShareConnectionStatus()
        {
            super();
        }


        @Override
        public void onReceive(Context context, Intent arg1)
        {

            ConnectivityManager cm = (ConnectivityManager) context
                    .getSystemService(Context.CONNECTIVITY_SERVICE);
            NetworkInfo activeNetwork = cm.getActiveNetworkInfo();
            boolean isConnected = activeNetwork != null
                    && activeNetwork.isConnectedOrConnecting();


            String uri = "//networkstatus/isConnected?" + String.valueOf(isConnected);

            Intent nsIntent = new Intent(context, RetroShareAndroidNotifyService.class);
            nsIntent.putExtra("uri", uri);
            context.startService(nsIntent);


            Log.i("RetroShareConnectionStatus", "onReceive " + isConnected );

        }



}
