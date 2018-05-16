#ifndef SOILMAT_H
#define SOILMAT_H

#include<QString>
#include<QColor>

class soilLayer
{
public:
  soilLayer();
  soilLayer(QString, double, double, double, double, double, double, QColor);
   ~soilLayer();

  void setLayerName(QString name) {layerName = name;}
  void setLayerThickness(double H){layerH = H;}
  void setLayerOverburdenStress(double sigma0){layerTopStress = sigma0;}
  void setLayerStiffness(double G){layerG = G;}
  void setLayerFrictionAng(double Phi){layerPhi = Phi;}
  void setLayerCohesion(double c){layerCohesion = c;}
  void setLayerUnitWeight(double gamma){layerGamma = gamma;}
  void setLayerSatUnitWeight(double gamma){layerGammaSat = gamma;}
  void setLayerTopStress(double stress){layerTopStress = stress;}
  void setLayerDepth(double depth){layerDepth = depth;}
  void setGWTdepth(double gwt){layerGWT = gwt;}
  void setLayerGWHead(double gwt){layerGWT = gwt;}
  void setWaterUnitWeight(double gamma){waterUnitWeight = gamma;}
  void setLayerColor(QColor color){layerColor = color;}

  QString getLayerName(){ return layerName;}
  QColor getLayerColor(){return layerColor;}
  double getLayerThickness(){return layerH;}
  double getLayerStiffness(){return layerG;}
  double getLayerFrictionAng(){return layerPhi;}
  double getLayerCohesion(){return layerCohesion;}
  double getLayerUnitWeight(){return layerGamma;}
  double getLayerSatUnitWeight(){return layerGammaSat;}
  double getLayerDepth(){return layerDepth;}
  double getLayerGWTdepth(){return layerGWT;}
  double getLayerTopStress();
  double getLayerBottomStress();
  double getEffectiveStress(double depth);

private:
  QString layerName;
  double layerTopStress;
  double layerDepth;
  double layerH;
  double layerGamma;
  double layerGammaSat;
  double layerG;
  double layerPhi;
  double layerCohesion;
  double layerGWT;
  double waterUnitWeight;
  QColor layerColor;

};

#endif // SOILMAT_H
