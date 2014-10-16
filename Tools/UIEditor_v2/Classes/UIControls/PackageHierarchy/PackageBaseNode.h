//
//  UIPackageModelNode.h
//  UIEditor
//
//  Created by Dmitry Belsky on 3.10.14.
//
//

#ifndef __UI_EDITOR_UI_PACKAGE_MODEL_NODE__
#define __UI_EDITOR_UI_PACKAGE_MODEL_NODE__

#include "DAVAEngine.h"

class PackageBaseNode : public DAVA::BaseObject
{
public:
    static const int FLAG_READ_ONLY = 0x01;
    static const int FLAG_CONTROL_CREATED_FROM_CLASS = 0x02;
    static const int FLAG_CONTROL_CREATED_FROM_PROTOTYPE = 0x04;
    static const int FLAG_CONTROL_CREATED_FROM_PROTOTYPE_CHILD = 0x08;

    static const int FLAGS_CONTROL = FLAG_CONTROL_CREATED_FROM_CLASS | FLAG_CONTROL_CREATED_FROM_PROTOTYPE | FLAG_CONTROL_CREATED_FROM_PROTOTYPE_CHILD;
    static const int FLAGS_INSTANCED_PROTOTYPE = FLAG_CONTROL_CREATED_FROM_PROTOTYPE | FLAG_CONTROL_CREATED_FROM_PROTOTYPE_CHILD;

public:
    PackageBaseNode(PackageBaseNode *parent);
    virtual ~PackageBaseNode();
    
    virtual int GetCount() const = 0;
    virtual PackageBaseNode *Get(int index) const = 0;
    int GetIndex(PackageBaseNode *node) const;
    
    PackageBaseNode *GetParent() const;
    void SetParent(PackageBaseNode *parent);
    
    virtual DAVA::String GetName() const;
    virtual DAVA::UIControl *GetControl() const;
    
    virtual int GetFlags() const = 0;
    
    virtual void debugDump(int depth);
    
private:
    PackageBaseNode *parent;
};




#endif // __UI_EDITOR_UI_PACKAGE_MODEL_NODE__