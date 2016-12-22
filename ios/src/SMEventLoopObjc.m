#include <Foundation/Foundation.h>
#include "SMEventLoopObjc.h"
#include "SMAsyncTask.h"

@implementation SMEventLoopObjc

- (void)post:(SMAsyncTask *)task {
    dispatch_async(dispatch_get_main_queue(), ^{
        [task execute];
    });
}

@end
