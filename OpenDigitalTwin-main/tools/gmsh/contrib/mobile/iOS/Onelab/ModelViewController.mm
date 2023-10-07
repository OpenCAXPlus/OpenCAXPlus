#import <QuartzCore/QuartzCore.h>
#import <Social/Social.h>

#import "ModelViewController.h"
#import "drawContext.h"
#import "iosUtils.h"
#import "Utils.h"

#import "AppDelegate.h"

@implementation UIErrorAlertView

-(void)dismissWithClickedButtonIndex:(NSInteger)buttonIndex animated:(BOOL)animated
{
  if(buttonIndex == 0) [super dismissWithClickedButtonIndex:buttonIndex animated:animated];
}
@end

@interface ModelViewController ()
- (void)configureView;
@end

@implementation ModelViewController

#pragma mark - Managing the detail item
@synthesize glView;

- (void)setDetailItem:(id)newDetailItem
{
  if (_detailItem != newDetailItem) {
    _detailItem = newDetailItem;
    // Update the view.
    [self configureView];
  }
}

- (void)configureView
{
  // Update the user interface for the detail item.
  if (self.detailItem) {
    self.detailDescriptionLabel.text = [self.detailItem description];
  }
}

-(void)viewWillAppear:(BOOL)animated
{
  if([[UIDevice currentDevice].model isEqualToString:@"iPad"] ||
     [[UIDevice currentDevice].model isEqualToString:@"iPad Simulator"]) {
    _loadingAlert = [[UIAlertView alloc] initWithTitle:@"Please wait..."
                                               message: @"The model is loading"
                                              delegate: self
                                     cancelButtonTitle: nil
                                     otherButtonTitles: nil];
    [_loadingAlert show];
  }
}

-(void)viewDidAppear:(BOOL)animated
{
  if(!_loadingAlert && self.initialModel != nil) {
    _loadingAlert = [[UIAlertView alloc] initWithTitle:@"Please wait..." message: @"The model is loading" delegate: self cancelButtonTitle: nil otherButtonTitles: nil];
    [_loadingAlert show];
  }
  _progressLabel.frame = CGRectMake(50, self.view.frame.size.height - 25, _progressLabel.frame.size.width, _progressLabel.frame.size.height);
  _progressIndicator.frame = CGRectMake(20, self.view.frame.size.height - 25, _progressIndicator.frame.size.width, _progressIndicator.frame.size.height);
  [_progressIndicator addGestureRecognizer:[[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleProgressIndicatorTap:)]];
  [_progressLabel setHidden:YES];
  [_progressIndicator setHidden:YES];
  [self.navigationController setToolbarHidden:YES animated:NO];
  if(self.initialModel != nil){
    [self.glView load:self.initialModel];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"refreshParameters" object:nil];
    //[self.initialModel release];
    self.initialModel = nil;
    [_loadingAlert dismissWithClickedButtonIndex:-1 animated:YES];
    //[_loadingAlert release];
    _loadingAlert = nil;
  }
}

