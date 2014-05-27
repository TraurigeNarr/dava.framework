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

#include "UI/UITextFieldImpl.h"

using namespace DAVA;

class JniTextField: public JniExtension
{
public:
    JniTextField(uint32_t id);

    void Create(Rect rect);
    void Destroy();
    void UpdateRect(const Rect & rect);
    void SetText(const char* text);
    void SetTextColor(float r, float g, float b, float a);
    void SetFontSize(float size);
    void SetIsPassword(bool isPassword);
    void SetTextAlign(int32_t align);
    void SetInputEnabled(bool value);
    void SetAutoCapitalizationType(int32_t value);
    void SetAutoCorrectionType(int32_t value);
    void SetSpellCheckingType(int32_t value);
    void SetKeyboardAppearanceType(int32_t value);
    void SetKeyboardType(int32_t value);
    void SetReturnKeyType(int32_t value);
    void SetEnableReturnKeyAutomatically(bool value);
    void ShowField();
    void HideField();
    void OpenKeyboard();
    void CloseKeyboard();
    uint32 GetCursorPos();
    void SetCursorPos(uint32 pos);

protected:
    virtual jclass GetJavaClass() const;
    virtual const char* GetJavaClassName() const;

public:
    static jclass gJavaClass;
    static const char* gJavaClassName;

private:
    uint32_t id;
};

jclass JniTextField::gJavaClass = NULL;
const char* JniTextField::gJavaClassName = NULL;

jclass JniTextField::GetJavaClass() const
{
	return gJavaClass;
}

const char* JniTextField::GetJavaClassName() const
{
	return gJavaClassName;
}

JniTextField::JniTextField(uint32_t id)
{
	this->id = id;
}

void JniTextField::Create(Rect controlRect)
{
	Rect rect = V2P(controlRect);
	jmethodID mid = GetMethodID("Create", "(IFFFF)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				rect.x,
				rect.y,
				rect.dx,
				rect.dy);
	}
}

void JniTextField::Destroy()
{
	jmethodID mid = GetMethodID("Destroy", "(I)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id);
	}
}

void JniTextField::UpdateRect(const Rect & controlRect)
{
	Rect rect = V2P(controlRect);
	jmethodID mid = GetMethodID("UpdateRect", "(IFFFF)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				rect.x,
				rect.y,
				rect.dx,
				rect.dy);
	}
}

void JniTextField::SetText(const char* text)
{
	jmethodID mid = GetMethodID("SetText", "(ILjava/lang/String;)V");
	if (mid)
	{
		jstring jStrDefaultText = GetEnvironment()->NewStringUTF(text);
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				jStrDefaultText);
		GetEnvironment()->DeleteLocalRef(jStrDefaultText);
	}
}

void JniTextField::SetTextColor(float r, float g, float b, float a)
{
	jmethodID mid = GetMethodID("SetTextColor", "(IFFFF)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				r,
				g,
				b,
				a);
	}
}

void JniTextField::SetFontSize(float size)
{
	jmethodID mid = GetMethodID("SetFontSize", "(IF)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				size);
	}
}

void JniTextField::SetIsPassword(bool isPassword)
{
	jmethodID mid = GetMethodID("SetIsPassword", "(IZ)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				isPassword);
	}
}

void JniTextField::SetTextAlign(int32_t align)
{
	jmethodID mid = GetMethodID("SetTextAlign", "(II)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				align);
	}
}

void JniTextField::SetInputEnabled(bool value)
{
	jmethodID mid = GetMethodID("SetInputEnabled", "(IZ)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				value);
	}
}

void JniTextField::SetAutoCapitalizationType(int32_t value)
{
	jmethodID mid = GetMethodID("SetAutoCapitalizationType", "(II)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				value);
	}
}

void JniTextField::SetAutoCorrectionType(int32_t value)
{
	jmethodID mid = GetMethodID("SetAutoCorrectionType", "(II)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				value);
	}
}

