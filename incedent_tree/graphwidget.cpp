/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "graphwidget.h"
#include "edge.h"
#include "node.h"
#include "bintree.h"
#include <math.h>

#include <QKeyEvent>

//! [0]
static const double Pi = 3.14159265358979323846264338327950288419717;
int i=0;
int Xd=1500;

int GraphWidget::Build(QWidget* parent, INCTR *tree, qreal x, qreal y, QGraphicsScene *scene, bool* gr, int size, int *gampath)
{
    Q_UNUSED(parent);
    Q_UNUSED(x);
    Q_UNUSED(y);

    int R=100*log(size);
    qreal delta1=0, delta2=-R;
    for(int j=0; j<size; j++)
    {
        tree[j].node = new Node(this, gr);
        //if(!scene)
        scene->addItem(tree[j].node);
        tree[j].node->setPos(delta1, delta2);
        while(i<tree[j].name.size())
        {
            tree[j].node->text_orig[i] = tree[j].name[i];//fix_needed
            tree[j].node->text[i] = tree[j].name[i];
            i++;
        }
        i=0;
        delta1=R*cos((j-1)*360*Pi/(size*180));
        delta2=R*sin((j-1)*360*Pi/(size*180));
    }
    for(int j=0; j<size; j++)
    {
        if(tree[j].next)
        {
            INCTR* temp=tree[j].next;
            for(int k=0; k<size; k++)
            {
                if(temp->name==tree[k].name)
                {
                    temp->node=tree[k].node;
                    if(tree[j].node->text_orig.size()<temp->node->text_orig)
                        temp->node->text_orig=tree[j].node->text_orig;
                }
            }
            while(temp->next)
            {
                temp=temp->next;
                for(int k=0; k<size; k++)
                {
                    if(temp->name==tree[k].name)
                    {
                        temp->node=tree[k].node;
                        if(tree[j].node->text_orig.size()<temp->node->text_orig)
                            temp->node->text_orig=tree[j].node->text_orig;
                    }
                }
            }
        }
    }
    for(int j=0; j<size; j++)
    {
        if(tree[j].next)
        {
            INCTR* temp=tree[j].next;
            if(gampath!=nullptr)
            {
            for(int i=0; i<size-2; i++)
                {
                    if((tree[j].turn==gampath[i] && temp->turn==gampath[i+1]) || (temp->turn==gampath[i] && tree[j].turn==gampath[i+1])
                            || (tree[j].turn==gampath[0] && temp->turn==gampath[size-1]) || (tree[j].turn==gampath[size-1] && temp->turn==gampath[0]))
                        scene->addItem((new Edge(temp->node, tree[j].node, true, temp->name.size(), tree[j].name.size(), true)));
                    else
                        scene->addItem((new Edge(temp->node, tree[j].node, true, temp->name.size(), tree[j].name.size(), false)));
                    while(temp->next)
                    {
                        temp=temp->next;
                        if((tree[j].turn==gampath[i] && temp->turn==gampath[i+1]) || (temp->turn==gampath[i] && tree[j].turn==gampath[i+1])
                                || (tree[j].turn==gampath[0] && temp->turn==gampath[size-1]) || (tree[j].turn==gampath[size-1] && temp->turn==gampath[0]))
                            scene->addItem((new Edge(temp->node, tree[j].node, true, temp->name.size(), tree[j].name.size(), true)));
                        else
                            scene->addItem((new Edge(temp->node, tree[j].node, true, temp->name.size(), tree[j].name.size(), false)));
                    }
                }
            }
            else
            {
                scene->addItem((new Edge(temp->node, tree[j].node, true, temp->name.size(), tree[j].name.size(), false)));
                while(temp->next)
                {
                    temp=temp->next;
                    scene->addItem((new Edge(temp->node, tree[j].node, true, temp->name.size(), tree[j].name.size(), false)));
                }
            }
        }
    }
//    //стрелки
//    if ((tree->prev)){
//        scene->addItem(new Edge(tree->node, tree->prev->node, true));
//    }
//    //стрелки
//    qreal delta;
//    int level = (y+200)/50;
//    delta = 0.9*Xd/pow(2, level+2);
//    if(tree->next)
//    {
//        Build(parent, tree->next, x+delta, y+50, scene, gr);
//        //tree=tree->next;
//    }

//    if (LeftBT(tree)!=nullptr)
//        Build(parent, LeftBT(tree), x+delta, y+50, scene, gr);
//    if (RightBT(tree)!=nullptr)
//        Build(parent, RightBT(tree), x-delta, y+50, scene, gr);
//    if ((LeftBT(tree)!=nullptr) && (RightBT(tree)!=nullptr)){
//        scene->addItem(new Edge(LeftBT(tree)->node, RightBT(tree)->node, false));
//        if ((RightBT(LeftBT(tree))!=nullptr) && ((LeftBT(RightBT(tree)))!=nullptr)){
//            scene->addItem(new Edge(RightBT(LeftBT(tree))->node, LeftBT(RightBT(tree))->node, false));
//          if ((RightBT(LeftBT(LeftBT(tree)))!=nullptr) && ((LeftBT(RightBT(RightBT(tree))))!=nullptr))
//             scene->addItem(new Edge(RightBT(RightBT(LeftBT(tree)))->node, LeftBT(LeftBT(RightBT(tree)))->node, false));
//        }
//    }
    return 0;
}

GraphWidget::GraphWidget(QWidget *parent, INCTR *tree, bool* gr, int size, int* gampath)
    : QGraphicsView(parent), timerId(0)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-(300*log(size)/2), -(300*log(size)/2), (300*log(size)), (300*log(size)));
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.98), qreal(0.98));
    setMinimumSize(750, 550);
    setWindowTitle(tr("Elastic Nodes"));
//! [0]

//! [1]
    Build(this, &tree[0], 0, 0, scene, gr, size, gampath);
}
//! [1]

//! [2]
void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 10);
}

void GraphWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    QList<Node *> nodes;
    foreach (QGraphicsItem *item, scene()->items()) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
    }

    foreach (Node *node, nodes)
        node->calculateForces();

    bool itemsMoved = false;
    foreach (Node *node, nodes) {
        if (node->advance())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}
//! [4]


//! [6]
void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    /*QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::green);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush); */
    painter->drawRect(sceneRect);

    // Text
    QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
                    sceneRect.width() - 4, sceneRect.height() - 4);
    QString message("Graph output");

    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->setPen(Qt::lightGray);
    painter->drawText(textRect.translated(2, 2), message);
    painter->setPen(Qt::black);
    painter->drawText(textRect, message);
}
//! [6]



