//
//  ObjCCalls.mm
//  GREHitParade
//
//  Created by Lâm Thái on 2/4/15.
//
//

#include "ObjCCalls.h"
#include "RootViewController.h"

void ObjCCalls::playSound(const char *text)
{
    [(RootViewController*)[UIApplication sharedApplication].keyWindow.rootViewController textSynthesis:[NSString stringWithUTF8String:text]];
}

bool ObjCCalls::showAd()
{
    return [(RootViewController*)[UIApplication sharedApplication].keyWindow.rootViewController showAd];
}

void ObjCCalls::openInAppStore()
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"https://itunes.apple.com/app/id966810494?ls=1&mt=8"]];
}