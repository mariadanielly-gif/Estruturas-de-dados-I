/*
 * ATIVIDADE 6 - Manipulação de Arquivos (texto e binário) com Raylib
 *
 * Este programa trabalha com:
 *
 * 1) ARQUIVO DE TEXTO:
 *    "placar.txt"
 *    - Guarda o histórico das pontuações.
 *    - Usa fprintf() para escrever.
 *    - Usa fscanf() para ler.
 *
 * 2) ARQUIVO BINÁRIO:
 *    "save.bin"
 *    - Guarda o estado completo da partida.
 *    - Guarda a pontuação.
 *    - Guarda todas as entidades do jogo.
 *    - Usa fwrite() para salvar.
 *    - Usa fread() para carregar.
 *
 * Conceitos utilizados:
 *    - fopen()
 *    - fclose()
 *    - fprintf()
 *    - fscanf()
 *    - fwrite()
 *    - fread()
 *    - malloc()
 *    - free()
 *    - ponteiros
 *    - struct
 *    - enum
 *    - union
 *    - Raylib
 *
 * Teclas:
 *    SETAS -> movimentam o jogador
 *    F5    -> salva a pontuação no placar.txt
 *    F6    -> salva o jogo no save.bin
 *    F9    -> carrega o jogo do save.bin
 *    ESC   -> sai do jogo
 *
 * Compilação Linux:
 *
 * gcc atividade6.c -o atividade6 -lraylib -lm -lpthread -ldl -lrt -lX11
 *
 * No MSYS2 UCRT64, dependendo da configuração:
 *
 * gcc atividade6.c -o atividade6.exe $(pkg-config --cflags --libs raylib)
 */

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* ---------------------------------------------------------
   CONSTANTES DO PROGRAMA
   --------------------------------------------------------- */

#define LARGURA_JANELA 800
#define ALTURA_JANELA 600

#define RAIO_JOGADOR 20.0f

#define MAX_ENTIDADES 30

#define TOTAL_INIMIGOS 5
#define TOTAL_ITENS 6

#define ARQUIVO_PLACAR "placar.txt"
#define ARQUIVO_SAVE "save.bin"


/* ---------------------------------------------------------
   ENUM
   Define os tipos de entidades existentes no jogo.
   --------------------------------------------------------- */

typedef enum
{
    ENTIDADE_JOGADOR,
    ENTIDADE_INIMIGO,
    ENTIDADE_ITEM

} TipoEntidade;


/* ---------------------------------------------------------
   UNION
   Uma entidade pode ter:
   - dano, quando for inimigo;
   - valor, quando for item.
   --------------------------------------------------------- */

typedef union
{
    int dano;
    int valor;

} ExtraEntidade;


/* ---------------------------------------------------------
   STRUCT ENTIDADE
   Guarda todas as informações de uma entidade.
   --------------------------------------------------------- */

typedef struct
{
    TipoEntidade tipo;

    Vector2 pos;

    float raio;

    int vida;

    Color cor;

    ExtraEntidade extra;

} Entidade;


/* ---------------------------------------------------------
   VETOR GLOBAL DE PONTEIROS
   --------------------------------------------------------- */

Entidade *vetorEntidades[MAX_ENTIDADES];

int totalEntidades = 0;


/* =========================================================
   FUNÇÃO: criarEntidade
   Cria uma nova entidade dinamicamente.
   ========================================================= */

Entidade *criarEntidade(TipoEntidade tipo, Vector2 pos)
{
    /* Aloca memória para uma Entidade */
    Entidade *e = (Entidade *)malloc(sizeof(Entidade));

    /* Verifica se a alocação funcionou */
    if (e == NULL)
    {
        return NULL;
    }

    /* Define o tipo da entidade */
    e->tipo = tipo;

    /* Define a posição */
    e->pos = pos;

    /* Define o raio de acordo com o tipo */
    e->raio =
        (tipo == ENTIDADE_JOGADOR) ? RAIO_JOGADOR :
        (tipo == ENTIDADE_INIMIGO) ? 15.0f :
        8.0f;


    /* Define as características de cada tipo */
    switch (tipo)
    {
        /* ---------------- JOGADOR ---------------- */

        case ENTIDADE_JOGADOR:

            e->vida = 100;

            e->cor = BLUE;

            break;


        /* ---------------- INIMIGO ---------------- */

        case ENTIDADE_INIMIGO:

            e->vida = 40;

            e->cor = MAROON;

            /* Define dano aleatório */
            e->extra.dano = GetRandomValue(5, 15);

            break;


        /* ---------------- ITEM ---------------- */

        case ENTIDADE_ITEM:

            e->vida = 1;

            e->cor = GOLD;

            /* Define valor aleatório */
            e->extra.valor = GetRandomValue(5, 20);

            break;
    }


    /* Retorna o endereço da entidade criada */
    return e;
}


