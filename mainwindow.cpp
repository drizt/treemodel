/*
 * MainWindow class
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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "treemodel.h"

#include <QAction>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _copyedItem(nullptr)
{
    ui->setupUi(this);

    TreeModel *model = new TreeModel(this);
    ui->treeView->setModel(model);

    QAction *action = new QAction("Copy", this);
    action->setShortcut(QKeySequence::Copy);
    connect(action, SIGNAL(triggered(bool)), SLOT(copyItem()));
    ui->treeView->addAction(action);

    action = new QAction("Paste", this);
    action->setShortcut(QKeySequence::Paste);
    connect(action, SIGNAL(triggered(bool)), SLOT(pasteItem()));
    ui->treeView->addAction(action);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::add()
{
    // QString text = QInputDialog::getText(this, "Input text", "Some text");
    TreeModel *model = qobject_cast<TreeModel*>(ui->treeView->model());
    model->add(QStringList{"New row"}, ui->treeView->currentIndex());
}

void MainWindow::remove()
{
    TreeModel *model = qobject_cast<TreeModel*>(ui->treeView->model());
    model->remove(ui->treeView->currentIndex());
}

void MainWindow::up()
{
    TreeModel *model = qobject_cast<TreeModel*>(ui->treeView->model());
    model->up(ui->treeView->currentIndex());
}

void MainWindow::down()
{
    TreeModel *model = qobject_cast<TreeModel*>(ui->treeView->model());
    model->down(ui->treeView->currentIndex());
}

void MainWindow::copyItem()
{
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid())
        return;

    _copyedItem = static_cast<TreeItem*>(index.internalPointer());

}

void MainWindow::pasteItem()
{
    if (!_copyedItem)
        return;

    QModelIndex parent = ui->treeView->currentIndex();
    TreeModel *model = qobject_cast<TreeModel*>(ui->treeView->model());
    model->add(_copyedItem->values(), parent);
}
