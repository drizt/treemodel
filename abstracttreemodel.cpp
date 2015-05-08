/*
 * AbstractTreeItem and AbstractTreeModel classes
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

AbstractTreeItem::AbstractTreeItem(AbstractTreeItem *parent)
    : _parent(0)
    , _children(QList<AbstractTreeItem*>())
{
    setParent(parent);
}

AbstractTreeItem::~AbstractTreeItem()
{
    qDeleteAll(_children);
    if (_parent)
        _parent->_children.removeOne(this);
}

void AbstractTreeItem::setRow(int row)
{
    Q_ASSERT(_parent);
    if (!_parent)
        return;

    _parent->_children.move(this->row(), row);
}

int AbstractTreeItem::row() const
{
    return _parent ? _parent->_children.indexOf(const_cast<AbstractTreeItem*>(this)) : 0;
}

void AbstractTreeItem::setParent(AbstractTreeItem *newParent)
{
    if (_parent)
        _parent->_children.removeOne(this);

    if (newParent)
        newParent->_children.append(this);

    _parent = newParent;
}

AbstractTreeItem *AbstractTreeItem::parent() const
{
    return _parent;
}

void AbstractTreeItem::insertChild(int row, AbstractTreeItem *child)
{
    Q_ASSERT(child);
    Q_ASSERT(!child->_parent);

    if (child->_parent)
        child->_parent->removeChild(child);

    child->_parent = this;
    _children.insert(row, child);
}

void AbstractTreeItem::appendChild(AbstractTreeItem *child)
{
    Q_ASSERT(child);
    Q_ASSERT(!child->_parent);

    if (child->_parent)
        child->_parent->removeChild(child);

    child->_parent = this;
    _children.append(child);
}

void AbstractTreeItem::removeChild(AbstractTreeItem *child)
{
    Q_ASSERT(child);
    Q_ASSERT(_children.contains(child));

    _children.removeOne(child);
}

QList<AbstractTreeItem*> AbstractTreeItem::children() const
{
    return _children;
}

void AbstractTreeItem::dump(int indent) const
{
    QString fill(indent, QChar(' '));
    qDebug() << qPrintable(fill + "{");
    qDebug() << qPrintable(fill + " " + toString());
    qDebug() << qPrintable(fill + " " + "this") << this;
    qDebug() << qPrintable(fill + " " + "parent") << _parent;
    qDebug() << qPrintable(fill + " " + "children") << _children;

    foreach (AbstractTreeItem *item, _children) {
        item->dump(indent + 1);
    }
    qDebug() << qPrintable(fill + "}");
}

AbstractTreeModel::AbstractTreeModel(AbstractTreeItem *root, QObject *parent)
    : QAbstractItemModel(parent)
    , _root(root)
{
}

AbstractTreeModel::~AbstractTreeModel()
{
    delete _root;
}

QModelIndex AbstractTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    AbstractTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = _root;
    else
        parentItem = static_cast<AbstractTreeItem*>(parent.internalPointer());

    AbstractTreeItem *childItem = 0;
    if (row >= 0 && row < parentItem->children().size())
        childItem = parentItem->children().at(row);

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();

}

QModelIndex	AbstractTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    AbstractTreeItem *childItem = static_cast<AbstractTreeItem*>(index.internalPointer());
    AbstractTreeItem *parentItem = childItem->parent();

    if (parentItem == _root)
        return QModelIndex();

    int row = 0;
    if (parentItem->parent())
        row = parentItem->row();

    return createIndex(row, 0, parentItem);
}

int	AbstractTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    AbstractTreeItem *parentItem;
    if (!parent.isValid())
        parentItem = _root;
    else
        parentItem = static_cast<AbstractTreeItem*>(parent.internalPointer());

    return parentItem->children().size();
}

AbstractTreeItem *AbstractTreeModel::root() const
{
    return _root;
}
