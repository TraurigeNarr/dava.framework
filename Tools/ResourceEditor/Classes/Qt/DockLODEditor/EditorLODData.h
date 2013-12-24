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



#ifndef __EDITOR_LOD_DATA_H__
#define __EDITOR_LOD_DATA_H__

#include <QObject>
#include "DAVAEngine.h"

class SceneEditor2;
class EntityGroup;
class Command2;

class EditorLODData: public QObject
{
    Q_OBJECT
    
public:

    EditorLODData();
    virtual ~EditorLODData();

    
    DAVA::int32 GetLayersCount() const;
    DAVA::float32 GetLayerDistance(DAVA::int32 layerNum) const;
    void SetLayerDistance(DAVA::int32 layerNum, DAVA::float32 distance);

	void UpdateDistances(const DAVA::Map<DAVA::int32, DAVA::float32> & lodDistances);

    DAVA::uint32 GetLayerTriangles(DAVA::int32 layerNum) const;

    void EnableForceDistance(bool enable);
    bool GetForceDistanceEnabled() const;
    
    void SetForceDistance(DAVA::float32 distance);
    DAVA::float32 GetForceDistance() const;

    void SetForceLayer(DAVA::int32 layer);
    DAVA::int32 GetForceLayer() const;

    void GetDataFromSelection();

    static void EnumerateLODsRecursive(DAVA::Entity *entity, DAVA::Vector<DAVA::LodComponent *> & lods);
    static DAVA::uint32 GetTrianglesForLodLayer(DAVA::LodComponent::LodData *lodData, bool checkVisibility);
    static DAVA::uint32 GetTrianglesForEntity(DAVA::Entity *entity, bool checkVisibility);
    
    //TODO: remove after lod editing implementation
    DAVA_DEPRECATED(void CopyLastLodToLod0());

signals:
    
    void DataChanged();
    
protected slots:
    void SceneActivated(SceneEditor2 *scene);
	void SceneDeactivated(SceneEditor2 *scene);
    void SceneStructureChanged(SceneEditor2 *scene, DAVA::Entity *parent);
	void SceneSelectionChanged(SceneEditor2 *scene, const EntityGroup *selected, const EntityGroup *deselected);

    void CommandExecuted(SceneEditor2 *scene, const Command2* command, bool redo);
    
protected:
    
    void ClearLODData();
    void ClearForceData();
    
    void UpdateForceData();

    
    void EnumerateSelectionLODs(SceneEditor2 * scene);

    void ResetForceState(DAVA::Entity *entity);
    
    DAVA::int32 GetLayersCount(DAVA::LodComponent *lod) const;
    
    
protected:

    DAVA::int32 lodLayersCount;
    DAVA::float32 lodDistances[DAVA::LodComponent::MAX_LOD_LAYERS];
    DAVA::uint32 lodTriangles[DAVA::LodComponent::MAX_LOD_LAYERS];

    
    bool forceDistanceEnabled;
    DAVA::float32 forceDistance;
    DAVA::int32 forceLayer;
    
    
    DAVA::Vector<DAVA::LodComponent *> lodData;
    
    SceneEditor2 *activeScene;
};

#endif //#ifndef __EDITOR_LOD_DATA_H__
