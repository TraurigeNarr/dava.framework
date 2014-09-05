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


#ifndef __BASESCREEN_H__
#define __BASESCREEN_H__

#include "DAVAEngine.h"
using namespace DAVA;

#include "GameCore.h"

class TestData
{
public:

    TestData()
    {
        name = String("");
        totalTime = minTime = maxTime = startTime = endTime = 0;
        maxTimeIndex = runCount = 0;
        userData = NULL;
    }
    
    String name;
    uint64 totalTime;
    uint64 minTime;
    uint64 maxTime;
    int32 maxTimeIndex;
    uint64 startTime;
    uint64 endTime;
    Vector<uint64> eachRunTime;
    int32 runCount;
    void * userData;
};

class BaseScreen: public UIScreen
{
protected:
	~BaseScreen(){}
public:

	BaseScreen();
	BaseScreen(const String & screenName, int32 skipBeforeTests = 10);
    int32 GetScreenId();

    virtual void WillAppear();
    virtual void DidAppear();
    virtual void Update(float32 timeElapsed);
    
    bool ReadyForTests();
    
    virtual int32 GetTestCount() = 0;
    virtual TestData * GetTestData(int32 iTest) = 0;
    
    virtual bool RunTest(int32 testNum) = 0;
    
private:
    static int32 globalScreenId; // 1, on create of screen increment  
    int32 currentScreenId;

    int32 skipCount;
    int32 skipCounter;
    bool readyForTests;
};


#endif // __BASESCREEN_H__