#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

int input(int min[16], int indefferent_kar[16], int *index_kar, int *in_kar);
void message();
void print_karnaugh(int Map[16], int num_var);
void print_table(int Q_binary[16][4], int length, int min[16]);
void add(int min[16],int indefferent_kar[16], int index_kar, int in_kar, int Map[16]);
void groups_23(int Map[16], int num_var);
void group2(int Map[16]);
void group3(int Map[16]);
void group4(int Map[16]);
void convert_binary(int min[16], int length, int Q_binary[16][4]);
int quine_mccluskey(int Q_binary[16][4], int length, int min[16], int match4[16][4], int teams4[16][4]);
int equal_teams(int row1[], int row2[], int num_cols);
void synthesis(int min[16], int match4[16][4], int l_teams, int teams4[16][4], int index_kar);

int main(int argc, char *argv[]){

    int num_var , ep;
    int min[16]; //max 16 for 4 variables in karnaugh and 256 for Quine-McCluskey
    int index_kar, index_qui, in_kar, length, l_teams;
    int indefferent_kar[16];
    int Map[16];
    int Q[16], Q_binary[16][4], match4[16][4], teams4[16][4];
    
    printf("Enter the number of the variables of a logic function\n");
    scanf("%d" , &num_var);

    index_kar = 0;
    index_qui = 0;
    in_kar = -1;

    while (num_var < 2 || num_var > 8){ // wrong input
        printf("You gave a wrong number of variables. Choose between 2-8\n");
        scanf("%d" , &num_var);
    }

    if(num_var >= 2 && num_var <= 3){ // karnaugh map

        length = input(min,indefferent_kar,&index_kar,&in_kar); // take info for minimums and indefferent situations
        add(min,indefferent_kar,index_kar,in_kar,Map);
        print_karnaugh(Map,num_var);
        groups_23(Map,num_var);
    }
    else if (num_var >= 5 && num_var <= 8){ // Quine-McCluskey
        printf("2");
    }
    else if (num_var == 4){

        printf("Choose beween Karnaugh map (0) and Quine-McCluskey algorithm (1)\n");
        scanf("%d", &ep);

        while(ep != 0 && ep != 1){ // wrong input
            printf("You gave wrong option choose between 0 and 1\n");
            scanf("%d", &ep);
        }

        if(ep == 0){ // karnaugh map

            length = input(min,indefferent_kar,&index_kar,&in_kar);
            add(min,indefferent_kar,index_kar,in_kar,Map);
            print_karnaugh(Map,num_var);
            group4(Map);
        }
        else{ //ep = 1  Quine-McCluskey

            length = input(min,indefferent_kar,&index_kar,&in_kar);  // store the numbers
            convert_binary(min, length, Q_binary);
            print_table(Q_binary, length, min);
            l_teams = quine_mccluskey(Q_binary, length, min, match4, teams4);
            synthesis(min, match4, l_teams, teams4, index_kar);
        }
    }
    
    return 0;
}

int input(int min[16], int indefferent_kar[16], int *index_kar, int *in_kar){

    int answer = 0 , ep2 = 0;
    int length;

    printf("Give the sum of minimums one at a time without the indifferent situations\n");
    message();
    
    while(answer == 0){ // input another number 
        printf("Number: ");
        scanf("%d" , &min[*index_kar]);
        
        printf("\nDo you want to add another? Enter 1 to stop and 0 to continue\n");
        scanf("%d", &answer);

        while(answer != 1 && answer != 0){ // wrong input 
            printf("Wrong input! Enter 1 to stop and 0 to continue\n");
            scanf("%d", &answer);
        }
        if(answer == 0){
            (*index_kar) ++; // increase num of minimums
        }
    }

    answer = 0;
    printf("Do you want to add indifferent situations? Enter 1 for no and 0 to continue\n");
    scanf("%d", &ep2);

    while(ep2 != 1 && ep2 != 0){ // wrong input
        printf("Wrong input! Enter 1 to stop and 0 to continue\n");
        scanf("%d", &ep2);
    }
    if(ep2 == 0){
        
        printf("Give the indifferent situations one at a time\n");
        message();

        while(answer == 0){
            (*in_kar) ++; // increase number of don't care
            printf("Number: ");
            scanf("%d" , &indefferent_kar[*in_kar]);
            
            printf("\nDo you want to add another? Enter 1 to stop and 0 to continue\n");
            scanf("%d", &answer);

            while(answer != 1 && answer != 0){ // wrong input
                printf("Wrong input! Enter 1 to stop and 0 to continue\n");
                scanf("%d", &answer);
            }
        }
    }

    length = *in_kar + *index_kar + 1; // sum of all numbers that array has
    return length;
}

void message(){

    printf("For 2 variables enter 0-3 minimums\n");
    printf("For 3 variables enter 0-7 minimums\n");
    printf("For 4 variables enter 0-16 minimums\n");
}

void print_karnaugh(int Map[16], int num_var){

    printf("\n** In the map the 2 represents don't care situations\n");
    printf("Karnaugh Map: \n\n");

    if(num_var == 2){ // 2 variables 
        printf("X/Y\t|\t0\t1\n");
        printf("---------------------------\n");
        printf("0\t|\t%d\t%d\n", Map[0] , Map[1]);
        printf("1\t|\t%d\t%d\n", Map[2] , Map[3]);
    }
    else if (num_var == 3){ // 3 variables 
        printf("X/YZ\t|\t00\t01\t11\t10\n");
        printf("----------------------------------------------\n");
        printf("0\t|\t%d\t%d\t%d\t%d\n", Map[0] , Map[1], Map[3], Map[2]);
        printf("1\t|\t%d\t%d\t%d\t%d\n", Map[4] , Map[5], Map[7] ,Map[6]);
    }
    else{ // num_var = 4
        printf("WX/YZ\t|\t00\t01\t11\t10\n");
        printf("----------------------------------------------\n");
        printf("00\t|\t%d\t%d\t%d\t%d\n", Map[0] , Map[1], Map[3], Map[2]);
        printf("01\t|\t%d\t%d\t%d\t%d\n", Map[4] , Map[5], Map[7] ,Map[6]);
        printf("11\t|\t%d\t%d\t%d\t%d\n", Map[12] , Map[13], Map[15], Map[14]);
        printf("10\t|\t%d\t%d\t%d\t%d\n", Map[8] , Map[9], Map[11] ,Map[10]);
    }
    
}

void add(int min[16],int indefferent_kar[16], int index_kar, int in_kar, int Map[16]){

    for(int i = 0; i < 16; i++){
        Map[i] = 0; // initialize with zero
    }

    for(int i = 0; i <= index_kar; i++){
        Map[min[i]] = 1; // add minimums
    }

    if(in_kar != -1){
        for(int i = 0; i <= in_kar; i++){
            Map[indefferent_kar[i]] = 2; // 2 = 'X' : indefferent situation
        }
    }
}

void groups_23(int Map[16], int num_var){

    if(num_var == 2){ 

        // Map[0] = m0
        // Map[1] = m1
        // Map[2] = m2
        // Map[3] = m3

        group2(Map);
        
    }
    else{ // 3 variables

        // Map[0] = m0
        // Map[1] = m1
        // Map[2] = m2
        // Map[3] = m3
        // Map[4] = m4
        // Map[5] = m5
        // Map[6] = m6
        // Map[7] = m7

        group3(Map);
    }
}

void group2(int Map[16]){
    // check all the cases that makes a team 
    if((Map[0] == 1 || Map[0] == 2) && (Map[1] == 1 || Map[1] == 2) && (Map[2] == 1 || Map[2] == 2) && (Map[3] == 1 || Map[3] == 2)){
        printf("group m0, m1, m2, m3: 1 \n");
        printf("F = 1 \n");
    }
    else if((Map[0] == 1 || Map[0] == 2) && (Map[1] == 1 || Map[1] == 2) && (Map[2] == 1 || Map[2] == 2)){
        printf("group m0, m1: X' \n");
        printf("group m0, m2: Y' \n");
        printf("F = X' + Y' \n");
    }
    else if((Map[0] == 1 || Map[0] == 2) && (Map[1] == 1 || Map[1] == 2) && (Map[3] == 1 || Map[3] == 2)){
        printf("group m0, m1: X' \n");
        printf("group m1, m3: Y \n");
        printf("F = X' + Y \n");
    }
    else if((Map[0] == 1 || Map[0] == 2) && (Map[2] == 1 || Map[2] == 2) && (Map[3] == 1 || Map[3] == 2)){
        printf("group m0, m2: Y' \n");
        printf("group m2, m3: X \n");
        printf("F = X + Y' \n");
    }
    else if((Map[1] == 1 || Map[1] == 2) && (Map[2] == 1 || Map[2] == 2) && (Map[3] == 1 || Map[3] == 2)){
        printf("group m1, m3: Y \n");
        printf("group m2, m3: X \n");
        printf("F = X + Y \n");
    }
    else if((Map[0] == 1 || Map[0] == 2) && (Map[1] == 1 || Map[1] == 2)){
        printf("group m0, m1: X' \n");
        printf("F = X' \n");
    }
    else if ((Map[2] == 1 || Map[2] == 2) && (Map[3] == 1 || Map[3] == 2)){
        printf("group m2, m3: X \n");
        printf("F = X \n");
    }
    else if ((Map[0] == 1 || Map[0] == 2) && (Map[2] == 1|| Map[2] == 2)){
        printf("group m0, m2: Y' \n");
        printf("F = Y' \n");
    }
    else if((Map[1] == 1 || Map[1] == 2) && (Map[3] == 1 || Map[3] == 2)){
        printf("group m1, m3: Y \n");
        printf("F = Y \n");
    }
    else if((Map[1] == 1) && (Map[2] == 1)){
        printf("group m1: X'Y \n");
        printf("group m2: XY' \n");
        printf("F = X'Y + XY' \n");
    }
    else if((Map[0] == 1 || Map[0] == 2) && (Map[3] == 1 || Map[3] == 2)){
        printf("group m0: X'Y' \n");
        printf("group m3: XY \n");
        printf("F = X'Y' + XY \n");
    }
    else if(Map[0] == 1){
        printf("group m0: X'Y' \n");
        printf("F = X'Y' \n");
    }
    else if(Map[1] == 1){
        printf("group m1: X'Y \n");
        printf("F = X'Y \n");
    }
    else if(Map[2] == 1){
        printf("group m2: XY' \n");
        printf("F = XY' \n");
    }
    else if(Map[3] == 1){
        printf("group m3: XY \n");
        printf("F = XY \n");
    }
}

