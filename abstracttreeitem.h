/*
 * Copyright (C) 2015  Ivan Romanov <drizt@land.ru>
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

#pragma once

#include <QList>
#include <QString>

class AbstractTreeItem
{
public:
    explicit AbstractTreeItem(AbstractTreeItem *parent = 0);
    virtual ~AbstractTreeItem();

    void setRow(int row);
    int row() const;

    void setParent(AbstractTreeItem *newParent);
    AbstractTreeItem *parent() const;

    void insertChild(int row, AbstractTreeItem *child);
    void appendChild(AbstractTreeItem *child);
    void removeChild(AbstractTreeItem *child);

    AbstractTreeItem *child(int row) const;
    int childCount() const;
    QList<AbstractTreeItem*> children() const;

    virtual AbstractTreeItem *clone() const = 0;
    void dump(int indent = 0) const;
    virtual QString toString() const = 0;

private:
    AbstractTreeItem *_parent;
    QList<AbstractTreeItem*> _children;
};
