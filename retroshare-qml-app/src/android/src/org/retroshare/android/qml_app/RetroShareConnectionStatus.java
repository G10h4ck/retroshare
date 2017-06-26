package org.retroshare.android.qml_app;

/**
 * Created by Angesoc on 24/06/17.
 */

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;

import android.util.Log;

import org.retroshare.android.qml_app.jni.NativeCalls;

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

            Log.i("RetroShareConnectionStatus", "onReceive " + isConnected );

        }



}