void group3(int Map[16]){
    // check all the cases that makes a team 
    if((Map[0] == 1 || Map[0] == 2) && (Map[1] == 1 || Map[1] == 2) && (Map[2] == 1 || Map[2] == 2) && (Map[3] == 1 || Map[3] == 2) && (Map[4] == 1 || Map[4] == 2) && (Map[5] == 1 || Map[5] == 2) && (Map[6] == 1 || Map[6] == 2) && (Map[7] == 1 || Map[7] == 2)){
        printf("group m0, m1, m2, m3, m4, m5, m6, m7:    F = 1 \n");
    }
    else if((Map[0] == 1 || Map[0] == 2) && (Map[1] == 1 || Map[1] == 2) && (Map[2] == 1 || Map[2] == 2) && (Map[3] == 1 || Map[3] == 2)){
        printf("group m0, m1, m2, m3: X' \n");

        if((Map[4] == 1 || Map[4] == 2) && (Map[5] == 1 || Map[5] == 2)){

            printf("group m0, m1, m4, m5: Y' \n");
            
            if((Map[7] == 1 || Map[7] == 2)){
                printf("group m1, m3, m5, m7: Z \n");
                printf("F = Y' + X' + Z \n");
            }
            else if((Map[6] == 1 || Map[6] == 2)){
                printf("group m0, m4, m2, m6: Z' \n");
                printf("F = Y' + X' + Z' \n");
            }
            else{
                printf("F = Y' + X' \n");
            }
        }
        else if((Map[4] == 1 || Map[4] == 2) && (Map[7] == 1 || Map[7] == 2)){

            if((Map[6] == 1 || Map[6] == 2)){
                printf("group m0, m4, m2, m6: Z' \n");
                printf("group m2, m3, m6, m7: Y \n");
                printf("F = Y + X' + Z' \n");
            }
            else{
                printf("group m0, m4: Y'Z' \n");
                printf("group m3, m7: YZ \n");
                printf("F = Y'Z' + YZ + X' \n");     
            }
        }
        else if((Map[4] == 1 || Map[4] == 2) && (Map[6] == 1 || Map[6] == 2)){
            printf("group m4, m6: XZ' \n");
            printf("F = XZ' + X' \n");
        }
        else if(Map[4] == 1){
            printf("group m0, m4: Y'Z' \n");
            printf("F = X' + Y'Z' \n");
        }
        else if(Map[5] == 1){
            printf("group m1, m5: Y'Z \n");
            printf("F = X' + Y'Z \n");
        }
        else if(Map[6] == 1){
            printf("group m2, m6: YZ' \n");
            printf("F = X' + YZ' \n");
        }
        else if(Map[7] == 1){
            printf("group m7, m3: YZ \n");
            printf("F = X' + YZ \n");
        }
        else{
            printf("F = X' \n");
        }
        
    }
    else if((Map[4] == 1 || Map[4] == 2) && (Map[5] == 1 || Map[5] == 2) && (Map[6] == 1 || Map[6] == 2) && (Map[7] == 1 || Map[7] == 2)){

        printf("group m4, m5, m6, m7: X \n");

        if((Map[0] == 1 || Map[0] == 2) && (Map[1] == 1 || Map[1] == 2)){
            printf("group m0, m1, m4, m5: Y' \n");

            if((Map[3] == 1 || Map[3] == 2)){
                printf("group m1, m3, m5, m7: Z \n");
                printf("F = X + Y' + Z \n");
            }
            else if((Map[2] == 1 || Map[2] == 2)){
                printf("group m0, m4, m6, m2: Z' \n");
                printf("F = X + Y' + Z' \n");
            }
            else{
                printf("F = X + Y' \n");
            }
        }
        else if((Map[2] == 1 || Map[2] == 2) && (Map[3] == 1 || Map[3] == 2)){

            printf("group m2, m3, m7, m6: Y \n");

            if((Map[0] == 1 || Map[0] == 2)){
                printf("group m0, m4, m6, m2: Z' \n");
                printf("F = X + Z' + Y \n");
            }
            else if((Map[1] == 1 || Map[1] == 2)){
                printf("group m1, m3, m5, m7: Z \n");
                printf("F = X + Y + Z \n");
            }
            else{
                printf("F = X + Y \n");
            }
        }
        else if((Map[0] == 1 || Map[0] == 2) && (Map[2] == 1 || Map[2] == 2)){
            printf("group m0, m4, m6, m2: Z' \n");
            printf("F = X + Z' \n");
        }
        else if(Map[0] == 1){
            printf("group m0, m4: Y'Z' \n");
            printf("F = X + Y'Z' \n");
        }
        else if(Map[1] == 1){
            printf("group m1, m5: Y'Z \n");
            printf("F = X + Y'Z \n");
        }
        else if(Map[3] == 1){
            printf("group m3, m7: YZ \n");
            printf("F = X + YZ \n");
        }
        else if(Map[2] == 1){
            printf("group m2, m6: YZ' \n");
            printf("F = X + YZ' \n");
        }
        else{
            printf("F = X \n");
        }
    }
    else if((Map[0] == 1 || Map[0] == 2) && (Map[1] == 1 || Map[1] == 2) && (Map[4] == 1 || Map[4] == 2) && (Map[5] == 1 || Map[5] == 2)){
        printf("group m0, m1, m4, m5: Y' \n");

        if((Map[3] == 1 || Map[3] == 2) && (Map[7] == 1 || Map[7] == 2)){
            printf("group m1, m3, m5, m7: Z \n");
            printf("F = Y' + Z \n");
        }
        else if((Map[2] == 1 || Map[2] == 2) && (Map[6] == 1 || Map[6] == 2)){
            printf("group m0, m2, m4, m6: Z' \n");
            printf("F = Y' + Z' \n");
        }
        else if((Map[3] == 1 || Map[3] == 2) && (Map[6] == 1)){
            printf("group m1, m3: X'Z \n");
            printf("group m4, m6: XZ' \n");
            printf("F = Y' + X'Z + XZ' \n");
        }
        else if(Map[3] == 1){
            printf("group m1, m3: X'Z \n");
            printf("F = Y' + X'Z \n");
        }
        else if(Map[2] == 1){
            printf("group m2, m0: X'Z' \n");
            printf("F = Y' + X'Z' \n");
        }
        else if(Map[7] == 1){
            printf("group m5, m7: XZ \n");
            printf("F = Y' + XZ \n");
        }
        else if(Map[6] == 1){
            printf("group m4, m6: XZ' \n");
            printf("F = Y' + XZ' \n");
        }
        else{
            printf("F = Y' \n");
        }
    }
    else if((Map[1] == 1 || Map[1] == 2) && (Map[3] == 1 || Map[3] == 2) && (Map[5] == 1 || Map[5] == 2) && (Map[7] == 1 || Map[7] == 2)){
        printf("group m1, m3, m5, m7: Z \n");

        if((Map[2] == 1 || Map[2] == 2) && (Map[6] == 1 || Map[6] == 2)){
            printf("group m2, m3, m6, m7: Y \n");
            printf("F = Y + Z \n");
        }
        else if((Map[0] == 1 || Map[0] == 2) && (Map[6] == 1 || Map[6] == 2)){
            printf("group m1, m0: X'Y' \n");
            printf("group m6, m7: XY \n");
            printf("F = Z + X'Y' + XY \n");
        }
        else if((Map[2] == 1 || Map[2] == 2) && (Map[4] == 1 || Map[4] == 2)){
            printf("group m2, m3: X'Y \n");
            printf("group m4, m5: XY' \n");
            printf("F = Z + X'Y + XY' \n");
        }
        else if(Map[0] == 1){
            printf("group m1, m0: X'Y' \n");
            printf("F = z + X'Y' \n");
        }
        else if(Map[4] == 1){
            printf("group m4, m5: XY' \n");
            printf("F = z + XY' \n");
        }
        else if(Map[2] == 1){
            printf("group m2, m3: X'Y \n");
            printf("F = z + X'Y \n");
        }
        else if(Map[6] == 1){
            printf("group m6, m7: XY \n");
            printf("F = z + XY \n");
        }
    }
    else if((Map[3] == 1 || Map[3] == 2) && (Map[2] == 1 || Map[2] == 2) && (Map[7] == 1 || Map[7] == 2) && (Map[6] == 1 || Map[6] == 2)){
        printf("group m2, m3, m6, m7: Y \n");

        if((Map[0] == 1 || Map[0] == 2) && (Map[4] == 1 || Map[4] == 2)){
            printf("group m2, m0, m6, m4: Z' \n");
            printf("F = Y + Z' \n");
        }
        else if((Map[0] == 1 || Map[0] == 2) && (Map[5] == 1 || Map[5] == 2)){
            printf("group m2, m0: X'Z' \n");
            printf("group m5, m7: XZ \n");
            printf("F = Y + X'Z' + XZ \n");
        }
        else if((Map[1] == 1 || Map[1] == 2) && (Map[4] == 1 || Map[4] == 2)){
            printf("group m1, m3: X'Z \n");
            printf("group m4, m6: XZ' \n");
            printf("F = Y + X'Z + XZ' \n");
        }
        else if(Map[0] == 1){
            printf("group m2, m0: X'Z' \n");
            printf("F = Y + X'Z' \n");
        }
        else if(Map[1] == 1){
            printf("group m1, m3: X'Z \n");
            printf("F = Y + X'Z \n");
        }
        else if(Map[4] == 1){
            printf("group m4, m6: XZ' \n");
            printf("F = Y + XZ' \n");
        }
        else if(Map[5] == 1){
            printf("group m5, m7: XZ \n");
            printf("F = Y + XZ \n");
        }
    }
    else if((Map[0] == 1 || Map[0] == 2) && (Map[2] == 1 || Map[2] == 2) && (Map[4] == 1 || Map[4] == 2) && (Map[6] == 1 || Map[6] == 2)){
        printf("group m0, m2, m4, m6: Z' \n");

        if((Map[1] == 1 || Map[1] == 2) && (Map[7] == 1 || Map[7] == 2)){
            printf("group m1, m0: X'Y' \n");
            printf("group m6, m7: XY \n");
            printf("F = Z' + X'Y' + XY \n");
        }
        else if((Map[3] == 1 || Map[3] == 2) && (Map[5] == 1 || Map[5] == 2)){
            printf("group m2, m3: X'Y \n");
            printf("group m4, m5: XY' \n");
            printf("F = Z' + X'Y + XY' \n");
        }
        else if(Map[1] == 1){
            printf("group m1, m0: X'Y' \n");
            printf("F = Z' + X'Y' \n");
        }
        else if(Map[3] == 1){
            printf("group m2, m3: X'Y \n");
            printf("F = Z' + X'Y \n");
        }
        else if(Map[5] == 1){
            printf("group m4, m5: XY' \n");
            printf("F = Z' + XY' \n");
        }
        else if(Map[7] == 1){
            printf("group m6, m7: XY \n");
            printf("F = Z' + XY \n");
        }
    }

    else if((Map[4] == 1 || Map[4] == 2) && (Map[5] == 1 || Map[5] == 2)){
        printf("m4, m5: XY' \n");

        if((Map[3] == 1 || Map[3] == 2) && (Map[7] == 1 || Map[7] == 2)){
            printf("m3, m7: YZ \n");
            
            if(Map[2] == 1){
                printf("m2: X'YZ' \n");
                printf("F = XY' + X'YZ' + YZ \n");
            }
            else if(Map[6] == 1){
                printf("m6: XYZ' \n");
                printf("F = XY' + XYZ' + YZ \n");
            }
            else if(Map[0] == 1){
                printf("m0: X'Y'Z' \n");
                printf("F = XY' + X'Y'Z' + YZ \n");
            }
            else if(Map[1] == 1){
                printf("m1: X'Y'Z \n");
                printf("F = XY' + X'Y'Z + YZ \n");
            }
            else{
                printf("F = XY' + YZ \n");
            }
        }
        else if((Map[2] == 1 || Map[2] == 2) && (Map[6] == 1 || Map[6] == 2)){
            printf("m2, m6: YZ' \n");
            
            if(Map[0] == 1){
                printf("m0: X'Y'Z' \n");
                printf("F = XY' + X'Y'Z' + YZ' \n");
            }
            else if(Map[1] == 1){
                
                if(Map[5] == 1 || Map[5] == 2){
                    printf("m1, m5: Y'Z \n");
                    printf("F = XY' + Y'Z + YZ' \n");
                }
                else{
                    printf("m1: X'Y'Z \n");
                    printf("F = XY' + X'Y'Z + YZ' \n");
                }
            }
            else if(Map[3] == 1){
                printf("m3: X'YZ \n");
                printf("F = XY' + X'YZ + YZ' \n");
            }
            else if(Map[7] == 1){
                printf("m7: XYZ \n");
                printf("F = XY' + XYZ + YZ' \n");
            }
            else{
                printf("F = XY' + YZ' \n");
            }
        }
        else if((Map[0] == 1 || Map[0] == 2) && (Map[2] == 1 || Map[2] == 2)){
            printf("m0, m2: X'Z' \n");
            
            if(Map[1] == 1){
                printf("m1: X'Y'Z \n");
                printf("F = XY' + X'Y'Z + X'Z' \n");
            }
            else if(Map[3] == 1){
                printf("m3: X'YZ \n");
                printf("F = XY' + X'YZ + X'Z' \n");
            }
            else if(Map[7] == 1){
                printf("m7: XYZ \n");
                printf("F = XY' + XYZ + X'Z' \n");
            }
            else if(Map[6] == 1){
                printf("m6: XYZ' \n");
                printf("F = XY' + XYZ' + X'Z' \n");
            }
            else{
                printf("F = XY' + X'Z' \n");
            }
        }
        else if(Map[3] == 1){
            printf("m3: X'YZ \n");
            printf("F = XY' + X'YZ \n");
        }
        else if(Map[7] == 1){
            printf("m7: XYZ \n");
            printf("F = XY' + XYZ \n");
        }
        else if(Map[2] == 1){
            printf("m2: X'YZ' \n");
            printf("F = XY' + X'YZ' \n");
        }
        else if(Map[6] == 1){
            printf("m6: XYZ' \n");
            printf("F = XY' + XYZ' \n");
        }
        else{
            printf("F = XY' \n");
        }
    }
    else if((Map[4] == 1 || Map[4] == 2) && (Map[6] == 1 || Map[6] == 2)){
        printf("m4, m6: XZ' \n");
        if((Map[0] == 1 || Map[0] == 2) && (Map[1] == 1 || Map[1] == 2)){

            printf("m1, m0: X'Y' \n");
            if(Map[7] == 1 || Map[7] == 2 && (Map[3] == 1 || Map[3] == 2)){
                printf("m3, m7: YZ \n");
                printf("F = XZ' + X'Y' + YZ \n");
            }
            else if(Map[3] == 1){
                printf("m1, m3: X'Z \n");
                printf("F = XZ' + X'Y' + X'Z \n");
            }
            else if(Map[7] == 1){
                printf("m6, m7: XY \n");
                printf("F = XZ' + X'Y' + XY \n");
            }
            else{
                printf("F = XZ' + X'Y'\n");
            }
        }
        else if((Map[5] == 1 || Map[5] == 2) && (Map[1] == 1 || Map[1] == 2)){
            printf("m1, m5: Y'Z \n");

            if(Map[2] == 1 || Map[2] == 2 && (Map[3] == 1 || Map[3] == 2)){
                printf("m3, m2: X'Y \n");
                printf("F = XZ' + Y'Z + X'Y \n");
            }
            else if(Map[3] == 1){
                printf("m1, m3: X'Z \n");
                printf("F = XZ' + Y'Z + X'Z \n");
            }
            else if(Map[2] == 1){
                printf("m6, m2: YZ' \n");
                printf("F = XZ' + Y'Z + YZ' \n");
            }
            else{
                printf("F = XZ' + Y'Z \n");
            }
        }
        else if((Map[2] == 1 || Map[2] == 2) && (Map[3] == 1 || Map[3] == 2)){
            printf("m2, m3: X'Y \n");

            if(Map[1] == 1){
                printf("m1, m3: X'Z \n");
                printf("F = XZ' + X'Y + X'Z \n");
            }
            else if(Map[5] == 1){
                printf("m6, m2: YZ' \n");
                printf("F = XZ' + X'Y + YZ' \n");
            }
            else{
                printf("F = XZ' + X'Y \n");
            }
        }
        else if((Map[1] == 1 || Map[1] == 2) && (Map[3] == 1 || Map[3] == 2)){
            printf("m1, m3: X'Z \n");

            if(Map[2] == 1){
                printf("m2, m6: YZ' \n");
                printf("F = XZ' + X'Z + YZ' \n");
            }
            else if(Map[7] == 1){
                printf("m7, m6: XY \n");
                printf("F = XZ' + X'Z + XY \n");
            }
            else{
                printf("F = XZ' + X'Z \n");
            }
        }
        else if((Map[7] == 1 || Map[7] == 2) && (Map[3] == 1 || Map[3] == 2)){
            printf("m7, m3: YZ \n");

            if(Map[0] == 1){
                printf("m0, m4: Y'Z' \n");
                printf("F = XZ' + YZ + Y'Z' \n");
            }
            else if(Map[1] == 1){
                printf("m1, m3: X'Z \n");
                printf("F = XZ' + YZ + X'Z \n");
            }
            else{
                printf("F = XZ' + YZ \n");
            }
        }
        else if(Map[1] == 1 && Map[2] == 1){
            printf("group m1: X'Y'Z \n");
            printf("group m2: X'YZ' \n");

            if(Map[7] == 1){
                printf("m7: XYZ \n");
                printf("F = XZ' + X'Y'Z + X'YZ' + XYZ \n");
            }
            else{
                printf("F = XZ' + X'Y'Z + X'YZ' \n");
            }
        }
        else if(Map[1] == 1){
            printf("group m1: X'Y'Z \n");
            printf("F = XZ' + X'Y'Z \n");
        }
        else if(Map[2] == 1){
            printf("group m2: X'YZ' \n");

            if(Map[7] == 1){
                printf("group m7: XYZ \n");

                if(Map[1] == 1){
                    printf("group m1: X'Y'Z \n");
                    printf("F = XZ' + X'YZ' + XYZ + X'Y'Z \n");
                }
                else{
                    printf("F = XZ' + X'YZ' + XYZ \n");
                }
            }
            else{
                printf("F = XZ' + X'YZ' \n");
            }
        }
        else if(Map[3] == 1){
            printf("group m3: X'YZ \n");

            if(Map[2] == 1 && Map[5] == 1){
                printf("group m2: X'YZ' \n");
                printf("group m5: XY'Z \n");
                printf("F = XZ' + X'YZ + X'YZ' + XY'Z \n");
            }
            else if(Map[2] == 1){
                printf("group m2: X'YZ' \n");
                printf("F = XZ' + X'YZ + X'YZ' \n");
            }
            else if(Map[5] == 1){
                printf("group m5: XY'Z \n");
                printf("F = XZ' + X'YZ + XY'Z \n");
            }
            else{
                printf("F = XZ' + X'YZ \n");
            }
        }
        else if(Map[5] == 1){
            printf("group m5: XY'Z \n");

            if(Map[2] == 1 && Map[7] == 1){
                printf("group m2: X'YZ' \n");
                printf("group m7: XYZ \n");
                printf("F = XZ' + XY'Z + X'YZ' + XYZ \n");
            }
            else if(Map[2] == 1){
                printf("group m2: X'YZ' \n");
                printf("F = XZ' + XY'Z + X'YZ' \n");
            }
            else if(Map[7] == 1){
                printf("group m7: XYZ \n");
                
                if(Map[1] == 1){
                    printf("group m1: X'Y'Z \n");
                    printf("F = XZ' + XY'Z + XYZ + X'Y'Z \n");
                }
                else{
                    printf("F = XZ' + XY'Z + XYZ \n");
                }
            }
            else{
                printf("F = XZ' + XY'Z \n");
            }
        }
        
    }
    else if((Map[0] == 1 || Map[0] == 2) && (Map[4] == 1 || Map[4] == 2)){

        printf("m0, m4: Y'Z' \n");

        if((Map[3] == 1 || Map[3] == 2) && (Map[7] == 1 || Map[7] == 2)){
            printf("m3, m7: YZ \n");
            
            if(Map[1] == 1){
                printf("m0, m1: X'Y' \n");
                printf("F = Y'Z' + X'Y' + YZ \n");
            }
            else if(Map[5] == 1){
                printf("m4, m5: XY' \n");
                printf("F = Y'Z' + XY' + YZ \n");
            }
            else if(Map[2] == 1){
                printf("m0, m2: X'Z' \n");
                printf("F = Y'Z' + X'Z' + YZ \n");
            }
            else if(Map[6] == 1){
                printf("m4, m6: XZ' \n");
                printf("F = Y'Z' + XZ' + YZ \n");
            }
            else{
                printf("F = Y'Z' + YZ \n");
            }
        }
        else if((Map[3] == 1 || Map[3] == 2) && (Map[1] == 1 || Map[1] == 2)){
            printf("m3, m1: X'Z \n");
            
            if(Map[7] == 1){
                printf("m7: XYZ \n");
                if(Map[6] == 1 || (Map[6] == 2)){
                    printf("m7, m6: XY\n");
                    printf("F = Y'Z' + XY + X'Z \n");
                }
                else{
                    printf("F = Y'Z' + XYZ + X'Z \n");
                }
            }
            else if(Map[5] == 1){
                printf("m4, m5: XY' \n");
                printf("F = Y'Z' + XY' + X'Z \n");
            }
            else if(Map[2] == 1){
                printf("m0, m2: X'Z' \n");
                printf("F = Y'Z' + X'Z' + X'Z \n");
            }
            else if(Map[6] == 1){
                printf("m4, m6: XZ' \n");
                printf("F = Y'Z' + XZ' + X'Z \n");
            }
            else{
                printf("F = Y'Z' + X'Z \n");
            }

        }
        else if((Map[5] == 1 || Map[5] == 2) && (Map[7] == 1 || Map[7] == 2)){
            printf("m5, m7: XZ \n");
            
            if(Map[3] == 1){
                
                if(Map[2] == 1 || Map[2] == 2){
                    printf("m3, m2: X'Y \n");
                    printf("F = Y'Z' + XZ + X'Y \n");
                }
                else{
                    printf("m3: X'YZ \n");
                    printf("F = Y'Z' + X'YZ + XZ \n");
                }
            }
            else if(Map[1] == 1){
                printf("m0, m1: X'Y' \n");
                if(Map[2] == 1){
                    printf("m0, m2: X'Z' \n");
                    printf("F = Y'Z' + X'Y' + XZ + X'Z' \n");
                }
                else if(Map[6] == 1){
                    printf("m4, m6: XZ' \n");
                    printf("F = Y'Z' + X'Y' + XZ + XZ' \n");
                }
                else{
                    printf("F = Y'Z' + X'Y' + XZ \n");
                }
            }
            else if(Map[2] == 1){
                printf("m0, m2: X'Z' \n");
                printf("F = Y'Z' + X'Z' + XZ \n");
            }
            else if(Map[6] == 1){
                printf("m4, m6: XZ' \n");
                printf("F = Y'Z' + XZ' + XZ \n");
            }
            else{
                printf("F = Y'Z' + XZ \n");
            }
        }
        else if((Map[2] == 1 || Map[2] == 2) && (Map[3] == 1 || Map[3] == 2)){
            printf("m2, m3: X'Y \n");
            
            if(Map[7] == 1){
                if(Map[5] == 1 || Map[5] == 2){
                    printf("m5, m7: XZ \n");
                    printf("F = Y'Z' + X'Y + XZ \n");
                }
                else{
                    printf("m7: XYZ \n");
                    printf("F = Y'Z' + XYZ + X'Y \n");
                }
            }
            else if(Map[1] == 1){
                printf("m0, m1: X'Y' \n");
                printf("F = Y'Z' + X'Y' + X'Y \n");
            }
            else if(Map[5] == 1){
                printf("m4, m5: XY' \n");
                printf("F = Y'Z' + XY' + X'Y \n");
            }
            else if(Map[6] == 1){
                printf("m4, m6: XZ' \n");
                printf("F = Y'Z' + XZ' + X'Y \n");
            }
            else{
                printf("F = Y'Z' + X'Y + X'Y \n");
            }

        }
        else if((Map[6] == 1 || Map[6] == 2) && (Map[7] == 1 || Map[7] == 2)){
            printf("m6, m7: XY \n");
            
            if(Map[3] == 1){
                printf("m3: X'YZ \n");
                printf("F = Y'Z' + X'YZ + XY \n");
            }
            else if(Map[1] == 1){
                printf("m0, m1: X'Y' \n");
                printf("F = Y'Z' + X'Y' + XY \n");
            }
            else if(Map[5] == 1){
                printf("m4, m5: XY' \n");
                printf("F = Y'Z' + XY' + XY \n");
            }
            else if(Map[2] == 1){
                printf("m0, m2: X'Z' \n");
                printf("F = Y'Z' + X'Z' + XY \n");
            }
            else{
                printf("F = Y'Z' + XY + XY \n");
            }

        }
        else if(Map[3] == 1){
            printf("m3: X'YZ \n");
            printf("F = Y'Z' + X'YZ \n");
        }
        else if(Map[7] == 1){
            printf("m7: XYZ \n");
            printf("F = Y'Z' + XYZ \n");
        }
        else if(Map[1] == 1){
            printf("m0, m1: X'Y' \n");
            printf("F = Y'Z' + X'Y' \n");
        }
        else if(Map[5] == 1){
            printf("m4, m5: XY' \n");
            printf("F = Y'Z' + XY' \n");
        }
        else if(Map[2] == 1){
            printf("m0, m2: X'Z' \n");
            printf("F = Y'Z' + X'Z' \n");
        }
        else if(Map[6] == 1){
            printf("m4, m6: XZ' \n");
            printf("F = Y'Z' + XZ' \n");
        }
        else{
            printf("F = Y'Z' \n");
        }
    }
    else if((Map[1] == 1 || Map[1] == 2) && (Map[3] == 1 || Map[3] == 2)){

        printf("m1, m3: X'Z \n");

        if((Map[4] == 1 || Map[4] == 2) && (Map[0] == 1 || Map[0] == 2)){
            printf("m4, m0: Y'Z' \n");
            
            if(Map[6] == 1){
                printf("m6: XYZ' \n");
                printf("F = X'Z + XYZ' + Y'Z' \n");
            }
            else if(Map[2] == 1){
                printf("m2: X'YZ' \n");
                printf("F = X'Z + X'YZ' + Y'Z' \n");
            }
            else if(Map[5] == 1){
                printf("m5: XY'Z \n");
                printf("F = X'Z + XY'Z + Y'Z' \n");
            }
            else if(Map[7] == 1){
                printf("m7: XYZ \n");
                printf("F = X'Z + XYZ + Y'Z' \n");
            }
            else{
                printf("F = X'Z + Y'Z' \n");
            }
        }
        else if(Map[4] == 1){
            printf("m4: XY'Z' \n");
            printf("F = X'Z + XY'Z' \n");
        }
        else if((Map[2] == 1 || Map[2] == 2) && (Map[6] == 1 || Map[6] == 2)){
            printf("m2, m6: YZ' \n");
            
            if(Map[4] == 1){
                printf("m4: XY'Z' \n");
                printf("F = X'Z + XY'Z' + YZ' \n");
            }
            else if(Map[0] == 1){
                printf("m0: X'Y'Z' \n");
                printf("F = X'Z + X'Y'Z' + YZ' \n");
            }
            else if(Map[5] == 1){
                printf("m5: XY'Z \n");
                printf("F = X'Z + XY'Z + YZ' \n");
            }
            else if(Map[7] == 1){
                printf("m7: XYZ \n");
                printf("F = X'Z + XYZ + YZ' \n");
            }
            else{
                printf("F = X'Z + YZ' \n");
            }
        }
        else if((Map[4] == 1 || Map[4] == 2) && (Map[6] == 1 || Map[6] == 2)){
            printf("m4, m6: XZ' \n");
            
            if(Map[0] == 1){
                printf("m0: X'Y'Z' \n");
                printf("F = X'Z + X'Y'Z' + XZ' \n");
            }
            else if(Map[2] == 1){
                printf("m2: X'YZ' \n");
                printf("F = X'Z + X'YZ' + XZ' \n");
            }
            else if(Map[5] == 1){
                printf("m5: XY'Z \n");
                printf("F = X'Z + XY'Z + XZ' \n");
            }
            else if(Map[7] == 1){
                printf("m7: XYZ \n");
                printf("F = X'Z + XYZ + XZ' \n");
            }
            else{
                printf("F = X'Z + xZ' + XZ' \n");
            }
        }
        else if(Map[6] == 1){
            printf("m6: XYZ' \n");
            printf("F = X'Z + XYZ' \n");
        }
        else if(Map[0] == 1){
            printf("m0: X'Y'Z' \n");
            printf("F = X'Z + X'Y'Z' \n");
        }
        else if(Map[2] == 1){
            printf("m2: X'YZ' \n");
            printf("F = X'Z + X'YZ' \n");
        }
        else if(Map[5] == 1){
            
            if(Map[1] == 1 || Map[1] == 2){
                printf("m5, m1: Y'Z \n");
                printf("F = X'Z + XY'Z + Y'Z \n");
            }
            else{
                printf("m5: XY'Z \n");
                printf("F = X'Z + XY'Z \n");
            }
        }
        else if(Map[7] == 1){
            printf("m7: XYZ \n");
            printf("F = X'Z + XYZ \n");
        }
        else{
            printf("F = X'Z \n");
        }
    }
    else if((Map[1] == 1 || Map[1] == 2) && (Map[5] == 1 || Map[5] == 2) && (Map[4] != 1 )){

        printf("m1, m5: Y'Z \n");

        if((Map[2] == 1 || Map[2] == 2) && (Map[6] == 1 || Map[6] == 2)){
            printf("m2, m6: YZ' \n");
            printf("F = Y'Z + YZ' \n");
        }
        else if(Map[2] == 1){
            if(Map[3] == 1 || Map[3] == 2){
                printf("m2, m3: X'Y \n");
                printf("F = Y'Z + X'Y \n");
            }
            else{
                printf("m2: X'YZ' \n");
                printf("F = Y'Z + X'YZ' \n");
            }
        }
        else if(Map[6] == 1){
            if(Map[7] == 1 || Map[7] == 2){
                printf("m6, m7: XY \n");
                printf("F = Y'Z + XY \n");
            }
            else{
                printf("m6: XYZ \n");
                printf("F = Y'Z + XYZ' \n");
            }
        }
        else if(Map[0] == 1){
            printf("m0, m1: X'Y' \n");
            printf("F = Y'Z + X'Y' \n");
        }
        else if(Map[4] == 1){
            printf("m4, m5: XY' \n");
            printf("F = Y'Z + XY' \n");
        }
        else if(Map[3] == 1){
            printf("m1, m3: X'Z \n");
            printf("F = Y'Z + X'Z \n");
        }
        else if(Map[7] == 1){
            printf("m5, m7: XZ \n");
            printf("F = Y'Z + XZ \n");
            
        }
        else{
            printf("F = Y'Z \n");
        }
    }
    else if((Map[0] == 1 || Map[0] == 2) && (Map[1] == 1 || Map[1] == 2)){

        printf("m0, m1: X'Y' \n");

        if((Map[7] == 1 || Map[7] == 2) && (Map[6] == 1 || Map[6] == 2)){
            printf("m7, m6: XY \n");
            
            if(Map[2] == 1){
                printf("m0, m2: X'Z' \n");
                printf("F = X'Y' + X'Z' + XY \n");
            }
            else if(Map[3] == 1){
                printf("m1, m3: X'Z \n");
                printf("F = X'Y' + X'Z + XY \n");
            }
            else if(Map[4] == 1){
                printf("m0, m4: Y'Z' \n");
                printf("F = X'Y' + Y'Z' + XY \n");
            }
            else if(Map[5] == 1){
                printf("m5, m7: XZ \n");
                printf("F = X'Y' + XZ + XY \n");
            }
            else{
                printf("F = X'Y' + XY \n");
            }
        }
        else if((Map[7] == 1 || Map[7] == 2) && (Map[3] == 1 || Map[3] == 2)){
            printf("m7, m3: YZ \n");
            
            if(Map[2] == 1){
                printf("m0, m2: X'Z' \n");
                printf("F = X'Y' + X'Z' + YZ \n");
            }
            else if(Map[4] == 1){
                printf("m0, m4: Y'Z' \n");
                printf("F = X'Y' + Y'Z' + YZ \n");
            }
            else if(Map[5] == 1){
                printf("m5, m7: XZ \n");
                printf("F = X'Y' + XZ + YZ \n");
            }
            else if(Map[6] == 1){
                printf("m6: XYZ' \n");
                printf("F = X'Y' + XYZ' + YZ \n");
            }
            else{
                printf("F = X'Y' + YZ \n");
            }

        }
        else if((Map[2] == 1 || Map[2] == 2) && (Map[6] == 1 || Map[6] == 2)){
            printf("m2, m6: YZ' \n");
            
            if(Map[4] == 1){
                printf("m0, m4: Y'Z' \n");
                printf("F = X'Y' + Y'Z' + YZ' \n");
            }
            else if(Map[5] == 1){
                printf("m5, m7: XZ \n");
                printf("F = X'Y' + XZ + YZ' \n");
            }
            else if(Map[7] == 1){
                printf("m7: XYZ \n");
                printf("F = X'Y' + XYZ + YZ' \n");
            }
            else if(Map[3] == 1){
                printf("m1, m3: X'Z \n");
                printf("F = X'Y' + X'Z + YZ' \n");
            }
            else{
                printf("F = X'Y' + YZ' \n");
            }
        }
        else if(Map[7] == 1){
            printf("m7: XYZ \n");
            printf("F = X'Y' + XYZ \n");
        }
        else if(Map[6] == 1){
            printf("m6: XYZ' \n");
            printf("F = X'Y' + XYZ' \n");
        }
        else if(Map[2] == 1){
            printf("m0, m2: X'Z' \n");
            printf("F = X'Y' + X'Z' \n");
        }
        else if(Map[3] == 1){
            printf("m1, m3: X'Z \n");
            printf("F = X'Y' + X'Z \n");
        }
        else{
            printf("F = X'Y' \n");
        }
    }
    else if((Map[2] == 1 || Map[2] == 2) && (Map[3] == 1 || Map[3] == 2)){

        printf("m2, m3: X'Y \n");

        if((Map[4] == 1 || Map[4] == 2) && (Map[5] == 1 || Map[5] == 2)){
           
           printf("m4, m5: XY' \n");

            if(Map[0] == 1){
                printf("m0, m2: X'Z' \n");
                printf("F = X'Y + X'Z' + XY' \n");
            }
            else if(Map[1] == 1){
                printf("m1, m3: X'Z \n");
                printf("F = X'Y + X'Z + XY' \n");
            }
            else if(Map[6] == 1){
                printf("m6, m2: YZ' \n");
                printf("F = X'Y + YZ' + XY' \n");
            }
            else if(Map[7] == 1){
                printf("m7, m3: YZ \n");
                printf("F = X'Y + YZ + XY' \n");
            }
            else{
                printf("F = X'Y + XY' \n");
            }
        }
        else if((Map[4] == 1 || Map[4] == 2) && (Map[0] == 1 || Map[0] == 2)){
            printf("m4, m0: Y'Z' \n");
            
            if(Map[5] == 1){
                printf("m5: XY'Z \n");
                printf("F = X'Y + XY'Z + Y'Z' \n");
            }
            else if(Map[6] == 1){
                printf("m6, m2: YZ' \n");
                printf("F = X'Y + YZ' + Y'Z' \n");
            }
            else if(Map[7] == 1){
                printf("m7, m3: YZ \n");
                printf("F = X'Y + YZ + Y'Z' \n");
            }
            else{
                printf("F = X'Y + Y'Z' \n");
            }

        }
        else if((Map[1] == 1 || Map[1] == 2) && (Map[5] == 1 || Map[5] == 2)){
            printf("m1, m5: Y'Z \n");
            printf("F = X'Y + Y'Z \n");

            if(Map[4] == 1){
                printf("m4: XY'Z' \n");
                printf("F = X'Y + XY'Z' \n");
            }
            else if(Map[0] == 1){
                printf("m0, m2: X'Z' \n");
                printf("F = X'Y + X'Z' \n");
            }
            else if(Map[6] == 1){
                printf("m6, m2: YZ' \n");
                printf("F = X'Y + YZ' \n");
            }
            else if(Map[7] == 1){
                printf("m7, m3: YZ \n");
                printf("F = X'Y + YZ \n");
            }

        }
        else if(Map[4] == 1){
            printf("m4: XY'Z' \n");
            printf("F = X'Y + XY'Z' \n");
        }
        else if(Map[5] == 1){
            printf("m5: XY'Z \n");
            printf("F = X'Y + XY'Z \n");
        }
        else if(Map[0] == 1){
            printf("m0, m2: X'Z' \n");
            printf("F = X'Y + X'Z' \n");
        }
        else if(Map[1] == 1){
            printf("m1, m3: X'Z \n");
            printf("F = X'Y + X'Z \n");
        }
        else if(Map[6] == 1){
            printf("m6, m2: YZ' \n");
            printf("F = X'Y + YZ' \n");
        }
        else if(Map[7] == 1){
            printf("m7, m3: YZ \n");
            printf("F = X'Y + YZ \n");
        }
        else{
            printf("F = X'Y \n");
        }
    }
    else if((Map[7] == 1 || Map[7] == 2) && (Map[5] == 1 || Map[5] == 2)){
        printf("m7, m5: XZ \n");

        if((Map[0] == 1 || Map[0] == 2) && (Map[2] == 1 || Map[2] == 2)){
            printf("m0, m2: X'Z' \n");
            
            if(Map[3] == 1){
                printf("m3: X'YZ \n");
                printf("F = XZ + X'YZ + X'Z' \n");
            }
            else if(Map[6] == 1){
                printf("m6: XYZ' \n");
                printf("F = XZ + XYZ' + X'Z' \n");
            }
            else if(Map[4] == 1){
                printf("m4: XY'Z' \n");
                printf("F = XZ + XY'Z' + X'Z' \n");
            }
            else if(Map[1] == 1){
                printf("m1: X'Y'Z \n");
                printf("F = XZ + X'Y'Z + X'Z' \n");
            }
            else{
                printf("F = XZ + X'Z' \n");
            }
        }
        else if((Map[0] == 1 || Map[0] == 2) && (Map[1] == 1 || Map[1] == 2)){
            printf("m0, m1: X'Y' \n");
            
            if(Map[3] == 1){
                printf("m3: X'YZ \n");
                printf("F = XZ + X'YZ + X'Y' \n");
            }
            else if(Map[6] == 1){
                printf("m6: XYZ' \n");
                printf("F = XZ + XYZ' + X'Y' \n");
            }
            else if(Map[4] == 1){
                printf("m4: XY'Z' \n");
                printf("F = XZ + XY'Z' + X'Y' \n");
            }
            else if(Map[2] == 1){
                printf("m2: X'YZ' \n");
                printf("F = XZ + X'YZ' + X'Y' \n");
            }
            else{
                printf("F = XZ + X'Y' \n");
            }
        }
        else if((Map[2] == 1 || Map[2] == 2) && (Map[3] == 1 || Map[3] == 2)){
            printf("m2, m3: X'Y \n");
            
            if(Map[6] == 1){
                printf("m6: XYZ' \n");
                printf("F = XZ + XYZ' + X'Y \n");
            }
            else if(Map[4] == 1){
                printf("m4: XY'Z' \n");
                printf("F = XZ + XY'Z' + X'Y \n");
            }
            else if(Map[1] == 1){
                printf("m1: X'Y'Z \n");
                printf("F = XZ + X'Y'Z + X'Y \n");
            }
            else if(Map[0] == 1){
                printf("m0: X'Y'Z' \n");
                printf("F = XZ + X'Y'Z' + X'Y \n");
            }
            else{
                printf("F = XZ + X'Y \n");
            }
        }
        else if((Map[2] == 1 || Map[2] == 2) && (Map[6] == 1 || Map[6] == 2)){
            printf("m2, m6: YZ' \n");
            printf("F = XZ + YZ' \n");
        }
        else if(Map[2] == 1){
            printf("m2: X'YZ' \n");
            printf("F = XZ + X'YZ' \n");
        }
        else if(Map[6] == 1){
            printf("m6: XYZ' \n");
            printf("F = XZ + XYZ' \n");
        }
        else if(Map[0] == 1){
            printf("m0: X'Y'Z' \n");
            printf("F = XZ + X'Y'Z' \n");
        }
        else{
            printf("F = XZ \n");
        }
    }
    else if((Map[7] == 1 || Map[7] == 2) && (Map[6] == 1 || Map[6] == 2)){
        printf("m7, m6: XY \n");

        if((Map[0] == 1 || Map[0] == 2) && (Map[2] == 1 || Map[2] == 2)){
            printf("m0, m2: X'Z' \n");
            
            if(Map[4] == 1){
                printf("m4, m6: XZ' \n");
                printf("F = XY + XZ' + X'Z' \n");
            }
            else if(Map[5] == 1){
                printf("m5, m7: XZ \n");
                printf("F = XY + XZ + X'Z' \n");
            }
            else if(Map[1] == 1){
                printf("m1: X'Y'Z' \n");
                printf("F = XY + X'Y'Z' + X'Z' \n");
            }
            else if(Map[3] == 1){
                printf("m3: X'YZ \n");
                printf("F = XY + X'YZ + X'Z' \n");
            }
            else{
                printf("F = XY + X'Z' \n");
            }
        }
        else if((Map[3] == 1 || Map[3] == 2) && (Map[1] == 1 || Map[1] == 2)){
            printf("m3, m1: X'Z \n");
            
            if(Map[4] == 1){
                printf("m4, m6: XZ' \n");
                printf("F = XY + XZ' + X'Z \n");
            }
            else if(Map[5] == 1){
                printf("m5, m7: XZ \n");
                printf("F = XY + XZ + X'Z \n");
            }
            else if(Map[2] == 1){
                printf("m2: X'YZ' \n");
                printf("F = XY + X'YZ' + X'Z \n");
            }
            else if(Map[0] == 1){
                printf("m0: X'Y'Z' \n");
                printf("F = XY + X'Y'Z' + X'Z \n");
            }
            else{
                printf("F = XY + X'Z \n");
            }
        }
        else if((Map[1] == 1 || Map[1] == 2) && (Map[5] == 1 || Map[5] == 2)){
            printf("m1, m5: Y'Z \n");
            
            if(Map[4] == 1){
                printf("m4, m6: XZ' \n");
                printf("F = XY + XZ' + Y'Z \n");
            }
            else if(Map[0] == 1){
                printf("m0: X'Y'Z' \n");
                printf("F = XY + X'Y'Z' + Y'Z \n");
            }
            else if(Map[3] == 1){
                printf("m3: X'YZ \n");
                printf("F = XY + X'YZ + Y'Z \n");
            }
            else if(Map[2] == 1){
                printf("m2: X'YZ' \n");
                printf("F = XY + X'YZ' + Y'Z \n");
            }
            else{
                printf("F = XY + Y'Z \n");
            }
        }
        else if((Map[0] == 1 || Map[0] == 2) && (Map[4] == 1 || Map[4] == 2)){
            printf("m0, m4: Y'Z' \n");
            
            if(Map[3] == 1){
                printf("m3: X'YZ \n");
                printf("F = XY + X'YZ + Y'Z' \n");
            }
            else if(Map[2] == 1){
                printf("m2: X'YZ' \n");
                printf("F = XY + X'YZ' + Y'Z' \n");
            }
            else if(Map[5] == 1){
                printf("m5, m7: XZ \n");
                printf("F = XY + XZ + Y'Z' \n");
            }
            else if(Map[1] == 1){
                printf("m1: X'Y'Z' \n");
                printf("F = XY + X'Y'Z' + Y'Z' \n");
            }
            else{
                printf("F = XY + Y'Z' \n");
            }
        }
        else if(Map[4] == 1){
            printf("m4, m6: XZ' \n");
            if(Map[1] == 1){
                printf("m1: X'Y'Z' \n");
                printf("F = XY + XZ' + X'Y'Z' \n");
            }
            else{
                printf("F = XY + XZ' \n");
            }
        }
        else if(Map[2] == 1){
            printf("m2, m6: YZ' \n");
            printf("F = XY + YZ' \n");
        }
        else if(Map[5] == 1){
            printf("m5, m7: XZ \n");
            printf("F = XY + XZ \n");
        }
        else if(Map[1] == 1){
            printf("m1: X'Y'Z' \n");
            printf("F = XY + X'Y'Z' \n");
        }
        else if(Map[0] == 1){
            printf("m0: X'Y'Z' \n");
            printf("F = XY + X'Y'Z' \n");
        }
        else{
            printf("F = XY \n");
        }
    }
    else if((Map[3] == 1 || Map[3] == 2) && (Map[7] == 1 || Map[7] == 2)){

        printf("m3, m7: YZ \n");

        if(Map[0] == 1){
            printf("m0: X'Y'Z' \n");
            printf("F = YZ + X'Y'Z' \n");
        }
        else if(Map[4] == 1){
            printf("m4: XY'Z' \n");
            printf("F = YZ + XY'Z' \n");
        }
        else if(Map[1] == 1){
            printf("m1, m3: X'Z \n");
            printf("F = YZ + X'Z \n");
        }
        else if(Map[5] == 1){
            printf("m5, m7: XZ \n");
            printf("F = YZ + XZ \n");
        }
        else if(Map[2] == 1){
            printf("m2, m3: X'Y \n");
            printf("F = YZ + X'Y \n");
        }
        else if(Map[6] == 1){
            printf("m6, m7: XY \n");
            printf("F = YZ + XY \n");
        }
        else{
            printf("F = YZ \n");
        }
    }
    else if((Map[2] == 1 || Map[2] == 2) && (Map[6] == 1 || Map[6] == 2)){

        printf("m2, m6: YZ' \n");

        if(Map[1] == 1){
            printf("m1: X'Y'Z \n");
            printf("F = YZ' + X'Y'Z \n");
        }
        else if(Map[5] == 1){
            printf("m5: XY'Z \n");
            printf("F = YZ' + XY'Z \n");
        }
        else if(Map[0] == 1){
            printf("m0, m2: X'Z' \n");
            printf("F = YZ' + X'Z' \n");
        }
        else if(Map[4] == 1){
            printf("m4, m6: XZ' \n");
            printf("F = YZ' + XZ' \n");
        }
        else if(Map[3] == 1){
            printf("m2, m3: X'Y \n");
            printf("F = YZ' + X'Y \n");
        }
        else if(Map[7] == 1){
            printf("m6, m7: XY \n");
            printf("F = YZ' + XY \n");
        }
        else{
            printf("F = YZ' \n");
        }
    }
    
    else if(Map[0] == 1){

        printf("group m0: X'Y'Z' \n");
        if(Map[3] == 1){

            printf("group m3: X'YZ \n");
            if(Map[5] == 1){
                printf("group m5: XY'Z \n");

                if(Map[6] == 1){
                    printf("group m6: XYZ' \n");
                    printf("F = X'Y'Z' + X'YZ + XY'Z + XY'Z \n");
                }
                else{
                    printf("F = X'Y'Z' + X'YZ + XY'Z \n");
                }
            }
            else if(Map[6] == 1){
                printf("group m6: XYZ' \n");
                printf("F = X'Y'Z' + X'YZ + XYZ' \n");
            }
            else{
                printf("F = X'Y'Z' + X'YZ \n");
            }
        }
        else if(Map[5] == 1){
            printf("group m5: XY'Z \n");

            if(Map[6] == 1){
                printf("group m6: XYZ' \n");
                printf("F = X'Y'Z' + XY'Z + XYZ' \n");
            }
            else{
                printf("F = X'Y'Z' + XY'Z \n");
            }
        }
        else if(Map[7] == 1){
            printf("group m7: XYZ \n");
            printf("F = X'Y'Z' + XYZ \n");
        }
        else if(Map[6] == 1){
            printf("group m6: XYZ' \n");
            printf("F = X'Y'Z' + XYZ' \n");
        }
        else{
            printf("F = X'Y'Z' \n");
        }
    }
    else if(Map[1] == 1){
        printf("group m1: X'Y'Z \n");

        if(Map[2] == 1){
            printf("group m2: X'YZ' \n");

            if(Map[4] == 1){
                printf("group m4: XY'Z' \n");

                if(Map[7] == 1){
                    printf("group m7: XYZ \n");
                    printf("F = X'Y'Z + X'YZ' + XY'Z' + XYZ\n");
                }
                else{
                    printf("F = X'Y'Z + X'YZ' + XY'Z' \n");
                }
            }
            else if(Map[7] == 1){
                printf("group m7: XYZ \n");
                printf("F = X'Y'Z + X'YZ' + XYZ \n");
            }
            else{
                printf("F = X'Y'Z + X'YZ' \n");
            }
        }
        else if(Map[4] == 1){
            printf("group m4: XY'Z' \n");
            if(Map[7] == 1){
                printf("group m7: XYZ \n");
                printf(" F = X'Y'Z + XY'Z' + XYZ \n");
            }
            else{
                printf("F = X'Y'Z + XY'Z' \n");
            }
        }
        else if(Map[6] == 1){
            printf("group m6: XYZ' \n");
            printf("F = X'Y'Z + XYZ' \n");
        }
        else if(Map[7] == 1){
            printf("group m7: XYZ \n");
            printf("F = X'Y'Z + XYZ \n");
        }
        else{
            printf("F = X'Y'Z \n");
        }
    }
    else if(Map[2] == 1){
        printf("group m2: X'YZ' \n");

        if(Map[4] == 1){
            printf("group m4: XY'Z' \n");

            if(Map[7] == 1){
                printf("group m7: XYZ \n");
                printf("F = X'YZ' + XY'Z' + XYZ \n");
            }
            else{
                printf("F = X'YZ' + XY'Z' \n");
            }
        }
        else if(Map[5] == 1){
            printf("group m5: XY'Z \n");
            printf("F = X'YZ' + XY'Z \n");
        }
        else if(Map[7] == 1){
            printf("group m7: XYZ \n");
            printf("F = X'YZ' + XYZ \n");
        }
        else{
            printf("F = X'YZ' \n");
        }
    }
    else if(Map[3] == 1){
        printf("group m3: X'YZ \n");

        if(Map[4] == 1){
            printf("group m4: XY'Z' \n");
            printf("F = X'YZ + XY'Z' \n");
        }
        else if(Map[5] == 1){
            printf("group m5: XY'Z \n");
            
            if(Map[6] == 1){
                printf("group m6: XYZ' \n");
                printf("F = X'YZ + XY'Z + XYZ' \n");
            }
            else{
                printf("F = X'YZ + XY'Z \n");
            }
        }
        else if(Map[6] == 1){
            printf("group m6: XYZ' \n");
            printf("F = X'YZ + XYZ' \n");
        }
        else{
            printf("F = X'YZ \n");
        }
    }
    else if(Map[4] == 1){
        printf("group m4: XY'Z' \n");

        if(Map[7] == 1){
            printf("group m7: XYZ \n");
            printf("F = XYZ + XY'Z' \n");
        }
        else{
            printf("F = XY'Z' \n");
        }
    }
    else if(Map[5] == 1){
        printf("group m5: XY'Z \n");

        if(Map[6] == 1){
            printf("group m6: XYZ' \n");
            printf("F = XYZ' + XY'Z \n");
        }
        else{
            printf("F = XY'Z \n");
        }
    }
    else if(Map[6] == 1){
        printf("group m6: XYZ' \n");
        printf("F = XYZ' \n");
    }
    else if(Map[7] == 1){
        printf("group m7: XYZ \n");
        printf("F = XYZ \n");
    }
}

