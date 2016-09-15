package com.ford.interns.parkit.parkitandroid;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Handler;
import android.support.v4.app.FragmentActivity;
import android.os.Bundle;
import android.support.v4.content.LocalBroadcastManager;
import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.BitmapDescriptorFactory;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.MarkerOptions;

import org.json.JSONArray;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;

public class MapsActivity extends FragmentActivity implements OnMapReadyCallback {
    private static final String[] LOCATION_PERMS={
            "android.permission.ACCESS_FINE_LOCATION"
    };
    private GoogleMap mMap;
    private final Handler handler = new Handler();
    private Runnable mapRunnable;
    ArrayList<ParkItSpot> ourSpotList;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_maps);
        ourSpotList = new ArrayList<>();
        // Obtain the SupportMapFragment and get notified when the map is ready to be used.
        SupportMapFragment mapFragment = (SupportMapFragment) getSupportFragmentManager()
                .findFragmentById(R.id.map);
        mapFragment.getMapAsync(this);

        IntentFilter mStatusIntentFilter = new IntentFilter(
                "com.ford.interns.parkit.parkitandroid.SPOT_DATA");
        ResponseReceiver mResponseReceiver =
                new ResponseReceiver();
        LocalBroadcastManager.getInstance(this).registerReceiver(
                mResponseReceiver,
                mStatusIntentFilter);

        requestPermissions(LOCATION_PERMS, 1337);
    }

    @Override
    protected void onStart() {
        super.onStart();
        try {
            if(mMap != null) {
                mMap.setMyLocationEnabled(true);
            }
        } catch (SecurityException e) {
            e.printStackTrace();
        }
        mapRunnable = new Runnable() {
            @Override
            public void run(){
                JSONIntentService.startActionFoo(getApplicationContext());
                handler.postDelayed(this, 10000);
            }
        };
        handler.postDelayed(mapRunnable, 1000);
    }

    @Override
    protected void onStop(){
        super.onStop();
        handler.removeCallbacks(mapRunnable);
    }

    /**
     * Manipulates the map once available.
     * This callback is triggered when the map is ready to be used.
     * This is where we can add markers or lines, add listeners or move the camera. In this case,
     * we just add a marker near Sydney, Australia.
     * If Google Play services is not installed on the device, the user will be prompted to install
     * it inside the SupportMapFragment. This method will only be triggered once the user has
     * installed Google Play services and returned to the app.
     */
    @Override
    public void onMapReady(GoogleMap googleMap) {
        mMap = googleMap;
        LatLng spot = new LatLng(37.40802, -122.14707);
        mMap.moveCamera(CameraUpdateFactory.newLatLngZoom(spot, 19));
        try {
            mMap.setMyLocationEnabled(true);
        } catch (SecurityException e) {
            e.printStackTrace();
        }
        JSONIntentService.startActionFoo(this);
    }

    private void drawSpotsToMap(){
        if(mMap != null) {
            mMap.clear();
            for (int i = 0; i < ourSpotList.size(); i++) {
                ParkItSpot tmpSpot = ourSpotList.get(i);
                LatLng spot = new LatLng(tmpSpot.getLat(), tmpSpot.getLng());
                DateFormat localDateFormat = DateFormat.getDateTimeInstance(SimpleDateFormat.SHORT, SimpleDateFormat.SHORT);
                String time = localDateFormat.format(tmpSpot.getUpdated_at());
                MarkerOptions markOpt = new MarkerOptions().position(spot).title(tmpSpot.getType())
                        .snippet(time);
                if(tmpSpot.getStatus().equals("open")) {
                    if(tmpSpot.getType().equals("Handicap")) {
                        mMap.addMarker(markOpt.icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_BLUE)));
                    } else if(tmpSpot.getType().equals("Vista")) {
                        mMap.addMarker(markOpt.icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_YELLOW)));
                    } else {
                        mMap.addMarker(markOpt.icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_GREEN)));
                    }
                } else if (tmpSpot.getStatus().equals("closed")) {
                    mMap.addMarker(markOpt.icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_RED)));
                } else {
                    mMap.addMarker(markOpt.icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_VIOLET)));
                }
            }
        }
    }

    // Broadcast receiver for receiving status updates from the IntentService
    private class ResponseReceiver extends BroadcastReceiver
    {
        // Called when the BroadcastReceiver gets an Intent it's registered to receive
        @Override
        public void onReceive(Context context, Intent intent) {
            if(intent.getAction().equals("com.ford.interns.parkit.parkitandroid.SPOT_DATA")){
                if(intent.getStringExtra("the_data") != null) {
                    try {
                        JSONArray jsonData = new JSONArray(intent.getStringExtra("the_data"));
                        ourSpotList.clear();
                        for(int i = 0; i < jsonData.length(); i++) {
                            ParkItSpot tmpSpot = new ParkItSpot(jsonData.getJSONObject(i));
                            ourSpotList.add(tmpSpot);
                            //Log.d("BroadcastReceiver", "Adding spot "+tmpSpot.getId());
                        }
                        drawSpotsToMap();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    }
}
