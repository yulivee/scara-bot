##### Table of Contents

[Axis Definition](#axis_def)
[Pins für die Lichtschranken](#pin_ls)
[Pinout an Motorplatine](#pin_motor)
[Kabelbelegung der einzelnen Motoren](#kabel_motor)
[Anschluss an die H-Bridges](#anschluss_hb)
[Pinout Flachstecker Scara-Bot](#pin_stecker)
[Aktuelle Tastenbelegung zur Steuerung](#tasten)
[Arduino Interrupt-Pins](#interrupt)

   
<a name="axis_def" href="#"></a>
### Axis Definition

| Axis Name     | Function      | Cool  |
| ------------- |:-------------:| -----:|
| SHOULDER | oberstes Armgelenk |
|UAJ        |U Arm Yaw       |    |
|UAE        |U Arm Ellbow       |    |
|SKE	|S K Ellbow       |    |
|SKF |S K F       |    |
|SKG |S K Gripper       |    |

<a name="pin_ls" href="#"></a>
###Pins für die Lichtschranken:
| Teil   | Flachstecker | Arduino |
| ------------- |:-------------:| -----:|
 Shoulder |     25/26    |  18/19
   UAJ    |     23/24    |
   UAE	  |     27/28    |
   SKE	  |     19/20    |
   SKF    |     21/22    |
   SKG 	  |     29/30    |

<a name="pin_motor" href="#"></a>
###Pinout an Motorplatine
Pin 1 ist der am nächsten zum Motor

|  Pin |  NAME  | Farbe   | Funktion
| ---- | ------ | ------- | :--------- 
  1   | `M_VCC`  | Rot     | Motor +24V
  2   | `M_GND`  | Schwarz | Motor GND 
  3   | `L_VCC`  | Orange  | Lichtschranke +5V (über Widerstand mit LS Ausgang verbunden)
  4   | `L_OUT1` | Weiß    | Ausgang LS 1
  5   | `L_GND`  | Grau    | Lichtschranke GND
  6   | `L_OUT2` | Gelb    | Ausgang LS 2
  
<a name="kabel_motor" href="#"></a>
### Kabelbelegung der einzelnen Motoren
| Farbe    | Name     | UAJ   | SKG   | SKF   | SKE   | Shoulder | UAE   | Z     |
| -------- | :------: | :---: | :---: | :---: | :---: | :------: | :---: | :---: |
| Rot      | `M_VCC`  | 6     | 12    | 4     | 2     | 8        | 10    | -     |
| Schwarz  | `M_GND`  | 5     | 11    | 3     | 1     | 7        | 9     | -     |
| Orange   | `L_VCC`  | 14    | 14    | 14    | 14    | 14       | 14    | -     |
| Weiß     | `L_OUT1` | 24    | 30    | 22    | 20    | 26       | 28    | -     |
| Grau     | `L_GND`  | 17    | 17    | 17    | 17    | 17       | 17    | -     |
| Gelb     | `L_OUT2` | 23    | 29    | 21    | 19    | 25       | 27    | -     |
| Getested |          | X     | X     | X     | X     |          |       | -     |

<a name="anschluss_hb" href="#"></a>
### Anschluss an die H-Bridges
| Funktion | Arduino-Pin | H-Bridge links | H-Bridge rechts | Farbe |
| -------- | :---------: | :------------: | :-------------: | :---: |
| Left     | D10         | IN4            | IN2             |
| Right    | D11         | IN3            | IN1             |
| Enable   | D4          | ENB            | ENA             |
| CNT0     | D2          |                |                 | Weiß
| CNT1     | D3          |                |                 | Gelb

<a name="pin_stecker" href="#"></a>
### Pinout Flachstecker Scara-Bot
| Pin  |   Name	 |   Color	|  Arm
| ---- | ------ | ------- | :--------- 
| 01	| `M_GND_SKE`    |    Black	| AL SKE
| 02	| `M_VCC_SKE`    |    Red		| AL SKE
| 03	| `M_GND_SKF`    |    Black	| AL SKF
| 04	| `M_VCC_SKF`    |    Red		| AL SKF
| 05      | `M_GND_UAJ`    |  Black	| UAJ
| 06	| `M_VCC_UAJ`    |    Red		| UAJ
| 07	| `M_GND_SHL`    |    Black	| SHOULDER
| 08	| `M_VCC_SHL`    |    Red		| SHOULDER
| 09      | `M_GND_UAE`    |  Black	| UAE
| 10      | `M_VCC_UAE`    |  Red		| UAE
| 11      | `M_GND_SKG`    |  Black	| AL SKG
| 12      | `M_VCC_SKG`    |  Red		| AL SKG
| 13      | `L_VCC_*` |          Orange	|AL SKE               
| 14      | `L_VCC_*` |          Orange	|SHOULDER / UAJ / UAE / AL SKG (Grün) 
| 15      | `L_VCC_*` |          Orange	|SHOULDER / UAJ / UAE / AL SKG (Grün)
| 16      | `L_GND_*` |          Gray        |SHOULDER / UAJ / UAE / AL SKG (Blau) / AL SKE / AL SKF
| 17      | `L_GND_*` |          Gray	|SHOULDER / UAJ / UAE / AL SKG (Blau) / AL SKE / AL SKF
| 18      | `L_GND_*` |          Gray	|SHOULDER / UAJ / UAE / AL SKG (Blau) / AL SKE / AL SKF
| 19      | `L_OUT2_SKE`   |  Yellow	| AL SKE
| 20      | `L_OUT1_SKE`   |  White	| AL SKE		
| 21      | `L_OUT2_SKF`   |  Yellow	| AL SKF
| 22      | `L_OUT1_SKF`   |  White	| AL SKF
| 23      | `L_OUT2_UAJ`   |  Yellow	| UAJ
| 24      | `L_OUT1_UAJ`   |  White	| UAJ
| 25      | `L_OUT1_SHL`   |  White	| SHOULDER (nachgemessen)
| 26      | `L_OUT2_SHL`   |  Yellow	| SHOULDER (nachgemessen)
| 27      | `L_OUT2_UAE`   |  Yellow	| UAE
| 28      | `L_OUT1_UAE`   |  White	| UAE
| 29      | `L_OUT2_SKG`   |  Yellow	| AL SKG
| 30      | `L_OUT1_SKG`   |  White	| AL SKG
| 31 | |                 Yellow	| AUX 9
| 32 | |                 White	| AUX 10
| 33 | |                 Yellow	| AUX 7
| 34 | |                 White	| AUX 8
| 35 | |                 Yellow	| AUX 5
| 36 | |                 White	| AUX 6
| 37 | |                 Yellow	| AUX 3
| 38 | |                 White	| AUX 4
| 39 | |                 Yellow	| AUX 1
| 40 | |                 White	| AUX 2

<a name="tasten" href="#"></a>
### Aktuelle Tastenbelegung zur Steuerung
| Taste | Keycode | Auswirkung |
| :---: | :-----: | ---------- |
| O |     | tempo-5 |
| P |     | tempo+5 |
| K | 107 | clicks-10 |
| L | 108 | clicks+10 |

<a name="interrupt" href="#"></a>
###Arduino Interrupt-Pins
2, 3, 18, 19, 20, 21

