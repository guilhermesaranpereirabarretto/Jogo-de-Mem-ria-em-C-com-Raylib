/* ======================== Documentação do Projeto ======================== */
/**
 * @file main.c
 * @brief Jogo de Memória
 * @authors Felipe Gonçalves, Guilherme Barretto e Geovanna Sargaço.
 * @date 06/2025
 * @version 0.1
 * @license GPL
 *
 * @note Descrição:
 * É um jogo simples de memorização, onde o jogador deve clicar
 * nos quadrados revelados anteriormente.
 *
 * Problemas encontrados: passar uma matriz para outra função.
 *
 * @warning Uso de IA:
 *  Autocomplete de código, auxílio na documentação e cálculos
 * para centralização de elementos graficos.
 */

/* ====================== Diretivas de Processamento ====================== */
/**
 * @brief Inclusão das bibliotecas necessárias para o funcionamento do jogo.
 *
 * @note Includes:
 * - raylib.h para partes visuais do jogo.
 * - stdio.h para debug com printf.
 * - stdlib.h usando a função rand() para gerar números aleatórios.
 * - time.h usando a função time(NULL) para obter o tempo atual.
 */

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* ========================= Protótipos de Funções ========================= */

void tela_de_inicio();
void tela_de_cadastro();
void tela_de_contagem();
int registro_do_jogador();
int definir_jogador();
int gerador_de_numeros(int i);
int tela_do_tabuleiro(int fase);
void fim_de_jogo();
void mostrar_ranking();
void aleatorizar_tabuleiro(int fase, int tamanho, int matriz[][tamanho]);
void limpar_matriz(int tamanho, int matriz[][tamanho]);
int definir_tamanho(int fase);
void desenhar_gabarito(int celulas, int matriz[][celulas], int cordenada);
void desenhar_interacao(int celulas, int prova[][celulas], int cordenada);
void calcular_pontos(int fase, int celulas, int gabarito[][celulas], int prova[][celulas]);
void desenhar_resultado(int celulas, int matriz[][celulas], int prova[][celulas], int cordenada);
void desenhar_contador(int tempo_atual, int tempo_inicio);
void esperar_tempo(int tempo_limite);
void temporizador(int tempo, int *pt_inicio, int *pt_estado);
void carregar_imagens();
void execucao_do_jogo();

/* =============================== Registros =============================== */
/**
 * @brief Estrutura para armazenar informações de jogadores.
 *
 * @param nick Apelido do jogador com no máximo 10 caracteres.
 * @param pontos Pontuação acumulada pelo jogador.
 * @param tempo Tempo de jogo.
 * @param fase Fase atual do jogo.
 */

typedef struct registo_de_jogadores
{
    char nick[10];
    int pontos;
    int tempo;
    int fase;
} registro;

/* =========================== Variáveis Globais =========================== */

registro jogador[4]; // Vetor de jogadores max = 4;
int contador_de_jogadores = 0;
int indice_do_jogador = 0;
const int largura_da_tela = 500;
const int altura_da_tela = 500;
int cliques = 0;
int estado_do_jogo = 0;

/* =========================== Função Principal ============================ */

int main()
{
    // Inicialização da semente para geração de números aleatórios.
    srand(time(0));
    execucao_do_jogo();
    return 0;
}

/* ======================= Desenvolvimento de Funções ====================== */

/**
 * @brief Função para executar cada tela do jogo.
 *
 * @note Esta função é para organizar o codigo, chamando as funções com
 * base no valor da váriavel estado_do_jogo();
 *
 * @author Felipe
 */
void execucao_do_jogo(){
    
    // Inicialização da janela.
    SetTraceLogLevel(5); // Definir o log como 5 (mostrar somente erros).
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(largura_da_tela, altura_da_tela, "Jogo da Memoria");

    // Definir o ícone da janela.
    Image icon = LoadImage("icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon);

    while(!WindowShouldClose()) {
        switch (estado_do_jogo) {
            case 0:
                tela_de_inicio();
                break;
            case 1:
                tela_de_cadastro();
                estado_do_jogo = 2;
                break;
            case 2:
                tela_do_tabuleiro(jogador[indice_do_jogador].fase);
                break;
            case 3:
                mostrar_ranking();
                break;
        }
    }
    CloseWindow();
    return;
}

/*Tela Inicio*/

void tela_de_inicio() {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKBLUE);
        DrawText("JOGO DA MEMÓRIA", 100, 100, 30, WHITE);
        DrawText("Clique para Iniciar", 130, 200, 20, LIGHTGRAY);
        EndDrawing();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            estado_do_jogo = 1;
            break;
        }
    }
}
/*Ranking*/

