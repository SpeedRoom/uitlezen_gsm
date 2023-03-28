// naam esp: espgsm
// wachtwoord esp: espgsm

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "FS.h"
#include "SPIFFS.h"
#include <SD.h>
#include <WiFi.h>
#include "OTAlib.h"
//scherm 1
#define TFT_SCK    18
#define TFT_MOSI   23
#define TFT_CS     15
#define TFT_DC     2
#define TFT_RESET  4
//scherm 2
#define TFT_CS2     13
#define TFT_DC2     14
#define TFT_RESET2  12
//Scherm 3
#define TFT_CS3     27
#define TFT_DC3     25
#define TFT_RESET3  26

//OTA
OTAlib ota("NETGEAR68", "excitedtuba713");




// Use hardware SPI (on ESP D4 and D8 as above)
Adafruit_ILI9341 tft1 = Adafruit_ILI9341(TFT_CS, TFT_DC,TFT_MOSI,TFT_SCK,TFT_RESET);
Adafruit_ILI9341 tft2 = Adafruit_ILI9341(TFT_CS2, TFT_DC2,TFT_MOSI,TFT_SCK,TFT_RESET2);
Adafruit_ILI9341 tft3 = Adafruit_ILI9341(TFT_CS3, TFT_DC3,TFT_MOSI,TFT_SCK,TFT_RESET3);


struct opmaak{
  String tekst;
  int TextSize;
  int TextColor;
};

opmaak tekst1[] = {
  {"Twee smokkelaars opgepakt na bootdrama in Italie waarbij 61 migranten om het leven kwamen",3,0XFFFF},
  {"27-02-23, 10:07 ",2,0xF800},
  {"Na het bootdrama waarbij tientallen dode migranten zijn aangespoeld aan de Italiaanse kust, zijn ",2,0xFFFF},
  {"twee ",2,0xFFF3},
  {"mensensmokkelaars aangehouden. Het Italiaanse Rai News schrijft dat ze zijn opgepakt voor het helpen bij en aanzetten tot illegale immigratie, maar de politie onderzoekt eveneens dood door schuld. De politie doet ook onderzoek naar de betrokkenheid van nog twee anderen.",2,0xFFFF},
  {"Bij het ongeluk zijn zeker 61 mensen overleden, onder wie dertien kinderen. Zo n tachtig mensen konden worden gered, maar er bestaat nog onduidelijkheid over hoeveel mensen aan boord waren van de boot. Getuigenissen lopen uiteen van 180 tot 250. Van de overlevenden zijn 22 mensen naar het ziekenhuis gebracht.",2,0xFFFF},
  {"De migranten vertrokken drie of vier dagen eerder uit de West-Turkse stad Izmir. De mensen aan boord waren vooral afkomstig uit Afghanistan, Irak, Iran en Syrie. De oude houten boot brak door de ruwe zee en ging ten onder. Volgens lokale media was de boot niet bestand tegen de hoge golven. De wrakstukken en de doden spoelden aan bij Steccato di Cutro in de regio Calabrie.",2,0xFFFF},
  {"De Italiaanse premier Giorgia Meloni reageerde geschokt, maar maakt zich ook kwaad over de mensensmokkelaars die zulke tragedies mogelijk maken. Buitenlandminister Matteo Piantedosi zegt dat gewetenloze criminelen veel geld verdienen door de migranten een reis naar Europa op gammele, niet zeewaardige bootjes aan te smeren.",2,0xFFFF},
  {"'Over de lijken van hun slachtoffers worden enorme winsten gemaakt die in drugs en in wapens worden geinvesteerd.'",2,0xFFFF}
};

