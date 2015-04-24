#include "Classes/Qt/DeviceInfo/DeviceInfo/MemProfInfoModel.h"

MemProfInfoModel::~MemProfInfoModel()
{

}
MemProfInfoModel::MemProfInfoModel()
{

}

int MemProfInfoModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{

    if (timedData.size() == 0 || timedData.last().statData.size() == 0)
        return 0;
    DAVA::uint32 tagDepth = timedData.last().statData[0].size();

    return  tagDepth;

}

int MemProfInfoModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    if (timedData.size() == 0)
        return 0;
    DAVA::uint32 poolCount = timedData.last().statData.size();
    return poolCount;
}

QVariant MemProfInfoModel::data(const QModelIndex& index, int role/* = Qt::DisplayRole*/) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();
    if (timedData.size() == 0)
        return QVariant();
    auto & latestData = timedData.last();
    int pool = index.row();
    int tag = index.column();
    if (tag >= latestData.statData.size() || pool >= latestData.statData[tag].size())
        return QVariant();
    return latestData.statData[tag][pool].allocByApp;
}
QVariant MemProfInfoModel::headerData(int section, Qt::Orientation orientation,
    int role /*= Qt::DisplayRole*/) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (timedData.size() == 0)
        return QVariant();

   

    if (orientation == Qt::Horizontal && tagNames.size() > section)
    {
        const TagsStat & stat = timedData.last();
        auto tagName = stat.tagNames[section ];
        return tagNames[tagName];
    }
    else if (poolNames.size() > section)
        return poolNames[section];


    if (orientation == Qt::Horizontal) {
        return QString("tag_") + std::to_string(section).c_str();
    }
    else
        return QString("pool_") + std::to_string(section).c_str();
    return QVariant();
}
void MemProfInfoModel::addMoreData(const DAVA::MMStat * data)
{

    DAVA::int32 rows = rowCount(), columns = columnCount();

    beginResetModel();
    TagsStat tagsStat;
    tagsStat.statData.resize(data->tags.depth + 1);
    tagsStat.tagNames.resize(data->tags.depth + 1);
    for (size_t i = 0; i < tagsStat.statData.size(); i++)
    {
        auto tagID = data->tags.stack[i];
        tagsStat.tagNames[i] = tagID;
        tagsStat.statData[i].resize(data->allocPoolCount);
        for (size_t u = 0; u < tagsStat.statData[i].size(); u++)
        {
           
            tagsStat.statData[i][u].allocByApp = data->poolStat[data->allocPoolCount*i + u].allocByApp;
            tagsStat.statData[i][u].allocTotal = data->poolStat[data->allocPoolCount*i + u].allocTotal;
        }
    }

    timedData[data->timestamp] = tagsStat;
    endResetModel();

}
void MemProfInfoModel::setConfig(const DAVA::MMStatConfig* statConfig)
{
    if (statConfig == nullptr) return;
    tagNames.resize(statConfig->tagCount);
    for (size_t i = 0; i < tagNames.size(); i++)
    {
        tagNames[i] = QString(statConfig->names[i].name);
    }
    poolNames.resize(statConfig->allocPoolCount);
    for (size_t j = 0; j < poolNames.size(); j++)
    {
        poolNames[j] = QString(statConfig->names[j + statConfig->tagCount].name);
    }
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
    emit headerDataChanged(Qt::Vertical, 0, rowCount());
}