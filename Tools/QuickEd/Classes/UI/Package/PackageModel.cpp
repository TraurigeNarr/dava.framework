#include "PackageModel.h"

#include <qicon.h>
#include <QAction>

#include "DAVAEngine.h"
#include "Base/ObjectFactory.h"

#include "UI/IconHelper.h"
#include "UI/Document.h"
#include "Utils/QtDavaConvertion.h"
#include "Model/PackageHierarchy/PackageNode.h"
#include "Model/PackageHierarchy/ControlNode.h"
#include "Model/PackageHierarchy/PackageControlsNode.h"
#include "Model/PackageHierarchy/ImportedPackagesNode.h"
#include "Model/PackageHierarchy/ControlPrototype.h"
#include "Model/YamlPackageSerializer.h"
#include "UI/Commands/PackageModelCommands.h"

#include "PackageMimeData.h"

using namespace DAVA;

PackageModel::PackageModel(Document *document)
    : QAbstractItemModel(document)
    , root(NULL)
    , document(document)
{
    root = SafeRetain(document->GetPackage());
}

PackageModel::~PackageModel()
{
    document = NULL;
    SafeRelease(root);
}

QModelIndex PackageModel::indexByNode(PackageBaseNode *node) const
{
    PackageBaseNode *parent = node->GetParent();
    if (parent == NULL)
        return QModelIndex();
    
    if (parent)
        return createIndex(parent->GetIndex(node), 0, node);
    else
        return createIndex(0, 0, parent);
}

QModelIndex PackageModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if (!parent.isValid())
        return createIndex(row, column, root->Get(row));

    PackageBaseNode *node = static_cast<PackageBaseNode*>(parent.internalPointer());
    return createIndex(row, column, node->Get(row));
}

QModelIndex PackageModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    PackageBaseNode *node = static_cast<PackageBaseNode*>(child.internalPointer());
    PackageBaseNode *parent = node->GetParent();
    if (parent == NULL || parent == root)
        return QModelIndex();
    
    if (parent->GetParent())
        return createIndex(parent->GetParent()->GetIndex(parent), 0, parent);
    else
        return createIndex(0, 0, parent);
}

int PackageModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return root ? root->GetCount() : 0;
    
    return static_cast<PackageBaseNode*>(parent.internalPointer())->GetCount();
}

int PackageModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 1;
}

QVariant PackageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    PackageBaseNode *node = static_cast<PackageBaseNode*>(index.internalPointer());
    
    int prototypeFlag = PackageBaseNode::FLAG_CONTROL_CREATED_FROM_PROTOTYPE | PackageBaseNode::FLAG_CONTROL_CREATED_FROM_PROTOTYPE_CHILD;
    int controlFlag = PackageBaseNode::FLAG_CONTROL_CREATED_FROM_CLASS | PackageBaseNode::FLAG_CONTROL_CREATED_FROM_PROTOTYPE | PackageBaseNode::FLAG_CONTROL_CREATED_FROM_PROTOTYPE_CHILD;
    int flags = node->GetFlags();
    switch(role)
    {
        case Qt::DisplayRole:
            return StringToQString(node->GetName());
            
        case Qt::DecorationRole:
            return node->GetControl() != NULL ? QIcon(IconHelper::GetIconPathForUIControl(node->GetControl())) : QVariant();
            
        case Qt::CheckStateRole:
            if (node->GetControl())
                return node->GetControl()->GetVisibleForUIEditor() ? Qt::Checked : Qt::Unchecked;
            else
                return QVariant();
            
        case Qt::ToolTipRole:
            if (node->GetControl() != NULL)
            {
                ControlNode *controlNode = DynamicTypeCheck<ControlNode *>(node);
                QString toolTip = QString("class: ") + controlNode->GetControl()->GetControlClassName().c_str();
                if (!controlNode->GetControl()->GetCustomControlClassName().empty())
                {
                    toolTip += QString("\ncustom class: ") + controlNode->GetControl()->GetCustomControlClassName().c_str();
                }

                if (controlNode->GetPrototype())
                {
                    bool withPackage = true; // TODO fix for currentPackage
                    toolTip += QString("\nprototype: ") + controlNode->GetPrototype()->GetName(withPackage).c_str();
                }
                return toolTip;
            }
            break;
            
        case Qt::TextColorRole:
            return (flags & prototypeFlag) != 0 ? QColor(Qt::blue) : QColor(Qt::black);
            
        case Qt::BackgroundRole:
            return (flags & controlFlag) == 0 ? QColor(Qt::lightGray) : QColor(Qt::white);
            
        case Qt::FontRole:
        {
            QFont myFont;
            if ((flags & PackageBaseNode::FLAG_CONTROL_CREATED_FROM_PROTOTYPE) != 0 || (flags & controlFlag) == 0)
                myFont.setBold(true);
            if ((flags & PackageBaseNode::FLAG_READ_ONLY) != 0)
                myFont.setItalic(true);
            
            return myFont;
        }
            
        default:
            return QVariant();
    }

    return QVariant();
}

