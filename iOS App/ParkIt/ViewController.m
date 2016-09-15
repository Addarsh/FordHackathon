//
//  ViewController.m
//  ParkIt
//
//  Created by Tatiana Ferreyra on 7/27/16.
//  Copyright © 2016 Tatiana Ferreyra. All rights reserved.
//

#import "ViewController.h"
#import <CoreLocation/CoreLocation.h>
#import <MapKit/MapKit.h>

@interface ViewController ()
//@property (weak, nonatomic) IBOutlet MKMapView *mapView;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.

#pragma mark - Map
    //Adding map
    //MKMapView *mapView = [[MKMapView alloc] initWithFrame:self.view.frame];
    //[self.view addSubview:mapView];
    _mapView.mapType = MKMapTypeHybrid;

   //Requesting user's location
    if (nil == _locationManager) {
        _locationManager = [[CLLocationManager alloc] init];
    }
    
    _locationManager.delegate = self;
    [_locationManager requestWhenInUseAuthorization];

   
    //Showing user location after authorization has been granted
    CLAuthorizationStatus authorizationStatus= [CLLocationManager authorizationStatus];
    
    if (authorizationStatus == kCLAuthorizationStatusAuthorized ||
        authorizationStatus == kCLAuthorizationStatusAuthorizedAlways ||
        authorizationStatus == kCLAuthorizationStatusAuthorizedWhenInUse) {
        
        
//        CLLocationCoordinate2D noLocation;
//        MKCoordinateRegion viewRegion = MKCoordinateRegionMakeWithDistance(noLocation, 50, 50);
//        MKCoordinateRegion adjustedRegion = [self.mapView regionThatFits:viewRegion];
//        [self.mapView setRegion:adjustedRegion animated:YES];
        self.mapView.showsUserLocation = YES;
        _locationManager.desiredAccuracy = kCLLocationAccuracyBest;
        _locationManager.distanceFilter = kCLLocationAccuracyHundredMeters;
        [self.locationManager startUpdatingLocation];

    }

    
    //Setting region to user location - find way to fix user location coordinates (currently hardcoded bc it is 0.0)
    //_mapView.userLocation.location.coordinate.latitude = 37.408325;
    //_mapView.userLocation.location.coordinate.longitude = -122.147166;
    MKCoordinateRegion region = MKCoordinateRegionMakeWithDistance(CLLocationCoordinate2DMake(37.408325, -122.147166),
                                                                   100, 100);
    [_mapView setRegion:region animated:YES];
    
//    MKCoordinateRegion theRegion = _mapView.region;
//    // Zoom in
//    theRegion.span.longitudeDelta *= 0.2;
//    theRegion.span.latitudeDelta *= 0.2;
//    [_mapView setRegion:theRegion animated:YES];
    
    
    
//    MapOverlay * mapOverlay = [[MapOverlay alloc] init];
//    [_mapView addOverlay:mapOverlay];
//    [mapOverlay release];
    
    
#pragma mark - Annotations
    
    MKPointAnnotation *spot20 = [[MKPointAnnotation alloc] init];
    [spot20 setCoordinate:CLLocationCoordinate2DMake(37.408035, -122.147079)];
    [spot20 setTitle:@"Spot 20 - Handicap"];
    [spot20 setSubtitle:@"Empty"];
    [_mapView addAnnotation:spot20];
    
    MKPointAnnotation *spot21 = [[MKPointAnnotation alloc] init];
    [spot21 setCoordinate:CLLocationCoordinate2DMake(37.408015, -122.147105)];
    [spot21 setTitle:@"Spot 21 - Handicap"];
    [spot21 setSubtitle:@"Empty"];
    [_mapView addAnnotation:spot21];
    
    MKPointAnnotation *spot22 = [[MKPointAnnotation alloc] init];
    [spot22 setCoordinate:CLLocationCoordinate2DMake(37.407995, -122.147122)];
    [spot22 setTitle:@"Spot 22"];
    [spot22 setSubtitle:@"Empty"];
    [_mapView addAnnotation:spot22];
    
    MKPointAnnotation *spot23 = [[MKPointAnnotation alloc] init];
    [spot23 setCoordinate:CLLocationCoordinate2DMake(37.407973, -122.147139)];
    [spot23 setTitle:@"Spot 23"];
    [spot23 setSubtitle:@"Empty"];
    [_mapView addAnnotation:spot23];
    
    MKPointAnnotation *spot24 = [[MKPointAnnotation alloc] init];
    [spot24 setCoordinate:CLLocationCoordinate2DMake(37.407948, -122.147155)];
    [spot24 setTitle:@"Spot 24"]; //look into set value for keys in dict
    [spot24 setSubtitle:@"Empty"];
    [_mapView addAnnotation:spot24];
    
    MKPinAnnotationView *view = [[MKPinAnnotationView alloc] initWithAnnotation:spot24 reuseIdentifier:nil];
    view.pinTintColor = [UIColor purpleColor];
    [_mapView addAnnotation:view];


    
}

- (MKOverlayView *)mapView:(MKMapView *)mapView viewForOverlay:(id <MKOverlay>)overlay {
    
//    MapOverlay *overlay = overlay;
//    MKOverlayView *mapOverlayView = [[[MKOverlayView alloc] initWithOverlay:overlay] autorelease];
//    
return 0;
    
}

-(void)locationManager:(CLLocationManager *)manager didUpdateToLocation:(CLLocation *)newLocation fromLocation:(CLLocation *)oldLocation {

}

- (void)mapView:(MKMapView *)mapView didUpdateUserLocation:(MKUserLocation *)userLocation
{
    NSLog(@"lat is %f : lon is %f",_mapView.userLocation.location.coordinate.latitude, _mapView.userLocation.location.coordinate.longitude);
    //[_mapView setCenterCoordinate:_mapView.userLocation.coordinate animated:YES];
    
}

-(void)viewWillLayoutSubviews {
    [super viewWillLayoutSubviews];
    self.mapView.frame = self.view.bounds;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