- (void)viewDidLoad
{
  [super viewDidLoad];
  // Do any additional setup after loading the view, typically from a nib.
  [self configureView];
  [_singleTap requireGestureRecognizerToFail:_doubleTap];
  scaleFactor = 1.;
  setObjCBridge((__bridge void*) self);
  [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(requestRender) name:@"requestRender" object:nil];

  _runStopButton = [[UIBarButtonItem alloc] initWithTitle:@"Run" style:UIBarButtonItemStylePlain target:self action:@selector(compute)];
  if([[UIDevice currentDevice].model isEqualToString:@"iPad"] ||
     [[UIDevice currentDevice].model isEqualToString:@"iPad Simulator"]){
    UIBarButtonItem *model = [[UIBarButtonItem alloc] initWithTitle:@"Model list" style:UIBarButtonItemStylePlain target:self action:@selector(showModelsList)];
    [self.navigationItem setRightBarButtonItems:[NSArray arrayWithObjects:_runStopButton, model, nil]];
  }
  else {
    UIBarButtonItem *settings = [[UIBarButtonItem alloc] initWithTitle:@"Parameters" style:UIBarButtonItemStylePlain target:self action:@selector(showSettings)];
    [self.navigationItem setRightBarButtonItems:[NSArray arrayWithObjects:_runStopButton, settings, nil]];
  }

  UIBarButtonItem *flexibleSpace = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace target:nil action:nil];
  UIBarButtonItem *prevButton = [[UIBarButtonItem alloc ] initWithBarButtonSystemItem: UIBarButtonSystemItemRewind target:self action:@selector(prevAnimation)];
  _stopButton = [[UIBarButtonItem alloc ] initWithBarButtonSystemItem: UIBarButtonSystemItemPause target:self action:@selector(stopAnimation:)];
  [_stopButton setEnabled:NO];
  _playButton = [[UIBarButtonItem alloc ] initWithBarButtonSystemItem: UIBarButtonSystemItemPlay target:self action:@selector(playAnimation:)];
  UIBarButtonItem *nextButton = [[UIBarButtonItem alloc ] initWithBarButtonSystemItem: UIBarButtonSystemItemFastForward target:self action:@selector(nextAnimation)];
  self.toolbarItems = [[NSArray alloc] initWithObjects:flexibleSpace, prevButton, _stopButton, _playButton, nextButton, flexibleSpace, nil];
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
  _progressLabel.frame = CGRectMake(50, self.view.frame.size.height - 25, _progressLabel.frame.size.width, _progressLabel.frame.size.height);
  _progressIndicator.frame = CGRectMake(20, self.view.frame.size.height - 25, _progressIndicator.frame.size.width, _progressIndicator.frame.size.height);
}

-(void)didReceiveMemoryWarning
{
  onelab_cb("stop");
  UILocalNotification *localNotif = [[UILocalNotification alloc] init];
  if ([UIApplication sharedApplication].applicationState == UIApplicationStateBackground && localNotif) {
    localNotif.alertBody = @"The computation had to stop because your device ran out of memory";
    localNotif.alertAction = @"View";
    localNotif.hasAction = true;
    localNotif.soundName = UILocalNotificationDefaultSoundName;
    localNotif.applicationIconBadgeNumber = 1;
    [[UIApplication sharedApplication] scheduleLocalNotification:localNotif];
  }
  [[UIApplication sharedApplication] endBackgroundTask: _computeBackgroundTaskIdentifier];
}

- (void)compute
{
  [_runStopButton setAction:@selector(stop)];
  [_runStopButton setTitle:@"Stop"];
  [_progressLabel setText:@""];
  [_progressLabel setHidden:NO];
  [_progressIndicator setHidden:NO];
  [_progressIndicator startAnimating];
  [[UIApplication sharedApplication] cancelAllLocalNotifications];
  dispatch_group_t group = dispatch_group_create();
  dispatch_queue_t queue = dispatch_queue_create("org.geuz.onelab", NULL);
  dispatch_group_async(group, queue, ^{
      _computeBackgroundTaskIdentifier = [[UIApplication sharedApplication] beginBackgroundTaskWithExpirationHandler:^{
          [[UIApplication sharedApplication] endBackgroundTask: _computeBackgroundTaskIdentifier];
          _computeBackgroundTaskIdentifier = UIBackgroundTaskInvalid;
        }];
      AppDelegate *appDelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
      appDelegate->compute = YES;
      onelab_cb("compute");
      appDelegate->compute = NO;
      UILocalNotification *localNotif = [[UILocalNotification alloc] init];
      if ([UIApplication sharedApplication].applicationState == UIApplicationStateBackground && localNotif) {
        localNotif.alertBody = @"Computation done!";
        localNotif.alertAction = @"View";
        localNotif.hasAction = true;
        localNotif.soundName = UILocalNotificationDefaultSoundName;
        localNotif.applicationIconBadgeNumber = 1;
        [[UIApplication sharedApplication] scheduleLocalNotification:localNotif];
      }
      [[UIApplication sharedApplication] endBackgroundTask: _computeBackgroundTaskIdentifier];
      _computeBackgroundTaskIdentifier = UIBackgroundTaskInvalid;
      [[NSNotificationCenter defaultCenter] postNotificationName:@"refreshParameters" object:nil];
    });

  dispatch_group_notify(group, queue, ^{
      [UIApplication sharedApplication].applicationIconBadgeNumber = -1;
      [self performSelectorOnMainThread:@selector(stopGUI) withObject:nil waitUntilDone:YES];
    });
}

