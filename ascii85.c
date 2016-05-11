#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

#define MAXZNAKOV 40

unsigned int pridajZnak(int znak, unsigned int N, int pocetZnakov){ //prida znak do N
    N = N | znak;                                                   //logicky sucet na 8 bitov mi ulozi znak co som nacital
    // printf("%d %d\n",znak,N);
    if (pocetZnakov%4!=0){                                          // ak som nacital 4. znak tak uz nebudem rotovat cislo
        N = N<<8;
    }
    return N;                                                       // vrati mi sucasny stav N
}

unsigned int pridajZnakDek(int znak, unsigned int N){ //prida znak do N pre dekoder
    N = N * 85 + (znak - 33);                                       //logicky sucet na 8 bitov mi ulozi znak co som nacital
    // printf("%d %d\n",znak,N);
    return N;                                                       // vrati mi sucasny stav N
}


void vypis(unsigned int N){
    int znak;
    int otocenie[5] = {0,0,0,0,0};
    for (int i = 4; i >= 0; i--){                               // jedno N delim 85 aby som dostal vysledok
        znak = N % 85 + 33;                                     // ukladam si to v opacnom poradi aby som to vedel lahko vypisat
        N = N / 85;                                             // v spravnom poradi, na to je pole otocnenie
        otocenie[i]=znak;
    }
    for (int i = 0; i < 5; i++){                                //toto uz je len vypis
        putchar(otocenie[i]);
    }
}

void vypisDek(unsigned int N){
    unsigned int znak = 255;
    int otocenie[4] = {0,0,0,0};
    for (int i = 3; i >= 0; i--){
        otocenie[i] = N & znak;
        N = N >> 8;
    }
    for (int i = 0; i < 4; i++){
        putchar(otocenie[i]);
    }
}

void zakoduj(){                                                     // telo funkcie ktora koduje, odtialto volam zvysok
    unsigned int N = 0; //cislo do ktoreho si ukladam 4 znaky co naskenujem
    int znak;           //co naskenujem
    int pocetZnakov = 0;//nech viem kolko znakov som naskenoval
    int navyseZnaky = 0;//aby som vedel zistit kolko znakov doplnit ak nie je pocet znakov delitelny 4

    rewind(stdin);
    while (pocetZnakov < MAXZNAKOV){                                //nacitavam tu znaky a zistujem ci som nenacital moc vela znakov
        znak = getchar();
        if ((znak!='\n')&&(znak != EOF)){
            pocetZnakov++;                                          //nacitany znak pridam do cisla N cez dalsiu funkciu
            N = pridajZnak(znak,N,pocetZnakov);
            if (pocetZnakov % 4 == 0){                              //ak som nacital 4 znaky tak ulozim N do pola z ktoreho ho neskor precitam
                vypis(N);
                N = 0;
            }
        }else{break;}                                               //ak nacitam enter tak ukoncim

    }

    if (pocetZnakov%4!=0){                                          //ak zistim ze nemam pocet cisel delitelny 4
        navyseZnaky=4-pocetZnakov%4;                                // tak pridam taky pocet znakov '\0' aby som to dorovnal
        for (int i=1; i<navyseZnaky+1; i++){
            N = N | '\0';
            if ((pocetZnakov+i)%4!=0){ N = N<<8; }                  // tiez si kontrolujem aby som to nahodou neposunul velakrat
        }                                                           // potom si upravene N ulozim do pola stvoric z ktoreho ho potom vyberem a vypisem
        vypis(N);
    }
    rewind(stdin);
    printf("\n");
}

void dekoduj(){
    unsigned int N = 0;
    int znak;
    int pocetZnakov = 0;

    while (pocetZnakov < MAXZNAKOV+10){                                //nacitavam tu znaky a zistujem ci som nenacital moc vela znakov
        znak = getchar();
        if (znak!='\n'){
            pocetZnakov++;                                          //nacitany znak pridam do cisla N cez dalsiu funkciu
            N = pridajZnakDek(znak,N);
            if (pocetZnakov % 5 == 0){                              //ak som nacital 5. znaky tak ulozim N do pola z ktoreho ho neskor precitam
                vypisDek(N);
                N = 0;
            }
        }else{break;}                                               //ak nacitam enter tak ukoncim
        if (znak == EOF){break;}

    }
    printf("\n");
}

int main(int argc, char * argv[]){

    if ( argc != 2 ){
        zakoduj();
    }else{
        if ( strcmp(argv[1],"-d")==0){
           dekoduj();
        }
    }
    /*
    zakoduj();
    dekoduj();
    */
    return 0;
}
