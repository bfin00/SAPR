#ifndef RENDERAREA_H
#define RENDERAREA_H
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QTimer>


struct Params;

class RenderArea: public QGraphicsView
{
    Q_OBJECT
public:
    RenderArea(QWidget* parent = nullptr);
public slots:
    void drawConstruction(const Params& params);
    void changeScene();
private:
    QGraphicsScene* _scene;
    QVector<QGraphicsRectItem*> _items;
    QGraphicsView* _view;
    QVector<QVector<double>> vecForDraw;
    void drawDistrLoads(const int& i, const Params& params,const double& coordX, const double& coordY);
    void drawFixedNodes(const Params& params, const double& coordY, const double& endX);

    void drawDistr(QPixmap& pixmap, const Params& params, const double& coordX, const double& coordY,
                                 const int& i);
    void drawCentredLoads(const int& i, const Params& params,const double& coordX, const double& coordY);
    void drawCentred(QPixmap& pixmap, const Params& params, const double& coordX, const double& coordY,
                     const int& i);

    void scaling();

};
#endif // RENDERAREA_H