void mostrar_ranking() {
    for (int i = 0; i < 4 - 1; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (jogador[j].pontos > jogador[i].pontos) {
                registro temp = jogador[i];
                jogador[i] = jogador[j];
                jogador[j] = temp;
            }
        }
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("RANKING", 180, 50, 30, GOLD);

        for (int i = 0; i < 4; i++) {
            char texto[100];
            sprintf(texto, "%dº %s - %d pontos", i + 1, jogador[i].nick, jogador[i].pontos);
            DrawText(texto, 100, 100 + i * 40, 20, WHITE);
        }

        DrawRectangle(150, 350, 200, 40, DARKGRAY);
        DrawText("REINICIAR", 180, 360, 20, WHITE);
        EndDrawing();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            if (mouse.x >= 150 && mouse.x <= 350 &&
                mouse.y >= 350 && mouse.y <= 390) {
                estado_do_jogo = 0;
                for (int i = 0; i < 4; i++) {
                    jogador[i].pontos = 0;
                    jogador[i].fase = 0;
                }
                break;
            }
        }
    }
}


/**
 * @brief Janela de cadastro do nome do jogador.
 *
 * @note:
 * Esta função cria uma janela interativa para o jogador digitar o seu nome:
 * - Verificando se o nome tem mais de 3 letras e menos de 10.
 * - Exibindo as letras em tempo real.
 * - Pode-se usar o backspace para apagar as letras.
 * Ao presssionar enter, chama a função tela_de_contagem().
 *
 * @author Felipe
 *
 * TODO: Substituir o jogador[0] por um jogador vazio com a função definir_jogador()
 */
void tela_de_cadastro()
{
    int tecla = 0, indice_do_nome = 0;
    char nome[10] = "\0";
    int mouse_no_retangulo = 0;
    indice_do_jogador = definir_jogador();
    int fase = jogador[indice_do_jogador].fase;
    //printf("fase %d", fase);
    /* Area de carregamento do rshape */
    //Texture2D fundo = LoadTexture("fundoInicio.png");  // Carregamento da imagem de fundo da tela de cadastro.
    Image image = LoadImage("fundoInicio.png"); // Carregar imagem na RAM.
    Texture2D fundo = LoadTextureFromImage(image); // Converter para textura, para usar a GPU memory (VRAM)
    UnloadImage(image); // Remover imagem da RAM.

    // gostei de aprender isso de definir antes e depois carregar a textura.
    Rectangle retangulo = {250/2, 250 - 50, 250, 50}; // Define o retangulo para deixar redondo.

    while (!WindowShouldClose())
    {
        //Verifica se o mouse está dentro do retangulo.
        //mouse_no_retangulo = (GetMouseX()>125 && GetMouseX()<375 && GetMouseY()>200 && GetMouseY()<250)?
        mouse_no_retangulo = (CheckCollisionPointRec(GetMousePosition(), retangulo))?
                             1: 0;
        if (mouse_no_retangulo)
        {
            SetMouseCursor(MOUSE_CURSOR_IBEAM); //Transforma o icone do mouse em um I.
            tecla = GetCharPressed();

            while (tecla>0)
            {
                if ((tecla >= 32) && (tecla <= 125) && indice_do_nome<10)
                {
                    nome[indice_do_nome] = tecla;
                    indice_do_nome++;
                    nome[indice_do_nome] = '\0';
                }
                tecla = GetCharPressed();
            }
            if (IsKeyPressed(KEY_ENTER) && indice_do_nome>3)
            {
                strcpy(jogador[indice_do_jogador].nick, nome);
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                break;
                //printf("Nome escolhido: %s\n", nome);

            }
            else if(IsKeyPressed(KEY_BACKSPACE) && indice_do_nome>0)
            {
                indice_do_nome--;
                //printf("Apagado a letra: %c\n", nome[indice_do_nome]);
                nome[indice_do_nome] = '\0';

            }
        }
        else
        {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT); // Volta o ícone do mouse para o padrão.
        }

        // Desenha na tela.
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(fundo, 0, 0, WHITE); // desenha o fundo.

        DrawText("Digite o seu nome:", 250/2, 180, 20, BLUE); // DrawText("texto", posição_x, posição_y, tamanho_fonte, cor)
        DrawRectangleRounded(retangulo, 0.1, 0, BLACK);

        if (mouse_no_retangulo)
        {
            DrawRectangleRoundedLines(retangulo, 0.1, 0, BLUE);
        }
        else
        {
            DrawRectangleRoundedLines(retangulo, 0.1, 0, GRAY);
        }

        DrawText(nome, 250/2 + 5, 200 + 8, 40, RED);
        EndDrawing();
    }
    printf("ok");
    return;
}

