#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

/********
  BUSCA
*********/

bool existe(std::vector<int> vet, int x) {
    for (int i = 0; i < vet.size(); i++)
        if(vet[i] == x)
        return true;
    return false;
}

int contar(std::vector<int> fila, int x) {
    int cont=0;
    for (int i = 0; i < fila.size(); i++) 
        if(x == fila[i]) cont++;
            return cont;
}


int procurar_valor(std::vector<int> fila, int x) {
    for (int i = 0; i < fila.size(); i++)
        if(fila[i] == x)
            return i;
    return -1;
}

int procurar_valor_apartir(std::vector<int> fila, int x, int inicio) {
    int i = 0;
    
   for(int i = inicio; i < fila.size(); i++)
        if(x == fila[i])
        return i;
    return -1;
}

/**************
  MELHOR CASO 
***************/

int procurar_menor(std::vector<int> fila) {
    int menor=0;
    for (int i = 0; i < fila.size(); i++)
        if(fila[i] < menor) menor = fila[i];
        return menor;
}

int procurar_menor_pos(std::vector<int> fila) {
    int menor=0, mpos=0;
    for (int i = 0; i < fila.size(); i++) {
        if(fila[i] < menor) {
            menor = fila[i];
            mpos = i;
        }
    }
    return mpos;
}

int procurar_menor_pos_apos(std::vector<int> fila, int inicio) {
    int menor=0;
    int pos=0;
    
    for(int i = inicio; i < fila.size(); i++) {
        if(fila[i] < menor) {
            menor = fila[i];
            pos = i;
        }
        return i;
    }
    return -1;
}

int procurar_melhor_pos_se(std::vector<int> fila){
    int menor=0;
    int Mpos=0;
    for (int i = 0; i < fila.size(); i++) {
        if(fila[i] < menor && fila[i] > 0) {
            menor = fila[i];
            Mpos = i;
        }
        return 1;
    }
    return -1;
}

/**************
    CONTAGEM 
***************/

float calcular_stress_medio(std::vector<float> fila) {
    float soma=0;
    float media=0;
   
    for (int i = 0; i < fila.size(); i++) {
        soma += fila[i];
    }
        
    for (int i = 0; i < fila.size(); i++) {
        media = soma/fila.size();
    }
    
    return media;   
}

char mais_homens_ou_mulheres(std::vector<int> fila) {
    int contH=0;
    int contM=0;
    
    for (int i = 0; i < fila.size(); i++) {
        if (fila[i] > 0) contH++;
    }
    for (int i = 0; i < fila.size(); i++) {
        if (fila[i] < 0) contM++;
    }
    
    if(contM > contH) std::cout << "mulheres"; 
    if(contM < contH) std::cout << "homens"; 
    if(contM == contH) std::cout << "empate"; 
    
}

char qual_metade_eh_mais_estressada(std::vector<int> fila) {
    int soma1 = 0;
    int soma2 = 0;
    int i =0;
    
    for (i = 0; i < fila.size()/2; i++) {
        soma1 += fila[i];
    }
    for (i/2; i < fila.size(); i++) {
        soma2 += fila[i];
    }
    
    if(soma1 == soma2) std::cout << "empate";
    if(soma1 > soma2) std::cout << "primeira";
    if(soma1 < soma2) std::cout << "segunda";
}

bool homens_sao_mais_estressados_que_mulheres(std::vector<int> fila) {
    float somaH=0; 
    float mediaH=0; 
    float contH=0;
    float somaM=0; 
    float mediaM=0;
    float contM=0;
   
    for (int i = 0; i < fila.size(); i++) {
        if(fila[i] > 0) {
            contH++;
        }
    }
    for (int i = 0; i < fila.size(); i++) {
        if(fila[i] > 0) {
            somaH += fila[i];
        }
    }
    for (int i = 0; i < fila.size(); i++) {
        if(fila[i] > 0) {
            mediaM = somaM/contM;
        }
    }
        
    for (int i = 0; i < fila.size(); i++) {
        if(fila[i] < 0) {
            contM++;
        }
    }    
    for (int i = 0; i < fila.size(); i++) {
        if(fila[i] < 0) {
            somaM += fila[i];
        }
    }
    for (int i = 0; i < fila.size(); i++) {
        if(fila[i] < 0) {
            mediaM = somaM/contM;
        }
    }
    
    for (int i = 0; i < fila.size(); i++)
        if(mediaH > mediaM)
        return (true);
    return (false);
}



int main()
{
    /************* BUSCA **************/
    
    std::cout << existe({-1, -50, -99}, -1) << "\n";
    std::cout << existe({-1, -50, -99}, 10) << "\n";

    std::cout << "\n\n";

    std::cout << contar({-1, -50, -1 -99}, -1) << "\n"; // 2
    std::cout << contar({-1, -50, -1, -99}, 10) << "\n"; // 0

    std::cout << "\n\n";

    
    std::cout << procurar_valor({-1, -50, -1 -99}, -50) << "\n"; // 1
    std::cout << procurar_valor({-1, -50, -1, -99}, 10) << "\n"; // -1

    std::cout << "\n\n";
    
    std::cout << procurar_valor_apartir({5, 3, -1, -50, -1, -99}, -1, 0) << "\n"; // 2
    std::cout << procurar_valor_apartir({5, 3, -1, -50, -1, -99}, -1, 3) << "\n";
    std::cout << procurar_valor_apartir({5, 3, -1, -50, -1, -99}, -1, 4) << "\n";
    

    std::cout << "\n\n";


    /************* MELHOR CASO **************/


    std::cout << procurar_menor({5, 3, -1, -50, -1, -99}) << "\n"; // -99

    std::cout << "\n\n";

    std::cout << procurar_menor_pos({5, 3, -1, -50, -1, -99})  << "\n";

    std::cout << "\n\n";
    
    std::cout << procurar_menor_pos_apos({5, 3, -1, -50, -1, 10}, 3)  << "\n";

    std::cout << "\n\n";

    std::cout << procurar_melhor_pos_se({5, 3, -1, -50, -1, -99}) << "\n";
    std::cout << procurar_melhor_pos_se({-1, -50, -1, -99}) << "\n";
    
    std::cout << "\n\n";


    /************* CONTAGEM **************/

    std::cout << calcular_stress_medio({5, 3, -1, -50, -1, -99}) << "\n";

    std::cout << "\n\n";

    
    std::cout << mais_homens_ou_mulheres({5, 3, -1, -50, -1, -99}) << "\n";
    std::cout << mais_homens_ou_mulheres({5, 3, 1, -50, -1, -99}) << "\n";
    std::cout << mais_homens_ou_mulheres({5, 3, 1, -50, -1, 99}) << "\n";

    std::cout << "\n\n";

    std:: cout << qual_metade_eh_mais_estressada({5, 3, -1, -50, -1, -99}) << "\n";
    std:: cout << qual_metade_eh_mais_estressada({50, 98, 2, -50, -1, -99}) << "\n";
    std:: cout << qual_metade_eh_mais_estressada({-51, 99, 1, -50, -1, -99}) << "\n";

    std::cout << "\n\n";

    std::cout << homens_sao_mais_estressados_que_mulheres({5, 3, 1, -50, 1, -99, -4}) << "\n"; 

    return 0;
}