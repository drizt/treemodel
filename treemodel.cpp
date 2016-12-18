/*
 * TreeItem and TreeModel classes
 *
 * Copyright 2015 Ivan Romanov <drizt@land.ru>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 */

#include "treemodel.h"

#include <QDebug>

TreeItem::TreeItem(const QStringList &values, AbstractTreeItem *parent)
    : AbstractTreeItem(parent)
    , _values(values)
{
}

TreeItem::~TreeItem()
{
}

void TreeItem::setValue(int column, const QString &name)
{
    while (column >= _values.size())
        _values << "";

    _values[column] = name;
}

QString TreeItem::value(int column) const
{
    return column < _values.size() ? _values.at(column) : "";
}

TreeItem *TreeItem::clone() const
{
    TreeItem *newItem = new TreeItem;
    newItem->_values = _values;

    foreach (AbstractTreeItem *child, children()) {
        newItem->appendChild(child->clone());
    }
    return newItem;
}

QString TreeItem::toString() const
{
    return _values.join(" ");
}

TreeModel::TreeModel(QObject *parent)
    : AbstractTreeModel(new TreeItem, parent)
{
}

TreeModel::~TreeModel()
{
}

void TreeModel::add(const QStringList &values, const QModelIndex &index)
{
    beginInsertRows(index, rowCount(index), rowCount(index));
    TreeItem *item;
    if (index.isValid())
        item = static_cast<TreeItem*>(index.internalPointer());
    else
        item = static_cast<TreeItem*>(root());

    new TreeItem(values, item);
    endInsertRows();
}

void TreeModel::remove(const QModelIndex &index)
{
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    if (item == root() || !item)
        return;

    beginRemoveRows(index.parent(), item->row(), item->row());
    delete item;
    endRemoveRows();
}

void TreeModel::up(const QModelIndex &index)
{
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    if (!item || root() == item)
        return;

    if (index.row() == 0)
        return;

    beginMoveRows(index.parent(), index.row(), index.row(), index.parent(), index.row() - 1);
    item->setRow(index.row() - 1);
    endMoveRows();
}

void TreeModel::down(const QModelIndex &index)
{
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    if (!item || root() == item)
        return;

    if (index.row() + 1 == rowCount(index.parent()))
        return;

    beginMoveRows(index.parent(), index.row(), index.row(), index.parent(), index.row() + 2);
    item->setRow(index.row() + 1);
    endMoveRows();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 3;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        TreeItem *node = static_cast<TreeItem*>(index.internalPointer());
        return node->value(index.column());
    }

    return QVariant();
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    if (role != Qt::EditRole && role != Qt::CheckStateRole && role < Qt::UserRole)
        return false;

    if (role == Qt::EditRole) {
        TreeItem *node = static_cast<TreeItem*>(index.internalPointer());
        node->setValue(index.column(), value.toString());
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Orientation::Horizontal && role == Qt::DisplayRole) {
        return QString("Column %1").arg(section);
    }
    else {
        return AbstractTreeModel::headerData(section, orientation, role);
    }
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return AbstractTreeModel::flags(index);

    Qt::ItemFlags f = Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    return f;
}
