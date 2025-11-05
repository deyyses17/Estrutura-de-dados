//Deyse Lannay
//Trabalho pr�tico 5

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> //pra criar numeros aleatorios, isso vai servir na minha funcao estoque automatico pra fazer a simulacao dos 100 produtos (dica da ia)

typedef struct Produto{
    int codigo;
    char tipo[20]; //Parafina, Quilha, Leash, Deck
    char descricao[100];
    float preco;
    struct Produto *prox;
    struct Produto *ant;
}Produto;

typedef struct Caixa{
    int codigo_caixa;
    int quantidade;
    int limite;
    int codigos_produtos[50];
    struct Caixa *prox; //implementei agora que precisamos procurar o c�digo de um produto especifico dentro da caixa com varios outros codigos
}Caixa;

typedef struct Venda{
    char tipo[20];
    int codigo;
    float preco;
}Venda;

typedef struct Cliente{
    char nome[30];
    char cpf[12];
    char cep[9];
    char rua[100];
    int numero;
    char complemento[50];
}Cliente;

typedef struct ClienteFic{
    char* Nome;
    char* Cpf;
    char* Cep;
    char* Rua;
    int Numero;
    char* Complemento;
}ClienteFic;

ClienteFic clientes[] = {
    {"Tatiane Fernandes", "11122233344", "62900000", "Rua das Ondas", 123, "Casa de frente para o mar"},
    {"Deyse Lannay", "22233344455", "62823000", "Avenida da Praia", 456, "Loja de pranchas"},
    {"Machado de Assis", "33344455566", "62920000", "Rua dos Livros", 789, "Sobrado com biblioteca"},
    {"Harry Potter", "44455566677", "67890000", "Rua dos Alfeneiros", 4, "Armario debaixo da escada"},
    {"Merliah Summers", "55566677788", "61200000", "Miami Beach", 101, "Casa da sereia"}
};

int TOTAL_CLIENTES = 5;

typedef struct Pedido{
    Venda venda;
    Cliente cliente;
    struct Pedido *prox;
}Pedido;

Pedido *inicio_fila = NULL;
Pedido *fim_fila = NULL;

//Lista de produtos que est�o disponiveis p/ venda, ordenada por pre�os
Produto *lista_dispo = NULL;

const char* categorias_validas[] = {"Parafina", "Leash","Quilha", "Deck"};
const int TOTAL_CATEG = 4;

//Pilha de estoque pra cada produto
Caixa *pilha_parafina = NULL;
Caixa *pilha_leash = NULL;
Caixa *pilha_deck = NULL;
Caixa *pilha_quilha = NULL;

void adicionar_produto(int codigo, char tipo[], char descricao[], float preco){
    Produto *NOVO = (Produto*)malloc(sizeof(Produto));
    NOVO->codigo = codigo;
    strcpy(NOVO->tipo, tipo);
    strcpy(NOVO->descricao, descricao);
    NOVO->preco = preco;
    NOVO->prox = NULL;
    NOVO->ant = NULL; //pq eh nossa primeira caixa

    //Lista vazia ou novo produto sendo mais barato
    if(lista_dispo == NULL || preco < lista_dispo->preco){
        NOVO->prox = lista_dispo;
        if(lista_dispo!=NULL){
        lista_dispo->ant = NOVO;
        }

        lista_dispo = NOVO;
    }else{ //Adiciona no final ou no meio
        Produto *atual = lista_dispo;
        while(atual->prox != NULL && atual->prox->preco < preco){
            atual = atual->prox;
        }
        NOVO->prox = atual->prox;
        NOVO->ant = atual;
        if(atual->prox !=NULL){
            atual->prox->ant = NOVO;
        }
        atual->prox = NOVO;
    }
    adicionar_estoque(codigo, tipo);
}