/* =========================================================
   FUNÇÃO: adicionarEntidade
   Adiciona uma entidade ao vetor.
   ========================================================= */

void adicionarEntidade(Entidade *e)
{
    /* Verifica se o ponteiro é válido */
    if (e == NULL)
    {
        return;
    }

    /* Verifica se o vetor está cheio */
    if (totalEntidades >= MAX_ENTIDADES)
    {
        free(e);
        return;
    }

    /* Coloca a entidade no vetor */
    vetorEntidades[totalEntidades] = e;

    /* Aumenta a quantidade */
    totalEntidades++;
}


/* =========================================================
   FUNÇÃO: removerEntidade
   Remove uma entidade do vetor.
   ========================================================= */

void removerEntidade(int indice)
{
    /* Verifica se o índice é válido */
    if (indice < 0 || indice >= totalEntidades)
    {
        return;
    }

    /* Libera a memória da entidade */
    free(vetorEntidades[indice]);

    /*
     * Coloca a última entidade na posição removida.
     * Isso evita deslocar todo o vetor.
     */
    vetorEntidades[indice] =
        vetorEntidades[totalEntidades - 1];

    /* Diminui a quantidade de entidades */
    totalEntidades--;
}


/* =========================================================
   FUNÇÃO: liberarTodasEntidades
   Libera toda a memória alocada.
   ========================================================= */

void liberarTodasEntidades(void)
{
    int i;

    for (i = 0; i < totalEntidades; i++)
    {
        free(vetorEntidades[i]);
    }

    totalEntidades = 0;
}


/* =========================================================
   FUNÇÃO: colidiu
   Verifica se duas entidades colidiram.
   ========================================================= */

bool colidiu(Entidade *a, Entidade *b)
{
    float dx;
    float dy;
    float distancia;

    dx = a->pos.x - b->pos.x;

    dy = a->pos.y - b->pos.y;

    distancia = sqrtf(dx * dx + dy * dy);

    return distancia <= (a->raio + b->raio);
}


/* =========================================================
   FUNÇÃO: desenharEntidade
   Desenha uma entidade na tela.
   ========================================================= */

void desenharEntidade(Entidade *e)
{
    /* Desenha o círculo */
    DrawCircleV(e->pos, e->raio, e->cor);

    /* Se for inimigo, mostra a vida */
    if (e->tipo == ENTIDADE_INIMIGO)
    {
        DrawText(
            TextFormat("%d", e->vida),
            e->pos.x - 8,
            e->pos.y - 26,
            14,
            BLACK
        );
    }
}


/* =========================================================
   ARQUIVO DE TEXTO
   ========================================================= */


/* =========================================================
   FUNÇÃO: salvarPlacarTexto
   Salva uma pontuação no arquivo placar.txt.
   ========================================================= */

void salvarPlacarTexto(int pontuacao)
{
    FILE *arquivo;

    /*
     * "a" significa:
     * abrir para escrita adicionando no final.
     */
    arquivo = fopen(ARQUIVO_PLACAR, "a");

    /* Verifica se conseguiu abrir */
    if (arquivo == NULL)
    {
        return;
    }

    /* Escreve a pontuação */
    fprintf(arquivo, "%d\n", pontuacao);

    /* Fecha o arquivo */
    fclose(arquivo);
}


/* =========================================================
   FUNÇÃO: lerMelhorPontuacao
   Lê todas as pontuações e encontra a maior.
   ========================================================= */

