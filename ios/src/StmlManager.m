//
//  StmlManager.m
//  RNSmobiler
//
//  Created by sun on 2016/12/16.
//  Copyright © 2016年 Smobiler. All rights reserved.
//

#import "StmlManager.h"
#import "RCTLog.h"
#import "SMLog.h"
#import "SMStml.h"

const NSString* TAG=@"StmlManager";

NSString* StmlEventConnecting = @"stml_connecting";
NSString* StmlEventConnected = @"stml_connected";
NSString* StmlEventDisconnect = @"stml_disconnected";
NSString* StmlEventSending=@"stml_sending";
NSString* StmlEventSended=@"stml_sended";
NSString* StmlEventReceived=@"stml_received";

@interface StmlManager ()

@property (nonatomic, strong) SMStml* stml_;
@property (nonatomic, assign) NSString* server_;
@property (nonatomic, assign) NSInteger port_;

@end

@implementation StmlManager

@synthesize stml_, server_, port_;

RCT_EXPORT_MODULE(StmlManager);

- (instancetype)init
{
  self = [super init];
  if (self) {
    [SMLog create:self];
    stml_ = [SMStml create];
    [stml_ setListener:self];
  }
  return self;
}

- (NSDictionary *)constantsToExport
{
  return @{ @"StmlEventConnecting": StmlEventConnecting,
            @"StmlEventConnected": StmlEventConnected,
            @"StmlEventDisconnect": StmlEventDisconnect,
            @"StmlEventSending": StmlEventSending,
            @"StmlEventSended": StmlEventSended,
            @"StmlEventReceived": StmlEventReceived
            };
}


- (NSArray<NSString *> *)supportedEvents
{
  return @[StmlEventConnecting, StmlEventConnected, StmlEventDisconnect, StmlEventSending, StmlEventSended, StmlEventReceived];
}


- (dispatch_queue_t)methodQueue
{
  return dispatch_queue_create("com.smobiler.rns.stml", DISPATCH_QUEUE_SERIAL);
}

#pragma mark - SMStmlListener
- (void)connecting
{
  RCTLogInfo(@"%@: connecting", TAG);
  
  [self sendEventWithName:StmlEventConnecting body:nil];
  
}

- (void)connected
{
  RCTLogInfo(@"%@: connected", TAG);
  [self sendEventWithName:StmlEventConnected body:nil];
}

- (void)disconnect:(NSString *)error
{
  [self sendEventWithName:StmlEventDisconnect body:@{
                                                     @"error": error
                                                     }];
}

- (void)sended:(NSString *)msg
{
  [self sendEventWithName:StmlEventSended body:nil];
}

- (void)sending:(NSString *)msg showloading:(BOOL)showloading
{
  [self sendEventWithName:StmlEventSending body:@{
                                                  @"showloading": @(showloading)
                                                  }];
}

- (void)received:(NSString *)msg
{
  [self sendEventWithName:StmlEventReceived body:@{
                                                 @"msg":msg
                                                 }];

}

#pragma mark - SMLogInterface
- (void)d:(NSString *)tag msg:(NSString *)msg
{
  RCTLog(@"%@: %@", tag, msg);
}

- (void)e:(NSString *)tag msg:(NSString *)msg
{
  RCTLogWarn(@"%@: %@", tag, msg);
}

- (void)w:(NSString *)tag msg:(NSString *)msg
{
  RCTLogWarn(@"%@: %@", tag, msg);
}

- (void)i:(NSString *)tag msg:(NSString *)msg
{
  RCTLogInfo(@"%@: %@", tag, msg);
}


RCT_EXPORT_METHOD(start:(NSString *)host port:(NSInteger )port)
{
  RCTLogInfo(@"%@: start conect: %@, %d", TAG, host, (int)port);
  
  server_ = host;
  port_ = port;
  
  [stml_ start:host port:(int32_t)port];
  
}

RCT_EXPORT_METHOD(post:(NSString *)msg)
{
  [stml_ post:msg showloading:false];
}

@end