void adicionar_estoque(int codigo, char *tipo){
    Caixa **pilha_categ = NULL;
    int limite;

    if(strcmp(tipo, "Parafina") == 0){
        pilha_categ = &pilha_parafina;
        limite = 50;
    }else if(strcmp(tipo, "Quilha") == 0){
        pilha_categ = &pilha_quilha;
        limite = 10;
    }else if(strcmp(tipo, "Leash") == 0){
        pilha_categ = &pilha_leash;
        limite = 25;
    }else{
        pilha_categ = &pilha_deck;
        limite = 5;
    }

    //fiz com ia, foi mal tati </3, mas aqui eh no caso onde a caixa ta cheia ou entao vazia
    if(*pilha_categ == NULL || (*pilha_categ)->quantidade >= limite){
        Caixa *nova_caixa = (Caixa*)malloc(sizeof(Caixa));
        nova_caixa->codigo_caixa = 1;
        nova_caixa->quantidade = 0;
        nova_caixa->limite = limite;

        for(int i = 0; i< 50; i++){
                nova_caixa->codigos_produtos[i] = -1;
        }
        nova_caixa->codigos_produtos[0] = codigo; //add produto
        nova_caixa->quantidade = 1;
        nova_caixa->prox = *pilha_categ;
        *pilha_categ = nova_caixa;
    }
    else{
        for(int i = 0; i < limite; i++){
            if((*pilha_categ)->codigos_produtos[i] == -1){
                (*pilha_categ)->codigos_produtos[i] = codigo;
                (*pilha_categ)->quantidade++;
                break;
            }
        }

    }
}
//IA <3
void estoque_automatico(){
    srand(time(NULL));
    char *tipos[] = {"Parafina", "Quilha", "Leash", "Deck"};
    char *marcas[] = {"Tati Surf", "ProWave", "Banana Wax", "Ocean Soul"};

    for (int i = 0; i <100; i++){
        int codigo = 1000 + i;
        float preco = 20.0 + (rand() % 1000) / 10.0;

        char *tipo = tipos[rand() % 4];
        char *marca = marcas[rand() % 4];
        char descricao[100];
        sprintf(descricao, "%s %s Modelo %d", marca, tipo, i+1);
        adicionar_produto(codigo, tipo, descricao, preco);
    }
}

void imprimir_lista(){
    Produto *atual = lista_dispo;
    printf("\n-------TATI SURF Co-------\n");
    while(atual != NULL){
        printf(" Codigo: %d\n Tipo: %s\n Descricao: %s\n Preco: %.2f\n\n", atual->codigo, atual->tipo, atual->descricao, atual->preco);
        Caixa *caixa = NULL;
        if(strcmp(atual->tipo, "Parafina") == 0){
            caixa = pilha_parafina;
        }else if (strcmp(atual->tipo, "Quilha") == 0) {
            caixa = pilha_quilha;
        } else if (strcmp(atual->tipo, "Leash") == 0) {
            caixa = pilha_leash;
        } else {
            caixa = pilha_deck;
        }

        int total_estoque = 0;
        while(caixa!=NULL){
            total_estoque += caixa->quantidade;
            caixa = caixa->prox;
        }
        printf("Restam %d unidades no estoque.\n", total_estoque);
        printf("------------------------------------------------\n");
        atual = atual->prox;
    }
}

void listar_por_categoria(const char* categoria){
    int categoria_valida = 0;
    for (int i = 0; i< TOTAL_CATEG; i++){
        if(strcmp(categoria, categorias_validas[i])== 0){
            categoria_valida = 1;
            break;
        }
    }

    if (!categoria_valida){
        printf("\nCategoria invalida! Tente novamente com: \n");
        for (int i = 0; i< TOTAL_CATEG; i++){
            printf("%s", categorias_validas[i]);
        }
    }

    Produto *atual = lista_dispo;
    printf("\n-------PRODUTOS DA CATEGORIA %s-------\n", categoria);
    int encontrados = 0;

    while(atual != NULL){
        if(strcmp(atual->tipo, categoria) == 0){
            printf("Codigo: %d\n Descricao: %s\n Preco: %.2f\n\n", atual->codigo, atual->descricao, atual->preco);
            encontrados++;
        }
        atual = atual->prox;

    }if (encontrados == 0){
        printf("Nenhum produto econtrado.\n");
    }
}

void listar_por_preco(float preco_min, float preco_max){
    Produto *atual = lista_dispo;
    int encontrados = 0;
    printf("\n-------PRODUTOS ENTRE R$%.2f E R$%.2f-------\n", preco_min,preco_max);

    while(atual != NULL && atual->preco <= preco_max){
        if(atual->preco >= preco_min){
            printf("\nCodigo: %d\n", atual->codigo);
            printf("Tipo: %s\n", atual->tipo);
            printf("Descricao: %s\n", atual->descricao);
            printf("Preco: R$%.2f\n\n", atual->preco);
            encontrados++;
        }
        atual = atual->prox;
    }
    if (encontrados == 0){
        printf("Nenhum produto encontrado nessa faixa de pre�o.\n");
    }
}

