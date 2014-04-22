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

#ifndef __SOUND_BROWSER_H__
#define __SOUND_BROWSER_H__

#include <QDialog>
#include <QTreeWidgetItem>
#include "DAVAEngine.h"

namespace Ui {
class FMODSoundBrowser;
}

class SceneEditor2;

class FMODSoundBrowser : public QDialog, public DAVA::Singleton<FMODSoundBrowser>
{
    Q_OBJECT
    
public:
    explicit FMODSoundBrowser(QWidget *parent = 0);
    virtual ~FMODSoundBrowser();

    DAVA::String GetSelectSoundEvent();

#ifdef DAVA_FMOD
    static DAVA::FilePath MakeFEVPathFromScenePath(const DAVA::FilePath & scenePath);
#endif

private slots:
    void OnEventSelected(QTreeWidgetItem * item, int column);
    void OnEventDoubleClicked(QTreeWidgetItem * item, int column);

    void OnSceneLoaded(SceneEditor2 * scene);
    void OnSceneClosed(SceneEditor2 * scene);

    void OnAccepted();
    void OnRejected();

private:
    void UpdateEventTree();

    void FillEventsTree(const DAVA::Vector<DAVA::String> & names);
    void SelectItemAndExpandTreeByEventName(const DAVA::String & eventName);
    
    void SetSelectedItem(QTreeWidgetItem * item);

    DAVA::Map<DAVA::Scene *, DAVA::FilePath> projectsMap;
    QTreeWidgetItem * selectedItem;
    Ui::FMODSoundBrowser *ui;
};

#endif // SOUNDBROWSER_H