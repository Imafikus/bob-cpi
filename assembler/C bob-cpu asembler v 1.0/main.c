#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
int memorija[256]; //niz koji ce da predstavlja memoriju
char instrukcije[256][128]; //niz stringova koji predstavlja instrukcije
int registri[8]; //niz koji predstavlja registre
int zf,pf,of,cf; //flegovi
int gr; //vodi racuna o tome da li je doslo do greske pri kucanju koda
int loadInstructions(){  //funkcija koja ucitava niz instrukcija iz txt fajla koji korisnik unosi
    char fileName[100];
    printf("Unesite putanju do vaseg programa:\n");
    scanf("%s",&fileName);
    FILE *file=fopen(fileName,"r");
    char linija[128];
    int i=0;
    while (fgets(&linija,128,file)!=NULL) {
        strcpy(instrukcije[i],linija); //ubacuje red po red u niz instrukcija
        i++;
    }
    return i; //i predstavlja broj instrukcija
}
int toInt(int a){  //konvertuje binarni broj u int
    int sum=0;
    int step=1;
    int b=0,c=0;
    while (a>0)
    {
        b=a%10;
        a=a/10;
        if(b) sum+=step;
        step*=2;
    }
    return sum;
}
void run(int i){  //funkcija koja se bavi obradom instrukcija
    char trenutnaInstrukcija[5]; //trenutna instrukcija koja se obradjuje
    int j;
    int a,b;
    char adresa[10];
    for(j=0;j<i;j++){
        int p=0;
        sscanf(instrukcije[j]+p,"%s",trenutnaInstrukcija); //iz niza instrukcija redom ucitavamo instrukcije
        p+=strlen(trenutnaInstrukcija)+1;
        if (strcmp(trenutnaInstrukcija,"MOV")==0){
            sscanf(instrukcije[j]+p,"%s",adresa);
            p+=strlen(adresa)+1;
            a=toInt(atoi(adresa)); //pretvaramo binarnu adresu u indeks niza registara
            sscanf(instrukcije[j]+p,"%s",adresa);
            b=toInt(atoi(adresa));
            registri[a]=registri[b];
        }
        else if(strcmp(trenutnaInstrukcija,"LOAD")==0){
            sscanf(instrukcije[j]+p,"%s",adresa);
            a=toInt(atoi(adresa));
            registri[0]=memorija[a];
        }
        else if(strcmp(trenutnaInstrukcija,"STORE")==0){
            sscanf(instrukcije[j]+p,"%s",adresa);
            a=toInt(atoi(adresa));
            memorija[a]=registri[0];
        }
        else if(strcmp(trenutnaInstrukcija,"ADD")==0){
            sscanf(instrukcije[j]+p,"%s",adresa);
            p+=strlen(adresa)+1;
            a=toInt(atoi(adresa));
            sscanf(instrukcije[j]+p,"%s",adresa);
            b=toInt(atoi(adresa));
            registri[b]=registri[a]+registri[b];
        }
        else if(strcmp(trenutnaInstrukcija,"SUB")==0){
            sscanf(instrukcije[j]+p,"%s",adresa);
            p+=strlen(adresa)+1;
            a=toInt(atoi(adresa));
            sscanf(instrukcije[j]+p,"%s",adresa);
            b=toInt(atoi(adresa));
            registri[b]=registri[b]-registri[a];
        }
        else if(strcmp(trenutnaInstrukcija,"ZERO")==0){
            sscanf(instrukcije[j]+p,"%s",adresa);
            a=toInt(atoi(adresa));
            registri[a]=0;
        }
        else if(strcmp(trenutnaInstrukcija,"JMP")==0){
            sscanf(instrukcije[j]+p,"%s",adresa);
            a=toInt(atoi(adresa));
            if (zf) j=registri[a];
            zf=0;
        }
        else if(strcmp(trenutnaInstrukcija,"SHL")==0){
            sscanf(instrukcije[j]+p,"%s",adresa);
            p+=strlen(adresa)+1;
            a=toInt(atoi(adresa));
            sscanf(instrukcije[j]+p,"%s",adresa);
            b=toInt(atoi(adresa));
            if(registri[a]>=8) registri[b]=0;
            else registri[b]=registri[b]<<registri[a];

        }
        else if(strcmp(trenutnaInstrukcija,"SHR")==0){
            sscanf(instrukcije[j]+p,"%s",adresa);
            p+=strlen(adresa)+1;
            a=toInt(atoi(adresa));
            sscanf(instrukcije[j]+p,"%s",adresa);
            b=toInt(atoi(adresa));
            registri[b]=registri[b]>>registri[a];
        }
        else if(strcmp(trenutnaInstrukcija,"ROL")==0){
            sscanf(instrukcije[j]+p,"%s",adresa);
            p+=strlen(adresa)+1;
            a=toInt(atoi(adresa));
            sscanf(instrukcije[j]+p,"%s",adresa);
            b=toInt(atoi(adresa));
            int t=registri[a]%9;//po modulu 9 zato sto je rotiranje periodicno
            int z=registri[b];
            registri[b]=0;
            int c;
            int pos;
            while(pos<8){
                c=z%2;
                z=z/2;
                if (c) registri[b]+=pow(2,(pos+t)%8);
                pos++;
            }
        }
        else if(strcmp(trenutnaInstrukcija,"ROR")==0){
            sscanf(instrukcije[j]+p,"%s",adresa);
            p+=strlen(adresa)+1;
            a=toInt(atoi(adresa));
            sscanf(instrukcije[j]+p,"%s",adresa);
            b=toInt(atoi(adresa));
            int t=8-registri[a]%9;//po modulu 9 zato sto je rotiranje periodicno, oduzima se od 8 zato sto je ROR za k pozicija isto sto i ROL za 8-k pozicija
            int z=registri[b];
            registri[b]=0;
            int c;
            int pos;
            while(pos<8){
                c=z%2;
                z=z/2;
                if (c) registri[b]+=pow(2,(pos+t)%8);
                pos++;
            }
        }
        else if(strcmp(trenutnaInstrukcija,"NEG")==0){
            sscanf(instrukcije[j]+p,"%s",adresa);
            p+=strlen(adresa)+1;
            a=toInt(atoi(adresa));
            registri[a]=255-registri[a];
        }
        else if(strcmp(trenutnaInstrukcija,"AND")==0){
            sscanf(instrukcije[j]+p,"%s",adresa);
            p+=strlen(adresa)+1;
            a=toInt(atoi(adresa));
            sscanf(instrukcije[j]+p,"%s",adresa);
            b=toInt(atoi(adresa));
            registri[0]=registri[a]&registri[b];
        }
        else if(strcmp(trenutnaInstrukcija,"OR")==0){
            sscanf(instrukcije[j]+p,"%s",adresa);
            p+=strlen(adresa)+1;
            a=toInt(atoi(adresa));
            sscanf(instrukcije[j]+p,"%s",adresa);
            b=toInt(atoi(adresa));
            registri[0]=registri[a]|registri[b];
        }
        else if(strcmp(trenutnaInstrukcija,"CMP")==0){
            sscanf(instrukcije[j]+p,"%s",adresa);
            p+=strlen(adresa)+1;
            a=toInt(atoi(adresa));
            sscanf(instrukcije[j]+p,"%s",adresa);
            b=toInt(atoi(adresa));
            if((registri[a]-registri[b])==0)
                zf=1;
        }
        else if(strcmp(trenutnaInstrukcija,"END")==0){
            printf("Izvrsavanje programa je zavrseno\n");
            return;
        }
        else{
            printf("Postoji greska u kodu\n");
            gr=1;
            return;
        }
        }
    }
int main()
{
    int j=0,i=0;
    i=loadInstructions(&instrukcije);
    for(j=0;j<i;j++){
        printf("%s\n",instrukcije[j]);
    }
    run(i);
    return 0;
}