//acho que aqui eh o melhor lugar pra se colocar a funcao de imprimir os dados das 4 pilhas pra fazer a verificacao
void imprimir_pilha(Caixa *topo, char *categoria){
    printf("\nPILHA DE CAIXAS DE %s:\n", categoria);
    int total_itens = 0;
    Caixa *atual = topo;

    while(atual != NULL){
        printf("Caixa %d (%d/%d itens): ", atual->codigo_caixa, atual->quantidade, atual->limite);

        for(int i = 0; i < atual->quantidade; i++){
            printf("%d", atual->codigos_produtos[i]);
            if (i <atual->quantidade - 1){ //pra que o ultimo codigo nao tenha virgula
                printf(", ");
            }
        }
        printf("\n");

        total_itens += atual->quantidade;
        atual = atual->prox;
    }
    printf("\nTOTAL DE ITENS: %d\n", total_itens);
}

//aqui criei uma funcao void pra cada tipo de produto pra mais pra frente ficar mais facil de chamar
void imprimir_estoque_parafina(){
    imprimir_pilha(pilha_parafina, "PARAFINA");
}
void imprimir_estoque_quilha(){
    imprimir_pilha(pilha_quilha, "QUILHA");
}
void imprimir_estoque_leash(){
    imprimir_pilha(pilha_leash, "LEASH");
}
void imprimir_estoque_deck(){
    imprimir_pilha(pilha_deck, "DECK");
}
void imprimir_todo_estoque(){
    imprimir_estoque_parafina();
    imprimir_estoque_quilha();
    imprimir_estoque_leash();
    imprimir_estoque_deck();
}

//IA
Produto* remover_da_lista(int codigo) {
    Produto *atual = lista_dispo;

    while (atual != NULL && atual->codigo != codigo) {
        atual = atual->prox;
    }

    if (atual == NULL) {
        return NULL;
    }

    if (atual->ant != NULL) {
        atual->ant->prox = atual->prox;
    } else {
        lista_dispo = atual->prox;
    }

    if (atual->prox != NULL) {
        atual->prox->ant = atual->ant;
    }

    atual->prox = NULL;
    atual->ant = NULL;

    return atual;
}

Venda* vender_produto(int codigo){
    Produto *produto = remover_da_lista(codigo);

    if(produto == NULL){
        printf("Produto nao encontrado!\n");
        return NULL;
    }

    Caixa **pilha_categ = NULL;

    if(strcmp(produto->tipo, "Parafina") == 0){
        pilha_categ = &pilha_parafina;
    }else if(strcmp(produto->tipo, "Quilha") == 0){
        pilha_categ = &pilha_quilha;
    }else if(strcmp(produto->tipo, "Leash") == 0){
        pilha_categ = &pilha_leash;
    }else{
        pilha_categ = &pilha_deck;
    }

    //Pilha aux pra guardar as caixas temporariamente, tamb�m fiz com IA </3
    Caixa *pilha_aux = NULL;
    Caixa *caixa_c_produto = NULL;
    int posicao = -1;

    while(*pilha_categ != NULL){
        Caixa *topo = *pilha_categ;

        for(int i = 0; i < topo->limite; i++){
            if(topo->codigos_produtos[i] == codigo){
                caixa_c_produto = topo;
                posicao = i;
                break;
            }
        }

        if(caixa_c_produto != NULL){
            break;
        }

        *pilha_categ = topo->prox;
        topo->prox = pilha_aux;
        pilha_aux = topo;
    }

    if(caixa_c_produto == NULL){
        printf("O produto nao esta no estoque.\n");
        while(pilha_aux != NULL){
            Caixa *temp = pilha_aux;
            pilha_aux = pilha_aux->prox;
            temp->prox = *pilha_categ;
            *pilha_categ = temp;
        }
        return NULL;
    }
    caixa_c_produto->codigos_produtos[posicao] = -1;
    caixa_c_produto->quantidade--;

    if(caixa_c_produto->quantidade == 0){
        Caixa *temp = *pilha_categ;
        *pilha_categ = (*pilha_categ)->prox;
        free(temp);
    }

    while(pilha_aux != NULL){
        Caixa *temp = pilha_aux;
        pilha_aux = pilha_aux->prox;
        temp->prox = *pilha_categ;
        *pilha_categ = temp;
    }

    Venda *venda = (Venda*)malloc(sizeof(Venda));
    strcpy(venda->tipo, produto->tipo);
    venda->codigo = produto->codigo;
    venda->preco = produto->preco;

    free(produto);
    return venda;
}