opmaak tekst2[] = {
  {"Nieuwe drug die huid doet rotten in opmars in VS: “Verslaafden zien eruit als zombies uit 'The Walking Dead'",3,0xFFFF},
  {"16-02-23, 16:07 ",2,0xF800},
  {"Een nieuwe dodelijk drug die letterlijk de huid van gebruikers doet wegrotten, is aan een opmars bezig in de Verenigde Staten. Het gaat om een mix van de sterke pijnstiller fentanyl en xylazine, een geneesmiddel dat gebruikt wordt om paarden en koeien te verdoven. Roepnaam: tranq. Elke vijf minuten sterft iemand door een overdosis drugs in de VS.",2,0xFFFF},
  {"Fentanyl alleen is al goed voor meer dan 70.000 doden per jaar. De combinatie met xylazine zou echter een nog verslavender en dodelijker cocktail vormen. De drug kost ook maar enkele dollars voor een zakje. Gebruikers zijn vaak urenlang verdoofd. Als ze weer bijkomen, is de high van de fentanyl vervlogen, waardoor ze meteen snakken naar meer. ",2,0xFFFF},
  {"Die nieuwe dosis hebben ze nodig om afkickverschijnselen te voorkomen en die zijn stevig: migraine, dubbel zicht, misselijkheid, slapende vingers en tenen, zweten en zware angstaanvallen.",2,0xFFFF},
  {"Ground Zero",3,0xFFFF},
  {"Tranq maakte eerst opgang in Philadelphia, dat beschouwd wordt als de 'ground zero' van de Amerikaanse opioïdencrisis. Gezondheidswerkers maakten zich meteen zorgen over de snelle verspreiding en de vreselijke open wonden die de drug veroorzaakt. Die vormen een schilferige korst van dood weefsel. Het kan maanden of zelfs jaren duren eer die genezen zijn. ",2,0xFFFF},
  {"In het ergste geval leiden ze tot amputatie. Sam (28) uit Kensington - even ten noorden van Philadelphia - is verslaafd aan tranq. 'Het maakt zombies van mensen', getuigt hij bij 'Sky News'. 'Tot negen maanden geleden had ik nooit wonden. Nu zitten er gaten in mijn benen en voeten. Ze schoonmaken is erg pijnlijk.' Dat heeft ook Brooke (38) ervaren. ",2,0xFFFF},
  {"Haar moeder, zus en vrouw stierven aan een overdosis en een jaar geleden belandde ze zelf in een rolstoel nadat haar rechterbeen geamputeerd moest worden. Oorzaak was een infectie, die het gevolg was van een tranq-wonde. Die drong door tot in het bot. Het maakte niet dat ze stopte met gebruiken. Haar rechteronderarm zit momenteel helemaal in een verband. ",2,0xFFFF},
  {"Als ze het verwijdert, zijn plekken zwart weefsel te zien, witte pezen en pus. Om geen afkickverschijnselen te krijgen, spuit ze meerdere keren per dag tranq in. Bang dat een inspuiting op een nieuwe plaats een nieuwe wonde zou veroorzaken, doet ze het met tegenzin in haar etterende onderarm. 'De tranq eet letterlijk je vlees op' vertelt ze aan 'The New York Times'. ",2,0xFFFF},
  {"'Het is zelfvernietiging op zijn best.' Ook in Chicago en San Francisco dook tranq intussen op. Gebruikers zijn zich volgens gezondheidsmedewerkers vaak niet bewust van het gevaar als ze de nieuwe drug gebruiken. Niet zelden eindigt het met de dood. Dat er meer overlijdens zijn, bevestigt dokter Richard Jorgensen, de lijkschouwer van DuPage County bij Chicago, aan ABC News.",2,0xFFFF},
  {"'We hadden 1 geval in 2021. Een jaar later waren dat er ",2,0xFFFF},
  {"vijftien",2,0xFFF3},
  {". Xylazine is nu aan een opmars bezig. Overal duiken beelden op van verslaafden die eruitzien als de zombies uit 'The Walking Dead'.'",2,0xFFFF},
  {"Extra probleem",3,0xFFFF},
  {"Extra probleem is dat xylazine niet reageert op naloxon, dat in de vorm van een neusspray gebruikt wordt om de onmiddellijke gevolgen van een overdosis opioïden tegen te gaan. Dat komt doordat xylazine eigenlijk geen opioide maar een verdovend middel is.",2,0xFFFF}
};