/**
 * @brief Substituidor de pontos.
 * Esta função é chamada, para definir o jogador que terá o seu score substituído:
 * - Prioriza um vetor de jogador não preenchido.
 * - Substitui o jogador com menor pontuação.
 *
 * @return int Indice do vetor do registro (0-4)
 *
 * @author Felipe
 */
int definir_jogador()
{
    int menor_pontos = 0;
    int numero_do_jogador;
    for(int i = 0; i < 5; i++)
    {
        if(jogador[i].pontos == 0)
        {
            numero_do_jogador = i;
            break; // sai do loop quando encontrar um jogador sem pontos.
        }
        else if(jogador[i].pontos < menor_pontos);
        {
            menor_pontos = jogador[i].pontos;
            numero_do_jogador = i;
        }
    }
    return numero_do_jogador;
}

/**
 * @brief Renderiza e gerencia a tela do tabuleiro de jogo.
 *
 * @note Esta função cria um tabuleiro de jogo com tamanho dinâmico baseado na fase atual,
 * gera posições aleatórias para quadrados azuis, e permite que o jogador interaja
 * clicando nos quadrados dentro de um tempo limite.
 *
 * @param fase Número da fase atual, que determina o tamanho do tabuleiro e número de quadrados
 * @return int Valor de retorno não utilizado (pode ser modificado para retornar resultado do jogo)
 *
 * @author Felipe
 */
int tela_do_tabuleiro(int fase)
{
    // fase = qtd_de_quadrados
    fase = (fase==0)? 3: fase;

    // Variáveis da matriz
    int celulas = definir_tamanho(fase);
    int matriz[celulas][celulas];
    int matriz_do_jogador[celulas][celulas];

    // Variáveis gráficas
    char txt_pontos[30];
    const int quadrado_tamanho = 50;
    const int espaco = 5;
    int cordenada = (largura_da_tela - (celulas * quadrado_tamanho) - espaco) / 2;

    limpar_matriz(celulas, matriz);
    limpar_matriz(celulas, matriz_do_jogador);
    aleatorizar_tabuleiro(fase, celulas, matriz);

    // Variaveis do temporizador
    int estado = 0;
    int tempo_inicial = time(NULL);
    int *pt_tempoi = &tempo_inicial;
    int *pt_estado = &estado;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        sprintf(txt_pontos, "Pontos: %d", jogador[0].pontos);
        DrawText(txt_pontos, 0, 0, 20, BLUE);

        if (estado == 0)
        {
            temporizador(5, pt_tempoi, pt_estado);
            desenhar_contador(time(NULL), tempo_inicial);
            desenhar_gabarito(celulas, matriz, cordenada);
        }
        else if (estado == 1)
        {
            temporizador(2, pt_tempoi, pt_estado);
            DrawText("Tempo esgotado!", 125, 235, 30, WHITE);
        }
        else if(estado == 2)
        {
            if (cliques < fase)
            {
                desenhar_interacao(celulas, matriz_do_jogador, cordenada);
            }
            if (cliques == fase)
            {
                calcular_pontos(fase, celulas, matriz, matriz_do_jogador);
            }
            if (cliques > fase)
            {
                temporizador(5, pt_tempoi, pt_estado);
                desenhar_resultado(celulas, matriz, matriz_do_jogador, cordenada);
            }
        }
        else if (estado == 3)
        {
            jogador[indice_do_jogador].fase = fase + 1;
            cliques = 0;
            break;
        }

        EndDrawing();
    }
    return 0;
}

/**
 * @brief Desenha o gabarito do jogo em uma matriz de células.
 *
 * Esta função renderiza o gabarito do jogo, colorindo cada célula em azul ou cinza
 * dependendo do seu estado no gabarito original.
 *
 * @param celulas Número de células em cada dimensão da matriz
 * @param gabarito Matriz 2D representando o gabarito do jogo
 * @param cordenada Coordenada inicial de desenho para posicionamento da matriz
 */
void desenhar_gabarito(int celulas, int gabarito[][celulas], int cordenada)
{
    int linha, coluna, x, y;
    for (linha = 0; linha < celulas; linha++)
    {
        for (coluna = 0; coluna < celulas; coluna++)
        {
            Color cor = (gabarito[linha][coluna] == 1) ? BLUE : GRAY;
            // 51 = tamanho do quadrado (50) + espaco (1)
            x = cordenada + coluna * 51;
            y = cordenada + linha * 51;
            DrawRectangle(x, y, 50, 50, cor);
        }
    }
}

