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

  for(int i=0;i<tw;i++){
    cout << "Dag " << i  << "\t";
    for(int j=0;j<n;j++){
        cout << aandelen[i][j] << "\t \t";      
    }    
    cout << rente_percentages[i] << endl;
  }
}  // drukAfInvoer

//****************************************************************************

double Beurs::winst(int aandeel, int aankoopDag){
    double inkoopPrijs = aandelen[MaxN][Maxtw];
    double tempWinst = lowest_double;
    for(int dag = aankoopDag; dag < MaxTw; dag++){

    }
}

//****************************************************************************

double Beurs::bepaalMaxBedragBU
         (vector <vector <pair <bool,int> > > &transacties)
{
  // TODO: implementeer deze memberfunctie
  for(int aandeel = 0; aandeel < MaxN; aandeel++){
      for(int dag = 0; dag < MaxTw; dag++){
          winst()
      }
  }

  return 0.0;

}  // bepaalMaxBedragBU

//****************************************************************************

double Beurs::bepaalMaxBedragRec (bool memo)
{
  // TODO: implementeer deze memberfunctie

  return 0.0;

}  // bepaalMaxBedragRec (memo)

//****************************************************************************

void Beurs::drukAfTransacties
            (vector <vector <pair <bool,int> > > transacties)
{
  // TODO: implementeer deze memberfunctie

}  // drukAfTransacties

