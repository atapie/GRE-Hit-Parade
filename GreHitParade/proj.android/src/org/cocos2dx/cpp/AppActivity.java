/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
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
package org.cocos2dx.cpp;

import java.util.Locale;

import org.cocos2dx.lib.Cocos2dxActivity;

import com.google.android.gms.ads.*;
import com.joto.grehit.R;

import android.speech.tts.TextToSpeech;
import android.speech.tts.TextToSpeech.OnInitListener;
import android.widget.Toast;

public class AppActivity extends Cocos2dxActivity implements OnInitListener {
	private static TextToSpeech myTTS = null;
	private static boolean ttsAvailable = false;
	
	// Ads
	private InterstitialAd interstitial = null;
	private void reloadInterstitialAd() {
		interstitial = new InterstitialAd(this);
	    interstitial.setAdUnitId(getString(R.string.banner_ad_unit_id));
	    interstitial.setAdListener(new AdListener() {
            @Override
            public void onAdClosed() {
            	reloadInterstitialAd();
            }
        });

	    AdRequest.Builder builder = new AdRequest.Builder();
	    //builder.addTestDevice("84E2034CD7B59DA216F81F5FEEFA476C");
	    interstitial.loadAd(builder.build());
	}
	public void showInterstitialAd() {
		if(interstitial.isLoaded()) interstitial.show();
		else reloadInterstitialAd();
	}
	
	@Override
	public void init() {
		super.init();
		try {
			myTTS = new TextToSpeech(this, this);
		} catch(Exception e) {
			Toast.makeText(this, "Unable to initialize Text-to-Speech engine...", Toast.LENGTH_LONG).show();
		}
		
		// Load Ad
		reloadInterstitialAd();
	}
	
	public void onInit(int initStatus) {
	    if (initStatus == TextToSpeech.SUCCESS) {
	    	ttsAvailable = true;
	    	try {
	    		myTTS.setLanguage(Locale.ENGLISH);
	    	} catch(Exception e) { }
	    } else {
	    	Toast.makeText(this, "Unable to initialize Text-to-Speech engine...", Toast.LENGTH_LONG).show();
	    }
	}
	
	public static void playSound(String text) {
		if(ttsAvailable) {
			try {
				myTTS.speak(text, TextToSpeech.QUEUE_FLUSH, null);
			} catch(Exception e) { }
		}
	}
	
	public static void showAd() {
		((AppActivity)getContext()).runOnUiThread(new Runnable()
	    {
	        @Override
	        public void run()
	        {
	        	((AppActivity)AppActivity.getContext()).showInterstitialAd();
	        }
	    });
	}
}
