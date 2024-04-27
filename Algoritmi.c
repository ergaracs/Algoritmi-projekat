#include<stdio.h>
#include<stdlib.h>

struct Grana{
    int pocetak;
    int kraj;
    int tezina;
};
struct Graf{
    int broj_cvorova;
    int **matrica;
    int *posecen;
};
struct Graf *inicijalizujGraf(int broj_cvorova) {
    struct Graf *graf=(struct Graf*)malloc(sizeof(struct Graf));
    if(graf==NULL){
        printf("Nije moguće alocirati memoriju za graf.\n");
        return NULL;
    }
    graf->broj_cvorova=broj_cvorova;
    graf->matrica=(int**)malloc(broj_cvorova*sizeof(int*));
    graf->posecen=(int*)malloc(broj_cvorova*sizeof(int));
    if(graf->matrica==NULL || graf->posecen==NULL){
        free(graf->matrica); 
        free(graf->posecen);
        free(graf);
        printf("Nije moguće alocirati memoriju za matricu ili niz posjećenosti.\n");
        return NULL;
    }
    for(int i=0;i<broj_cvorova;i++){
        graf->matrica[i]=(int*)malloc(broj_cvorova*sizeof(int));
        if(graf->matrica[i]==NULL){
            for(int j=0;j<i;j++){
                free(graf->matrica[j]);
            }
            free(graf->matrica);
            free(graf->posecen);
            free(graf);
            printf("Nije moguće alocirati memoriju za red matrice.\n");
            return NULL;
        }
        for(int j=0;j<broj_cvorova;j++) {
            graf->matrica[i][j]=0;  
        }
        graf->posecen[i]=0;
    }
return graf;
}
void dodajGranu(struct Graf *graf, int pocetak, int kraj, int tezina){
    graf->matrica[pocetak][kraj]=tezina;
    graf->matrica[kraj][pocetak]=tezina;
}
void ispisujGraf(struct Graf *graf, int broj_cvorova) {
    printf("Matrica:\n");
    for(int i=0;i<broj_cvorova;i++){
        for(int j=0;j<broj_cvorova;j++) {
            printf("%d\t", graf->matrica[i][j]);
        }
        printf("\n");
    }
}
int nadjiKoren(int roditelj[], int i){
    while(roditelj[i]!=i){
        i=roditelj[i];
    }
return i;
}
void spojiSkupove(int roditelj[], int rang[], int x, int y){
    int koren_x=nadjiKoren(roditelj, x);
    int koren_y=nadjiKoren(roditelj, y);

    if (rang[koren_x]<rang[koren_y]){
        roditelj[koren_x]=koren_y;
    } 
    else if(rang[koren_x]>rang[koren_y]){
        roditelj[koren_y] = koren_x;
    }
    else{
        roditelj[koren_y] = koren_x;
        rang[koren_x]++;
    }
}
int uporediGrane(const void *a, const void *b){
    struct Grana* grana1=(struct Grana*)a;
    struct Grana* grana2=(struct Grana*)b;
    return grana1->tezina-grana2->tezina;
}
void kruskal(struct Graf *graf){
    int broj_cvorova=graf->broj_cvorova;
    struct Grana *grane=(struct Grana*)malloc(broj_cvorova*broj_cvorova*sizeof(struct Grana));

    int broj_grana=0;
    for(int i=0;i<broj_cvorova;i++){
        for(int j=i+1;j<broj_cvorova;j++){  
            if(graf->matrica[i][j]!=0) {
                grane[broj_grana].pocetak=i;
                grane[broj_grana].kraj=j;
                grane[broj_grana].tezina=graf->matrica[i][j];
                broj_grana++;
            }
        }
    }
    qsort(grane, broj_grana, sizeof(struct Grana), uporediGrane);

    struct Grana *MST=(struct Grana*)malloc((broj_cvorova-1)*sizeof(struct Grana));
    int *roditelj=(int*)malloc(broj_cvorova*sizeof(int));
    int *rang=(int*)malloc(broj_cvorova*sizeof(int));

    for(int i=0;i<broj_cvorova;i++){
        roditelj[i]=i;
        rang[i]=0;
    }
    int e=0,ukupna_tezina=0;  
    for(int i=0;i<broj_grana;i++){
        struct Grana trenutna_grana=grane[i];
        int koren_pocetak=nadjiKoren(roditelj, trenutna_grana.pocetak);
        int koren_kraj=nadjiKoren(roditelj, trenutna_grana.kraj);

        if(koren_pocetak!=koren_kraj){   
            MST[e++]=trenutna_grana;
            spojiSkupove(roditelj,rang,koren_pocetak,koren_kraj);
            ukupna_tezina+=trenutna_grana.tezina;
        }
    }
    printf("Minimalno razapinjuce stablo (MST) je:\n");
    for(int i=0;i<broj_cvorova-1;i++){
        printf("(%d,%d) - %d\n",MST[i].pocetak,MST[i].kraj,MST[i].tezina);
    }
    printf("Ukupna tezina MST-a: %d\n",ukupna_tezina);

    free(grane);
    free(MST);
    free(roditelj);
    free(rang);
}
void ispisiCiklus(int ciklus[],int n){
    printf("Hamiltonov ciklus: ");
    for(int i=0;i<n;i++){
        printf("%d ",ciklus[i]);
    }
    printf("%d\n",ciklus[0]); 
}
void nadjiHamiltonovCiklus(struct Graf* graf, int ciklus[], int pos){
    if(pos==graf->broj_cvorova){
        ispisiCiklus(ciklus,pos);
        return;
    }

    int trenutni_cvor=ciklus[pos-1];
    for(int sledeci_cvor=0;sledeci_cvor<graf->broj_cvorova;sledeci_cvor++){
        if(!graf->posecen[sledeci_cvor] && graf->matrica[trenutni_cvor][sledeci_cvor]!=0){
            graf->posecen[sledeci_cvor]=1;
            ciklus[pos]=sledeci_cvor;
            nadjiHamiltonovCiklus(graf,ciklus,pos+1);
            graf->posecen[sledeci_cvor]=0; 
        }
    }
    if(pos==1){
        printf("U Grafu nema vise Hamiltonovih ciklusa.\n");
    }
};
int main(int argc, char *argv){
    int broj_cvorova=10;
    struct Graf *graf=inicijalizujGraf(broj_cvorova);
    int *ciklus=(int*)malloc(broj_cvorova*sizeof(int));
    ciklus[0]=0; 
    graf->posecen[0]=1; 

    /*dodajGranu(graf,0,1,8);
    dodajGranu(graf,1,2,5);
    dodajGranu(graf,2,3,3);
    dodajGranu(graf,3,4,7);
    dodajGranu(graf,4,0,9);*/
    dodajGranu(graf,0,1,10);
    dodajGranu(graf,0,2,6);
    dodajGranu(graf,1,3,5);
    dodajGranu(graf,2,3,10);
    dodajGranu(graf,2,4,7);
    dodajGranu(graf,3,5,9);
    dodajGranu(graf,4,6,11);
    dodajGranu(graf,5,7,4);
    dodajGranu(graf,6,8,12);
    dodajGranu(graf,7,9,8);
    dodajGranu(graf,8,0,3);
    dodajGranu(graf,9,1,5);
    
    ispisujGraf(graf,broj_cvorova);
    kruskal(graf);
    nadjiHamiltonovCiklus(graf,ciklus,1); 

    if(graf!=NULL){
        printf("Uspesno inicijalizovan graf!\n");
        for(int i=0;i<broj_cvorova;i++){ 
            free(graf->matrica[i]);
        }
        free(graf->matrica);
        free(ciklus);
        free(graf->posecen);
        free(graf);
    }
return 0;
};