opmaak tekst3[] = {
  {"Hoboken en Mechelen opnieuw opgeschrikt door vermoedelijk drugsgeweld",3,0xFFFF},
  {"16-02-2023, 08:11",2,0xF800},
  {"Afgelopen nacht werd brand gesticht in een frituur in het Antwerpse district Hoboken. Omwonenden van frituur Delux op de hoek van de Kioskplaats en de Elststraat hoorden woensdagnacht even voor 3.00 uur een knal waarna brand ontstond aan de voordeur van de horecazaak. De brandweer kwam ter plekke en kon het vuur snel blussen. De voordeur van het handelspand raakte zwaar beschadigd.",2,0xFFFF},
  {"De politie, het labo en ontmijningsdienst Dovo onderzochten ter plaatse hoe de brand gesticht werd. Het parket vermoedt dat een bidon met benzine voor de frituur werd geplaatst en vervolgens tot ontploffing gebracht.",2,0xFFFF},
  {"Er bestaat een groot vermoeden dat de explosie en brandstichting opnieuw gelinkt zijn aan het drugsmilieu. Een van de drie eigenaars van frituur Delux is de 42-jarige M.K., een bekende in het milieu die tot de entourage van de tot ",2,0xFFFF},
  {"acht ",2,0xFFF3},
  {"jaar cel veroordeelde drugsbaas Abdelilah 'Black' E.M. gerekend wordt.",2,0xFFFF},
  {"M.K. zou zelf al geruime tijd in het buitenland verblijven. In mei 2018 werd een frituur in Deurne, waar K. toen zaakvoerder was, doorzeefd met kogels. Een maand geleden vond ook al een aanslag plaats op de Kioskplaats. Toen werd een woning onder vuur genomen. Een week later was er een ontploffing in de hal van een appartementsgebouw.",2,0xFFFF},
  {"Brandbom in Mechelen",3,0xFFFF},
  {"In Mechelen is donderdagochtend een aanslag gepleegd met een brandbom in de Bethanienstraat. 'We werden wakker van luide knallen. We dachten eerst aan een gasontploffing, tot we de vlammen zagen', zegt een buurtbewoner aan Gazet van Antwerpen.",2,0xFFFF},
  {"Van de geviseerde woning liep de voordeur schade op, bij de buren een stuk van de gevel en een fiets. Ook een geparkeerde auto raakte beschadigd. Een camera in de buurt heeft de aanslag naar verluidt gefilmd. Daarop is een jongeman te zien die een fles naar de woning gooit, een paar stappen achteruit stapt en de ontploffing vanop een afstand filmt.",2,0xFFFF},
  {"Het parket heeft een onderzoeksrechter gevorderd om een onderzoek in te stellen naar de feiten. Er wordt nagegaan of er een verband is met het drugsmilieu. 'We nemen deze zaak zeer ernstig', klinkt het.",2,0xFFFF},
  {"Voorlopig zijn er geen aanwijzingen dat er een link zou zijn met de aanslag van afgelopen nacht in Hoboken.",2,0xFFFF}
};


void Alinea6_1(){
  tft1.fillScreen(ILI9341_BLACK);
  tft1.setCursor(0, 0);
  tft1.setTextColor(ILI9341_WHITE);  tft1.setTextSize(2);
  tft1.println();
  tft1.setTextSize(3);
  tft1.println();
  tft1.setTextSize(2);
  tft1.println();
  delay(500);
}