//tati me perdoa, aqui tive que modificar com IA porque tava dando mutio erro.
//eu digitava os dados do cliente e meu prompt meio que travava, o menu
//(que tem que aparecer a cada fim de uma execu�ao) nao tava aparecendo, meu prompt simplesmente sumia,
//entao tive que apelar, nao que seja algo novo, mas enfim kkkkkkkkkk

void adicionar_na_fila(Venda *venda){
    Pedido *novo_pedido = (Pedido*)malloc(sizeof(Pedido));
    novo_pedido->venda = *venda;
    novo_pedido->prox = NULL;

    printf("\n------DADOS DO CLIENTE------\n");

    while(getchar() != '\n');

    printf("Nome: ");
    scanf("%29[^\n]", novo_pedido->cliente.nome);
    getchar();

    printf("CPF: ");
    scanf("%11s", novo_pedido->cliente.cpf);
    getchar();

    printf("CEP: ");
    scanf("%8s", novo_pedido->cliente.cep);
    getchar();

    printf("Rua: ");
    scanf("%99[^\n]", novo_pedido->cliente.rua);
    getchar();

    printf("Numero: ");
    scanf("%d", novo_pedido->cliente.numero);
    getchar();

    printf("Complemento: ");
    scanf("%49[^\n]", novo_pedido->cliente.complemento);

    if(inicio_fila == 0){ //se minha fila estiver vazia, meu novo pedido sera o primeiro e ultimo
        inicio_fila = fim_fila = novo_pedido;
    }else{
        fim_fila->prox = novo_pedido;
        fim_fila = novo_pedido;
    }
    printf("\nPedido adicionado a fila de entregas!\n");
}
//vender 20 produtos: (IA </3, mas isso eh "bom", porque eu sinceramente pensei em digitar 100 novos produtos pra simular as vendas e as entregas kkkkkkkkrying)
void simular_vendas(int quant){
    printf("\nVENDAS: \n");
    for(int i = 0; i< quant; i++){
        int codigo_aleatorio = 1000 + rand() % 100;
        Venda *venda = vender_produto(codigo_aleatorio);

        if(venda != NULL){
            printf("%d. Venda realizada: Codigo: %d (%s).\n", i + 1, venda->codigo, venda->tipo);


            int clientE = i % TOTAL_CLIENTES; //aqui escolhe um cliente fixo pras compras
            ClienteFic cliente = clientes[clientE];

            Pedido *novo_pedido = (Pedido*)malloc(sizeof(Pedido));
            novo_pedido->venda = *venda;

            strcpy(novo_pedido->cliente.nome, cliente.Nome);
            strcpy(novo_pedido->cliente.cpf, cliente.Cpf);
            strcpy(novo_pedido->cliente.cep, cliente.Cep);
            strcpy(novo_pedido->cliente.rua, cliente.Rua);
            novo_pedido->cliente.numero = cliente.Numero;
            strcpy(novo_pedido->cliente.complemento, cliente.Complemento);
            novo_pedido->prox = NULL;

            if(inicio_fila == NULL){
                inicio_fila = fim_fila = novo_pedido;
            }else{
                fim_fila->prox = novo_pedido;
                fim_fila = novo_pedido;
            }
        }else{
            printf("%d. Produto %d nao encontrado/esgotado!\n", i + 1, codigo_aleatorio);
        }
    }
}

void entregas(){ //processamento de entregas
    if(inicio_fila == NULL){
        printf("\nNenhum pedido na fila de entregas.\n");
        return;
    }
    Pedido *pedido = inicio_fila;
    inicio_fila = inicio_fila->prox;
    if(inicio_fila == NULL){
        fim_fila = NULL;
    }
    printf("\nPRODUTO SAINDO PARA ENTREGA!\n");
    printf("Produto: %s (Codigo: %d)\n", pedido->venda.tipo, pedido->venda.codigo);
    printf("Nome do cliente: %s\n", pedido->cliente.nome);
    printf("Cpf do cliente: %s\n", pedido->cliente.cpf);
    printf("Local de entrega: %s, %d, %s - %s\n", pedido->cliente.rua, pedido->cliente.numero, pedido->cliente.complemento, pedido->cliente.cep);
    free(pedido);
}
//simula�ao das 10 entregas:
void simular_entregas(int quant){
    printf("\nENTREGAS: \n");
    for(int i = 0; i < quant; i++){
        if(inicio_fila == NULL){
            printf("\nFila de entrega vazia.\n");
            break;
        }
        printf("\n---ENTREGA %d---\n", i + 1);
        entregas();
    }
}