void JniTextField::SetSpellCheckingType(int32_t value)
{
	jmethodID mid = GetMethodID("SetSpellCheckingType", "(II)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				value);
	}
}

void JniTextField::SetKeyboardAppearanceType(int32_t value)
{
	jmethodID mid = GetMethodID("SetKeyboardAppearanceType", "(II)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				value);
	}
}

void JniTextField::SetKeyboardType(int32_t value)
{
	jmethodID mid = GetMethodID("SetKeyboardType", "(II)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				value);
	}
}

void JniTextField::SetReturnKeyType(int32_t value)
{
	jmethodID mid = GetMethodID("SetReturnKeyType", "(II)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				value);
	}
}

void JniTextField::SetEnableReturnKeyAutomatically(bool value)
{
	jmethodID mid = GetMethodID("SetEnableReturnKeyAutomatically", "(IZ)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				value);
	}
}

void JniTextField::HideField()
{
	jmethodID mid = GetMethodID("HideField", "(I)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id);
	}
}

void JniTextField::ShowField()
{
	jmethodID mid = GetMethodID("ShowField", "(I)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id);
	}
}

void JniTextField::OpenKeyboard()
{
	jmethodID mid = GetMethodID("OpenKeyboard", "(I)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id);
	}
}

void JniTextField::CloseKeyboard()
{
	jmethodID mid = GetMethodID("CloseKeyboard", "(I)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id);
	}
}

uint32 JniTextField::GetCursorPos()
{
	jmethodID mid = GetMethodID("GetCursorPos", "(I)I");
	if (!mid)
		return 0;

	return GetEnvironment()->CallStaticIntMethod(GetJavaClass(), mid, id);
}

void JniTextField::SetCursorPos(uint32 pos)
{
	jmethodID mid = GetMethodID("SetCursorPos", "(II)V");
	if (!mid)
		return;
	GetEnvironment()->CallStaticVoidMethod(GetJavaClass(), mid, id, pos);
}

uint32_t UITextFieldImpl::sId = 0;
DAVA::Map<uint32_t, UITextFieldImpl*> UITextFieldImpl::controls;

UITextFieldImpl::UITextFieldImpl(UITextField* tf)
{
	textField = tf;
	id = sId++;
	rect = textField->GetRect();
	JniTextField jniTextField(id);
	jniTextField.Create(rect);

	controls[id] = this;
}

UITextFieldImpl::~UITextFieldImpl()
{
	controls.erase(id);

	JniTextField jniTextField(id);
	jniTextField.Destroy();
}

void UITextFieldImpl::OpenKeyboard()
{
	JniTextField jniTextField(id);
	jniTextField.OpenKeyboard();
}

void UITextFieldImpl::CloseKeyboard()
{
	JniTextField jniTextField(id);
	jniTextField.CloseKeyboard();
}

void UITextFieldImpl::GetText(WideString & string) const
{
	string = text;
}

void UITextFieldImpl::SetText(const WideString & string)
{
	if (text.compare(string) != 0)
	{
		text = string;
		JniTextField jniTextField(id);
		String utfText = WStringToString(text);
		jniTextField.SetText(utfText.c_str());
	}
}

void UITextFieldImpl::UpdateRect(const Rect & newRect, float32 timeElapsed)
{
	if (newRect != rect)
	{
		rect = newRect;
		JniTextField jniTextField(id);
		jniTextField.UpdateRect(rect);
	}
}

void UITextFieldImpl::SetTextColor(const DAVA::Color &color)
{
	JniTextField jniTextField(id);
	jniTextField.SetTextColor(color.r, color.g, color.b, color.a);
}

void UITextFieldImpl::SetFontSize(float size)
{
	JniTextField jniTextField(id);
	jniTextField.SetFontSize(size);
}

void UITextFieldImpl::SetTextAlign(DAVA::int32 newAlign)
{
	align = newAlign;
	JniTextField jniTextField(id);
	jniTextField.SetTextAlign(align);
}

DAVA::int32 UITextFieldImpl::GetTextAlign()
{
	return align;
}

