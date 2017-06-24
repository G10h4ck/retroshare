package org.retroshare.android.qml_app;

/**
 * Created by Angesoc on 24/06/17.
 */
public class ConnectionStatus {

    private boolean isNetworkAvailable() {

        ConnectivityManager cm =
                (ConnectivityManager)context.getSystemService(Context.CONNECTIVITY_SERVICE);

        NetworkInfo activeNetwork = cm.getActiveNetworkInfo();
        boolean isConnected = activeNetwork != null &&
                activeNetwork.isConnectedOrConnecting();

        return isConnected


    }

}
