#ifndef PID_H_INCLUDED
#define PID_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/* #define MOVEP 15 //P Motorspannung pro Schritt - Genauigkeit. Erzeugt Zittern und schießt übers Ziel raus
#define MOVEI 0.6 //I Erhöht Motorspannung bei kleinen Fehlern - lässt die letzten bisschen Fehler verschwinden
#define MOVED 12 //D Rechtzeitig bremsen - dämpft Zittern und rauschießen
#define MOVEMAX 255
#define MOVEIMAX 100
*/
//MADA
int joint = 3; //diese Zahl anpassen je nachdem welches Gelenk angesteuert werden soll von dem Chip
float mada[3][7]={
  {15,15,15,15,15,15,15}, //P-Anteile
  {0.6,0.6,0.6,0.6,0.6,0.6,0.6}, //I-Anteile
  {12,12,12,12,12,12,12} //D-Anteile
};

#define GOVERNOR_FREQ 100
#define BOUNDS(var, max) if ((var)>(max)) (var)=(max); if ( (var)<-(max)) (var)=-(max);

void timer_init();

#ifdef __cplusplus
}
#endif

#endif
