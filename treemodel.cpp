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

TreeItem::TreeItem(const QString &name, AbstractTreeItem *parent)
    : AbstractTreeItem(parent)
    , _name(name)
{
}

TreeItem::~TreeItem()
{
}

QString TreeItem::name() const
{
    return _name;
}

TreeItem *TreeItem::clone() const
{
    TreeItem *newItem = new TreeItem;
    newItem->_name = _name;

    foreach (AbstractTreeItem *child, children()) {
        newItem->appendChild(child->clone());
    }
    return newItem;
}

QString TreeItem::toString() const
{
    return "name " + _name;
}

TreeModel::TreeModel(QObject *parent)
    : AbstractTreeModel(new TreeItem, parent)
{
    new TreeItem("test1", root());
    TreeItem *node = new TreeItem("test2", root());
    node = new TreeItem("test3", node);
    new TreeItem("test4", node);
    root()->dump();
}

TreeModel::~TreeModel()
{
}

void TreeModel::add(const QString &name, const QModelIndex &index)
{
    beginInsertRows(index, rowCount(index), rowCount(index));
    TreeItem *item;
    if (index.isValid())
        item = static_cast<TreeItem*>(index.internalPointer());
    else
        item = static_cast<TreeItem*>(root());

    new TreeItem(name, item);
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
    return 1;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        TreeItem *node = static_cast<TreeItem*>(index.internalPointer());
        if (node) {
            return node->name();
        }
    }

    return QVariant();
}
