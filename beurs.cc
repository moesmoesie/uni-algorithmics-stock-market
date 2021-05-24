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

pair<int, double> Beurs::AandeelMaxWinst(int aandeel, int aankoopDag){
    double tempWinst;s
    int verkoopDag = -1;
    double winst = lowest_double;

    for(int dag = aankoopDag+1; dag < MaxTw; dag++){
        tempWinst = berekenWinst(aandeel, aankoopDag, dag);
        if(tempWinst > winst){
            winst = tempWinst;
            verkoopDag = dag;
        }
    }
    pair <int, double> maxWinst = make_pair(verkoopDag, winst);
    return maxWinst;
}

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

//****************************************************************************

double Beurs::berekenWinst(int aandeel, int aankoopDag, int verkoopDag){
    provisie_kosten = aandelen[aandeel][aankoopDag]*(provisie/100);
    double inkoopPrijs = aandelen[aandeel][aankoopDag]+provisie;
    double verkoopPrijs = aandelen[aandeel][verkoopDag]-provisie;
    int winst;

    for(int dag = aankoopDag; aankoopDag < verkoopDag; aankoopDag++){
        double rente_percentage = rente_percentages[dag];
    }

    winst = verkoopPrijs-inkoopPrijs;
    return winst;
}

//****************************************************************************

double Beurs::bepaalMaxBedragBU
         (vector <vector <pair <bool,int> > > &transacties)
{
    double bedrag = b0;
    bedrag
  // TODO: implementeer deze memberfunctie
  for(int aandeel = 0; aandeel < MaxN; aandeel++){
      for(int dag = 0; dag < MaxTw; dag++){

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

