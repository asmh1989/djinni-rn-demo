// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from thread.djinni

#import <Foundation/Foundation.h>
@class SMAsyncTask;


@protocol SMThreadLauncher

/** name is nullable */
- (void)startThread:(nonnull NSString *)name
              runFn:(nullable SMAsyncTask *)runFn;

@end
