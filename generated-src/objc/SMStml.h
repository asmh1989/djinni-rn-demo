// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from stml.djinni

#import <Foundation/Foundation.h>
@class SMCache;
@class SMStml;
@protocol SMEventLoop;
@protocol SMLogInterface;
@protocol SMStmlListener;
@protocol SMThreadLauncher;


/** stml */
@interface SMStml : NSObject

- (void)start:(nonnull NSString *)server
         port:(int32_t)port;

- (void)setKeeplive:(BOOL)alive;

- (void)setTimeout:(int32_t)timeout;

- (void)forceReconnect;

- (void)post:(nonnull NSString *)msg
 showloading:(BOOL)showloading;

- (void)bye;

- (void)setListener:(nullable id<SMStmlListener>)listener;

+ (nullable SMStml *)create:(nullable id<SMLogInterface>)log
                      cache:(nullable SMCache *)cache
                   uiThread:(nullable id<SMEventLoop>)uiThread
                   launcher:(nullable id<SMThreadLauncher>)launcher;

@end
