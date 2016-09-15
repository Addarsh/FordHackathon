//
//  MapOverlay.h
//  ParkIt
//
//  Created by Ferreyra, Tatiana (T.G.) on 8/2/16.
//  Copyright © 2016 Tatiana Ferreyra. All rights reserved.
//

#include <UIKit/UIKit.h>
//#import <MapKit/MapKit.h>


#ifndef MapOverlay_h
#define MapOverlay_h

@interface MapOverlay : NSObject <MKOverlay> {
    
}

- (MKMapRect)boundingMapRect;
@property (nonatomic, readonly) CLLocationCoordinate2D coordinate;

@end

#endif /* MapOverlay_h */
