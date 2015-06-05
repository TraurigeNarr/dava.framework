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


#ifndef __TEST_LIST_SCREEN_H__
#define __TEST_LIST_SCREEN_H__

#include "BaseScreen.h"
#include "UI/UIList.h"

using namespace DAVA;

class TestListScreen : public UIScreen, public UIListDelegate
{
protected:
    virtual ~TestListScreen();

public:
    TestListScreen();
    
    void LoadResources() override;
    void UnloadResources() override;
    
    void AddTestScreen(BaseScreen *screen);
    
//UIListDelegate interface
private:
    float32 CellHeight(UIList * list, int32 index) override;
    int32 ElementsCount(UIList * list) override;
    UIListCell *CellAtIndex(UIList *list, int32 index) override;
    void OnCellSelected(UIList *forList, UIListCell *selectedCell) override;

private:
    Vector<BaseScreen *> testScreens;
    UIList *testsGrid;
    float32 cellHeight;
};

#endif