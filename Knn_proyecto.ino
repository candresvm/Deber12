/*
 *   UNIVERSIDAD TECNICA DEL NORTE 
 *          FICA-CITEL
 *       SISTEMAS EMBEBIDOS
 *       CHRISTIAN VILLEGAS
 *Knn medición de datos en tiempo real
 */
#include "datos.h"
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <MsTimer2.h>
// datos de prueba

//cambio de variable
int respuesta;
int knn (int k, int etiquetas, int tam_col, int col_fil); // definir la func
int cont=0;

int sensor_mq135 = 0;
int sensor_mq7 = 1;

int on=0; 
int tiempo=60; 
int cont1=0; 
int j=0;
int k=0;

void setup() {
Serial.begin(9600);
MsTimer2::set(1000,reloj);
set_sleep_mode(SLEEP_MODE_STANDBY);
sleep_enable();
Serial.println("INICIO DEL SISTEMA");
Serial.println("#  MQ135  MQ7     Tipo de Aire");

}

void loop() {
  sensor_mq135 = analogRead(0);
  sensor_mq7 = analogRead(1);
  delay(1000);
  respuesta=knn(3,2,3,160);
  
  switch(respuesta){
    case 1:
      cont++;
      for(;k<1;k++){
        Serial.println(String(cont)+String("   ")+String(sensor_mq135)+String("   ")+String(sensor_mq7)+String("    ")+String("Aire Contaminado"));
        delay(250);
        MsTimer2::start();
        tiempo=0;
        on=0;
      }
    break;
    case 2:
      cont++;
      for(;k<1;k++){
        Serial.println(String(cont)+String("   ")+String(sensor_mq135)+String("   ")+String(sensor_mq7)+String("    ")+String("Aire Limpio"));
        delay(250);
        MsTimer2::start();
        tiempo=0;
        on=0;
      }
    break;
    }
}

int knn (int k, int etiquetas, int tam_col, int tam_fil){
int col;
int fil=0;
int i=0;
int j;
float aux;
float aux_etiqueta;
float potencia;
float raiz;
int label;
String salida="";

float datos_prueba [3]={sensor_mq135,sensor_mq7,2.0};
/*  
 *   matriz de k-vecinos
 *   |1   |2   |3  | -> # vecinos mas cercanos
 *   | 1  |   1| 2 | -> etiqueta correspondiente
 *   |0.12|0.18|025|-> valor de las k distancias menores
 */
 float matriz_k [3][k];
 for(;i<k;i++){
  matriz_k[0][i]=i+1.00;  //  |   1 |   2   |   3   |
  matriz_k[1][i]=0; //     |  0  |   0   |   0   |
  matriz_k[2][i]=2500.0+i;// |2500 | 25001 | 25002 | 
  }
  i=0;
 /*
  *  matriz de etiquetas
  *  |1  |2  |3  | -> # de etiquetas
  *  |2  |1  |0  | -> el conteo de cada una de ellas con respecto a matriz k_vecinos   
  */
  float matriz_eti[2][etiquetas];
  for(;i<etiquetas;i++){
      matriz_eti[0][i]=i+1.0; // |  1 |  2 |  3  |
      matriz_eti[1][i]=0.0;   // |  0 |  0 |  0  |
    }

   for(;fil<tam_fil;fil++){
      for(col=0;col<tam_col-1;col++){ // menos la ultima columna que corresponde a la etiqueta
          potencia=potencia+pow(matriz[fil][col]-datos_prueba[col],2);
        }
        raiz=sqrt(potencia);
        potencia=0;
        // comparacion de la nueva distancia con solo la mayor distancia almacenada en matriz_k
        if(raiz<matriz_k[2][k-1]){
          matriz_k[2][k-1]=raiz;
          matriz_k[1][k-1]=matriz[fil][tam_col-1]; 
          // ordenar matriz por metodo de burbuja
          for(i=0;i<k;i++){
             for(j=i+1;j<k;j++){
               if(matriz_k[2][i]>matriz_k[2][j]){
                 // distancia
                  aux=matriz_k[2][i];
                  matriz_k[2][i]=matriz_k[2][j];
                  matriz_k[2][j]=aux;
                  //etiqueta
                  aux_etiqueta=matriz_k[1][i];
                  matriz_k[1][i]=matriz_k[1][j];
                  matriz_k[1][j]=aux_etiqueta;
                }//end if
              } //end for j
            } // end for i
        } // end if
    } // end lectura matriz.h
    //buscar etiqueta con mayor repitencia
    for(i=0;i<etiquetas;i++){
      for(j=0;j<k;j++){
          if(matriz_eti[0][i]==matriz_k[1][j]){
            matriz_eti[1][i]++;
          }
        }
      }

    for(i=0;i<etiquetas-1;i++){
       if(matriz_eti[1][i]<matriz_eti[1][i+1]) // buscar el valor menor entre conteo de etiquetas
        label=(int)matriz_eti[0][i+1];    //asgina a label la etiqueta de mayor valor
        else 
        label=(int)matriz_eti[0][i];      //asgina a label la etiqueta de mayor valor
      }  
 return label;
};

void reloj() {
  cont1++;
  power_adc_disable();
  //Serial.println(("Lectura en:")+String(60-cont));
  if(cont1==tiempo){
    power_adc_enable();
    cont1=0;
  }
  if(cont1>=60){
    wdt_enable(WDTO_8S);
    sleep_mode();
  }
}
