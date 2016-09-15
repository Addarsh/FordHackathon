//
//  MapOverlay.m
//  ParkIt
//
//  Created by Ferreyra, Tatiana (T.G.) on 8/2/16.
//  Copyright © 2016 Tatiana Ferreyra. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MapKit/MapKit.h>
#import <CoreLocation/CoreLocation.h>

//@interface MapOverlay ()
//
//@end

@implementation MapOverlay : NSObject 

- (id)initWithCoordinate:(CLLocationCoordinate2D)coordinate {
    self = [super init];
    if (self != nil) {
        
        
    }
    return self;
}

- (CLLocationCoordinate2D)coordinate
{
    CLLocationCoordinate2D coord1 = {
        37.434999,-122.16121
    };
    
    return coord1;
}

- (MKMapRect)boundingMapRect
{
    
    MKMapPoint upperLeft = MKMapPointForCoordinate(self.coordinate);
    
    MKMapRect bounds = MKMapRectMake(upperLeft.x, upperLeft.y, 2000, 2000);
    return bounds;
}


@end