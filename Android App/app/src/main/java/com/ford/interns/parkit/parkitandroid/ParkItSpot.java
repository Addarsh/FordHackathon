package com.ford.interns.parkit.parkitandroid;

import org.json.JSONException;
import org.json.JSONObject;

import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by bhass1 on 8/1/16.
 */
public class ParkItSpot {
    int id;
    String status;
    double lat;
    double lng;
    Date updated_at;
    String type;

    public ParkItSpot() {
        id = -1;
        status = "Unknown";
        lat = 37.4081386;
        lng = -122.1480287;
        updated_at = null;
        type = "Invalid";
    }

    public ParkItSpot(JSONObject jsonSpot) throws JSONException {
        id = jsonSpot.getInt("id");
        status = jsonSpot.getString("status");
        lat = jsonSpot.getDouble("lat");
        lng = jsonSpot.getDouble("lng");
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss.SSS'Z'");
        try {
            updated_at = sdf.parse((String) jsonSpot.get("updated_at"));
        } catch (Exception e) {
            e.printStackTrace();
            updated_at = new Date();
        }
        type = jsonSpot.getString("type");
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public double getLat() {
        return lat;
    }

    public void setLat(double lat) {
        this.lat = lat;
    }

    public double getLng() {
        return lng;
    }

    public void setLng(double lng) {
        this.lng = lng;
    }

    public Date getUpdated_at() {
        return updated_at;
    }

    public void setUpdated_at(Date updated_at) {
        this.updated_at = updated_at;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }
}
