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

void ObjCCalls::showAd()
{
    [(RootViewController*)[UIApplication sharedApplication].keyWindow.rootViewController showAd];
}