void print_table(int Q_binary[16][4], int length, int min[16]){

    printf("\t\tW\tX\tY\tZ\n");
    printf("---------------------------------------------\n");
    for(int i = 0; i <= length; i++){
        printf("%d\t|\t%d\t%d\t%d\t%d\n", min[i], Q_binary[i][3], Q_binary[i][2], Q_binary[i][1], Q_binary[i][0]);
    }
}

void convert_binary(int min[16], int length, int Q_binary[16][4]){

    int pow_num;

    for(int j = 0; j <= length; j++){

        for(int i = 3; i >= 0; i--){

            pow_num = pow(2,i); // 2^i
            if((min[j] / pow_num) % 2 == 1){ // check if the division result has modulus by 2 equal 1

                Q_binary[j][i] = 1; // if it does then the digit is 1
            }
            else{
                Q_binary[j][i] = 0; // else its 0
            }
        }
    }
}

int quine_mccluskey(int Q_binary[16][4], int length, int min[16], int match4[16][4], int teams4[16][4]) {

    int counter = 0, spot, index, flag = 0;
    int groups = -1 , groups4 = -1, mat = -1;
    int teams[16][4]; // teams of 2
    int match[16][2]; // minimums in group of 2
    // teams4[16][4] ->  teams of 4

    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 4; j++){
            teams4[i][j] = -1;  // initialize with -1 cause we have 0 as a bit
        }
    }
    
    printf("\n------------------------------------------\n"); // teams of 2
    printf("\n\tW\tX\tY\tZ\n");

    for(int i = length - 1; i >= 0; i--){ // for all the numbers
        for(int k = length - 1; k >= i; k--){ // for all the numbers after i
            counter = 0; // counter for different bits
            for(int j = 3; j >= 0; j--){ // for all columns
                if(Q_binary[i][j] != Q_binary[k+1][j]){ // if there is a differ bit
                    counter++; // increase counter
                    spot = j; // store the column
                }
            }
            if(counter == 1){ // if there is 1 bit differ
                groups++; // we have a group
                match[groups][0] = min[i];
                match[groups][1] = min[k+1];
                printf("\n%d,%d\t", min[i], min[k+1]); // print the 2 numbers of the team

                for(int j = 3; j >= 0; j--){ // for all the columns
                    if(j != spot){ // if the bit isn;t the same as the differ bit
                        printf("%d\t" , Q_binary[i][j]); // print the bit
                        teams[groups][j] = Q_binary[i][j]; // store the team
                    }
                    else{
                        printf("-\t"); // else print - cause we have find the different bit
                        teams[groups][j] = 2;  // "-" = 2 store
                    }
                }
                printf("\n");
            }
        }
    }
    

    printf("\n------------------------------------------\n"); // teams of 4
    printf("\n\t\tW\tX\tY\tZ\n");

    for(int i = 0; i < groups; i++){ // for all the groups of 2
        for(int k = i; k < groups; k++){ // for all the groups of 2 after i row

            counter = 0;
            index = -1;
            flag = 0;

            for(int j = 3; j >= 0; j--){ // for all columns
                if((teams[i][j] == 2) && (teams[k+1][j] == 2)){ // if you find "-" on the same spot in teams
                    for(int l = 3; l >= 0; l--){ // for all the columns
                        if(teams[i][l] != teams[k+1][l]){ // if you find differ bit
                            counter++; // increase counter
                            spot = l; // store column of differ bit
                        }
                    }
                }
            }
            
            if(counter == 1){ // if there is only one bit difference 
                groups4++; // we have a group of 4
                index = k+1; // team with match "-" and 1 bit difference
                teams4[groups4][0] = match[i][0]; // store the 4 numbers of the team
                teams4[groups4][1] = match[i][1];
                teams4[groups4][2] = match[index][0];
                teams4[groups4][3] = match[index][1];
            
                for (int l = 0; l < groups4; l++) { // for all the teams of 4

                    flag = equal_teams(teams4[l], teams4[groups4], 4); // check if the team is printed before
                    if (flag == 1) { // if the team has already been printed
                        break; // stop the loop 
                    }
                }

                if(flag == 0){ // if the team isn't printed before
                    mat++;
                    printf("%d,%d,%d,%d  \t", teams4[groups4][0] , teams4[groups4][1], teams4[groups4][2], teams4[groups4][3]);

                    for(int j = 3; j >= 0; j--){ // for all the columns
                        if(j != spot){ // if the column isn't the same as the column differ bit is at
                            if(teams[i][j] == 2){ // if its 2 then is the "-"
                                printf("-\t");
                                match4[mat][j] = 2;
                            }
                            else{
                                printf("%d\t" , teams[i][j]);
                                match4[mat][j] = teams[i][j];
                            }
                        }
                        else{ // its the new different bit print "-"
                            printf("-\t");
                            match4[mat][j] = 2;
                        }
                    }
                    printf("\n\n");
                }
            }
        }
    }

    return mat;
}

