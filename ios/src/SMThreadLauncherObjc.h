#pragma once
#include "SMThreadLauncher.h"

@interface SMThreadLauncherObjc : NSObject <SMThreadLauncher>

- (void)startThread:(NSString *)name runFn:(SMAsyncTask *)runFn;

@end