int lerMelhorPontuacao(void)
{
    FILE *arquivo;

    int melhor = 0;

    int valor = 0;


    /*
     * "r" significa leitura.
     */
    arquivo = fopen(ARQUIVO_PLACAR, "r");

    /* Se não existir arquivo, retorna 0 */
    if (arquivo == NULL)
    {
        return 0;
    }


    /*
     * Continua lendo enquanto conseguir
     * encontrar um número inteiro.
     */
    while (fscanf(arquivo, "%d", &valor) == 1)
    {
        /* Verifica se encontrou uma pontuação maior */
        if (valor > melhor)
        {
            melhor = valor;
        }
    }


    /* Fecha o arquivo */
    fclose(arquivo);

    return melhor;
}


/* =========================================================
   ARQUIVO BINÁRIO
   ========================================================= */


/* =========================================================
   FUNÇÃO: salvarJogoBinario
   Salva:
   - quantidade de entidades;
   - pontuação;
   - todas as entidades.
   ========================================================= */

bool salvarJogoBinario(int pontuacao)
{
    FILE *arquivo;

    int i;


    /*
     * "wb":
     * w = escrita
     * b = binário
     */
    arquivo = fopen(ARQUIVO_SAVE, "wb");

    /* Verifica se conseguiu abrir */
    if (arquivo == NULL)
    {
        return false;
    }


    /* -----------------------------------------
       Salva a quantidade de entidades
       ----------------------------------------- */

    if (fwrite(
            &totalEntidades,
            sizeof(int),
            1,
            arquivo
        ) != 1)
    {
        fclose(arquivo);

        return false;
    }


    /* -----------------------------------------
       Salva a pontuação
       ----------------------------------------- */

    if (fwrite(
            &pontuacao,
            sizeof(int),
            1,
            arquivo
        ) != 1)
    {
        fclose(arquivo);

        return false;
    }


    /* -----------------------------------------
       Salva todas as entidades
       ----------------------------------------- */

    for (i = 0; i < totalEntidades; i++)
    {
        if (fwrite(
                vetorEntidades[i],
                sizeof(Entidade),
                1,
                arquivo
            ) != 1)
        {
            fclose(arquivo);

            return false;
        }
    }


    /* Fecha o arquivo */
    fclose(arquivo);

    return true;
}


/* =========================================================
   FUNÇÃO: carregarJogoBinario
   Carrega:
   - quantidade de entidades;
   - pontuação;
   - todas as entidades.
   ========================================================= */

bool carregarJogoBinario(int *pontuacao)
{
    FILE *arquivo;

    int totalSalvo;

    int i;


    /*
     * "rb":
     * r = leitura
     * b = binário
     */
    arquivo = fopen(ARQUIVO_SAVE, "rb");

    /* Se não encontrou o arquivo */
    if (arquivo == NULL)
    {
        return false;
    }


    /* -----------------------------------------
       Lê a quantidade de entidades
       ----------------------------------------- */

    if (fread(
            &totalSalvo,
            sizeof(int),
            1,
            arquivo
        ) != 1)
    {
        fclose(arquivo);

        return false;
    }


    /* -----------------------------------------
       Verifica se a quantidade é válida
       ----------------------------------------- */

    if (totalSalvo < 1 || totalSalvo > MAX_ENTIDADES)
    {
        fclose(arquivo);

        return false;
    }


    /* -----------------------------------------
       Lê a pontuação
       ----------------------------------------- */

    if (fread(
            pontuacao,
            sizeof(int),
            1,
            arquivo
        ) != 1)
    {
        fclose(arquivo);

        return false;
    }


    /* -----------------------------------------
       Libera o jogo atual
       ----------------------------------------- */

    liberarTodasEntidades();


    /* -----------------------------------------
       Carrega as entidades
       ----------------------------------------- */

    for (i = 0; i < totalSalvo; i++)
    {
        Entidade *e;

        /* Aloca memória para a entidade */
        e = (Entidade *)malloc(sizeof(Entidade));

        /* Verifica a alocação */
        if (e == NULL)
        {
            liberarTodasEntidades();

            fclose(arquivo);

            return false;
        }


        /* Lê a entidade do arquivo */
        if (fread(
                e,
                sizeof(Entidade),
                1,
                arquivo
            ) != 1)
        {
            free(e);

            liberarTodasEntidades();

            fclose(arquivo);

            return false;
        }


        /* Adiciona a entidade ao vetor */
        adicionarEntidade(e);
    }


    /* Fecha o arquivo */
    fclose(arquivo);

    return true;
}