bool PackageModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    
    PackageBaseNode *node = static_cast<PackageBaseNode*>(index.internalPointer());
    
    if (role == Qt::CheckStateRole)
    {
        if (node->GetControl())
            node->GetControl()->SetVisibleForUIEditor(value.toBool());
        return true;
    }
    return false;
}

Qt::ItemFlags PackageModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    
    Qt::ItemFlags flags = QAbstractItemModel::flags(index) | Qt::ItemIsUserCheckable;
    
    const PackageBaseNode *node = static_cast<PackageBaseNode*>(index.internalPointer());
    if ((node->GetFlags() & PackageBaseNode::FLAGS_CONTROL) != 0)
        flags |= Qt::ItemIsDragEnabled;
    if ((node->GetFlags() & PackageBaseNode::FLAG_READ_ONLY) == 0)
        flags |= Qt::ItemIsDropEnabled;
    
    return flags;
}

Qt::DropActions PackageModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList PackageModel::mimeTypes() const
{
    QStringList types;
    types << "application/packageModel";
    types << "text/plain";
    return types;
}

QMimeData *PackageModel::mimeData(const QModelIndexList &indexes) const
{
    PackageMimeData *mimeData = new PackageMimeData();
    
    Vector<ControlNode*> nodes;
    foreach (QModelIndex index, indexes)
    {
        if (index.isValid())
        {
            mimeData->SetIndex(index);
            
            PackageBaseNode *node = static_cast<PackageBaseNode*>(index.internalPointer());
            ControlNode *controlNode = dynamic_cast<ControlNode*>(node);
            if (controlNode && controlNode->GetCreationType() != ControlNode::CREATED_FROM_PROTOTYPE_CHILD)
                nodes.push_back(controlNode);
        }
    }
    
    YamlPackageSerializer serializer;
    document->GetPackage()->Serialize(&serializer, nodes);
    String str = serializer.WriteToString();
    mimeData->setText(QString::fromStdString(str));

    return mimeData;
}

bool PackageModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
        return true;
    
    int rowIndex;
    if (row != -1)
        rowIndex = row;
    else if (parent.isValid())
        rowIndex = rowCount(parent);
    else
        rowIndex = rowCount(QModelIndex());

    if (data->hasFormat("application/packageModel"))
    {
        const PackageMimeData *controlMimeData = dynamic_cast<const PackageMimeData*>(data);
        if (!controlMimeData)
            return false;
        
        if (column > 0)
            return false;

        QModelIndex srcIndex = controlMimeData->GetIndex();
        QModelIndex srcParent = srcIndex.parent();
        QModelIndex dstParent = parent;
        int dstRow = rowIndex;
        
        if (action == Qt::CopyAction)
        {
            QUndoCommand *changeCommand = new CopyItemModelCommand(this, srcIndex, dstRow, dstParent);
            document->UndoStack()->push(changeCommand);
            return true;
        }
        else if (action == Qt::MoveAction)
        {
            int srcRow = srcIndex.row();
            
            if (srcIndex == dstParent)
            {
                return false;
            }
            
            if (srcParent == dstParent && (dstRow == srcRow || dstRow == srcRow + 1))
            {
                return false;
            }

            ControlNode *sourceNode = dynamic_cast<ControlNode*>(static_cast<PackageBaseNode*>(index(srcRow, 0, srcParent).internalPointer()));
            if (!sourceNode || (sourceNode->GetFlags() & PackageBaseNode::FLAG_READ_ONLY) != 0)
            {
                return false;
            }

            document->UndoStack()->push(new MoveItemModelCommand(this, srcIndex, dstRow, dstParent));
            return true;
        }
        else if (action == Qt::LinkAction)
        {
            DVASSERT(false);
            return true;
        }
    }
    else if (data->hasFormat("text/plain") && data->hasText())
    {
        QModelIndex dstParent = parent;
        int dstRow = rowIndex;
        QUndoCommand *changeCommand = new InsertControlNodeCommand(this, data->text(), dstRow, dstParent);
        document->UndoStack()->push(changeCommand);
        return true;
    }

    return false;
}