void UITextFieldImpl::ShowField()
{
	JniTextField jniTextField(id);
	jniTextField.ShowField();
}

void UITextFieldImpl::HideField()
{
	JniTextField jniTextField(id);
	jniTextField.HideField();
}

void UITextFieldImpl::SetVisible(bool isVisible)
{
	if (isVisible)
		ShowField();
	else
		HideField();
}

void UITextFieldImpl::SetIsPassword(bool isPassword)
{
	JniTextField jniTextField(id);
	jniTextField.SetIsPassword(isPassword);
}

void UITextFieldImpl::SetInputEnabled(bool value)
{
	JniTextField jniTextField(id);
	jniTextField.SetInputEnabled(value);
}

// Keyboard traits.
void UITextFieldImpl::SetAutoCapitalizationType(DAVA::int32 value)
{
	JniTextField jniTextField(id);
	jniTextField.SetAutoCapitalizationType(value);
}

void UITextFieldImpl::SetAutoCorrectionType(DAVA::int32 value)
{
	JniTextField jniTextField(id);
	jniTextField.SetAutoCorrectionType(value);
}

void UITextFieldImpl::SetSpellCheckingType(DAVA::int32 value)
{
	JniTextField jniTextField(id);
	jniTextField.SetSpellCheckingType(value);
}

void UITextFieldImpl::SetKeyboardAppearanceType(DAVA::int32 value)
{
	JniTextField jniTextField(id);
	jniTextField.SetKeyboardAppearanceType(value);
}

void UITextFieldImpl::SetKeyboardType(DAVA::int32 value)
{
	JniTextField jniTextField(id);
	jniTextField.SetKeyboardType(value);
}

void UITextFieldImpl::SetReturnKeyType(DAVA::int32 value)
{
	JniTextField jniTextField(id);
	jniTextField.SetReturnKeyType(value);
}

void UITextFieldImpl::SetEnableReturnKeyAutomatically(bool value)
{
	JniTextField jniTextField(id);
	jniTextField.SetEnableReturnKeyAutomatically(value);
}

uint32 UITextFieldImpl::GetCursorPos()
{
	JniTextField jniTextField(id);
	return jniTextField.GetCursorPos();
}

void UITextFieldImpl::SetCursorPos(uint32 pos)
{
	JniTextField jniTextField(id);
	return jniTextField.SetCursorPos(pos);
}

bool UITextFieldImpl::TextFieldKeyPressed(int32 replacementLocation, int32 replacementLength, const WideString &text)
{
	bool res = true;
	UITextFieldDelegate* delegate = textField->GetDelegate();
	if (delegate)
		res = delegate->TextFieldKeyPressed(textField, replacementLocation, replacementLength, text);

	if (res)
	{
		WideString curText = textField->GetText();
		if (curText.length() >= replacementLocation)
		{
			curText.replace(replacementLocation, replacementLength, text);
			this->text = curText;
		}
	}
	return res;
}

bool UITextFieldImpl::TextFieldKeyPressed(uint32_t id, int32 replacementLocation, int32 replacementLength, const WideString &text)
{
	UITextFieldImpl* control = GetUITextFieldAndroid(id);
	if (!control)
		return false;

	return control->TextFieldKeyPressed(replacementLocation, replacementLength, text);
}

void UITextFieldImpl::TextFieldShouldReturn()
{
	UITextFieldDelegate* delegate = textField->GetDelegate();
	if (delegate)
		delegate->TextFieldShouldReturn(textField);
}

void UITextFieldImpl::TextFieldShouldReturn(uint32_t id)
{
	UITextFieldImpl* control = GetUITextFieldAndroid(id);
	if (!control)
		return;

	control->TextFieldShouldReturn();
}

UITextFieldImpl* UITextFieldImpl::GetUITextFieldAndroid(uint32_t id)
{
	DAVA::Map<uint32_t, UITextFieldImpl*>::iterator iter = controls.find(id);
	if (iter != controls.end())
		return iter->second;

	return NULL;
}