int equal_teams(int row1[], int row2[], int num_cols) {

    int A[17] = {0}; // assuming values in the matrix are between 0 and 16 cause we have 4 variables 

    for (int i = 0; i < num_cols; i++) {
        A[row1[i]]++;
        A[row2[i]]--;
    }
    for (int i = 0; i < 17; i++) {
        if (A[i] != 0) {
            return 0;
        }
    }
    return 1;
}

void synthesis(int min[16], int match4[16][4], int l_teams, int teams4[16][4], int index_kar){

    // match4  ->  binary
    // teams4  ->  decimal

    char vars[l_teams][4][3]; // rows,columns,size of string "XY\0"
    int one_time[16] = {0};
    int essential[16] = {0};
    int printed_row[l_teams+1];
    int index = 0;
    int printed, element, lines[l_teams];

    printf("----------------------------------------------------\n");

    for(int i = l_teams; i >= 0; i--){ // check which variable is going to be in the last array (x,y,z,w)
        printf("%d,%d,%d,%d \t", teams4[i][0] , teams4[i][1], teams4[i][2], teams4[i][3]);

        strcpy(vars[i][0], "");

        // FOR W
        if(match4[i][3] == 1){ // 1 : normal
            printf(" W ");
            strcat(vars[i][0], "W");  // store them
        }
        else if(match4[i][3] == 0){ // 0 : supplementary
            printf(" W' ");
            strcat(vars[i][0], "W'");
        }
        // not else cause in case match4[i][0] == 2 we have "-" so the variable doesn't participate

        strcpy(vars[i][1], "");

        // FOR X
        if(match4[i][2] == 1){ // 1 : normal
            printf(" X ");
            strcat(vars[i][1], "X");
        }
        else if(match4[i][2] == 0){ // 0 : supplementary
            printf(" X' ");
            strcat(vars[i][1], "X'");
        }

        strcpy(vars[i][2], "");

        // FOR Y
        if(match4[i][1] == 1){ // 1 : normal
            printf(" Y ");
            strcat(vars[i][2], "Y");
        }
        else if(match4[i][1] == 0){ // 0 : supplementary
            printf(" Y' ");
            strcat(vars[i][2], "Y'");
        }

        strcpy(vars[i][3], "");

        // FOR Z
        if(match4[i][0] == 1){ // 1 : normal
            printf(" Z ");
            strcat(vars[i][3], "Z");
        }
        else if(match4[i][0] == 0){ // 0 : supplementary
            printf(" Z' ");
            strcat(vars[i][3], "Z'");
        }

        printf("\n\n");
    }
    
    for(int i = 0; i <= l_teams; i++){ // in all the teams check which element exist only one time 
        for(int j = 0; j < 4; j++){ // 4 numbers
            one_time[teams4[i][j]]++;
        }
    }

    printf("Essential prime implicants are: ");
    for(int i = 0; i < 16; i++){ // max 16 minimums
        if(one_time[i] == 1){ // we find it 1 time 
            essential[index] = i;
            index++;
        }
    }

    for(int i = 0; i <= l_teams; i++){
        printed_row[i] = 0; // used it as a flag for each row
    }

    for(int k = 0; k < index; k++){
        element = essential[k];
        for(int i = 0; i <= l_teams; i++){
            
            printed = printed_row[i];
            for(int j = 0; j < 4; j++){ // teams of 4 = columns
                if(element == teams4[i][j] && !printed){ // if you find the row and it isn't already printed

                    lines[i] = i;
                    for(int l = 0; l < 4; l++){ // columns to print the whole row
                        printf("%d ", teams4[i][l]);
                    }
                    printed_row[i] = 1; // row is printed
                    break;
                }
            }
        }
    }

    printf("\nPrime implicants are: ");
    for(int i = 0; i <= l_teams; i++){
        if(lines[i] != i){
            lines[i] = i;
            for(int j = 0; j < 4; j++){
                printf("%d ", teams4[i][j]);
            }
            break;
        }
    }

    int flag = 1;
    printf("\n\nF(W,X,Y,Z) = ");

    for(int i = 0; i <= l_teams; i++){
        if(lines[i] != i){
            continue; // skip the lines we dont need
        }
        if(!flag){
            printf(" + ");
        }
        for(int j = 0; j < 4; j++){
            printf("%s", vars[i][j]);
        }
        flag = 0;
    }

    // i tied to print them in a table but i didnt make it
    // printf("----------------------------------------------------\n");
    // printf(" \t ");
    // for(int i = 0; i <= index_kar; i++){
    //     printf("%d \t", min[i]);
    // }
    // printf("\n");

    // for (int i = l_teams; i >= 0; i--) { 

    //     for (int k = 0; k < 4; k++) {
    //         printf("%s", vars[i][k]);
    //     }
        
    //     printf("\t  ");
    //     for(int j = 0; j <= index_kar; j++){

    //         for (int k = 0; k < 4; k++) {
    //             if(teams4[i][k] == j){
    //                 printx = 1;
    //                 break;
    //             }
    //         }

    //         if(printx){
    //             printf("X\t");
    //         }
    //         else{
    //             printf("\t");
    //         }
    //     }
    //     printf("\n");
    // }

}

