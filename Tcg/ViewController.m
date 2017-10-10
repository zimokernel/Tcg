//
//  ViewController.m
//  Tcg
//
//  Created by ttc on 09/10/2017.
//  Copyright © 2017 TY. All rights reserved.
//

#import "ViewController.h"
#import "es_common.h"

extern void triangle_main(ESContext *ctx);


@interface ViewController()
{
    ESContext _esContext;
}
@property(strong, nonatomic) EAGLContext *context;
@property(strong, nonatomic) GLKBaseEffect *effect;
-(void)setupGL;
-(void)tearDownGL;
@end

@implementation ViewController

-(void)viewDidLoad {
    [super viewDidLoad];
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    
    if(!self.context) {
        NSLog(@"Failed to create ES context");
    }
    GLKView *view =(GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [self setupGL];
}

-(void)dealloc
{
    [self tearDownGL];
    if([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

-(void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    if([self isViewLoaded] &&([[self view] window] == nil)) {
        self.view = nil;
        [self tearDownGL];
        if([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }
    // Dispose of any resources that can be recreated.
}

-(void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    memset(&_esContext, 0, sizeof(_esContext));
    triangle_main(&_esContext);
}

-(void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    
    if(_esContext.shutdownfunc) {
        _esContext.shutdownfunc(&_esContext);
    }
}

-(void)update
{
    if(_esContext.updatefunc) {
        _esContext.updatefunc(&_esContext, self.timeSinceLastUpdate);
    }
}

-(void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    _esContext.width = (GLint)view.drawableWidth;
    _esContext.height = (GLint)view.drawableHeight;
    
    if(_esContext.drawfunc) {
        _esContext.drawfunc(&_esContext);
    }
}

@end