void PackageModel::InsertItem(const QString &name, int dstRow, const QModelIndex &dstParent)
{
    String controlName = QStringToString(name);
    size_t slashIndex = controlName.find("/");
    ControlNode *node = NULL;
    
    if (slashIndex != String::npos)
    {
        String packName = controlName.substr(0, slashIndex);
        controlName = controlName.substr(slashIndex + 1, controlName.size() - slashIndex - 1);
        PackageControlsNode *packageControls = root->GetImportedPackagesNode()->FindPackageControlsNodeByName(packName);
        if (packageControls)
        {
            ControlNode *prototypeControl = packageControls->FindControlNodeByName(controlName);
            if (prototypeControl)
            {
                node = ControlNode::CreateFromPrototype(prototypeControl, packageControls->GetPackageRef());
            }
        }
    }
    else
    {
        UIControl *control = ObjectFactory::Instance()->New<UIControl>(controlName);
        if (control)
        {
            node = ControlNode::CreateFromControl(control);
            SafeRelease(control);
        }
        else
        {
            ControlNode *prototypeControl = root->GetPackageControlsNode()->FindControlNodeByName(controlName);
            if (prototypeControl)
            {
                node = ControlNode::CreateFromPrototype(prototypeControl, root->GetPackageControlsNode()->GetPackageRef());
            }
        }
    }
    
    if (node)
    {
        InsertItem(node, dstRow, dstParent);
        SafeRelease(node);
    }
    else
    {
        DVASSERT(false);
    }
}

void PackageModel::InsertItem(ControlNode *node, int dstRow, const QModelIndex &dstParent)
{
    beginInsertRows(dstParent, dstRow, dstRow);
    InsertNode(node, dstParent, dstRow);
    endInsertRows();
}

void PackageModel::InsertImportedPackage(PackageControlsNode *node, int dstRow, const QModelIndex &dstParent)
{
    beginInsertRows(dstParent, dstRow, dstRow);
    
    if (dstParent.isValid())
    {
        PackageBaseNode *parentNode = static_cast<PackageBaseNode*>(dstParent.internalPointer());
        ImportedPackagesNode *importedPackagesNode = dynamic_cast<ImportedPackagesNode*>(parentNode);
        
        if (importedPackagesNode)
        {
            const QModelIndex &insertBelowIndex = index(dstRow, 0, dstParent);
            PackageControlsNode *belowThis = static_cast<PackageControlsNode*>(insertBelowIndex.internalPointer());
            if (belowThis)
                importedPackagesNode->InsertBelow(node, belowThis);
            else
                importedPackagesNode->Add(node);
        }
    }
    else
    {
        DVASSERT(false);
    }
    
    endInsertRows();
}

void PackageModel::MoveItem(const QModelIndex &srcItem, int dstRow, const QModelIndex &dstParent)
{
    ControlNode *sourceNode(dynamic_cast<ControlNode*>(static_cast<PackageBaseNode*>(srcItem.internalPointer())));
    if (sourceNode)
    {
        beginMoveRows(srcItem.parent(), srcItem.row(), srcItem.row(), dstParent, dstRow);
        {
            SafeRetain(sourceNode);
            RemoveNode(sourceNode);
            InsertNode(sourceNode, dstParent, dstRow);
            SafeRelease(sourceNode);
        }
        endMoveRows();
    }
    else
    {
        DVASSERT(false);
    }
}