void group4(int Map[16]){

    // check all the cases that makes a team 

    if( (Map[0] == 1 || Map[0] == 2) && (Map[1] == 1 || Map[1] == 2) && (Map[2] == 1 || Map[2] == 2) && 
        (Map[3] == 1 || Map[3] == 2) && (Map[4] == 1 || Map[4] == 2) && (Map[5] == 1 || Map[5] == 2) && 
        (Map[6] == 1 || Map[6] == 2) && (Map[7] == 1 || Map[7] == 2) && (Map[8] == 1 || Map[8] == 2) && 
        (Map[9] == 1 || Map[9] == 2) && (Map[10] == 1 || Map[10] == 2) && (Map[11] == 1 || Map[11] == 2) && 
        (Map[12] == 1 || Map[12] == 2) && (Map[13] == 1 || Map[13] == 2) && (Map[14] == 1 || Map[14] == 2) && (Map[15] == 1 || Map[15] == 2)){
        printf("group m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15:    F = 1 \n");
    }
    // GROUPS OF 8
    else if((Map[0] == 1 || Map[0] == 2) && (Map[1] == 1 || Map[1] == 2) && (Map[2] == 1 || Map[2] == 2) && (Map[3] == 1 || Map[3] == 2) && (Map[4] == 1 || Map[4] == 2) && (Map[5] == 1 || Map[5] == 2) && (Map[6] == 1 || Map[6] == 2) && (Map[7] == 1 || Map[7] == 2)){
        
        printf("group m0, m1, m2, m3, m4, m5, m6, m7: W' \n");

        if((Map[8] == 1 || Map[8] == 2) && (Map[9] == 1 || Map[9] == 2) && (Map[12] == 1 || Map[12] == 2) && (Map[13] == 1 || Map[13] == 2)){
            printf("group m8, m9, m12, m13: WY' \n");

            if(Map[15] == 1 && (Map[14] == 2 ||  Map[14] == 1)){
                printf("group m12, m13, m14, m15: WX \n");

                if(Map[10] == 1){
                    printf("group m10, m8: WX'Z' \n");
                    printf("F = W' + WY' + WX + WX'Z' \n");
                }
                else if(Map[11] == 1){
                    printf("group m11, m15: WYZ \n");
                    printf("F = W' + WY' + WX + WYZ \n");
                }
                else{
                    printf("F = W' + WY' + WX \n");
                }
            }
            else if(Map[14] == 1 && (Map[15] == 2 ||  Map[15] == 1)){
                printf("group m12, m13, m14, m15: WX \n");

                if(Map[10] == 1){
                    printf("group m10, m8: WX'Z' \n");
                    printf("F = W' + WY' + WX + WX'Z' \n");
                }
                else if(Map[11] == 1){
                    printf("group m11, m15: WYZ \n");
                    printf("F = W' + WY' + WX + WYZ \n");
                }
                else{
                    printf("F = W' + WY' + WX \n");
                }
            }
            else if(Map[10] == 1 && (Map[11] == 2 ||  Map[11] == 1)){
                printf("group m10, m11, m8, m9: WX' \n");
                
                if(Map[14] == 1){
                    printf("group m14, m12: WXZ' \n");
                    printf("F = W' + WY' + WX' + WXZ' \n");
                }
                else if(Map[15] == 1){
                    printf("group m11, m15: WYZ \n");
                    printf("F = W' + WY' + WX' + WYZ \n");
                }
                else{
                    printf("F = W' + WY' + WX' \n");
                }
            }
             else if(Map[11] == 1 && (Map[10] == 2 ||  Map[10] == 1)){
                printf("group m10, m11, m8, m9: WX' \n");
                
                if(Map[14] == 1){
                    printf("group m14, m12: WXZ' \n");
                    printf("F = W' + WY' + WX' + WXZ' \n");
                }
                else if(Map[15] == 1){
                    printf("group m11, m15: WYZ \n");
                    printf("F = W' + WY' + WX' + WYZ \n");
                }
                else{
                    printf("F = W' + WY' + WX' \n");
                }
            }
            else if(Map[15] == 1 && Map[10] == 1){
                printf("group m15, m13: WXZ \n");
                printf("group m10, m8: WX'Z' \n");
                printf("F = W' + WY' + WXZ + WX'Z' \n");
            }
            else if(Map[15] == 1){
                printf("group m15, m13: WXZ \n");
                printf("F = W' + WY' + WXZ \n");
            }
            else if(Map[14] == 1){
                printf("group m14, m12: WXZ' \n");
                printf("F = W' + WY' + WXZ' \n");
            }
            else if(Map[10] == 1){
                printf("group m10, m8: WX'Z' \n");
                printf("F = W' + WY' + WX'Z' \n");
            }
            else if(Map[11] == 1){
                printf("group m11, m9: WX'Z \n");
                printf("F = W' + WY' + WX'Z \n");
            }
        }
        else if((Map[9] == 1 || Map[9] == 2) && (Map[13] == 1 || Map[13] == 2) && (Map[11] == 1 || Map[11] == 2) && (Map[15] == 1 || Map[15] == 2)){
            printf("group m11, m9, m15, m13: WY' \n");

            if(Map[12] == 1){
                if((Map[8] == 1 || Map[8] == 2)){
                    printf("group m12, m8: WY'Z' \n");
                    printf("F = W' + WY' + WY'Z' \n");
                }
                else if((Map[10] == 1 || Map[10] == 2)){
                    printf("group m12, m13: WXY' \n");
                    printf("group m10, m11: WX'Y \n");
                    printf("F = W' + WY' + WXY' + WX'Y \n");
                }
                else if((Map[14] == 1 || Map[14] == 2)){
                    printf("group m12, m14: WXZ' \n");
                    printf("F = W' + WY' + WXZ' \n");
                }
                else{
                    printf("group m12, m13: WXY' \n");
                    printf("F = W' + WY' + WXY' \n");
                }
            }
            else if(Map[8] == 1){
                if((Map[12] == 1 || Map[12] == 2)){
                    printf("group m12, m8: WY'Z' \n");
                    printf("F = W' + WY' + WY'Z' \n");
                }
                else if((Map[10] == 1 || Map[10] == 2)){
                    printf("group m8, m10: WX'Z' \n");
                    printf("F = W' + WY' + WX'Z' \n");
                }
                else if((Map[14] == 1 || Map[14] == 2)){
                    printf("group m15, m14: WXY \n");
                    printf("group m8, m9: WX'Y' \n");
                    printf("F = W' + WY' + WXY + WX'Y' \n");
                }
                else{
                    printf("group m8, m9: WX'Y' \n");
                    printf("F = W' + WY' + WX'Y' \n");
                }
            }
            else if(Map[10] == 1){
                if((Map[12] == 1 || Map[12] == 2)){
                    printf("group m12, m13: WXY' \n");
                    printf("group m10, m11: WX'Y \n");
                    printf("F = W' + WY' + WXY' + WX'Y \n");
                }
                else if((Map[8] == 1 || Map[8] == 2)){
                    printf("group m8, m10: WX'Z' \n");
                    printf("F = W' + WY' + WX'Z' \n");
                }
                else if((Map[14] == 1 || Map[14] == 2)){
                    printf("group m10, m14: WYZ' \n");
                    printf("F = W' + WY' + WYZ' \n");
                }
                else{
                    printf("group m10, m11: WX'Y \n");
                    printf("F = W' + WY' + WX'Y \n");
                }
            }
            else{
                printf("F = W' + WY' \n");
            }
        }
        else if((Map[10] == 1 || Map[10] == 2) && (Map[11] == 1 || Map[11] == 2) && (Map[14] == 1 || Map[14] == 2) && (Map[15] == 1 || Map[15] == 2)){
            
        }
        else if((Map[8] == 1 || Map[8] == 2) && (Map[12] == 1 || Map[12] == 2) && (Map[10] == 1 || Map[10] == 2) && (Map[14] == 1 || Map[14] == 2)){

        }
        else if((Map[12] == 1 || Map[12] == 2) && (Map[13] == 1 || Map[13] == 2) && (Map[14] == 1 || Map[14] == 2) && (Map[15] == 1 || Map[15] == 2)){

        }
        else if((Map[8] == 1 || Map[8] == 2) && (Map[9] == 1 || Map[9] == 2) && (Map[10] == 1 || Map[10] == 2) && (Map[11] == 1 || Map[11] == 2)){

        }
        else{
            printf("F = W' \n");
        }
    }
    else if((Map[8] == 1 || Map[8] == 2) && (Map[9] == 1 || Map[9] == 2) && (Map[10] == 1 || Map[10] == 2) && (Map[11] == 1 || Map[11] == 2) && (Map[12] == 1 || Map[12] == 2) && (Map[13] == 1 || Map[13] == 2) && (Map[14] == 1 || Map[14] == 2) && (Map[15] == 1 || Map[15] == 2)){
        printf("group m8, m9, m10, m11, m12, m13, m14, m15: W \n");

        if ((Map[0] == 1 || Map[0] == 2) && (Map[1] == 1 || Map[1] == 2) && (Map[4] == 1 || Map[4] == 2) && (Map[5] == 1 || Map[5] == 2)){
            printf("group m0, m1, m4, m5: W'Y' \n");

            if((Map[2] == 1 && (Map[3] == 1 || Map[3] == 2)) || (Map[3] == 1 && (Map[2] == 1 || Map[2] == 2))){
                printf("group m2, m3: W'X'Y \n");
                printf("F = W + W'Y' + W'X'Y \n");
            }
            else if((Map[2] == 1 && (Map[6] == 1 || Map[6] == 2)) || (Map[6] == 1 && (Map[2] == 1 || Map[2] == 2))){
                printf("group m2, m6: W'YZ' \n");
                printf("F = W + W'Y' + W'YZ' \n");
            }
            else if(Map[2] == 1 && Map[7] == 1){
                printf("group m2, m0: W'X'Z' \n");
                printf("group m7, m5: W'XZ \n");
                printf("F = W + W'Y' + W'X'Z' + W'XZ \n");
            }
            else if((Map[3] == 1 && (Map[7] == 1 || Map[7] == 2)) || (Map[7] == 1 && (Map[3] == 1 || Map[3] == 2))){
                printf("group m3, m7: W'YZ \n");
                printf("F = W + W'Y' + W'YZ \n");
            }
            else if((Map[7] == 1 && (Map[6] == 1 || Map[6] == 2)) || (Map[6] == 1 && (Map[7] == 1 || Map[7] == 2))){
                printf("group m7, m6: W'XY \n");
                printf("F = W + W'Y' + W'XY \n");
            }
            if(Map[3] == 1 && Map[6] == 1){
                printf("group m3, m1: W'X'Z \n");
                printf("group m6, m4: W'XZ' \n");
                printf("F = W + W'Y' + W'X'Z + W'XZ' \n");
            }
            else if(Map[3] == 1){
                printf("group m3, m1: W'X'Z \n");
                printf("F = W + W'Y' + W'X'Z \n");
            }
            else if(Map[2] == 1){
                printf("group m2, m0: W'X'Z' \n");
                printf("F = W + W'Y' + W'X'Z' \n");
            }
            else if(Map[6] == 1){
                printf("group m6, m4: W'XZ' \n");
                printf("F = W + W'Y' + W'XZ' \n");
            }
            else if(Map[7] == 1){
                printf("group m7, m5: W'XZ \n");
                printf("F = W + W'Y' + W'XZ \n");
            }
            else{
                printf("F = W + W'Y' \n");
            }
        }
        else if ((Map[0] == 1 || Map[0] == 2) && (Map[1] == 1 || Map[1] == 2) && (Map[4] == 1 || Map[4] == 2) && (Map[5] == 1 || Map[5] == 2)){
            printf("group m3, m1, m7, m5: W'Z \n");

            if ((Map[0] == 1 || Map[0] == 2) && (Map[4] == 1 || Map[4] == 2)) {
                printf("group m0, m4: W'Y'Z' \n");
                printf("F = W + W'Z + W'Y'Z' \n");
            }
            else if ((Map[0] == 1 || Map[0] == 2) && (Map[2] == 1 || Map[2] == 2)) {
                printf("group m0, m2: W'X'Z' \n");
                printf("F = W + W'Z + W'X'Z' \n");
            }
            else if ((Map[0] == 1 || Map[0] == 2) && (Map[6] == 1 || Map[6] == 2)) {
                printf("group m0, m1: W'X'Y' \n");
                printf("group m6, m7: W'XY \n");
                printf("F = W + W'Z + W'X'Y' + W'XY \n");
            }
            else if ((Map[2] == 1 || Map[2] == 2) && (Map[4] == 1 || Map[4] == 2)) {
                printf("group m2, m3: W'X'Y \n");
                printf("group m4, m5: W'XY' \n");
                printf("F = W + W'Z + W'X'Y + W'XY' \n");
            }
            else if ((Map[2] == 1 || Map[2] == 2) && (Map[6] == 1 || Map[6] == 2)) {
                printf("group m2, m6: W'YZ' \n");
                printf("F = W + W'Z + W'YZ' \n");
            }
            else if ((Map[6] == 1 || Map[6] == 2) && (Map[4] == 1 || Map[4] == 2)) {
                printf("group m6, m4: W'XZ' \n");
                printf("F = W + W'Z + W'XZ' \n");
            }
            else if (Map[0] == 1 || Map[0] == 2) {
                printf("group m0, m1: W'X'Y' \n");
                printf("F = W + W'Z + W'X'Y' \n");
            }
            else if (Map[4] == 1 || Map[4] == 2) {
                printf("group m4, m5: W'XY' \n");
                printf("F = W + W'Z + W'XY' \n");
            }
            else if (Map[2] == 1 || Map[2] == 2) {
                printf("group m2, m3: W'X'Y \n");
                printf("F = W + W'Z + W'X'Y \n");
            }
            else if (Map[6] == 1 || Map[6] == 2) {
                printf("group m6, m7: W'XY \n");
                printf("F = W + W'Z + W'XY \n");
            }
            else{
                printf("F = W + W'Z \n");
            }
        }
        else if ((Map[2] == 1 || Map[2] == 2) && (Map[3] == 1 || Map[3] == 2) && (Map[6] == 1 || Map[6] == 2) && (Map[7] == 1 || Map[7] == 2)) {
            printf("group m2, m3, m6, m7: W'Y \n");

            if ((Map[0] == 1 || Map[0] == 2) && (Map[1] == 1 || Map[1] == 2)) {
                printf("group m1, m0: W'X'Y' \n");
                printf("F = W + W'Y + W'X'Y' \n");
            }
            else if ((Map[0] == 1 || Map[0] == 2) && (Map[4] == 1 || Map[4] == 2)) {
                printf("group m4, m0: W'Y'Z' \n");
                printf("F = W + W'Y + W'Y'Z' \n");
            }
            else if ((Map[5] == 1 || Map[5] == 2) && (Map[1] == 1 || Map[1] == 2)) {
                printf("group m5, m1: W'Y'Z' \n");
                printf("F = W + W'Y + W'Y'Z' \n");
            }
            else if ((Map[4] == 1 || Map[4] == 2) && (Map[5] == 1 || Map[5] == 2)) {
                printf("group m4, m5: W'XY' \n");
                printf("F = W + W'Y + W'XY' \n");
            }
            else if(Map[0] == 1){
                printf("group m0, m2: W'X'Z' \n");
                printf("F = W + W'Y + W'X'Z' \n");
            }
            else if(Map[1] == 1){
                printf("group m1, m3: W'X'Z \n");
                printf("F = W + W'Y + W'X'Z \n");
            }
            else if(Map[4] == 1){
                printf("group m4, m6: W'XZ' \n");
                printf("F = W + W'Y + W'XZ' \n");
            }
            else if(Map[5] == 1){
                printf("group m5, m7: W'XZ \n");
                printf("F = W + W'Y + W'XZ \n");
            }
            else{
                printf("F = W + W'Y \n");
            }
        }
    }
    else if((Map[0] == 1 || Map[0] == 2) && (Map[1] == 1 || Map[1] == 2) && (Map[4] == 1 || Map[4] == 2) && (Map[5] == 1 || Map[5] == 2) && (Map[8] == 1 || Map[8] == 2) && (Map[9] == 1 || Map[9] == 2) && (Map[12] == 1 || Map[12] == 2) && (Map[13] == 1 || Map[13] == 2)){
        printf("group m8, m9, m0, m1, m12, m13, m4, m5: F = Y' \n");
    }
    else if((Map[2] == 1 || Map[2] == 2) && (Map[3] == 1 || Map[3] == 2) && (Map[6] == 1 || Map[6] == 2) && (Map[7] == 1 || Map[7] == 2) && (Map[14] == 1 || Map[14] == 2) && (Map[15] == 1 || Map[15] == 2) && (Map[10] == 1 || Map[10] == 2) && (Map[11] == 1 || Map[11] == 2)){
        printf("group m2, m3, m6, m7, m14, m15, m10, m11: F = Y \n");
    }
    else if((Map[0] == 1 || Map[0] == 2) && (Map[2] == 1 || Map[2] == 2) && (Map[4] == 1 || Map[4] == 2) && (Map[6] == 1 || Map[6] == 2) && (Map[12] == 1 || Map[12] == 2) && (Map[14] == 1 || Map[14] == 2) && (Map[8] == 1 || Map[8] == 2) && (Map[10] == 1 || Map[10] == 2)){
        printf("group m0, m2, m6, m4, m12, m14, m10, m8: F = Z' \n");
    }
    else if((Map[1] == 1 || Map[1] == 2) && (Map[3] == 1 || Map[3] == 2) && (Map[5] == 1 || Map[5] == 2) && (Map[7] == 1 || Map[7] == 2) && (Map[13] == 1 || Map[13] == 2) && (Map[15] == 1 || Map[15] == 2) && (Map[9] == 1 || Map[9] == 2) && (Map[11] == 1 || Map[11] == 2)){
        printf("group m1, m3, m7, m5, m13, m15, m11, m9: F = Z \n");
    }
    else if((Map[0] == 1 || Map[0] == 2) && (Map[1] == 1 || Map[1] == 2) && (Map[2] == 1 || Map[2] == 2) && (Map[3] == 1 || Map[3] == 2) && (Map[8] == 1 || Map[8] == 2) && (Map[9] == 1 || Map[9] == 2) && (Map[10] == 1 || Map[10] == 2) && (Map[11] == 1 || Map[11] == 2)){
        printf("group m0, m1, m2, m3, m8, m9, m10, m11:  F = X' \n");
    }
    else if((Map[4] == 1 || Map[4] == 2) && (Map[5] == 1 || Map[5] == 2) && (Map[6] == 1 || Map[6] == 2) && (Map[7] == 1 || Map[7] == 2) && (Map[12] == 1 || Map[12] == 2) && (Map[13] == 1 || Map[13] == 2) && (Map[14] == 1 || Map[14] == 2) && (Map[15] == 1 || Map[15] == 2)){
        printf("group m4, m5, m6, m7, m12, m13, m14, m15:  F = X \n");
    }
    // GROUPS OF 4
    else if((Map[0] == 1 || Map[0] == 2) && (Map[1] == 1 || Map[1] == 2) && (Map[4] == 1 || Map[4] == 2) && (Map[5] == 1 || Map[5] == 2)){
        printf("group m0,m1,m4,m5: F = W'Y' \n");
    }
    else if((Map[0] == 1 || Map[0] == 2) && (Map[2] == 1 || Map[2] == 2) && (Map[4] == 1 || Map[4] == 2) && (Map[6] == 1 || Map[6] == 2)){
        printf("group m0,m2,m4,m6: F = W'Z' \n");
    }
    else if((Map[3] == 1 || Map[3] == 2) && (Map[1] == 1 || Map[1] == 2) && (Map[7] == 1 || Map[7] == 2) && (Map[5] == 1 || Map[5] == 2)){
        printf("group m3,m1,m7,m5: F = W'Z \n");
    }
    else if((Map[2] == 1 || Map[2] == 2) && (Map[3] == 1 || Map[3] == 2) && (Map[6] == 1 || Map[6] == 2) && (Map[7] == 1 || Map[7] == 2)){
        printf("group m2,m3,m6,m7: F = W'Y \n");
    }
    else if((Map[12] == 1 || Map[12] == 2) && (Map[13] == 1 || Map[13] == 2) && (Map[4] == 1 || Map[4] == 2) && (Map[5] == 1 || Map[5] == 2)){
        printf("group m13,m12,m4,m5: F = XY' \n");
    }
    else if((Map[12] == 1 || Map[12] == 2) && (Map[14] == 1 || Map[14] == 2) && (Map[6] == 1 || Map[6] == 2) && (Map[4] == 1 || Map[4] == 2)){
        printf("group m14,m12,m4,m6: F = XZ' \n");
    }
    else if((Map[13] == 1 || Map[13] == 2) && (Map[7] == 1 || Map[7] == 2) && (Map[15] == 1 || Map[15] == 2) && (Map[5] == 1 || Map[5] == 2)){
        printf("group m15,m13,m7,m5: F = XZ \n");
    }
    else if((Map[6] == 1 || Map[6] == 2) && (Map[7] == 1 || Map[7] == 2) && (Map[14] == 1 || Map[14] == 2) && (Map[15] == 1 || Map[15] == 2)){
        printf("group m7,m6,m14,m15: F = XY \n");
    }
    else if((Map[8] == 1 || Map[8] == 2) && (Map[9] == 1 || Map[9] == 2) && (Map[12] == 1 || Map[12] == 2) && (Map[13] == 1 || Map[13] == 2)){
        printf("group m8,m12,m9,m13: F = WY' \n");
    }
    else if((Map[8] == 1 || Map[8] == 2) && (Map[12] == 1 || Map[12] == 2) && (Map[10] == 1 || Map[10] == 2) && (Map[14] == 1 || Map[14] == 2)){
        printf("group m8,m12,m14,m10: F = WZ' \n");
    }
    else if((Map[13] == 1 || Map[13] == 2) && (Map[9] == 1 || Map[9] == 2) && (Map[11] == 1 || Map[11] == 2) && (Map[15] == 1 || Map[15] == 2)){
        printf("group m9,m11,m13,m15: F = WZ \n");
    }
    else if((Map[10] == 1 || Map[10] == 2) && (Map[11] == 1 || Map[11] == 2) && (Map[14] == 1 || Map[14] == 2) && (Map[15] == 1 || Map[15] == 2)){
        printf("group m10,m11,m14,m15: F = WY \n");
    }
    else if((Map[0] == 1 || Map[0] == 2) && (Map[1] == 1 || Map[1] == 2) && (Map[8] == 1 || Map[8] == 2) && (Map[9] == 1 || Map[9] == 2)){
        printf("group m0,m1,m8,m9: F = X'Y' \n");
    }
    else if((Map[3] == 1 || Map[3] == 2) && (Map[1] == 1 || Map[1] == 2) && (Map[9] == 1 || Map[9] == 2) && (Map[11] == 1 || Map[11] == 2)){
        printf("group m3,m1,m9,m11: F = X'Z \n");
    }
    else if((Map[10] == 1 || Map[10] == 2) && (Map[11] == 1 || Map[11] == 2) && (Map[2] == 1 || Map[2] == 2) && (Map[3] == 1 || Map[3] == 2)){
        printf("group m2,m3,m10,m11: F = X'Y \n");
    }
    else if((Map[0] == 1 || Map[0] == 2) && (Map[2] == 1 || Map[2] == 2) && (Map[8] == 1 || Map[8] == 2) && (Map[10] == 1 || Map[10] == 2)){
        printf("group m0,m10,m2,m8: F = X'Z' \n");
    }
    else if((Map[0] == 1 || Map[0] == 2) && (Map[1] == 1 || Map[1] == 2) && (Map[2] == 1 || Map[2] == 2) && (Map[3] == 1 || Map[3] == 2)){
        printf("group m0,m1,m2,m3: F = W'X' \n");
    }
    else if((Map[6] == 1 || Map[6] == 2) && (Map[7] == 1 || Map[7] == 2) && (Map[4] == 1 || Map[4] == 2) && (Map[5] == 1 || Map[5] == 2)){
        printf("group m7,m6,m4,m5: F = W'X \n");
    }
    else if((Map[10] == 1 || Map[10] == 2) && (Map[11] == 1 || Map[11] == 2) && (Map[8] == 1 || Map[8] == 2) && (Map[9] == 1 || Map[9] == 2)){
        printf("group m8,m9,m10,m11: F = WX' \n");
    }
    else if((Map[12] == 1 || Map[12] == 2) && (Map[13] == 1 || Map[13] == 2) && (Map[14] == 1 || Map[14] == 2) && (Map[15] == 1 || Map[15] == 2)){
        printf("group m12,m13,m14,m15: F = WX \n");
    }
    else if((Map[0] == 1 || Map[0] == 2) && (Map[12] == 1 || Map[12] == 2) && (Map[4] == 1 || Map[4] == 2) && (Map[8] == 1 || Map[8] == 2)){
        printf("group m0,m12,m4,m8: F = Y'Z' \n");
    }
    else if((Map[13] == 1 || Map[13] == 2) && (Map[1] == 1 || Map[1] == 2) && (Map[9] == 1 || Map[9] == 2) && (Map[5] == 1 || Map[5] == 2)){
        printf("group m13,m1,m9,m5: F = Y'Z \n");
    }
    else if((Map[3] == 1 || Map[3] == 2) && (Map[11] == 1 || Map[11] == 2) && (Map[7] == 1 || Map[7] == 2) && (Map[15] == 1 || Map[15] == 2)){
        printf("group m3,m11,m7,m15: F = YZ \n");
    }
    else if((Map[10] == 1 || Map[10] == 2) && (Map[2] == 1 || Map[2] == 2) && (Map[14] == 1 || Map[14] == 2) && (Map[6] == 1 || Map[6] == 2)){
        printf("group m10,m6,m14,m2: F = YZ' \n");
    }
    // GROUPS OF 2
    
    // GROUPS OF 1
    else if(Map[0] == 1){
        printf("group m0: F = W'X'Y'Z' \n");
    }
    else if(Map[1] == 1){
        printf("group m1: F = W'X'Y'Z \n");
    }
    else if(Map[2] == 1){
        printf("group m2: F = W'X'YZ' \n");
    }
    else if(Map[3] == 1){
        printf("group m3: F = W'X'YZ \n");
    }
    else if(Map[4] == 1){
        printf("group m4: F = W'XY'Z' \n");
    }
    else if(Map[5] == 1){
        printf("group m5: F = W'XY'Z \n");
    }
    else if(Map[6] == 1){
        printf("group m6: F = W'XYZ' \n");
    }
    else if(Map[7] == 1){
        printf("group m7: F = W'XYZ \n");
    }
    else if(Map[8] == 1){
        printf("group m8: F = WX'Y'Z' \n");
    }
    else if(Map[9] == 1){
        printf("group m9: F = WX'Y'Z \n");
    }
    else if(Map[10] == 1){
        printf("group m10: F = WX'YZ' \n");
    }
    else if(Map[11] == 1){
        printf("group m11: F = WX'YZ \n");
    }
    else if(Map[12] == 1){
        printf("group m12: F = WXY'Z' \n");
    }
    else if(Map[13] == 1){
        printf("group m0: F = WXY'Z \n");
    }
    else if(Map[14] == 1){
        printf("group m14: F = WXYZ' \n");
    }
    else if(Map[15] == 1){
        printf("group m15: F = WXYZ \n");
    }
}