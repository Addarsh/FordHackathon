//
//  ViewController.h
//  ParkIt
//
//  Created by Tatiana Ferreyra on 7/27/16.
//  Copyright © 2016 Tatiana Ferreyra. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <MapKit/MapKit.h>

@interface ViewController : UIViewController
@property (weak, nonatomic) IBOutlet MKMapView *mapView;

//@property (nonatomic, assign) MKMapView * mapView;
@property (strong, nonatomic) CLLocationManager *locationManager;
@property(nonatomic) MKCoordinateRegion region;
@property(nonatomic) BOOL showsBuildings;


@end

