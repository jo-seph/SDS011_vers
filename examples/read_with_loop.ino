/*
  read_with_loop.ino


  send setWork, Read and setSleep in a loop up to five times
  to be sure each of them finishes with error = 0
  and sds011 will be in sleep after reding

*/


//--SDS011  Anfang ------------------------------------------------------------------------

// Soll nur aufwachen, lesen, schlafen, Pause 145 Sekunden warten

#include <SDS011_vers.h>
SDS011_vers my_sdsv;

float p10 = 0.0,   p25 = 0.0;
int sds_error,  zuerst = 0,   Pause = 10000;              // Pause sync zu airrohr 145000
unsigned long sds_wait = 0;

void Essds()
{
  if ((millis() >= sds_wait) or (zuerst < 1))
  {
    int sds_round = 0, _error = 0;
    do {
      _error = my_sdsv.SetWork();
      Serial.print("SDS Work _error:               "); Serial.println(_error);
      sds_round++;
      Serial.print("SDS wake round:                "); Serial.println(sds_round);
      if ( _error > 0 ) {
        delay(100);
      }
    }
    while ((_error not_eq 0) and (sds_round < 5));


    sds_round = 0;
    do {
      sds_error = my_sdsv.read_q(&p25, &p10);
      Serial.print("SDS read_q sds_error:          "); Serial.println(sds_error);
      sds_round++;
      Serial.print("SDS round:                     "); Serial.println(sds_round);
      if ( sds_error > 0) {
        delay(3000);
      }
    }
    while ((sds_error not_eq 0) and (sds_round < 5));


    sds_round = 0;
    do {
      _error = my_sdsv.SetSleep();
      Serial.print("SDS sleep _error:              "); Serial.println(_error);
      sds_round++;
      Serial.print("SDS sleep round:               "); Serial.println(sds_round);
      if ( sds_error > 0) {
        delay(100);
      }
    }
    while ((_error not_eq 0) and (sds_round < 5));

    /**/



    if (sds_error == 0)
    {
      String TextzeileSDS = "2,5: " + String(p25, 0) + "   10: " + String(p10, 0) + "";
      Serial.println();
      //      Serial.print  ("SDS DeviceID: "); Serial.print(DevID1r); Serial.print(" "); Serial.println(DevID2r);
      Serial.println();
      Serial.println("SDS           " + TextzeileSDS);

    }

    zuerst = 1;
    sds_wait = millis() + Pause;        // Zeit hochsetzen

  }
  //
  else {
    // my_sdsv.sleep();
    //Serial.print(" else sleep ");
  }
}
//---ende sds--------------------------------------------------------------------------------

void setup()
{

  Serial.begin(9600);                 // für Ausgabe im seriellen Monitor


  my_sdsv.begin(D1, D2);   //  my_sds.begin(RX-ESP(TX-SDS),TX-ESP(RX-SDS));

  int _error = my_sdsv.SetQueryReportingMode();
  Serial.print(" Setup SetQueryReportingMode _error:  "); Serial.println(_error);
  _error = my_sdsv.SetContinuousMode();
  Serial.print(" Setup SetContinuousMode _error:      "); Serial.println(_error);
  _error = my_sdsv.SetSleep();
  Serial.print(" Setup SDS sleep _error:              "); Serial.println(_error);
  /**/

}

void loop()
{

  Essds();
  //delay(5000);
  yield();
}
