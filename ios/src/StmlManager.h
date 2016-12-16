//
//  StmlManager.h
//  RNSmobiler
//
//  Created by sun on 2016/12/16.
//  Copyright © 2016年 Smobiler. All rights reserved.
//

#import "RCTBridgeModule.h"
#import "SMLogInterface.h"
#import "SMStmlListener.h"
#import "RCTEventEmitter.h"

@interface StmlManager : RCTEventEmitter <RCTBridgeModule, SMLogInterface, SMStmlListener>

@end
