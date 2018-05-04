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
#include "inctree.h"
#include <math.h>

#include <QKeyEvent>

//! [0]
static const double Pi = 3.14159265358979323846264338327950288419717;
int i=0, k=0;
int Xd=1500;
int* this_gampath= new int;
INCTR* this_tree= new INCTR;
int time_sec=0;
int this_finish=0;

int GraphWidget::Build(QWidget* parent, INCTR *tree, qreal x, qreal y, QGraphicsScene *scene, bool* gr, int size, int *gampath)
{
    Q_UNUSED(parent);
    Q_UNUSED(x);
    Q_UNUSED(y);

    int R=100*log(size);
    qreal delta1=R, delta2=0;
    for(int j=0; j<size; j++)
    {
        tree[j].node = new Node(this, gr);
        //if(!scene)
        scene->addItem(tree[j].node);
        tree[j].node->setPos(delta1, delta2);
        tree[j].node->text_orig=tree[j].name;//fix_needed
        tree[j].node->text=tree[j].name;
        delta1=R*cos((j+1)*(360/(size))*(Pi/180));
        delta2=R*sin((j+1)*(360/(size))*(Pi/180));
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
    if(gampath==nullptr)
    {
        for(int j=0; j<size; j++)
        {
            if(tree[j].next)
            {
                INCTR* temp=tree[j].next;
                scene->addItem((new Edge(temp->node, tree[j].node, true, temp->name.size(), tree[j].name.size(), false, temp->weight, temp->weight_cur)));
                while(temp->next)
                {
                    temp=temp->next;
                        scene->addItem((new Edge(temp->node, tree[j].node, true, temp->name.size(), tree[j].name.size(), false, temp->weight, temp->weight_cur)));
                }
            }
        }
    }
    else
    {
        int add=5;
        for(int j=0; j<size; j++)
        {
            for(int i=0; i<size-1; i++)
            {
                if(tree[j].next)
                {
                    INCTR* temp=tree[j].next;
                    {
                        if((tree[j].turn==gampath[i] && temp->turn==gampath[i+1]) /*|| (tree[j].turn==gampath[i+1] && temp->turn==gampath[i])*/
                                /*|| (tree[j].turn==gampath[0] && temp->turn==gampath[size-1])*/ || (tree[j].turn==gampath[size-1] && temp->turn==gampath[0])){
    //                        if((tree[j].turn==gampath[i] && temp->turn==gampath[i+1]) || /*(tree[j].turn==gampath[i+1] && temp->turn==gampath[i])*/
    //                                /*|| (tree[j].turn==gampath[0] && temp->turn==gampath[size-1]) ||*/ (tree[j].turn==gampath[size-1] && temp->turn==gampath[0]))
                            while(add!=0)
                            {
                            scene->addItem((new Edge(temp->node, tree[j].node, true, temp->name.size(), tree[j].name.size(), true, temp->weight, temp->weight_cur)));
                            add--;
                            }
                            add=5;
                        }
                        else
                            scene->addItem((new Edge(temp->node, tree[j].node, true, temp->name.size(), tree[j].name.size(), false, temp->weight, temp->weight_cur)));
                        while(temp->next)
                        {
                            temp=temp->next;
                            if((tree[j].turn==gampath[i] && temp->turn==gampath[i+1]) /*|| (tree[j].turn==gampath[i+1] && temp->turn==gampath[i])*/
                                    /*|| (tree[j].turn==gampath[0] && temp->turn==gampath[size-1])*/ || (tree[j].turn==gampath[size-1] && temp->turn==gampath[0])){
                                while(add!=0)
                                {
                                scene->addItem((new Edge(temp->node, tree[j].node, true, temp->name.size(), tree[j].name.size(), true, temp->weight, temp->weight_cur)));
                                add--;
                                }
                                add=5;
                            }
                            else
                                scene->addItem((new Edge(temp->node, tree[j].node, true, temp->name.size(), tree[j].name.size(), false, temp->weight, temp->weight_cur)));
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int GraphWidget::Build(QWidget* parent, INCTR *tree, qreal x, qreal y, QGraphicsScene *scene, bool* gr, int size, int mseconds, int *fullgampath, int finish)
{
    Q_UNUSED(parent);
    Q_UNUSED(x);
    Q_UNUSED(y);
    int R=100*log(size);
    qreal delta1=R, delta2=0;
    for(int j=0; j<size; j++)
    {
        tree[j].node = new Node(this, gr);
        //if(!scene)
        scene->addItem(tree[j].node);
        tree[j].node->setPos(delta1, delta2);
        tree[j].node->text_orig=tree[j].name;//fix_needed
        tree[j].node->text=tree[j].name;
        delta1=R*cos((j+1)*360*Pi/(size*180));
        delta2=R*sin((j+1)*360*Pi/(size*180));
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
                scene->addItem((new Edge(temp->node, tree[j].node, true, temp->name.size(), tree[j].name.size(), false, temp->weight, 0)));
                while(temp->next)
                {
                    temp=temp->next;
                        scene->addItem((new Edge(temp->node, tree[j].node, true, temp->name.size(), tree[j].name.size(), false, temp->weight, 0)));
                }
            }
        }
    this_gampath=fullgampath;
    this_tree=tree;
    for(int i=0; i<size; i++)
    {
        INCTR* temp=&this_tree[i];
        temp->weight_cur=0;
        while(temp->next)
        {
            temp=temp->next;
            temp->weight_cur=0;
        }
    }
    time_sec=mseconds;
    this_finish=finish;
    timer=new QTimer();
    i=0;
    connect(timer, SIGNAL(timeout()), this, SLOT(slotAlarmTimer()));
    timer->start(mseconds);
    i=0;
    return 0;
}

void GraphWidget::slotAlarmTimer()
{
    int j=0, this_weight=0;
    INCTR* temp=new INCTR;
    bool flag=false;
    if(abs(this_gampath[i])==this_finish)
    {
        foreach (QGraphicsItem *item, scene()->items())
        {
            Edge *line = qgraphicsitem_cast<Edge *>(item);
            if (line)
            {
                if(line->gamilt_clr==true)
                {
                    this->scene()->removeItem(item);
                }
            }
        }
        foreach (QGraphicsItem *item, scene()->items())
        {
            Edge *line = qgraphicsitem_cast<Edge *>(item);
            if (line)
            {
                for(j=k; j<i; j++)
                {
                    if(this_gampath[j]==this_gampath[j+2] && this_gampath[j+1]!=this_finish)
                        j+=2;
                    if(this_gampath[j+1]>0)
                    {
                        if(line->source==this_tree[abs(this_gampath[j+1])-1].node && line->dest==this_tree[abs(this_gampath[j])-1].node && line->gamilt_clr==false)
                        {
                            if(j<i)
                            {
                                this->scene()->removeItem(item);
                                temp=&this_tree[abs(this_gampath[j])-1];
                                while (temp->next)
                                {
                                    temp=temp->next;
                                    if(temp->turn==abs(this_gampath[j+1]))
                                    {
                                        temp->weight_cur+=this_gampath[i+1];
                                        this_weight=temp->weight_cur;
                                    }
                                }
                                this->scene()->addItem((new Edge(this_tree[abs(this_gampath[j+1])-1].node, this_tree[abs(this_gampath[j])-1].node, true,
                                        this_tree[abs(this_gampath[j+1])-1].name.size(), this_tree[abs(this_gampath[j])-1].name.size(), false, line->weight, this_weight)));
                            }
                        }
                    }
                    else if(this_gampath[j+1]<0)
                    {
                        if(line->source==this_tree[abs(this_gampath[j])-1].node && line->dest==this_tree[abs(this_gampath[j+1])-1].node && line->gamilt_clr==false)
                        {
                            if(j<i)
                            {
                                this->scene()->removeItem(item);
                                temp=&this_tree[abs(this_gampath[j+1])-1];
                                while (temp->next)
                                {
                                    temp=temp->next;
                                    if(temp->turn==abs(this_gampath[j]))
                                    {
                                        temp->weight_cur-=this_gampath[i+1];
                                        this_weight=temp->weight_cur;
                                    }
                                }
                                this->scene()->addItem((new Edge(this_tree[abs(this_gampath[j])-1].node, this_tree[abs(this_gampath[j+1])-1].node, true,
                                        this_tree[abs(this_gampath[j])-1].name.size(), this_tree[abs(this_gampath[j+1])-1].name.size(), false, line->weight, this_weight)));
                            }
                        }
                    }
                }
            }
        }
        i+=2;
        k=i;
    }
    else if(abs(this_gampath[i+1])!=0)
    {
        foreach (QGraphicsItem *item, scene()->items())
        {
            Edge *line = qgraphicsitem_cast<Edge *>(item);
            if (line)
            {
                if(line->source==this_tree[abs(this_gampath[i])-1].node && line->dest==this_tree[abs(this_gampath[i+1])-1].node && line->gamilt_clr==true)
                {
                    this->scene()->removeItem(item);
                    flag=true;
                }
            }
        }
        if(flag!=true)
        {
            this->scene()->addItem((new Edge(this_tree[abs(this_gampath[i+1])-1].node, this_tree[abs(this_gampath[i])-1].node, true,
                    this_tree[abs(this_gampath[i+1])-1].name.size(), this_tree[abs(this_gampath[i])-1].name.size(), true, 0/*this_tree[this_gampath[i]-1].weight*/, 0)));
            flag=false;
        }
        i++;
    }
    else
    {
        timer->stop();
        k=0;
        j=0;
        i=0;
    }
}

GraphWidget::GraphWidget(QWidget *parent, INCTR *tree, bool* gr, int size, int mseconds, int* gampath, int finish)
    : QGraphicsView(parent), timerId(0)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-((300*log(size)+150)/2), -((300*log(size)+150)/2), (300*log(size)+150), (300*log(size)+150));
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
    if(mseconds==0)
        Build(this, &tree[0], 0, 0, scene, gr, size, gampath);
    else
        Build(this, &tree[0], 0, 0, scene, gr, size, mseconds, gampath, finish);
}
//! [1]

//! [2]
void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
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



