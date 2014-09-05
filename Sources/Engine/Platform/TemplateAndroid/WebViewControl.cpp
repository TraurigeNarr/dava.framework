/*==================================================================================
    Copyright (c) 2008, binaryzebra
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    * Neither the name of the binaryzebra nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE binaryzebra AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL binaryzebra BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
=====================================================================================*/



#include "WebViewControl.h"
#include "FileSystem/Logger.h"

namespace DAVA
{

int WebViewControl::webViewIdCount = 0;

jclass JniWebView::gJavaClass = NULL;
const char* JniWebView::gJavaClassName = NULL;
JniWebView::CONTROLS_MAP JniWebView::controls;


jclass JniWebView::GetJavaClass() const
{
	return gJavaClass;
}

const char* JniWebView::GetJavaClassName() const
{
	return gJavaClassName;
}

void JniWebView::Initialize(WebViewControl* control, int id, const Rect& controlRect)
{
	controls[id] = control;
	Rect rect = V2P(controlRect);

	jmethodID mid = GetMethodID("Initialize", "(IFFFF)V");
	if (mid)
		GetEnvironment()->CallStaticVoidMethod(GetJavaClass(), mid, id, rect.x, rect.y, rect.dx, rect.dy);
}

void JniWebView::Deinitialize(int id)
{
	controls.erase(id);
	jmethodID mid = GetMethodID("Deinitialize", "(I)V");
	if (mid)
		GetEnvironment()->CallStaticVoidMethod(GetJavaClass(), mid, id);
}

void JniWebView::OpenURL(int id, const String& urlToOpen)
{
	jmethodID mid = GetMethodID("OpenURL", "(ILjava/lang/String;)V");
	if (mid)
	{
		jstring jUrlToOpen = GetEnvironment()->NewStringUTF(urlToOpen.c_str());
		GetEnvironment()->CallStaticVoidMethod(GetJavaClass(), mid, id, jUrlToOpen);
		GetEnvironment()->DeleteLocalRef(jUrlToOpen);
	}
}

void JniWebView::OpenFromBuffer(int id, const String& string, const String& baseUrl)
{
	jmethodID mid = GetMethodID("OpenFromBuffer", "(ILjava/lang/String;Ljava/lang/String;)V");
	if (mid)
	{
		jstring jString = GetEnvironment()->NewStringUTF(string.c_str());
		jstring jBaseUrl = GetEnvironment()->NewStringUTF(baseUrl.c_str());
		GetEnvironment()->CallStaticVoidMethod(GetJavaClass(), mid, id, jString, jBaseUrl);
		GetEnvironment()->DeleteLocalRef(jString);
		GetEnvironment()->DeleteLocalRef(jBaseUrl);
	}
}

void JniWebView::SetRect(int id, const Rect& controlRect)
{
	Rect rect = V2P(controlRect);
	jmethodID mid = GetMethodID("SetRect", "(IFFFF)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(GetJavaClass(), mid, id, rect.x, rect.y, rect.dx, rect.dy);
	}
}

void JniWebView::SetVisible(int id, bool isVisible)
{
	jmethodID mid = GetMethodID("SetVisible", "(IZ)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(GetJavaClass(), mid, id, isVisible);
	}
}

void JniWebView::SetBackgroundTransparency(int id, bool enabled)
{
	jmethodID mid = GetMethodID("SetBackgroundTransparency", "(IZ)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(GetJavaClass(), mid, id, enabled);
	}
}

IUIWebViewDelegate::eAction JniWebView::URLChanged(int id, const String& newURL)
{
	CONTROLS_MAP::iterator iter = controls.find(id);
	if (iter == controls.end())
	{
		Logger::Debug("Error web view id=%d", id);
		return IUIWebViewDelegate::PROCESS_IN_WEBVIEW;
	}

	WebViewControl* control = iter->second;
	IUIWebViewDelegate *delegate = control->delegate;
	if (!delegate)
		return IUIWebViewDelegate::PROCESS_IN_WEBVIEW;

	return delegate->URLChanged(control->webView, newURL, true);
}

void JniWebView::PageLoaded(int id)
{
	CONTROLS_MAP::iterator iter = controls.find(id);
	if (iter == controls.end())
	{
		Logger::Debug("Error web view id=%d", id);
		return;
	}

	WebViewControl* control = iter->second;
	IUIWebViewDelegate *delegate = control->delegate;
	if (delegate)
	{
		delegate->PageLoaded(control->webView);
	}
}

WebViewControl::WebViewControl()
{
	delegate = NULL;
	webView = NULL;

	webViewId = webViewIdCount++;
}

WebViewControl::~WebViewControl()
{
	JniWebView jniWebView;
	jniWebView.Deinitialize(webViewId);
}

void WebViewControl::Initialize(const Rect& rect)
{
	JniWebView jniWebView;
	jniWebView.Initialize(this, webViewId, rect);
}

void WebViewControl::OpenURL(const String& urlToOpen)
{
	JniWebView jniWebView;
	jniWebView.OpenURL(webViewId, urlToOpen);
}

void WebViewControl::OpenFromBuffer(const String& data, const FilePath& urlToOpen)
{
	JniWebView jniWebView;
	jniWebView.OpenFromBuffer(webViewId, data, urlToOpen.GetAbsolutePathname());
}

void WebViewControl::SetRect(const Rect& rect)
{
	JniWebView jniWebView;
	jniWebView.SetRect(webViewId, rect);
}

void WebViewControl::SetVisible(bool isVisible, bool hierarchic)
{
	JniWebView jniWebView;
	jniWebView.SetVisible(webViewId, isVisible);
}

void WebViewControl::SetDelegate(DAVA::IUIWebViewDelegate *delegate, DAVA::UIWebView* webView)
{
	this->delegate = delegate;
	this->webView = webView;
}

void WebViewControl::SetBackgroundTransparency(bool enabled)
{
	JniWebView jniWebView;
	jniWebView.SetBackgroundTransparency(webViewId, enabled);
}

}//namespace DAVA