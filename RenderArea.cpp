#include "RenderArea.h"
#include "Tables.h"
#include <QGraphicsPixmapItem>
RenderArea::RenderArea(QWidget* parent): QGraphicsView(parent)
{
    setMinimumSize(200, 400);
    _scene = new QGraphicsScene();
    _view = new QGraphicsView(_scene);
    setScene(_scene);
    connect(_scene, &QGraphicsScene::changed, this, &RenderArea::changeScene);
}

void RenderArea::drawConstruction(const Params& params)
{
    vecForDraw = params.type_for_each_rod;
    scaling();
    _scene->clear();
    double coordX = 0;
    double coordY = 0;
    double endX = 0;
    _items.resize(vecForDraw.size());
    for (auto i = 0; i < _items.size(); ++i)
    {     
        endX += vecForDraw[i][4];
        if (i == 0)
        {
            coordX = 0;
            coordY = vecForDraw[i][1]/2;
        }
        else
            coordX = coordX + vecForDraw[i-1][4];
        _items[i] = new QGraphicsRectItem(coordX, coordY - vecForDraw[i][1]/2,
                                         vecForDraw[i][4], vecForDraw[i][1]);
        _scene->addItem(_items[i]);

        drawDistrLoads(i, params, coordX, coordY);    // отрисовка продольных нагрузок
        drawCentredLoads(i, params, coordX, coordY);  // отрисовка сосредоточенных нагрузок
    }
    drawCentredLoads(params.centred_loads.size() - 1, params, endX, coordY);

    drawFixedNodes(params, coordY, endX);
}
void RenderArea::drawDistrLoads(const int& i, const Params& params,const double& coordX, const double& coordY)
{
    if (params.distr_loads[i] > 0)
    {
        QPixmap pixmap(":/resources/img/distrLoads+.jpg");
        drawDistr(pixmap, params, coordX, coordY, i);
    }
    if (params.distr_loads[i] < 0)
    {
        QPixmap pixmap(":/resources/img/distrLoads-.jpg");
        drawDistr(pixmap, params, coordX, coordY, i);
    }
}

void RenderArea::drawFixedNodes(const Params& params, const double& coordY, const double& endX)
{
    if (params.fixed_nodes[0] == 1)
    {
        QPixmap pixmap(":/resources/img/left.jpg");
        QGraphicsPixmapItem* item = _scene->addPixmap(pixmap);
        item->setPos(0 - pixmap.width(), coordY - pixmap.height()/2);
    }
    if (params.fixed_nodes[1] == params.centred_loads.size())
    {
        QPixmap pixmap(":/resources/img/right.jpg");
        QGraphicsPixmapItem* item = _scene->addPixmap(pixmap);
        item->setPos(endX + 1, coordY - pixmap.height()/2);
    }
}

void RenderArea::drawDistr(QPixmap& pixmap, const Params& params, const double& coordX, const double& coordY,
                                         const int& i)
{
        double a = vecForDraw[i][4];
        double coord_for_distr = coordX;
        int check_scale_for_distr = static_cast<int>(vecForDraw[i][4]/pixmap.width());
        int rest = int(a) % pixmap.width();
        if (pixmap.width() <= vecForDraw[i][4])
            pixmap = pixmap.scaled(pixmap.width(), static_cast<int>(vecForDraw[i][1]/6), Qt::IgnoreAspectRatio);
        else
            pixmap = pixmap.scaled(static_cast<int>(vecForDraw[i][4]),
                    static_cast<int>(vecForDraw[i][1]/6), Qt::IgnoreAspectRatio);
        for (auto j = 0; j <= check_scale_for_distr; ++j)
        {
            if (j == check_scale_for_distr)
            {
                pixmap = pixmap.copy(0, 0, rest, pixmap.height());
                QGraphicsPixmapItem* item = _scene->addPixmap(pixmap);
                item->setPos(coord_for_distr, coordY - pixmap.height()/2);
            }
            else
            {
                QGraphicsPixmapItem* item = _scene->addPixmap(pixmap);
                item->setPos(coord_for_distr, coordY - pixmap.height()/2);
            }
            coord_for_distr += pixmap.width();

        }
}

void RenderArea::drawCentredLoads(const int& i, const Params& params, const double& coordX, const double& coordY)
{
        if (params.centred_loads[i] > 0)
        {
            QPixmap pixmap(":/resources/img/centredLoads+.jpg");
            drawCentred(pixmap, params, coordX, coordY, i);
        }
        if (params.centred_loads[i] < 0)
        {
            QPixmap pixmap(":/resources/img/centredLoads-.jpg");
            drawCentred(pixmap, params, coordX, coordY, i);
        }
}

void RenderArea::drawCentred(QPixmap &pixmap, const Params &params, const double &coordX, const double &coordY, const int &i)
{
    int j = i;
    if (i == params.centred_loads.size() - 1)
    {
        --j;
        if (params.centred_loads[i] < 0)
        {
            pixmap = pixmap.scaled(static_cast<int>(vecForDraw[j][4]/6),
                    static_cast<int>(vecForDraw[j][1]/6), Qt::IgnoreAspectRatio);
        }
        if (params.centred_loads[i] > 0)
        {
            pixmap = pixmap.scaled(static_cast<int>(vecForDraw[j][4]/6),
                    static_cast<int>(vecForDraw[j][1]/6), Qt::IgnoreAspectRatio);
        }
    }
    else
    {
        if (params.centred_loads[i] > 0 || i == 0)
            pixmap = pixmap.scaled(static_cast<int>(vecForDraw[i][4]/6),
                    static_cast<int>(vecForDraw[i][1]/6), Qt::IgnoreAspectRatio);

        if (params.centred_loads[i] < 0 && i != 0)
            pixmap = pixmap.scaled(static_cast<int>(vecForDraw[i-1][4]/6),
                    static_cast<int>(vecForDraw[i-1][1]/6), Qt::IgnoreAspectRatio);
    }

    QGraphicsPixmapItem* item = _scene->addPixmap(pixmap);
    if (params.centred_loads[i] > 0)
        item->setPos(coordX, coordY - pixmap.height()/2);
    else if (params.centred_loads[i] < 0)
        item->setPos(coordX - pixmap.width(), coordY - pixmap.height()/2);

}
void RenderArea::changeScene()
{
    QRectF rect = _scene->itemsBoundingRect();
    _scene->setSceneRect(rect);
}

void RenderArea::scaling()
{
    double allLength = 0;
    double maxWidth = 0;
    for (auto i = 0; i < vecForDraw.size(); ++i)
    {
        allLength += vecForDraw[i][4];
        if (vecForDraw[i][1] > maxWidth)
            maxWidth = vecForDraw[i][1];
    }

    double kForL = 1600 / allLength;
    double kForW = 400 / maxWidth;

    for (auto i = 0; i < vecForDraw.size(); ++i)
    {
        vecForDraw[i][1] = vecForDraw[i][1]*kForW;
        vecForDraw[i][4] = vecForDraw[i][4]*kForL;
    }
}
