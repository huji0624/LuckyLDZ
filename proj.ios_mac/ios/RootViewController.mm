/****************************************************************************
 Copyright (c) 2013      cocos2d-x.org
 Copyright (c) 2013-2014 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "RootViewController.h"
#import "cocos2d.h"
#import "CCEAGLView.h"

#import "GADBannerView.h"
#import "GADRequest.h"
#import "IOSNDKHelper.h"

@interface RootViewController()

@end

@implementation RootViewController


 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
        [IOSNDKHelper SetNDKReciever:self];
    }
    return self;
}

-(void)setAd:(id)p{
    NSDictionary *dic = p;
    NSString *tag = [dic objectForKey:@"setAd"];
    if ([tag isEqualToString:@"1"]) {
        bannerView_.hidden = NO;
    }else{
        bannerView_.hidden = YES;
    }
}

/*
 value
 cat
 */
-(void)leaderBoardControl:(id)param{
    if (![[GKLocalPlayer localPlayer] isAuthenticated]) {
        return;
    }
    
    NSDictionary *dic = param;
    if ([dic objectForKey:@"value"]) {
        NSNumber *value = [dic objectForKey:@"value"];
        NSString *cat = [dic objectForKey:@"cat"];
        GKScore *score = [[GKScore alloc] initWithLeaderboardIdentifier:cat];
        score.value = [value intValue];
        [score reportScoreWithCompletionHandler:^(NSError *error) {
            NSLog(@"upload score.");
        }];
    }else{
        NSString *cat = [dic objectForKey:@"cat"];
        GKGameCenterViewController *gameCenterController = [[GKGameCenterViewController alloc] init];
        if (gameCenterController != NULL)
        {
            gameCenterController.gameCenterDelegate = self;
            gameCenterController.leaderboardIdentifier = cat;
            gameCenterController.leaderboardTimeScope = GKLeaderboardTimeScopeAllTime;
            [self presentModalViewController: gameCenterController animated: YES];
        }
    }
}

-(void)gameCenterViewControllerDidFinish:(GKGameCenterViewController *)gameCenterViewController{
    [gameCenterViewController dismissViewControllerAnimated:YES completion:nil];
}

-(void)showReviewAlert:(id)param{
    NSDictionary *dic = param;
    NSString *title = [dic objectForKey:@"title"];
    NSString *message = [dic objectForKey:@"message"];
    NSString *cancel = [dic objectForKey:@"cancel"];
    NSString *ok = [dic objectForKey:@"ok"];
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title message:message delegate:self cancelButtonTitle:cancel otherButtonTitles:ok, nil];
    [alert show];
}

// Called when a button is clicked. The view will be automatically dismissed after this call returns
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex{
    if (buttonIndex==1) {
        [self goReview:nil];
    }
}

-(void)goReview:(id)param{
    NSString *appId = APP_ID;
    SKStoreProductViewController *reviewVC = [[SKStoreProductViewController alloc] init];
    if (reviewVC) {
        NSDictionary *dic = [NSDictionary dictionaryWithObject:appId forKey:SKStoreProductParameterITunesItemIdentifier];
        [reviewVC setDelegate:self];
        [reviewVC loadProductWithParameters:dic completionBlock:^(BOOL result, NSError *error) {
            if (!result) {
                [reviewVC dismissViewControllerAnimated:YES completion:nil];
                NSLog(@"error loadProduct %@",error.localizedFailureReason);
            }
        }];
    }else{
        // before iOS6.0
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"itms-apps://ax.itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=%@", appId]]];
    }
}

-(void)productViewControllerDidFinish:(SKStoreProductViewController *)viewController{
    [viewController dismissViewControllerAnimated:YES completion:nil];
}

// Override to allow orientations other than the default portrait orientation.
// This method is deprecated on ios6
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return UIInterfaceOrientationIsLandscape( interfaceOrientation );
}

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger) supportedInterfaceOrientations{
#ifdef __IPHONE_6_0
    return UIInterfaceOrientationMaskAllButUpsideDown;
#endif
}

- (BOOL) shouldAutorotate {
    return YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];

    cocos2d::GLView *glview = cocos2d::Director::getInstance()->getOpenGLView();

    if (glview)
    {
        CCEAGLView *eaglview = (CCEAGLView*) glview->getEAGLView();

        if (eaglview)
        {
            CGSize s = CGSizeMake([eaglview getWidth], [eaglview getHeight]);
            cocos2d::Application::getInstance()->applicationScreenSizeChanged((int) s.width, (int) s.height);
        }
    }
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden
{
    return YES;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];

    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    bannerView_.delegate = nil;
    [bannerView_ release];
    
    [super dealloc];
}

//GAD
- (GADRequest *)createRequest {
    GADRequest *request = [GADRequest request];
    
    // Make the request for a test ad. Put in an identifier for the simulator as
    // well as any devices you want to receive test ads.
    request.testDevices =
    [NSArray arrayWithObjects:
     @"6327f00c44091bbcb01c6f269ba72489",
     // TODO: Add your device/simulator test identifiers here. They are
     // printed to the console when the app is launched.
     nil];
    return request;
}

- (void)initGADBannerWithAdPositionAtTop:(BOOL)isAdPositionAtTop {
    isAdPositionAtTop_ = isAdPositionAtTop;
    
    // NOTE:
    // Add your publisher ID here and fill in the GADAdSize constant for the ad
    // you would like to request.
    bannerView_ = [[GADBannerView alloc] initWithAdSize:kGADAdSizeSmartBannerPortrait];
    bannerView_.adUnitID = @"a153a14e5305ecd";
    bannerView_.delegate = self;
    [bannerView_ setRootViewController:self];
    
    [self.view addSubview:bannerView_];
    [bannerView_ loadRequest:[self createRequest]];
    // Use the status bar orientation since we haven't signed up for orientation
    // change notifications for this class.
    [self resizeViewsForOrientation:
     [[UIApplication sharedApplication] statusBarOrientation]];
    
    bannerView_.hidden = YES;
}

- (void)resizeViewsForOrientation:(UIInterfaceOrientation)toInt {
    // If the banner hasn't been created yet, no need for resizing views.
   
}

#pragma mark GADBannerViewDelegate impl

- (void)adViewDidReceiveAd:(GADBannerView *)adView {
    NSLog(@"Received ad");
}

- (void)adView:(GADBannerView *)view didFailToReceiveAdWithError:(GADRequestError *)error {
    NSLog(@"Failed to receive ad with error: %@", [error localizedFailureReason]);
}


@end
