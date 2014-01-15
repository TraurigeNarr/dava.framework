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



#ifndef __SCENE_GRAPH_H__
#define __SCENE_GRAPH_H__

#include "DAVAEngine.h"
#include "GraphBase.h"

using namespace DAVA;

class SceneGraph: public GraphBase
{
protected:
    virtual ~SceneGraph();
public:
    SceneGraph(GraphBaseDelegate *newDelegate, const Rect &rect);
    
    virtual void SelectNode(BaseObject *node);
    virtual void RemoveWorkingNode();
    virtual void UpdatePropertyPanel();

    virtual void RefreshGraph();

    void SetSize(const Vector2 &newSize);

protected:

    void RemoveRootNodes();

    //NodesPropertyDelegate
    virtual bool IsNodeExpandable(UIHierarchy *forHierarchy, void *forNode);
    virtual int32 ChildrenCount(UIHierarchy *forHierarchy, void *forParent);
    virtual void *ChildAtIndex(UIHierarchy *forHierarchy, void *forParent, int32 index);
    virtual void DragAndDrop(void *who, void *target, int32 mode);

    virtual void FillCell(UIHierarchyCell *cell, void *node);
    virtual void SelectHierarchyNode(UIHierarchyNode * node);

    virtual void CreateGraphPanel(const Rect &rect);
    
	void OnRemoveRootNodesButtonPressed(BaseObject * obj, void *, void *);
	void OnRemoveNodeButtonPressed(BaseObject * obj, void *, void *);
    void OnEnableDebugFlagsPressed(BaseObject * obj, void *, void *);
    void OnBakeMatricesPressed(BaseObject * obj, void *, void *);
    void OnBuildQuadTreePressed(BaseObject * obj, void *, void *);
    void OnRefreshGraph(BaseObject * obj, void *, void *);

    void RecreatePropertiesPanelForNode(Entity *node);

    
    Entity * workingNode;
};



#endif // __SCENE_GRAPH_H__