-(void)stopGUI
{
  [_runStopButton setAction:@selector(compute)];
  [_runStopButton setTitle:@"Run"];
  [_progressLabel setHidden:YES];
  [_progressIndicator stopAnimating];
  [_progressIndicator setHidden:YES];
}

- (void)stop
{
  onelab_cb("stop");
}

-(void)playAnimation:(UIBarButtonItem *)sender
{
  [_playButton setEnabled:NO];
  [_stopButton setEnabled:YES];
  _animation = [NSTimer scheduledTimerWithTimeInterval:0.5 target:self selector:@selector(nextAnimation) userInfo:nil repeats:YES];
}

-(void)stopAnimation:(UIBarButtonItem *)sender
{
  [_animation invalidate];
  _animation = nil;
  [_playButton setEnabled:YES];
  [_stopButton setEnabled:NO];
}

-(void)nextAnimation { animation_next(); [self requestRender]; }

-(void)prevAnimation { animation_prev(); [self requestRender]; }

-(IBAction)pinch:(UIPinchGestureRecognizer *)sender
{
  if([sender numberOfTouches] <= 2) {
    float mScale = scaleFactor;
    if (sender.state == UIGestureRecognizerStateBegan)
      mScale = scaleFactor;
    else if(sender.state == UIGestureRecognizerStateChanged)
      mScale = scaleFactor * [sender scale];
    else if(sender.state == UIGestureRecognizerStateEnded){
      scaleFactor *= [sender scale];
      mScale = scaleFactor;
    }
    else if(sender.state == UIGestureRecognizerStateCancelled){
      mScale = scaleFactor;
    }
    mScale = MAX(0.1, mScale);
    glView->mContext->eventHandler(2,mScale);
  }
  [glView drawView];
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
  UITouch *touch = [[event allTouches] anyObject];
  CGPoint touchPoint = [touch locationInView:self.view];
  glView->mContext->eventHandler(0, touchPoint.x, touchPoint.y);
}

-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
  [self touchesEnded:touches withEvent:event];
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
  UITouch *touch = [[event allTouches] anyObject];
  CGPoint touchPoint = [touch locationInView:self.view];
  glView->mContext->eventHandler(4, touchPoint.x, touchPoint.y);
}

- (IBAction)singleTap:(UITapGestureRecognizer *)sender {
  [self.navigationController setToolbarHidden:(!(self.navigationController.toolbarHidden && !((AppDelegate *)[UIApplication sharedApplication].delegate)->compute && number_of_animation() > 0)) animated:YES];
}

- (IBAction)doubleTap:(UITapGestureRecognizer *)sender {
  scaleFactor = 1;
  glView->mContext->eventHandler(10);
  [glView drawView];
}

- (void) showModelsList
{
  if(((AppDelegate *)[UIApplication sharedApplication].delegate)->compute) {
    UIAlertView *alert;
    alert = [[UIAlertView alloc] initWithTitle:@"Can't show the model list" message:@"The computation has to complete before you can select another model." delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil, nil];
    [alert show];
    return;
  }
  if([[UIDevice currentDevice].model isEqualToString:@"iPad"] ||
     [[UIDevice currentDevice].model isEqualToString:@"iPad Simulator"]){
    AppDelegate *appDelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
    [UIView transitionWithView:appDelegate.window
                      duration:0.5
                       options:UIViewAnimationOptionTransitionFlipFromRight
                    animations:^{ appDelegate.window.rootViewController = appDelegate.modelListController; }
    completion:nil];
  }
  [self.navigationController popToRootViewControllerAnimated:YES];
}

- (void) showSettings
{
  [self performSegueWithIdentifier:@"showSettingsSegue" sender:self];
}

- (void)viewDidUnload
{
  [self setGlView:nil];
  [self setProgressLabel:nil];
  [self setProgressIndicator:nil];
  [self setSingleTap:nil];
  [self setDoubleTap:nil];
  [super viewDidUnload];
  // Release any retained subviews of the main view.
}

- (void)requestRender
{
  if([[UIApplication sharedApplication] applicationState] == UIApplicationStateActive)
    [glView drawView];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
  return YES;
}

#pragma mark - Split view

-(BOOL)splitViewController:(UISplitViewController *)svc shouldHideViewController:(UIViewController *)vc
             inOrientation:(UIInterfaceOrientation)orientation
{
	return NO;
}

