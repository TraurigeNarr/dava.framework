#include "NameProperty.h"

#include "../PackageHierarchy/ControlNode.h"
#include "../PackageSerializer.h"

#include "UI/UIControl.h"

using namespace DAVA;

NameProperty::NameProperty(ControlNode *anControl, const NameProperty *sourceProperty, eCloneType cloneType)
    : ValueProperty("Name")
    , control(anControl) // weak ptr
    , prototypeProperty(nullptr)
{
    if (sourceProperty)
    {
        control->GetControl()->SetName(sourceProperty->GetValue().AsString());

        if (cloneType == CT_INHERIT && control->GetCreationType() == ControlNode::CREATED_FROM_PROTOTYPE_CHILD)
        {
            prototypeProperty = sourceProperty;
        }
    }
}

NameProperty::~NameProperty()
{
    control = nullptr; // weak ptr
    prototypeProperty = nullptr; // weak ptr
}

void NameProperty::Refresh()
{
    if (prototypeProperty)
    {
        SetDefaultValue(prototypeProperty->GetValue());
        ApplyValue(defaultValue);
    }
    ValueProperty::Refresh();
}

AbstractProperty *NameProperty::FindPropertyByPrototype(AbstractProperty *prototype)
{
    return prototypeProperty == prototype ? this : nullptr;
}

void NameProperty::Serialize(PackageSerializer *serializer) const
{
    switch (control->GetCreationType())
    {
        case ControlNode::CREATED_FROM_PROTOTYPE:
        case ControlNode::CREATED_FROM_CLASS:
            serializer->PutValue("name", control->GetName());
            break;

        case ControlNode::CREATED_FROM_PROTOTYPE_CHILD:
            serializer->PutValue("path", control->GetPathToPrototypeChild(false));
            break;
            
        default:
            DVASSERT(false);
    }
}

bool NameProperty::IsReadOnly() const
{
    return control->GetCreationType() == ControlNode::CREATED_FROM_PROTOTYPE_CHILD || ValueProperty::IsReadOnly();
}

NameProperty::ePropertyType NameProperty::GetType() const
{
    return TYPE_VARIANT;
}

VariantType NameProperty::GetValue() const
{
    return VariantType(control->GetName());
}

bool NameProperty::IsReplaced() const
{
    return control->GetCreationType() != ControlNode::CREATED_FROM_PROTOTYPE_CHILD;
}

void NameProperty::ApplyValue(const DAVA::VariantType &value)
{
    if (value.GetType() == VariantType::TYPE_STRING)
    {
        control->GetControl()->SetName(value.AsString());
    }
    else
    {
        DVASSERT(false);
    }
}