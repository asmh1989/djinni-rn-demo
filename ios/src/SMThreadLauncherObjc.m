#include <Foundation/Foundation.h>
#include "SMThreadLauncherObjc.h"
#include "SMAsyncTask.h"

@implementation SMThreadLauncherObjc

- (void)startThread:(NSString *)name runFn:(SMAsyncTask *)runFn {
    NSThread *thread = [[NSThread alloc] initWithTarget:runFn selector:@selector(execute) object:nil];
    if (name) {
        [thread setName:name];
    }
    [thread start];
}

@end