/**
 * @brief Desenha e gerencia a interação do jogador com o tabuleiro de jogo.
 *
 * Esta função renderiza os quadrados do tabuleiro, permitindo que o jogador
 * interaja através de cliques do mouse. Quadrados podem ser marcados com
 * clique esquerdo e desmarcados com clique direito.
 *
 * @param celulas Número de células em cada dimensão do tabuleiro
 * @param prova Matriz 2D representando o estado de cliques do jogador
 * @param cordenada Coordenada inicial de desenho para posicionamento da matriz
 */
void desenhar_interacao(int celulas, int prova[][celulas], int cordenada)
{
    int linha, coluna, x, y;
    int quadrado_tamanho = 50;

    for (int linha = 0; linha < celulas; linha++)
    {
        for (int coluna = 0; coluna < celulas; coluna++)
        {
            Color cor = GRAY;
            if (prova[linha][coluna] == 1)
            {
                cor = GREEN;
            }

            x = cordenada + coluna * 51;
            y = cordenada + linha * 51;

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (GetMouseX() >= x && GetMouseX() <= x + quadrado_tamanho &&
                        GetMouseY() >= y && GetMouseY() <= y + quadrado_tamanho)
                {
                    if (prova[linha][coluna] == 0)
                    {
                        cliques++;
                        prova[linha][coluna] = 1;
                    }
                }
            }
            else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
            {
                if (GetMouseX() >= x && GetMouseX() <= x + quadrado_tamanho &&
                        GetMouseY() >= y && GetMouseY() <= y + quadrado_tamanho)
                {
                    if (prova[linha][coluna] == 1)
                    {
                        cliques--;
                        prova[linha][coluna] = 0;
                    }
                }
            }

            DrawRectangle(x, y, quadrado_tamanho, quadrado_tamanho, cor);
        }
    }
}

// Função para calcular os pontos após o término da fase de cliques
/**
 * @brief Calcula os pontos com base na comparação da prova e o gabarito.
 *
 * @param fase O número da fase atual do jogo
 * @param celulas Tamanho da matriz quadrada de células
 * @param gabarito Matriz representando o gabarito correto do jogo
 * @param prova Matriz representando os cliques do jogador
 *
 * @note Atribui 100 pontos para cada célula corretamente clicada
 * @note Incrementa o contador de cliques para evitar múltiplas execuções
 */
void calcular_pontos(int fase, int celulas, int gabarito[][celulas], int prova[][celulas])
{
    int cache = 0;
    // Esta parte do código só deve ser executada uma vez após os cliques terminarem
    for (int linha = 0; linha < celulas; linha++)
    {
        for (int coluna = 0; coluna < celulas; coluna++)
        {
            if (gabarito[linha][coluna] == 1 && prova[linha][coluna] == 1)
            {
                jogador[indice_do_jogador].pontos += 100;
                cache++;
            }
        }
    }
    if(!(cache == fase)){
        estado_do_jogo = 3;
    }
    cliques++;
}

// Função para desenhar o resultado final
/**
 * @brief Desenha o resultado final do jogo, mostrando o estado de cada célula.
 *
 * @param celulas Tamanho da matriz quadrada de células
 * @param gabarito Matriz representando o gabarito correto do jogo
 * @param prova Matriz representando os cliques do jogador
 * @param cordenada Coordenada inicial para desenho dos quadrados
 *
 * @note Usa cores diferentes para representar diferentes tipos de resultados:
 * - BLUE: Célula que deveria ser clicada, mas não foi (erro de omissão)
 * - RED: Célula clicada que não deveria ter sido (erro de inclusão)
 * - GREEN: Célula corretamente clicada
 * - GRAY: Célula corretamente não clicada
 */
void desenhar_resultado(int celulas, int gabarito[][celulas], int prova[][celulas], int cordenada)
{
    int linha, coluna, x, y;
    for (linha = 0; linha < celulas; linha++)
    {
        for (coluna = 0; coluna < celulas; coluna++)
        {
            Color cor = GRAY;
            if (gabarito[linha][coluna] == 1 && prova[linha][coluna] == 0)
            {
                cor = BLUE; // Era para ser clicado, mas não foi
            }
            else if (gabarito[linha][coluna] == 0 && prova[linha][coluna] == 1)
            {
                cor = RED; // Não era para ser clicado, mas foi
            }
            else if (gabarito[linha][coluna] == 1 && prova[linha][coluna] == 1)
            {
                cor = GREEN; // Acertou
            }
            else if (gabarito[linha][coluna] == 0 && prova[linha][coluna] == 0)
            {
                cor = GRAY; // Acertou não clicando
            }
            x = cordenada + coluna * 51;
            y = cordenada + linha * 51;
            DrawRectangle(x, y, 50, 50, cor);
        }
    }
}