int main(){

    adicionar_produto(1800, "Parafina", "Parafina de Surf by Fuwax", 39.00);
    adicionar_produto(1888, "Parafina", "Parafina Surf All Stick", 29.40);
    adicionar_produto(1290, "Parafina", "Parafina Banana Was Warm", 15.00);
    adicionar_produto(1222, "Quilha", "Quilha Kitesurf 30mm", 241.53);
    adicionar_produto(8881, "Quilha", "Quilha Banana Wax Twin Keel Biquilha", 64.00);
    adicionar_produto(3117, "Quilha", "Quilha Expans Enxaixe 2", 85.00);
    adicionar_produto(7906, "Deck", "Deck Dianteiro Laranja", 130.85);
    adicionar_produto(7891, "Deck", "Deck Para Prancha de Surf Antiderrapante - Deck Surf", 46.90);
    adicionar_produto(2000, "Deck", "Deck Profissional Soulfins", 174.90);
    adicionar_produto(0014, "Leash", "Leash Surf", 193.03);
    adicionar_produto(0011, "Leash", "Leash de Surf Banana Wax Regular 6 Simples", 75.90);
    adicionar_produto(4301, "Leash", "Leash Strep Cordinha Para Prancha de Surf", 104.90);
    adicionar_produto(2343, "Quilha", "Barbatana para dar direcionamento pro surfista", 56.89);

    estoque_automatico();

    int opcao;
    char categoria[20]; //Parafina, Quilha, Leash e Deck

    do{
        printf("\nBEM-VINDO(A) A TATI SURF Co!\n");
        printf("\n-------MENU-------\n");
        printf("1. Listar todos os produtos.\n");
        printf("2. Listar por categoria.\n");
        printf("3. Buscar por preco.\n");
        printf("4. Verificar estoque.\n");
        printf("5. Comprar - (ATENCAO: Para realizar a compra, veja a lista de produtos disponiveis e seus codigos, voce precisara dele pra realizar sua compra).\n");
        printf("6. Processar entregas.\n");
        printf("7. Sair.\n");
        printf("---EXTRA (disponivel apenas para Tati):--- \n");
        printf("8. Simulacao de venda.\n");
        printf("9. Simulacao de entrega.\n");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);

        switch(opcao){
        case 1:{
            imprimir_lista();
            break;
        }
        case 2: {
            printf("\nCategorias validas: Parafina, Quilha, Leash e Deck.\n");
            printf("\nDigite a categoria desejada: ");
            scanf("%s", categoria);
            listar_por_categoria(categoria);
            break;
        }
        case 3:{
            float min, max;
            printf("\nDigite o preco minimo: \n");
            scanf("%f", &min);
            printf("\nDigite o preco maximo: \n");
            scanf("%f", &max);
            listar_por_preco(min, max);
            break;
        }
        case 4:{
            printf("\n------VERIFICACAO DO ESTOQUE------\n");
            imprimir_todo_estoque();
            break;
        }
        case 5:{
            int codigo;
            printf("Digite o codigo do produto a ser comprado: ");
            scanf("%d", &codigo);
            Venda *venda = vender_produto(codigo);
            if(venda!=NULL){
                printf("Compra realizada! Muito obrigada! Acompanhe os dados do seu produto logo abaixo: \n");
                printf("Produto: %s | Codigo: %d | Preco: R$%.2f\n", venda->tipo, venda->codigo, venda->preco);
                free(venda);
            }
            break;
        }
        case 6:{
                entregas();
                break;}
        case 7:{
            printf("Tchau! Foi bom ter voce por aqui. Ate mais! (Na proxima ve se compra alguma coisa pelo menos...)\n");
            break;
        }
        case 8:{
            simular_vendas(20);
            break;
        }
        case 9:{
            simular_entregas(10);
            break;
        }
        default:
            printf("Opcao invalida!\n");
        }

    }while(opcao != 9);

    system("pause");
    return 0;
}
