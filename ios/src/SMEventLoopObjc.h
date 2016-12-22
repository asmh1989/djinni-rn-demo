#pragma once
#include "SMEventLoop.h"

@interface SMEventLoopObjc : NSObject <SMEventLoop>

- (void)post:(SMAsyncTask *)task;

@end
