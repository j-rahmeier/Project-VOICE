////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: Helper functions for interfacing with Apple AVFoundation.
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2023: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#include "AVFoundationHelper.h"
#import <AVFoundation/AVFoundation.h>

#include "Ratio.h"

void AVEnumerateSourceDevicesAVFoundation(std::vector<AVSourceEnumerator::Device>& outDevices)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
 
    AVMediaType mediaType = AVMediaTypeVideo;
    NSMutableArray *deviceTypes = nil;
    deviceTypes = [NSMutableArray arrayWithArray:@[AVCaptureDeviceTypeBuiltInWideAngleCamera]];
    [deviceTypes addObject: AVCaptureDeviceTypeDeskViewCamera];
#if (__MAC_OS_X_VERSION_MIN_REQUIRED >= 140000)
    [deviceTypes addObject: AVCaptureDeviceTypeContinuityCamera];
#endif

    AVCaptureDeviceDiscoverySession *captureDeviceDiscoverySession =
         [AVCaptureDeviceDiscoverySession
         discoverySessionWithDeviceTypes:deviceTypes
                               mediaType:mediaType
                                position:AVCaptureDevicePositionUnspecified];
    NSArray<AVCaptureDevice*> *devices = [captureDeviceDiscoverySession devices];
    
    for (id device in devices) {
        AVSourceEnumerator::Device d;
        d.driver = "avfoundation";
        NSString* uniqueID = [(AVCaptureDevice*)device uniqueID];
        d.name = [uniqueID UTF8String];
        d.friendlyName = [[device localizedName] UTF8String];
        d.url = d.friendlyName + ":";
        outDevices.push_back(d);
    }
    [pool release];
}

std::vector<VideoMediaType> AVListVideoMediaTypesAVFoundation(const AVSourceEnumerator::Device& inDevice)
{
    std::vector<VideoMediaType> mediaTypes;

    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    NSString* uniqueID = [NSString stringWithUTF8String:inDevice.name.c_str()];
    AVCaptureDevice* captureDevice = [AVCaptureDevice deviceWithUniqueID:uniqueID];
    if (captureDevice) {
        NSObject* format = nil;
        for (format in [captureDevice valueForKey:@"formats"]) {
            CMFormatDescriptionRef formatDescription = (CMFormatDescriptionRef) [format performSelector:@selector(formatDescription)];
            CMVideoDimensions dimensions = CMVideoFormatDescriptionGetDimensions(formatDescription);
            
            NSObject* range = nil;
            for (range in [format valueForKey:@"videoSupportedFrameRateRanges"]) {
                VideoMediaType mediaType;
                mediaType.width = dimensions.width;
                mediaType.height = dimensions.height;
                mediaType.pxN = 1;
                mediaType.pxD = 1;
                
                double min_framerate = 0;
                double max_framerate = 0;
                [[range valueForKey:@"minFrameRate"] getValue:&min_framerate];
                [[range valueForKey:@"maxFrameRate"] getValue:&max_framerate];
                
                Ratio fps = Ratio::From<double>(max_framerate); // only maxFrameRate used by ffmpeg
                mediaType.framerateN = fps.Numerator();
                mediaType.framerateD = fps.Denominator();
                mediaTypes.push_back(mediaType);
            }
        }
    }
    [pool release];
    return mediaTypes;
}

bool AVFoundationGetCameraAccess()
{
    AVAuthorizationStatus status = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];
    if (status == AVAuthorizationStatusAuthorized) {
        return true;
    }

    if (status == AVAuthorizationStatusNotDetermined) {
        
        dispatch_group_t group = ::dispatch_group_create();
        __block bool accessGranted = false;
        ::dispatch_group_enter(group);
        [AVCaptureDevice requestAccessForMediaType:AVMediaTypeVideo completionHandler:^(BOOL granted) {
            accessGranted = granted;
            ::dispatch_group_leave(group);
        }];
        ::dispatch_group_wait(group, DISPATCH_TIME_FOREVER);
        ::dispatch_release(group);
        
        status = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];
    }
    return status == AVAuthorizationStatusAuthorized;
}
