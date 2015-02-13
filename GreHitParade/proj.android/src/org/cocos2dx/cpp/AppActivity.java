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

import android.speech.tts.TextToSpeech;
import android.speech.tts.TextToSpeech.OnInitListener;
import android.widget.Toast;

public class AppActivity extends Cocos2dxActivity implements OnInitListener {
	private static TextToSpeech myTTS = null;
	private static boolean ttsAvailable = false;
	
	@Override
	public void init() {
		super.init();
		try {
			myTTS = new TextToSpeech(this, this);
		} catch(Exception e) {
			Toast.makeText(this, "Unable to initialize Text-to-Speech engine...", Toast.LENGTH_LONG).show();
		}
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
}