/**
 * @brief Gera um número aleatório inteiro entre 0 e i-1.
 *
 * @param i Limite superior para geração do número aleatório
 * @return Um número inteiro aleatório no intervalo [0, i-1]
 *
 * @note Usa a função rand() para gerar o número aleatório
 */
int gerador_de_numeros(int i)
{
    int n;
    n = rand() % i;
    return n;
}

/**
 * @brief Limpa uma matriz bidimensional, definindo todos os seus elementos como zero.
 *
 * @param tamanho Dimensão da matriz quadrada
 * @param matriz Ponteiro para a matriz a ser limpa
 */
void limpar_matriz(int tamanho, int matriz[][tamanho])
{
    int linha, coluna;
    /* Loop para preencher as matrizes com 0 */
    for(linha = 0; linha<tamanho; linha++)
    {
        for(coluna = 0; coluna<tamanho; coluna++)
        {
            matriz[linha][coluna] = 0;
        } /* fim do loop das colunas */
    } /* fim do loop das linhas */
}

/**
 * @brief Preenche aleatoriamente uma matriz com o número de células iguais à fase atual.
 *
 * @param fase Número da fase atual, que determina quantas células serão preenchidas
 * @param tamanho Dimensão da matriz quadrada
 * @param matriz Ponteiro para a matriz a ser aleatorizada
 *
 * @note Preenche células aleatorias que estão vazias.
 */
void aleatorizar_tabuleiro(int fase, int tamanho, int matriz[][tamanho])
{
    int linha_aleatoria, coluna_aleatoria, i;
    for(i=0; i<fase;)
    {
        linha_aleatoria = gerador_de_numeros(tamanho);
        coluna_aleatoria = gerador_de_numeros(tamanho);
        if (matriz[linha_aleatoria][coluna_aleatoria] == 0)
        {
            matriz[linha_aleatoria][coluna_aleatoria] = 1;
            i++;
        }
    }
}

/**
 * @brief Determina o tamanho do tabuleiro com base no número da fase.
 *
 * @param fase Número da fase atual do jogo
 * @return Tamanho do tabuleiro (4, 5 ou 6) dependendo do número da fase
 *
 * @note O tamanho do tabuleiro aumenta progressivamente com o avanço das fases
 */
int definir_tamanho(int fase)
{
    int tamanho;
    tamanho = (fase <= 5)? 4 : (fase <= 10)? 5 : 6;
    return tamanho;
}

/**
 * @brief Desenha o contador de tempo regressivo na tela de gabarito.
 *
 * @param tempo_atual Tempo atual do sistema.
 * @param tempo_inicio Tempo de início do contador.
 *
 * @note Desenha a barra de progresso azul e o contador regressivo de 4s.
 */
void desenhar_contador(int tempo_atual, int tempo_inicio)
{
    int tempo_decorrido, progresso, countdown;
    tempo_decorrido = tempo_atual - tempo_inicio;
    progresso = tempo_decorrido * 50;
    countdown = 4 - tempo_decorrido;
    DrawRectangle(150, 400, progresso, 20, BLUE);
    DrawRectangleLines(150, 400, 200, 20, DARKGRAY);
    DrawText(TextFormat("%d", countdown), 250, 400, 20, WHITE);
}

/**
 * @brief Função para controlar o temporizador.
 *
 * @param tempo Tempo decorrido.
 * @param pt_tempoi Ponteiro para o tempo inicial.
 * @param pt_estado Ponteiro para o estado do jogo.
 *
 * @note Esta função calcula o tempo decorrido e atualiza o estado do jogo quando o tempo se esgota.
 */
void temporizador(int tempo, int *pt_tempoi, int *pt_estado)
{
    int tempo_atual, calculo;
    tempo_atual = time(NULL);
    calculo = (*pt_tempoi + tempo) - tempo_atual;
    //printf("%d %d %d \n", *pt_tempoi, tempo_atual, calculo);
    if(calculo <= 0)
    {
        *pt_estado = *pt_estado + 1;
        *pt_tempoi = time(NULL);
    }
}
