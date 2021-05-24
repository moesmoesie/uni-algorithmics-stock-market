// Implementatie van klasse Beurs

#include <iostream>
#include <fstream>
#include "standaard.h"
#include "beurs.h"
using namespace std;

//*************************************************************************

// Default constructor
Beurs::Beurs ()
{
  // TODO: implementeer (zo nodig) deze constructor

}  // default constructor

//****************************************************************************

bool Beurs::leesIn (const char* invoernaam){
  ifstream file;
  file.open(invoernaam);
  if(!file.is_open()){
      return false;
  }
  file >> tw >> n >> provisie >> b0;
  if(tw<1 || tw>100 || n<1 || n>8){
    return false;
  }
  for(int i=0;i<=tw;i++){
    for(int j=0;j<n;j++){
      file >> aandelen[i][j];
    }
  }
  for(int i=0;i<tw;i++){
    file >> rente_percentages[i];
  }
  return true;
}  // leesIn

//****************************************************************************

void Beurs::drukAfInvoer ()
{
  cout << "\t";
  for(int i=1;i<=n;i++){
    cout << "Aandeel " << i << '\t';
  }
  cout << "Rente Percentage" << endl;

  for(int i=0;i<=tw;i++){
    cout << "Dag " << i  << "\t";
    for(int j=0;j<n;j++){
        cout << aandelen[i][j] << "\t \t";      
    }    
    if(i != tw){
      cout << rente_percentages[i] << endl;
    }
  }
}  // drukAfInvoer

//****************************************************************************

double get_max(double i, double j) {
    return i > j ? i : j;
}

double Beurs::bepaalMaxBedragBU(vector <vector <pair <bool,int> > > &transacties){
  int T[tw + 1];
  T[0] = b0;

  double min_bedrag = T[0];
  int previousStock = 0;
  for(int i =1;i<=tw;i++){
    int stock = -1;
    for(int j=0;j<pow(2,n);j++){
      double value = nieuweTotaleBedrag(previousStock,j,i - 1) + T[0];
      if(value > min_bedrag){
        min_bedrag = value;
        stock = j;
      }
    }
    previousStock = stock;
    T[i] = min_bedrag;
  }


  return T[tw + 1];
}  // bepaalMaxBedragBU

//****************************************************************************
double Beurs::nieuweTotaleBedrag(int huidigeAandelen, int nieuweAandelen,int dag){
  bitset<MaxN> a1 = bitset<MaxN>(huidigeAandelen);
  bitset<MaxN> a2 = bitset<MaxN>(nieuweAandelen);
  double bedrag = 0;
  for(int i=0;i<n;i++){
    if(a1[i] != a2[i]){
      if(a2[i] == 1){
        double extra = 1 + (provisie / 100);
        bedrag -= aandelen[dag][i] * extra;
      }else{
        bedrag += aandelen[dag][i] * (1 - (provisie / 100));
      }
    }
  }
  return bedrag;
}

double Beurs::bepaalMaxBedragRecHelper(int huidigeAandelen,int dag){
  if(dag == 0){
    double bedrag = b0 + nieuweTotaleBedrag(0,huidigeAandelen,dag);
    if (bedrag < 0){
      return -1;
    }
    return bedrag;
  }
  double besteBedrag = 0;
  bool geweest = false;
  for(int i=0;i<=pow(2,n)-1;i++){
    double bedrag = bepaalMaxBedragRecHelper(i, dag - 1);
    if(bedrag != -1){
      bedrag = krijgBedragPlusRente(bedrag,dag-1);
      bedrag+= nieuweTotaleBedrag(i,huidigeAandelen,dag);
      if(bedrag > besteBedrag){
        besteBedrag = bedrag;
        geweest = true;
      }
    }
  }
  if(!geweest){
    return -1;
  }
  return besteBedrag;
}
double Beurs::bepaalMaxBedragRecMemoHelper(int huidigeAandelen,int dag){  
  if(hulpTabel[dag][huidigeAandelen] < 0){
    if(dag == 0){
      double bedrag = b0 + nieuweTotaleBedrag(0,huidigeAandelen,dag);
      if (bedrag >= 0){
        hulpTabel[dag][huidigeAandelen] = bedrag;
      }
    }else{
    for(int i=0;i<=pow(2,n)-1;i++){
      double bedrag = bepaalMaxBedragRecMemoHelper(i, dag - 1);
      if(bedrag != -1){
        bedrag = krijgBedragPlusRente(bedrag,dag-1);
        bedrag+= nieuweTotaleBedrag(i,huidigeAandelen,dag);
        if(bedrag > hulpTabel[dag][huidigeAandelen]){
            hulpTabel[dag][huidigeAandelen] = bedrag;
        }
      }
    }
    }
  }
  return hulpTabel[dag][huidigeAandelen];
}

double Beurs::krijgBedragPlusRente(double bedrag, int dag){
  return bedrag * ((rente_percentages[dag]/100) + 1);
}

double Beurs::bepaalMaxBedragRec (bool memo){
  double value;
  if(memo){
    for(int i=0;i<=tw;i++){
      for(int j=0;j<=pow(2,n)-1;j++){
        hulpTabel[i][j] = -1;
      }
    }
    value = bepaalMaxBedragRecMemoHelper(0,tw);
  }else{
    value = bepaalMaxBedragRecHelper(0,tw);
  }
  return value;
}  // bepaalMaxBedragRec (memo)

//****************************************************************************

void Beurs::drukAfTransacties
            (vector <vector <pair <bool,int> > > transacties)
{
  // TODO: implementeer deze memberfunctie

}  // drukAfTransacties

