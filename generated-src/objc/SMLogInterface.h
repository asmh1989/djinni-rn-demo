// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from stml.djinni

#import <Foundation/Foundation.h>


/** log */
@protocol SMLogInterface

- (void)d:(nonnull NSString *)tag
      msg:(nonnull NSString *)msg;

- (void)w:(nonnull NSString *)tag
      msg:(nonnull NSString *)msg;

- (void)e:(nonnull NSString *)tag
      msg:(nonnull NSString *)msg;

- (void)i:(nonnull NSString *)tag
      msg:(nonnull NSString *)msg;

@end