void PackageModel::CopyItem(const QModelIndex &srcItem, int dstRow, const QModelIndex &dstParent)
{
    ControlNode *sourceNode = dynamic_cast<ControlNode*>(static_cast<PackageBaseNode*>(srcItem.internalPointer()));
    if (sourceNode)
    {
        beginInsertRows(dstParent, dstRow, dstRow);
        {
            ControlNode* node = sourceNode->Clone();
            InsertNode(node, dstParent, dstRow);
            SafeRelease(node);
        }
        endInsertRows();
    }
    else
    {
        DVASSERT(false);
    }
}

void PackageModel::RemoveItem(const QModelIndex &srcItem)
{
    beginRemoveRows(srcItem.parent(), srcItem.row(), srcItem.row());
    RemoveNode(static_cast<PackageBaseNode*>(srcItem.internalPointer()));
    endRemoveRows();
}

void PackageModel::InsertNode(ControlNode *node, const QModelIndex &parent, int dstRow)
{
    if (parent.isValid())
    {
        PackageBaseNode *parentNode = static_cast<PackageBaseNode*>(parent.internalPointer());
        if ((parentNode->GetFlags() & PackageBaseNode::FLAG_READ_ONLY) == 0)
        {
            const QModelIndex &insertBelowIndex = index(dstRow, 0, parent);
            if ((parentNode->GetFlags() & PackageBaseNode::FLAGS_CONTROL) != 0)
            {
                ControlNode *parentControl = dynamic_cast<ControlNode*>(parentNode);
                if (parentControl)
                {
                    ControlNode *belowThis = static_cast<ControlNode*>(insertBelowIndex.internalPointer());
                    if (belowThis)
                        parentControl->InsertBelow(node, belowThis);
                    else
                        parentControl->Add(node);
                }
                else
                {
                    DVASSERT(false);
                }
            }
            else
            {
                PackageControlsNode *parentControls = dynamic_cast<PackageControlsNode*>(parentNode);
                if (parentControls)
                {
                    ControlNode *belowThis = static_cast<ControlNode*>(insertBelowIndex.internalPointer());
                    if (belowThis)
                        parentControls->InsertBelow(node, belowThis);
                    else
                        parentControls->Add(node);
                }
                else
                {
                    DVASSERT(false);
                }
            }
        }
    }
    else
    {
        DVASSERT(false);
    }
}

void PackageModel::RemoveNode(PackageBaseNode *node)
{
    ControlNode *controlNode = dynamic_cast<ControlNode*>(node);
    if (controlNode)
    {
        RemoveControlNode(controlNode);
    }
    else
    {
        PackageControlsNode *packageControlsNode = dynamic_cast<PackageControlsNode*>(node);
        RemovePackageControlsNode(packageControlsNode);
    }
}

void PackageModel::RemoveControlNode(ControlNode *node)
{
    PackageBaseNode *parentNode = node->GetParent();
    if ((parentNode->GetFlags() & PackageBaseNode::FLAG_READ_ONLY) == 0)
    {
        PackageControlsNode *controls = dynamic_cast<PackageControlsNode*>(parentNode);
        if (controls)
        {
            controls->Remove(node);
        }
        else
        {
            ControlNode *parentControl = dynamic_cast<ControlNode*>(parentNode);
            if (parentControl)
            {
                parentControl->Remove(node);
            }
            else
            {
                DVASSERT(false);
            }
        }
    }
    else
    {
        DVASSERT(false);
    }
}

void PackageModel::RemovePackageControlsNode(PackageControlsNode *node)
{
    PackageBaseNode *parentNode = node->GetParent();
    ImportedPackagesNode *importedPackagesNode = dynamic_cast<ImportedPackagesNode*>(parentNode);
    if (importedPackagesNode)
    {
        importedPackagesNode->Remove(node);
    }
    else
    {
        DVASSERT(false);
    }
}