/* =========================================================
   FUNÇÃO PRINCIPAL
   ========================================================= */

int main(void)
{
    /* Inicializa a semente dos números aleatórios */
    srand((unsigned int)time(NULL));


    /* =====================================================
       INICIA A JANELA DO RAYLIB
       ===================================================== */

    InitWindow(
        LARGURA_JANELA,
        ALTURA_JANELA,
        "Atividade 6 - Manipulacao de Arquivos"
    );


    /* Define 60 quadros por segundo */
    SetTargetFPS(60);


    /* =====================================================
       CRIA O JOGADOR
       ===================================================== */

    Entidade *jogador;

    jogador = criarEntidade(
        ENTIDADE_JOGADOR,
        (Vector2){
            LARGURA_JANELA / 2.0f,
            ALTURA_JANELA / 2.0f
        }
    );


    /* Adiciona o jogador ao vetor */
    adicionarEntidade(jogador);


    /* =====================================================
       CRIA OS INIMIGOS
       ===================================================== */

    for (int i = 0; i < TOTAL_INIMIGOS; i++)
    {
        Vector2 pos;

        pos.x = GetRandomValue(
            30,
            LARGURA_JANELA - 30
        );

        pos.y = GetRandomValue(
            30,
            ALTURA_JANELA - 30
        );

        adicionarEntidade(
            criarEntidade(
                ENTIDADE_INIMIGO,
                pos
            )
        );
    }


    /* =====================================================
       CRIA OS ITENS
       ===================================================== */

    for (int i = 0; i < TOTAL_ITENS; i++)
    {
        Vector2 pos;

        pos.x = GetRandomValue(
            30,
            LARGURA_JANELA - 30
        );

        pos.y = GetRandomValue(
            30,
            ALTURA_JANELA - 30
        );

        adicionarEntidade(
            criarEntidade(
                ENTIDADE_ITEM,
                pos
            )
        );
    }


    /* =====================================================
       VARIÁVEIS DO JOGO
       ===================================================== */

    int pontuacao = 0;

    /*
     * Lê o recorde do arquivo de texto.
     */
    int melhorPontuacao = lerMelhorPontuacao();


    /*
     * Mensagem exibida na tela.
     */
    char mensagem[64] = "";


    /*
     * Controla quanto tempo a mensagem fica na tela.
     */
    float tempoMensagem = 0.0f;


    /* =====================================================
       LOOP PRINCIPAL
       ===================================================== */

    while (!WindowShouldClose())
    {
        /* -------------------------------------------------
           MOVIMENTAÇÃO DO JOGADOR
           ------------------------------------------------- */

        float vel = 250.0f * GetFrameTime();


        if (IsKeyDown(KEY_RIGHT))
        {
            jogador->pos.x += vel;
        }


        if (IsKeyDown(KEY_LEFT))
        {
            jogador->pos.x -= vel;
        }


        if (IsKeyDown(KEY_UP))
        {
            jogador->pos.y -= vel;
        }


        if (IsKeyDown(KEY_DOWN))
        {
            jogador->pos.y += vel;
        }


        /* -------------------------------------------------
           VERIFICA COLISÕES
           ------------------------------------------------- */

        for (int i = 1; i < totalEntidades; i++)
        {
            Entidade *e = vetorEntidades[i];


            /* Se não colidiu, passa para a próxima */
            if (!colidiu(jogador, e))
            {
                continue;
            }


            /* ---------------------------------------------
               COLISÃO COM ITEM
               --------------------------------------------- */

            if (e->tipo == ENTIDADE_ITEM)
            {
                /*
                 * Soma o valor do item à pontuação.
                 */
                pontuacao += e->extra.valor;


                /*
                 * Remove o item.
                 */
                removerEntidade(i);


                /*
                 * Como o último elemento foi colocado
                 * na posição removida, voltamos uma posição.
                 */
                i--;
            }


            /* ---------------------------------------------
               COLISÃO COM INIMIGO
               --------------------------------------------- */

            else if (e->tipo == ENTIDADE_INIMIGO)
            {
                /*
                 * Diminui a vida do jogador.
                 */
                jogador->vida -= e->extra.dano;


                /*
                 * Evita vida negativa.
                 */
                if (jogador->vida < 0)
                {
                    jogador->vida = 0;
                }
            }
        }


        /* =================================================
           F5 - SALVAR PLACAR
           ================================================= */

        if (IsKeyPressed(KEY_F5))
        {
            /* Salva a pontuação no arquivo de texto */
            salvarPlacarTexto(pontuacao);


            /* Atualiza o recorde */
            if (pontuacao > melhorPontuacao)
            {
                melhorPontuacao = pontuacao;
            }


            /* Mostra mensagem */
            TextCopy(
                mensagem,
                "Placar salvo em placar.txt!"
            );


            tempoMensagem = 2.0f;
        }


        /* =================================================
           F6 - SALVAR JOGO
           ================================================= */

        if (IsKeyPressed(KEY_F6))
        {
            /*
             * Agora o save guarda:
             * - entidades
             * - pontuação
             */
            bool ok = salvarJogoBinario(pontuacao);


            if (ok)
            {
                TextCopy(
                    mensagem,
                    "Jogo salvo em save.bin!"
                );
            }
            else
            {
                TextCopy(
                    mensagem,
                    "Erro ao salvar save.bin!"
                );
            }


            tempoMensagem = 2.0f;
        }


        /* =================================================
           F9 - CARREGAR JOGO
           ================================================= */

        if (IsKeyPressed(KEY_F9))
        {
            /*
             * Carrega as entidades e a pontuação.
             */
            bool ok = carregarJogoBinario(&pontuacao);


            if (ok)
            {
                /*
                 * As entidades antigas foram liberadas.
                 * Então precisamos atualizar o ponteiro
                 * para apontar para o novo jogador.
                 */
                jogador = vetorEntidades[0];


                /*
                 * Atualiza o recorde caso a pontuação
                 * carregada seja maior.
                 */
                if (pontuacao > melhorPontuacao)
                {
                    melhorPontuacao = pontuacao;
                }


                TextCopy(
                    mensagem,
                    "Jogo carregado de save.bin!"
                );
            }
            else
            {
                TextCopy(
                    mensagem,
                    "Erro ao carregar save.bin!"
                );
            }


            tempoMensagem = 2.0f;
        }


        /* =================================================
           CONTROLA O TEMPO DA MENSAGEM
           ================================================= */

        if (tempoMensagem > 0.0f)
        {
            tempoMensagem -= GetFrameTime();
        }


        /* =================================================
           DESENHO
           ================================================= */

        BeginDrawing();


        /* Limpa a tela */
        ClearBackground(RAYWHITE);


        /* -------------------------------------------------
           DESENHA TODAS AS ENTIDADES
           ------------------------------------------------- */

        for (int i = 0; i < totalEntidades; i++)
        {
            desenharEntidade(
                vetorEntidades[i]
            );
        }


        /* -------------------------------------------------
           INFORMAÇÕES DO JOGO
           ------------------------------------------------- */

        DrawText(
            TextFormat(
                "Vida: %d   Pontuacao: %d   Recorde: %d",
                jogador->vida,
                pontuacao,
                melhorPontuacao
            ),
            10,
            10,
            22,
            DARKGRAY
        );


        /* -------------------------------------------------
           INSTRUÇÕES
           ------------------------------------------------- */

        DrawText(
            "F5 salva placar (texto) | F6 salva jogo (binario) | F9 carrega jogo",
            10,
            34,
            18,
            GRAY
        );


        DrawText(
            "Setas movem o jogador | ESC sai",
            10,
            ALTURA_JANELA - 25,
            16,
            GRAY
        );


        /* -------------------------------------------------
           MOSTRA MENSAGEM
           ------------------------------------------------- */

        if (tempoMensagem > 0.0f)
        {
            DrawText(
                mensagem,
                10,
                58,
                20,
                DARKGREEN
            );
        }


        /* Finaliza o desenho */
        EndDrawing();
    }


    /* =====================================================
       FINALIZAÇÃO
       ===================================================== */

    /*
     * Libera toda a memória das entidades.
     */
    liberarTodasEntidades();


    /*
     * Fecha a janela do Raylib.
     */
    CloseWindow();


    return 0;
}