void setup() {

  // OTA
  ota.setHostname("espgsm");  
  ota.setPassword("espgsm");
  ota.begin();
  
  Serial.begin(9600);
  Serial.println("ILI9341 Test!"); 
 
  tft1.begin();
  tft2.begin();
  tft3.begin();
  tft3.setRotation(0);
  tft2.setRotation(0);
  tft1.setRotation(0);
}
int i = 0;
int j = 0;
int z = 0;
void loop() {
 int size_tekst1=sizeof(tekst1)/sizeof(tekst1[1]); 
    if (i==2){
      tft1.fillScreen(ILI9341_BLACK);
      tft1.setCursor(0, 0);
      tft1.setTextColor(tekst1[i].TextColor);  tft1.setTextSize(tekst1[i].TextSize);
      tft1.print(tekst1[i].tekst);
      tft1.setTextColor(tekst1[i+1].TextColor);  tft1.setTextSize(tekst1[i+1].TextSize);
      tft1.print(tekst1[i+1].tekst);
      tft1.setTextColor(tekst1[i+2].TextColor);  tft1.setTextSize(tekst1[i+2].TextSize);
      tft1.print(tekst1[i+2].tekst);
      i+=3;
    }
    else if (i==0){
      tft1.fillScreen(ILI9341_BLACK);
      tft1.setCursor(0, 0);
      tft1.setTextColor(tekst1[i].TextColor);  tft1.setTextSize(tekst1[i].TextSize);
      tft1.println(tekst1[i].tekst);
      tft1.setTextColor(tekst1[i+1].TextColor);  tft1.setTextSize(tekst1[i+1].TextSize);
      tft1.println(tekst1[i+1].tekst);
      i=2;
    }
    
    else{
      tft1.fillScreen(ILI9341_BLACK);
      tft1.setCursor(0, 0);
      tft1.setTextColor(tekst1[i].TextColor);  tft1.setTextSize(tekst1[i].TextSize);
      tft1.println(tekst1[i].tekst);
      i++;
    }
    if (i==(size_tekst1)){
      i=0;
    }
    int size_tekst2=sizeof(tekst2)/sizeof(tekst2[1]); 
    if (j==11){
      tft2.fillScreen(ILI9341_BLACK);
      tft2.setCursor(0, 0);
      tft2.setTextColor(tekst2[j].TextColor);  tft2.setTextSize(tekst2[j].TextSize);
      tft2.print(tekst2[j].tekst);
      tft2.setTextColor(tekst2[j+1].TextColor);  tft2.setTextSize(tekst2[j+1].TextSize);
      tft2.print(tekst2[j+1].tekst);
      tft2.setTextColor(tekst2[j+2].TextColor);  tft2.setTextSize(tekst2[j+2].TextSize);
      tft2.print(tekst2[j+2].tekst);
      j+=3;
    }

   
    else if ((j==0) || (j ==5)||(j==14)){
      tft2.fillScreen(ILI9341_BLACK);
      tft2.setCursor(0, 0);
      tft2.setTextColor(tekst2[j].TextColor);  tft2.setTextSize(tekst2[j].TextSize);
      tft2.println(tekst2[j].tekst);
      tft2.setTextColor(tekst2[j+1].TextColor);  tft2.setTextSize(tekst2[j+1].TextSize);
      tft2.println(tekst2[j+1].tekst);
      j+=2;
    }
    
    else{
      tft2.fillScreen(ILI9341_BLACK);
      tft2.setCursor(0, 0);
      tft2.setTextColor(tekst2[j].TextColor);  tft2.setTextSize(tekst2[j].TextSize);
      tft2.println(tekst2[j].tekst);
      j++;
    }
    if (j==(size_tekst2)){
      j=0;
    }

    int size_tekst3=sizeof(tekst3)/sizeof(tekst3[1]); 
    if (z==4){
      tft3.fillScreen(ILI9341_BLACK);
      tft3.setCursor(0, 0);
      tft3.setTextColor(tekst3[z].TextColor);  tft3.setTextSize(tekst3[z].TextSize);
      tft3.print(tekst3[z].tekst);
      tft3.setTextColor(tekst3[z+1].TextColor);  tft3.setTextSize(tekst3[z+1].TextSize);
      tft3.print(tekst3[z+1].tekst);
      tft3.setTextColor(tekst3[z+2].TextColor);  tft3.setTextSize(tekst3[z+2].TextSize);
      tft3.print(tekst3[z+2].tekst);
      z+=3;
    }

   
    else if ((z==0) || (z ==8)){
      tft3.fillScreen(ILI9341_BLACK);
      tft3.setCursor(0, 0);
      tft3.setTextColor(tekst3[z].TextColor);  tft3.setTextSize(tekst3[z].TextSize);
      tft3.println(tekst3[z].tekst);
      tft3.setTextColor(tekst3[z+1].TextColor);  tft3.setTextSize(tekst3[z+1].TextSize);
      tft3.println(tekst3[z+1].tekst);
      z+=2;
    }
    
    else{
      tft3.fillScreen(ILI9341_BLACK);
      tft3.setCursor(0, 0);
      tft3.setTextColor(tekst3[z].TextColor);  tft3.setTextSize(tekst3[z].TextSize);
      tft3.println(tekst3[z].tekst);
      z++;
    }
    if (z==(size_tekst3)){
      z=0;
    }
  
    delay(10000);
}
