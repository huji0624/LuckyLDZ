#import <UIKit/UIKit.h>

int main(int argc, char *argv[]) {
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, @"AppController");
    [pool release];
    return retVal;
}

#include <stdio.h>
//FILE *fopen$UNIX2003( const char *filename, const char *mode )
//{
//    return fopen(filename, mode);
//}
size_t fwrite$UNIX2003( const void *a, size_t b, size_t c, FILE *d )
{
	return fwrite(a, b, c, d);
}
int strerror$UNIX2003(int i){
	return strerror(i);
}
