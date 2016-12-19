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

#pragma once

#include "abstracttreeitem.h"
#include "abstracttreemodel.h"

#include <QStringList>

class TreeItem : public AbstractTreeItem
{
public:
    explicit TreeItem(const QStringList &values = QStringList(), AbstractTreeItem *parent = 0);
    ~TreeItem();

    void setValue(int column, const QString &name);
    QString value(int column) const;

    void setValues(const QStringList &values);
    QStringList values() const;

    TreeItem *clone() const;
    QString toString() const;

private:
    QStringList _values;
};

class TreeModel : public AbstractTreeModel
{
    Q_OBJECT

public:
    explicit TreeModel(QObject *parent = 0);
    ~TreeModel();

    void add(const QStringList &values, const QModelIndex &index);
    void remove(const QModelIndex &index);
    void up(const QModelIndex &index);
    void down(const QModelIndex &index);

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const;
};