void messageFromCpp (void *self, std::string level, std::string msg)
{
  if(level == "RequestRender"){
    [(__bridge id)self performSelectorOnMainThread:@selector(requestRender) withObject:nil waitUntilDone:YES];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"refreshParameters" object:nil];
  }
  else if(level == "Progress"){
    [(__bridge id) self performSelectorOnMainThread:@selector(setProgress:)
                                         withObject:[Utils getStringFromCString:msg.c_str()] waitUntilDone:YES];
  }
  else if(level == "Error"){
    [(__bridge id) self performSelectorOnMainThread:@selector(showError:)
                                         withObject:[Utils getStringFromCString:msg.c_str()] waitUntilDone:YES];
  }
}

-(void)setProgress:(NSString *)progress
{
  [_progressLabel setText:progress];
}

-(void)showError:(NSString *)msg
{
  // only show first error
  if(_errorAlert != nil && _errorAlert.visible) return;
  // remove document path from error message
  NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
  NSString *docPath = [[paths objectAtIndex:0] stringByAppendingString:@"/"];
  NSString *str = [msg stringByReplacingOccurrencesOfString:docPath withString:@""];
  _errorAlert = [[UIErrorAlertView alloc] initWithTitle:@"Error" message:str delegate:self
                                      cancelButtonTitle:@"Dismiss" otherButtonTitles:nil, nil];
  [_errorAlert show];
}

-(void)handleProgressIndicatorTap:(id)sender
{
  [_progressLabel setHidden:!_progressLabel.hidden];
}

void getBitmap(void *self, const char *text, int textsize, unsigned char **map,
               int *height, int *width, int *realWidth)
{
  [(__bridge id)self getBitmapFromStringObjC:text withTextSize:textsize inMap:map
                                    inHeight:height inWidth:width inRealWidth:realWidth];
}

-(void) getBitmapFromStringObjC:(const char *)text withTextSize:(int)textsize inMap:(unsigned char **)map
                       inHeight:(int *)height inWidth:(int *)width inRealWidth:(int *) realWidth
{
  UILabel *lbl = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 1024, 7*textsize/6)];
  lbl.font = [UIFont systemFontOfSize:textsize];
  [lbl setText:[Utils getStringFromCString:text]];
  [lbl setBackgroundColor:[UIColor clearColor]];
  CGSize lblSize = [[lbl text] sizeWithAttributes:@{NSFontAttributeName:[lbl font]}];
  *realWidth = lblSize.width;
  int i=2;
  while(i<*realWidth) i*=2;
  *width = i;
  *height = lblSize.height;
  i=2;
  while(i<*height) i*=2;
  *height = i;

  UIGraphicsBeginImageContextWithOptions(CGSizeMake(*width, *height), NO, 0.0);
  [lbl.layer renderInContext:UIGraphicsGetCurrentContext()];
  UIImage *img = UIGraphicsGetImageFromCurrentImageContext();
  UIGraphicsEndImageContext();
  CGImageRef bitmap = [img CGImage];
  CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
  unsigned char *rawData = (unsigned char*) calloc(*height * *width * 4, sizeof(unsigned char));
  *map = (unsigned char*) calloc(*height * *width, sizeof(unsigned char));
  NSUInteger bytesPerPixel = 4;
  NSUInteger bytesPerRow = bytesPerPixel * *width;
  NSUInteger bitsPerComponent = 8;
  CGContextRef context = CGBitmapContextCreate(rawData, *width, *height,
                                               bitsPerComponent, bytesPerRow, colorSpace,
                                               kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
  CGColorSpaceRelease(colorSpace);

  CGContextDrawImage(context, CGRectMake(0, 0, *width, *height), bitmap);
  CGContextRelease(context);

  // rawData contains the image data in the RGBA8888 pixel format.
  for (int byteIndex = 0 ; byteIndex < *width * *height * 4 ; byteIndex+=4)
    *(*map+byteIndex/4) = rawData[byteIndex + 3];
  free(rawData);
}

- (IBAction)startRotation:(UIButton *)sender {
  glView->rotate = !glView->rotate;
  if(glView->rotate)
    [sender setImage:[UIImage imageNamed:@"icon_translate.png"] forState:UIControlStateNormal];
  else
    [sender setImage:[UIImage imageNamed:@"icon_rotate.png"] forState:UIControlStateNormal];
}

@end
