#include "raylib.h"
#include <stdio.h> 
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define MAX_INPUT_CHARS 9
#define MAX_PREGUNTA_LENGTH 100
#define MAX_OPCIONES 4
#define MAX_OPCION_LENGTH 50

typedef struct
{
    int id; 
    char usuario[MAX_INPUT_CHARS+1];
    int ganancias;
}Usuario;

typedef struct {
    char pregunta[100];
    char opcionA[50];
    char opcionB[50];
    char opcionC[50];
    char opcionD[50];
    char respuestaCorrecta;
} Pregunta;

typedef enum GameScreen
{
    LOGO = 0,
    MENU,
    USERNAME,
    TUTORIAL,
    GAMEPLAY,
    POINTS,
    ENDING,
    CREDITS
} GameScreen;


bool IsValidChar(int key)
{
    return ((key >= 48 && key <= 57) || // 0-9
            (key >= 65 && key <= 90) || // A-Z
            (key >= 97 && key <= 122)|| // a-z
            (key == 8) ); 
}

void createFile()
{
    FILE *file;

    file = fopen("reg.txt", "r");
    if(file == NULL)
    {
        file = fopen("reg.txt", "w");
    }
    fclose(file);
}

void loadFile(Usuario usuarios[], int *numUsuarios)
{
    FILE *archivo_leer = fopen("reg.txt", "r");
    if (archivo_leer == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    char linea[150];
    *numUsuarios = 0;
    while (fgets(linea, sizeof(linea), archivo_leer)) {
        if (sscanf(linea, "%s %d", usuarios[*numUsuarios].usuario, &usuarios[*numUsuarios].ganancias) == 2) {
            usuarios[*numUsuarios].id = *numUsuarios + 1;
            (*numUsuarios)++;
        }
    }

    fclose(archivo_leer);
}

int main(void)
{
    srand(time(NULL));

    Usuario usuarios[100];
    int numUsuarios = 0;

    createFile();
    loadFile(usuarios, &numUsuarios);
    
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "QUIEN QUIERE SER MILLONARIO: JUEGO");

    GameScreen currentScreen = LOGO;

    int framesCounter = 0; 

    SetTargetFPS(60); 

    InitAudioDevice();
    Sound introSound = LoadSound("resources/intro.wav");
    Sound startSound = LoadSound("resources/pregunta/start.wav");
    Sound loopSound = LoadSound("resources/pregunta/loop.wav");

    Sound correctaSound = LoadSound("resources/pregunta/correcta.wav");
    Sound incorrectaSound = LoadSound("resources/pregunta/incorrecta.wav");

    Sound endgameSound = LoadSound("resources/pregunta/endgame.wav");

    bool introSoundPlayed = false;
    bool startSoundPlayed = false;
    bool loopSoundPlayed = false;
    bool endgameSoundPlayed = false;

    bool incorrectaSoundPlayed = false;
    bool correctaSoundPlayed = false;


/*****************************************************************************/


    //VARIABLES LOGO
    Texture2D loadingTexture = LoadTexture("resources/cargando.png");
    const char *loadingText = "Cargando";
    int textSize = MeasureText(loadingText, 20);
    Vector2 textPosition = {(float)(screenWidth - textSize) / 2 + 20, (float)screenHeight / 2 + 50 + 130};
    float timer = 0.0f;
    float interval = 0.5f;
    int dotCount = 0;
    const int maxDots = 3;


    //VARIABLES MENU
    Rectangle tutorialButtonBounds = {screenWidth / 2.0f - 60, screenHeight / 2.0f + 55, 144, 50};
    Rectangle pointsButtonBounds = {screenWidth / 2.0f - 105, screenHeight / 2.0f + 110, 229, 46};
    Rectangle creditosButtonBounds = {screenWidth / 2.0f -60, screenHeight / 2.0f + 162, 151, 46};
    Texture2D menuTexture = LoadTexture("resources/menu.png");
    Texture2D tutorialButtonNormal = LoadTexture("resources/botones/jugar_normal.png");
    Texture2D tutorialButtonHover = LoadTexture("resources/botones/jugar_hover.png");
    Texture2D pointsButtonNormal = LoadTexture("resources/botones/puntos_normal.png");
    Texture2D pointsButtonHover = LoadTexture("resources/botones/puntos_hover.png");
    Texture2D creditsButtonNormal = LoadTexture("resources/botones/credits_normal.png");
    Texture2D creditsButtonHover = LoadTexture("resources/botones/credits_hover.png");
    bool creditsButtonHovered = false;
    bool pointsButtonHovered = false;
    bool tutorialButtonHovered = false;


    //VARIABLES USERNAME
    char name[MAX_INPUT_CHARS + 1] = "\0";
    int letterCount = 0;
    Rectangle textBox = {screenWidth / 2.0f - 200, 284, 225, 50};
    bool mouseOnText = false; 
    Texture2D usernameTexture = LoadTexture("resources/escribetunombre.png");
    Rectangle proceedButtonBounds = {screenWidth / 2.0f - 50 + 180, 270, 110, 80}; // (x, y, width, height)


    //VARIABLES TUTORIAL
    Texture2D tutorialTexture = LoadTexture("resources/tutorial.png");
    Rectangle buttonBounds = {650, 300, 100, 80};


    //VARIABLES GAMEPLAY
    Texture2D gameplayTexture1 = LoadTexture("resources/preguntas/1.png");
    Texture2D gameplayTexture2 = LoadTexture("resources/preguntas/2.png");
    Texture2D gameplayTexture3 = LoadTexture("resources/preguntas/3.png");
    Texture2D gameplayTexture4 = LoadTexture("resources/preguntas/4.png");
    Texture2D gameplayTexture5 = LoadTexture("resources/preguntas/5.png");
    Texture2D gameplayTexture6 = LoadTexture("resources/preguntas/6.png");
    Texture2D gameplayTexture7 = LoadTexture("resources/preguntas/7.png");
    Texture2D gameplayTexture8 = LoadTexture("resources/preguntas/8.png");
    Texture2D gameplayTexture9 = LoadTexture("resources/preguntas/9.png");
    Texture2D gameplayTexture10 = LoadTexture("resources/preguntas/10.png");
    Texture2D gameplayTexture11 = LoadTexture("resources/preguntas/11.png");
    Texture2D gameplayTexture12 = LoadTexture("resources/preguntas/12.png");
    Texture2D gameplayTexture13 = LoadTexture("resources/preguntas/13.png");
    Texture2D gameplayTexture14 = LoadTexture("resources/preguntas/14.png");
    Texture2D gameplayTexture15 = LoadTexture("resources/preguntas/15.png");
    Texture2D gameplayTexture16 = LoadTexture("resources/preguntas/16.png");
    Texture2D comodinButtonNormal = LoadTexture("resources/botones/comodin_normal.png");
    Texture2D comodinButtonHover = LoadTexture("resources/botones/comodin_hover.png");
    Texture2D comodinButtonClicked = LoadTexture("resources/botones/comodin_click.png");
    Texture2D pregunta_hover = LoadTexture("resources/hover_pregunta.png");

    Rectangle opcAbuttonBounds = {40, 320, 227, 46};
    Rectangle opcBbuttonBounds = {307, 320, 227, 46};
    Rectangle opcCbuttonBounds = {40, 383, 227, 46};
    Rectangle opcDbuttonBounds = {307, 383, 227, 46};

    int pregunta_actual = 1;
    int victoria = 0;
    int derrota = 0;
    char opc_correcta_actual;
    char opc_correcta_usuario;
    int respondido = 0;

    bool comodin_clicked = false;
    bool comodinButtonHovered = false;
    bool opcAButtonHovered = false;
    bool opcBButtonHovered = false;
    bool opcCButtonHovered = false;
    bool opcDButtonHovered = false;

    bool juego_actual_subido = false;

    Rectangle comodinButtonBounds = {screenWidth / 2.0f - 80, screenHeight / 2.0f - 100, 144, 50};

    Pregunta preguntas[16] = {
        {"¿Cuál es la capital de Francia?", "A) Berlín", "B) Madrid", "C) París", "D) Roma", 'C'},
        {"¿Cuál es el océano más grande del mundo?", "A) Atlántico", "B) Índico", "C) Ártico", "D) Pacífico", 'D'},
        {"¿Quién pintó la Mona Lisa?", "A) Vincent van Gogh", "B) Pablo Picasso", "C) Leonardo da Vinci", "D) Rembrandt", 'C'},
        {"¿Cuál es el metal más ligero?", "A) Oro", "B) Plomo", "C) Litio", "D) Mercurio", 'C'},
        {"¿Cuál es el planeta más cercano al Sol?", "A) Venus", "B) Tierra", "C) Marte", "D) Mercurio", 'D'},
        {"¿Cuál es el país más grande \ndel mundo por área?", "A) Canadá", "B) China", "C) Rusia", "D) Estados Unidos", 'C'},
        {"¿Cuál es el río más largo del mundo?", "A) Amazonas", "B) Nilo", "C) Yangtsé", "D) Misisipi", 'B'},
        {"¿Cuál es el órgano más grande del\n cuerpo humano?", "A) Hígado", "B) Cerebro", "C) Piel", "D) Corazón", 'C'},
        {"¿Cuál es la moneda oficial de Japón?", "A) Yen", "B) Won", "C) Yuan", "D) Dólar", 'A'},
        {"¿Cuál es el idioma más hablado en el mundo?", "A) Inglés", "B) Mandarín", "C) Español", "D) Hindi", 'B'},
        {"¿Cuál es la ciudad más poblada del mundo?", "A) Tokio", "B) Nueva York", "C) São Paulo", "D) Mumbai", 'A'},
        {"¿En qué país se encuentra la Torre Eiffel?", "A) España", "B) Italia", "C) Francia", "D) Alemania", 'C'},
        {"¿Cuál es el desierto más grande del mundo?", "A) Sahara", "B) Gobi", "C) Kalahari", "D) Antártida", 'D'},
        {"¿Cuál es la capital de Australia?", "A) Sídney", "B) Melbourne", "C) Canberra", "D) Brisbane", 'C'},
        {"¿En qué año comenzó la \nPrimera Guerra Mundial?", "A) 1905", "B) 1914", "C) 1921", "D) 1939", 'B'},
        {"¿Cuál es la capital de Finlandia?", "A) Oslo", "B) Estocolmo", "C) Helsinki", "D) Copenhague", 'C'}
    };

    int comodin_opc_int; //a=0 b=1 c=2 d=3 respuesta correcta de la pregunta actual
    char comodin_opc_alternative_char; //char de la respuesta alternativa 
    int disponible_comodin = 1;


    //VARIABLES POINTS
    Texture2D pointsTexture = LoadTexture("resources/puntos.png");



    //VARIABLES ENDING

    Texture2D win = LoadTexture("resources/win.png");
    Texture2D gameover = LoadTexture("resources/gameover.png");

    bool archivo_guardado = false;

    int ganancias_temp = 0;

    int vect_ganancias[16] =
    {
        100,
        200,
        300,
        400,
        500,
        1000,
        2000,
        4000,
        8000,
        16000,
        32000,
        64000,
        125000,
        250000,
        500000,
        1000000
    };


    //VARIABLES CREDITS
    Texture2D creditsTexture = LoadTexture("resources/creditos.png");
    const char *creditsText[] = {
        "WHO WANTS TO BE MILLIONAIRE",
        "QUIEN QUIERE SER MILLONARIO",
        "son MARCAS REGISTRADAS creadas por David Briggs,",
        "Steven Knight y Mike Whitehill en 1999,",
        "el presente juego es una demo BASADA",
        "en dicho PRODUCTO pero no exactamente idéntica.",
        "",
        "Juego desarrollado por:",
        "Alejandra Miranda y Yaeli Kikunaga,",
        "estudiantes en la Facultad de Ingeniería,",
        "Arquitectura y Diseño en la",
        "Universidad Autónoma de Baja California,",
        "a cargo el Mtro. Pedro Nuñez Yépiz.",
        "",
        "Haz CLICK para REGRESAR al MENU."
    };
    int creditsLines = sizeof(creditsText) / sizeof(creditsText[0]);
    int creditsPositionY = screenHeight;



/*****************************************************************************/




    while (!WindowShouldClose())
    {
        timer += GetFrameTime();

        if (currentScreen == LOGO && timer >= interval)
        {
            timer = 0.0f;
            dotCount = (dotCount + 1) % (maxDots + 1);
        }

        if (currentScreen == MENU && !introSoundPlayed)
        {
            PlaySound(introSound);
            introSoundPlayed = true;
        }

        if (currentScreen == GAMEPLAY && !startSoundPlayed)
        {
            StopSound(introSound);
            PlaySound(startSound);
            startSoundPlayed = true;
            PlaySound(loopSound);
        }

        switch (currentScreen)
        {
        case LOGO:
        {
            framesCounter++;

            if (framesCounter > 260)
            {
                framesCounter = 0;
                currentScreen = MENU;
            }
        }
        break;
        case MENU:
        {

            archivo_guardado = false;

            pregunta_actual = 1;
            victoria = 0; //
            derrota = 0; //
            respondido = 0; //

            char name[MAX_INPUT_CHARS + 1] = "\0";
            int letterCount = 0;

            bool mouseOnText = false;

            ganancias_temp = 0;

            comodin_clicked = false;
            disponible_comodin = 1;

            juego_actual_subido = false;


            startSoundPlayed = false;
            loopSoundPlayed = false;
            endgameSoundPlayed = false;

            if(IsSoundPlaying(loopSound))
            {
                StopSound(loopSound);
            }

            if(IsSoundPlaying(endgameSound))
            {
                StopSound(endgameSound);
            }


            // Detectar el estado del mouse en los botones
            if (CheckCollisionPointRec(GetMousePosition(), creditosButtonBounds))
            {
                creditsButtonHovered = true;
            }
            else
            {
                creditsButtonHovered = false;
            }

            if (CheckCollisionPointRec(GetMousePosition(), pointsButtonBounds))
            {
                pointsButtonHovered = true;
            }
            else
            {
                pointsButtonHovered = false;
            }

            if (CheckCollisionPointRec(GetMousePosition(), tutorialButtonBounds))
            {
                tutorialButtonHovered = true;
            }
            else
            {
                tutorialButtonHovered = false;
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 mousePoint = GetMousePosition();
                if (CheckCollisionPointRec(mousePoint, tutorialButtonBounds))
                {
                    currentScreen = TUTORIAL; 
                }
                else if (CheckCollisionPointRec(mousePoint, pointsButtonBounds))
                {
                    currentScreen = POINTS; 
                }
                else if (CheckCollisionPointRec(mousePoint, creditosButtonBounds))
                {
                    currentScreen = CREDITS; 
                }
            }
        }
        break;
        case USERNAME:
        {
            if (CheckCollisionPointRec(GetMousePosition(), textBox))
                mouseOnText = true;
            else
                mouseOnText = false;

            if (mouseOnText)
            {
                SetMouseCursor(MOUSE_CURSOR_IBEAM);

                int key = GetCharPressed();
                while (key > 0)
                {
                    if (IsValidChar(key) && (letterCount < MAX_INPUT_CHARS))
                    {
                        name[letterCount] = (char)key;
                        name[letterCount + 1] = '\0'; 
                        letterCount++;
                    }

                    key = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE)) 
                {
                    letterCount--;
                    if (letterCount < 0)
                        letterCount = 0;
                    name[letterCount] = '\0';
                }
            }
            else
            {
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            }

            if (mouseOnText)
            {
                framesCounter++;
            }
            else
            {
                framesCounter = 0;
            }

            if (strlen(name) > 0 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 mousePoint = GetMousePosition();
                if (CheckCollisionPointRec(mousePoint, proceedButtonBounds))
                {
                    currentScreen = GAMEPLAY; 
                }
            }
        }
        break;
        case TUTORIAL:
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 mousePoint = GetMousePosition();
                if (CheckCollisionPointRec(mousePoint, buttonBounds))
                {
                    currentScreen = USERNAME; 
                }
            }
        }
        break;
        case GAMEPLAY:
        {

            if(!IsSoundPlaying(loopSound))
            {
                PlaySound(loopSound);
            }

            //copiamos la respuesta actual
            opc_correcta_actual = preguntas[pregunta_actual-1].respuestaCorrecta;

            switch (opc_correcta_actual)
            {
            case 'A':
                comodin_opc_int = 0; 
                comodin_opc_alternative_char = 'D';
                break;
            case 'B':
                comodin_opc_int = 1; 
                comodin_opc_alternative_char = 'C';
                break;
            case 'C':
                comodin_opc_int = 2; 
                comodin_opc_alternative_char = 'A';
                break;
            case 'D':
                comodin_opc_int = 3;
                comodin_opc_alternative_char = 'B'; 
                break;
            }
            

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 mousePoint = GetMousePosition();

                if (CheckCollisionPointRec(mousePoint, opcAbuttonBounds))
                {
                    respondido = 1;
                    opc_correcta_usuario = 'A';
                }
                if (CheckCollisionPointRec(mousePoint, opcBbuttonBounds))
                {
                    respondido = 1;
                    opc_correcta_usuario = 'B';
                }
                if (CheckCollisionPointRec(mousePoint, opcCbuttonBounds))
                {
                    respondido = 1;
                    opc_correcta_usuario = 'C';
                }
                if (CheckCollisionPointRec(mousePoint, opcDbuttonBounds))
                {
                    respondido = 1;
                    opc_correcta_usuario = 'D';
                }
            }

            if (respondido)
            {
                if(opc_correcta_actual == opc_correcta_usuario)
                {
                    victoria = 1;
                    PlaySound(correctaSound);

                    pregunta_actual++;
                    comodin_clicked = false;
                    respondido = 0;

                    if(!juego_actual_subido)
                    {
                        ganancias_temp = vect_ganancias[pregunta_actual-2];
                        usuarios[numUsuarios].ganancias = ganancias_temp;
                        strcpy(usuarios[numUsuarios].usuario, name);
                        usuarios[numUsuarios].id = numUsuarios + 1;
                        numUsuarios++;
                        juego_actual_subido = true;
                    }

                    if(pregunta_actual > 16)
                    {
                        currentScreen = ENDING;
                    }
                }
                else
                {
                    derrota = 1;
                    comodin_clicked = false;

                    framesCounter = 0;
                    respondido = 0;

                    if(!juego_actual_subido)
                    {
                        if(pregunta_actual > 1)
                        {
                            ganancias_temp = vect_ganancias[pregunta_actual-2];
                            usuarios[numUsuarios].ganancias = ganancias_temp;
                            strcpy(usuarios[numUsuarios].usuario, name);
                            usuarios[numUsuarios].id = numUsuarios + 1;
                            numUsuarios++;
                            juego_actual_subido = true;
                        }
                    }

                    currentScreen = ENDING;
                }
            }

            if (CheckCollisionPointRec(GetMousePosition(), opcAbuttonBounds))
            {
                opcAButtonHovered = true;
            }
            else
            {
                opcAButtonHovered = false;
            }

            if (CheckCollisionPointRec(GetMousePosition(), opcBbuttonBounds))
            {
                opcBButtonHovered = true;
            }
            else
            {
                opcBButtonHovered = false;
            }

            if (CheckCollisionPointRec(GetMousePosition(), opcCbuttonBounds))
            {
                opcCButtonHovered = true;
            }
            else
            {
                opcCButtonHovered = false;
            }

            if (CheckCollisionPointRec(GetMousePosition(), opcDbuttonBounds))
            {
                opcDButtonHovered = true;
            }
            else
            {
                opcDButtonHovered = false;
            }

            if (CheckCollisionPointRec(GetMousePosition(), comodinButtonBounds))
            {
                comodinButtonHovered = true;
            }
            else
            {
                comodinButtonHovered = false;
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !comodin_clicked && disponible_comodin)
            {
                Vector2 mousePoint = GetMousePosition();
                if (CheckCollisionPointRec(mousePoint, comodinButtonBounds))
                {
                    comodin_clicked = true;
                    disponible_comodin = 0;
                }
            }
        }
        break;
        case POINTS:
        {

            StopSound(loopSound);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                currentScreen = MENU;
            }
        }
        break;

        case ENDING:
            ClearBackground(BLACK);
            StopSound(loopSound);

            static bool sonidoReproducido = false;

            if(!sonidoReproducido) {
                if(pregunta_actual > 16) {
                    PlaySound(endgameSound);
                } else {
                    PlaySound(incorrectaSound);
                }
                sonidoReproducido = true;
            }

            if(!archivo_guardado)
            {
                if(pregunta_actual != 1)
                {
                    ganancias_temp = vect_ganancias[pregunta_actual-2];
                    FILE* archivo_escribir = fopen("reg.txt", "a");
                    if (archivo_escribir != NULL)
                    {
                        fprintf(archivo_escribir, "%s %d\n", name, ganancias_temp);
                        fclose(archivo_escribir);
                    }
                }

                archivo_guardado = true;
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                currentScreen = POINTS;
                sonidoReproducido = false;
            }
        break;

        case CREDITS:
        {
            ClearBackground(BLACK);
            StopSound(loopSound);
            StopSound(endgameSound);

            for (int i = 0; i < creditsLines; i++)
            {
                DrawText(creditsText[i], (screenWidth - MeasureText(creditsText[i], 20)) / 2, creditsPositionY + i * 30, 20, RAYWHITE);
            }

            creditsPositionY -= 0.5f;

            if (creditsPositionY + creditsLines * 30 < 0)
            {
                creditsPositionY = screenHeight;
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                currentScreen = MENU;
            }
        }
        break;


        default:
            break;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (currentScreen)
        {
        case LOGO:
        {
            ClearBackground(WHITE);
            DrawTexture(loadingTexture, (screenWidth - loadingTexture.width) / 2, (screenHeight - loadingTexture.height) / 2, WHITE);
            DrawText(TextFormat("%s%s", loadingText, (dotCount == 0 ? "" : (dotCount == 1 ? "." : (dotCount == 2 ? ".." : "...")))), textPosition.x, textPosition.y, 20, PURPLE);
        }
        break;
        case MENU:
        {
            ClearBackground(RAYWHITE);

            DrawTexture(menuTexture, (screenWidth - menuTexture.width) / 2, (screenHeight - menuTexture.height) / 2, WHITE);

            if (tutorialButtonHovered)
            {
                DrawTexture(tutorialButtonHover, tutorialButtonBounds.x, tutorialButtonBounds.y, WHITE);
            }
            else
            {
                DrawTexture(tutorialButtonNormal, tutorialButtonBounds.x, tutorialButtonBounds.y, WHITE);
            }

            if (pointsButtonHovered)
            {
                DrawTexture(pointsButtonHover, pointsButtonBounds.x, pointsButtonBounds.y, WHITE);
            }
            else
            {
                DrawTexture(pointsButtonNormal, pointsButtonBounds.x, pointsButtonBounds.y, WHITE);
            }

            if (creditsButtonHovered)
            {
                DrawTexture(creditsButtonHover, creditosButtonBounds.x, creditosButtonBounds.y, WHITE);
            }
            else
            {
                DrawTexture(creditsButtonNormal, creditosButtonBounds.x, creditosButtonBounds.y, WHITE);
            }
        }
        break;
        case USERNAME:
        {
            ClearBackground(RAYWHITE);
            DrawTexture(usernameTexture, (screenWidth - usernameTexture.width) / 2, (screenHeight - usernameTexture.height) / 2, WHITE);

            DrawText("HAZ CLICK SOBRE EL ESPACIO PARA COMENZAR A ESCRIBIR", 90, 20, 20, GRAY);

            DrawRectangleRec(textBox, BLANK);
            if (mouseOnText)
            {
                DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, BLANK);
            }

            DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, VIOLET);

            DrawText(TextFormat("INGRESADO %i/%i", letterCount, MAX_INPUT_CHARS), 420, 246, 20, DARKGRAY);

            if (letterCount < MAX_INPUT_CHARS)
            {
                if (((framesCounter / 20) % 2) == 0)
                    DrawText("_", (int)textBox.x + 8 + MeasureText(name, 40), (int)textBox.y + 12, 40, VIOLET);
            }
            else
            {
                DrawText("Limite alcanzado", 130, 352, 20, GRAY);
            }

            if (strlen(name) > 0)
            {
                DrawRectangleRec(proceedButtonBounds, BLANK);
            }
        }
        break;
        case TUTORIAL:
        {
            ClearBackground(WHITE);
            DrawTexture(tutorialTexture, (screenWidth - tutorialTexture.width) / 2, (screenHeight - tutorialTexture.height) / 2, WHITE);
        }
        break;
        case GAMEPLAY:
        {
            ClearBackground(WHITE);
            //if(!derrota)
            //{
                switch(pregunta_actual)
                {
                    case 1:
                    {
                        derrota = 0;
                        victoria = 0;

                        DrawTexture(gameplayTexture1, (screenWidth - gameplayTexture1.width) / 2, (screenHeight - gameplayTexture1.height) / 2, WHITE);
                        DrawText(preguntas[pregunta_actual-1].pregunta, 60,250,16,WHITE);

                        if(comodin_clicked)
                        {
                            if(opc_correcta_actual == 'A' || comodin_opc_alternative_char == 'A')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,RED);
                            }

                            if(opc_correcta_actual == 'B' || comodin_opc_alternative_char == 'B')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,RED);
                            }

                            if(opc_correcta_actual == 'C' || comodin_opc_alternative_char == 'C')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,RED);
                            }
                            
                            if(opc_correcta_actual == 'D' || comodin_opc_alternative_char == 'D')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,RED);
                            }
                            
                        }
                        else
                        {
                            DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,WHITE);
                        }

                        DrawRectangleRec(opcAbuttonBounds, BLANK);
                        DrawRectangleRec(opcBbuttonBounds, BLANK);
                        DrawRectangleRec(opcCbuttonBounds, BLANK);
                        DrawRectangleRec(opcDbuttonBounds, BLANK);

                        if(opcAButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 15,308, WHITE);
                        }

                        if(opcBButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 280,308, WHITE);
                        }

                        if(opcCButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 18,372, WHITE);
                        }

                        if(opcDButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 284,371, WHITE);
                        }

                    }
                    break;
                    case 2:
                    {
                        derrota = 0;
                        victoria = 0;
                        
                        DrawTexture(gameplayTexture2, (screenWidth - gameplayTexture2.width) / 2, (screenHeight - gameplayTexture2.height) / 2, WHITE);
                        DrawText(preguntas[pregunta_actual-1].pregunta, 60,250,16,WHITE);

                        if(comodin_clicked)
                        {
                            if(opc_correcta_actual == 'A' || comodin_opc_alternative_char == 'A')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,RED);
                            }

                            if(opc_correcta_actual == 'B' || comodin_opc_alternative_char == 'B')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,RED);
                            }

                            if(opc_correcta_actual == 'C' || comodin_opc_alternative_char == 'C')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,RED);
                            }
                            
                            if(opc_correcta_actual == 'D' || comodin_opc_alternative_char == 'D')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,RED);
                            }
                            
                        }
                        else
                        {
                            DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,WHITE);
                        }

                        DrawRectangleRec(opcAbuttonBounds, BLANK);
                        DrawRectangleRec(opcBbuttonBounds, BLANK);
                        DrawRectangleRec(opcCbuttonBounds, BLANK);
                        DrawRectangleRec(opcDbuttonBounds, BLANK);

                        if(opcAButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 15,308, WHITE);
                        }

                        if(opcBButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 280,308, WHITE);
                        }

                        if(opcCButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 18,372, WHITE);
                        }

                        if(opcDButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 284,371, WHITE);
                        }
                    }
                    break;
                    case 3:
                    {
                        derrota = 0;
                        victoria = 0;
                        DrawTexture(gameplayTexture3, (screenWidth - gameplayTexture3.width) / 2, (screenHeight - gameplayTexture3.height) / 2, WHITE);
                        DrawText(preguntas[pregunta_actual-1].pregunta, 60,250,20,WHITE);


                        if(comodin_clicked)
                        {
                            if(opc_correcta_actual == 'A' || comodin_opc_alternative_char == 'A')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,RED);
                            }

                            if(opc_correcta_actual == 'B' || comodin_opc_alternative_char == 'B')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,RED);
                            }

                            if(opc_correcta_actual == 'C' || comodin_opc_alternative_char == 'C')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,RED);
                            }
                            
                            if(opc_correcta_actual == 'D' || comodin_opc_alternative_char == 'D')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,RED);
                            }
                        }
                        else
                        {
                            DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,WHITE);
                        }

                        DrawRectangleRec(opcAbuttonBounds, BLANK);
                        DrawRectangleRec(opcBbuttonBounds, BLANK);
                        DrawRectangleRec(opcCbuttonBounds, BLANK);
                        DrawRectangleRec(opcDbuttonBounds, BLANK);

                        if(opcAButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 15,308, WHITE);
                        }

                        if(opcBButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 280,308, WHITE);
                        }

                        if(opcCButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 18,372, WHITE);
                        }

                        if(opcDButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 284,371, WHITE);
                        }
                    }
                    break;
                    case 4:
                    {
                        derrota = 0;
                        victoria = 0;
                        DrawTexture(gameplayTexture4, (screenWidth - gameplayTexture4.width) / 2, (screenHeight - gameplayTexture4.height) / 2, WHITE);
                        DrawText(preguntas[pregunta_actual-1].pregunta, 60,250,20,WHITE);


                        if(comodin_clicked)
                        {
                            if(opc_correcta_actual == 'A' || comodin_opc_alternative_char == 'A')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,RED);
                            }

                            if(opc_correcta_actual == 'B' || comodin_opc_alternative_char == 'B')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,RED);
                            }

                            if(opc_correcta_actual == 'C' || comodin_opc_alternative_char == 'C')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,RED);
                            }
                            
                            if(opc_correcta_actual == 'D' || comodin_opc_alternative_char == 'D')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,RED);
                            }
                            
                        }
                        else
                        {
                            DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,WHITE);
                        }


                        DrawRectangleRec(opcAbuttonBounds, BLANK);
                        DrawRectangleRec(opcBbuttonBounds, BLANK);
                        DrawRectangleRec(opcCbuttonBounds, BLANK);
                        DrawRectangleRec(opcDbuttonBounds, BLANK);

                        if(opcAButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 15,308, WHITE);
                        }

                        if(opcBButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 280,308, WHITE);
                        }

                        if(opcCButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 18,372, WHITE);
                        }

                        if(opcDButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 284,371, WHITE);
                        }
                    }
                    break;
                    case 5:
                    {
                        derrota = 0;
                        victoria = 0;
                        DrawTexture(gameplayTexture5, (screenWidth - gameplayTexture5.width) / 2, (screenHeight - gameplayTexture5.height) / 2, WHITE);
                        DrawText(preguntas[pregunta_actual-1].pregunta, 60,250,20,WHITE);


                        if(comodin_clicked)
                        {
                            if(opc_correcta_actual == 'A' || comodin_opc_alternative_char == 'A')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,RED);
                            }

                            if(opc_correcta_actual == 'B' || comodin_opc_alternative_char == 'B')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,RED);
                            }

                            if(opc_correcta_actual == 'C' || comodin_opc_alternative_char == 'C')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,RED);
                            }
                            
                            if(opc_correcta_actual == 'D' || comodin_opc_alternative_char == 'D')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,RED);
                            }
                            
                        }
                        else
                        {
                            DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,WHITE);
                        }


                        DrawRectangleRec(opcAbuttonBounds, BLANK);
                        DrawRectangleRec(opcBbuttonBounds, BLANK);
                        DrawRectangleRec(opcCbuttonBounds, BLANK);
                        DrawRectangleRec(opcDbuttonBounds, BLANK);

                        if(opcAButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 15,308, WHITE);
                        }

                        if(opcBButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 280,308, WHITE);
                        }

                        if(opcCButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 18,372, WHITE);
                        }

                        if(opcDButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 284,371, WHITE);
                        }
                    }
                    break;
                    case 6:
                    {
                        derrota = 0;
                        victoria = 0;
                        DrawTexture(gameplayTexture6, (screenWidth - gameplayTexture6.width) / 2, (screenHeight - gameplayTexture6.height) / 2, WHITE);
                        DrawText(preguntas[pregunta_actual-1].pregunta, 60,250,20,WHITE);


                        if(comodin_clicked)
                        {
                            if(opc_correcta_actual == 'A' || comodin_opc_alternative_char == 'A')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,RED);
                            }

                            if(opc_correcta_actual == 'B' || comodin_opc_alternative_char == 'B')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,RED);
                            }

                            if(opc_correcta_actual == 'C' || comodin_opc_alternative_char == 'C')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,RED);
                            }
                            
                            if(opc_correcta_actual == 'D' || comodin_opc_alternative_char == 'D')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,RED);
                            }
                            
                        }
                        else
                        {
                            DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,WHITE);
                        }


                        DrawRectangleRec(opcAbuttonBounds, BLANK);
                        DrawRectangleRec(opcBbuttonBounds, BLANK);
                        DrawRectangleRec(opcCbuttonBounds, BLANK);
                        DrawRectangleRec(opcDbuttonBounds, BLANK);

                        if(opcAButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 15,308, WHITE);
                        }

                        if(opcBButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 280,308, WHITE);
                        }

                        if(opcCButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 18,372, WHITE);
                        }

                        if(opcDButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 284,371, WHITE);
                        }
                    }
                    break;
                    case 7:
                    {
                        derrota = 0;
                        victoria = 0;
                        DrawTexture(gameplayTexture7, (screenWidth - gameplayTexture7.width) / 2, (screenHeight - gameplayTexture7.height) / 2, WHITE);
                        DrawText(preguntas[pregunta_actual-1].pregunta, 60,250,20,WHITE);


                        if(comodin_clicked)
                        {
                            if(opc_correcta_actual == 'A' || comodin_opc_alternative_char == 'A')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,RED);
                            }

                            if(opc_correcta_actual == 'B' || comodin_opc_alternative_char == 'B')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,RED);
                            }

                            if(opc_correcta_actual == 'C' || comodin_opc_alternative_char == 'C')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,RED);
                            }
                            
                            if(opc_correcta_actual == 'D' || comodin_opc_alternative_char == 'D')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,RED);
                            }
                            
                        }
                        else
                        {
                            DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,WHITE);
                        }


                        DrawRectangleRec(opcAbuttonBounds, BLANK);
                        DrawRectangleRec(opcBbuttonBounds, BLANK);
                        DrawRectangleRec(opcCbuttonBounds, BLANK);
                        DrawRectangleRec(opcDbuttonBounds, BLANK);

                        if(opcAButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 15,308, WHITE);
                        }

                        if(opcBButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 280,308, WHITE);
                        }

                        if(opcCButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 18,372, WHITE);
                        }

                        if(opcDButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 284,371, WHITE);
                        }
                    }
                    break;
                    case 8:
                    {
                        derrota = 0;
                        victoria = 0;
                        DrawTexture(gameplayTexture8, (screenWidth - gameplayTexture8.width) / 2, (screenHeight - gameplayTexture8.height) / 2, WHITE);
                        DrawText(preguntas[pregunta_actual-1].pregunta, 60,250,20,WHITE);


                        if(comodin_clicked)
                        {
                            if(opc_correcta_actual == 'A' || comodin_opc_alternative_char == 'A')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,RED);
                            }

                            if(opc_correcta_actual == 'B' || comodin_opc_alternative_char == 'B')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,RED);
                            }

                            if(opc_correcta_actual == 'C' || comodin_opc_alternative_char == 'C')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,RED);
                            }
                            
                            if(opc_correcta_actual == 'D' || comodin_opc_alternative_char == 'D')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,RED);
                            }
                            
                        }
                        else
                        {
                            DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,WHITE);
                        }


                        DrawRectangleRec(opcAbuttonBounds, BLANK);
                        DrawRectangleRec(opcBbuttonBounds, BLANK);
                        DrawRectangleRec(opcCbuttonBounds, BLANK);
                        DrawRectangleRec(opcDbuttonBounds, BLANK);

                        if(opcAButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 15,308, WHITE);
                        }

                        if(opcBButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 280,308, WHITE);
                        }

                        if(opcCButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 18,372, WHITE);
                        }

                        if(opcDButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 284,371, WHITE);
                        }
                    }
                    break;
                    case 9:
                    {
                        derrota = 0;
                        victoria = 0;
                        DrawTexture(gameplayTexture9, (screenWidth - gameplayTexture9.width) / 2, (screenHeight - gameplayTexture9.height) / 2, WHITE);
                        DrawText(preguntas[pregunta_actual-1].pregunta, 60,250,20,WHITE);


                        if(comodin_clicked)
                        {
                            if(opc_correcta_actual == 'A' || comodin_opc_alternative_char == 'A')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,RED);
                            }

                            if(opc_correcta_actual == 'B' || comodin_opc_alternative_char == 'B')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,RED);
                            }

                            if(opc_correcta_actual == 'C' || comodin_opc_alternative_char == 'C')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,RED);
                            }
                            
                            if(opc_correcta_actual == 'D' || comodin_opc_alternative_char == 'D')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,RED);
                            }
                            
                        }
                        else
                        {
                            DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,WHITE);
                        }


                        DrawRectangleRec(opcAbuttonBounds, BLANK);
                        DrawRectangleRec(opcBbuttonBounds, BLANK);
                        DrawRectangleRec(opcCbuttonBounds, BLANK);
                        DrawRectangleRec(opcDbuttonBounds, BLANK);

                        if(opcAButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 15,308, WHITE);
                        }

                        if(opcBButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 280,308, WHITE);
                        }

                        if(opcCButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 18,372, WHITE);
                        }

                        if(opcDButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 284,371, WHITE);
                        }
                    }
                    break;
                    case 10:
                    {
                        derrota = 0;
                        victoria = 0;
                        DrawTexture(gameplayTexture10, (screenWidth - gameplayTexture10.width) / 2, (screenHeight - gameplayTexture10.height) / 2, WHITE);
                        DrawText(preguntas[pregunta_actual-1].pregunta, 60,250,20,WHITE);


                        if(comodin_clicked)
                        {
                            if(opc_correcta_actual == 'A' || comodin_opc_alternative_char == 'A')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,RED);
                            }

                            if(opc_correcta_actual == 'B' || comodin_opc_alternative_char == 'B')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,RED);
                            }

                            if(opc_correcta_actual == 'C' || comodin_opc_alternative_char == 'C')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,RED);
                            }
                            
                            if(opc_correcta_actual == 'D' || comodin_opc_alternative_char == 'D')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,RED);
                            }
                            
                        }
                        else
                        {
                            DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,WHITE);
                        }


                        DrawRectangleRec(opcAbuttonBounds, BLANK);
                        DrawRectangleRec(opcBbuttonBounds, BLANK);
                        DrawRectangleRec(opcCbuttonBounds, BLANK);
                        DrawRectangleRec(opcDbuttonBounds, BLANK);

                        if(opcAButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 15,308, WHITE);
                        }

                        if(opcBButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 280,308, WHITE);
                        }

                        if(opcCButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 18,372, WHITE);
                        }

                        if(opcDButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 284,371, WHITE);
                        }
                    }
                    break;
                    case 11:
                    {
                        derrota = 0;
                        victoria = 0;
                        DrawTexture(gameplayTexture11, (screenWidth - gameplayTexture11.width) / 2, (screenHeight - gameplayTexture11.height) / 2, WHITE);
                        DrawText(preguntas[pregunta_actual-1].pregunta, 60,250,20,WHITE);


                        if(comodin_clicked)
                        {
                            if(opc_correcta_actual == 'A' || comodin_opc_alternative_char == 'A')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,RED);
                            }

                            if(opc_correcta_actual == 'B' || comodin_opc_alternative_char == 'B')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,RED);
                            }

                            if(opc_correcta_actual == 'C' || comodin_opc_alternative_char == 'C')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,RED);
                            }
                            
                            if(opc_correcta_actual == 'D' || comodin_opc_alternative_char == 'D')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,RED);
                            }
                            
                        }
                        else
                        {
                            DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,WHITE);
                        }


                        DrawRectangleRec(opcAbuttonBounds, BLANK);
                        DrawRectangleRec(opcBbuttonBounds, BLANK);
                        DrawRectangleRec(opcCbuttonBounds, BLANK);
                        DrawRectangleRec(opcDbuttonBounds, BLANK);

                        if(opcAButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 15,308, WHITE);
                        }

                        if(opcBButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 280,308, WHITE);
                        }

                        if(opcCButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 18,372, WHITE);
                        }

                        if(opcDButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 284,371, WHITE);
                        }
                    }
                    break;
                    case 12:
                    {
                        derrota = 0;
                        victoria = 0;
                        DrawTexture(gameplayTexture12, (screenWidth - gameplayTexture12.width) / 2, (screenHeight - gameplayTexture12.height) / 2, WHITE);
                        DrawText(preguntas[pregunta_actual-1].pregunta, 60,250,20,WHITE);


                        if(comodin_clicked)
                        {
                            if(opc_correcta_actual == 'A' || comodin_opc_alternative_char == 'A')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,RED);
                            }

                            if(opc_correcta_actual == 'B' || comodin_opc_alternative_char == 'B')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,RED);
                            }

                            if(opc_correcta_actual == 'C' || comodin_opc_alternative_char == 'C')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,RED);
                            }
                            
                            if(opc_correcta_actual == 'D' || comodin_opc_alternative_char == 'D')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,RED);
                            }
                            
                        }
                        else
                        {
                            DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,WHITE);
                        }


                        DrawRectangleRec(opcAbuttonBounds, BLANK);
                        DrawRectangleRec(opcBbuttonBounds, BLANK);
                        DrawRectangleRec(opcCbuttonBounds, BLANK);
                        DrawRectangleRec(opcDbuttonBounds, BLANK);

                        if(opcAButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 15,308, WHITE);
                        }

                        if(opcBButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 280,308, WHITE);
                        }

                        if(opcCButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 18,372, WHITE);
                        }

                        if(opcDButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 284,371, WHITE);
                        }
                    }
                    break;
                    case 13:
                    {
                        derrota = 0;
                        victoria = 0;
                        DrawTexture(gameplayTexture13, (screenWidth - gameplayTexture13.width) / 2, (screenHeight - gameplayTexture13.height) / 2, WHITE);
                        DrawText(preguntas[pregunta_actual-1].pregunta, 60,250,20,WHITE);


                        if(comodin_clicked)
                        {
                            if(opc_correcta_actual == 'A' || comodin_opc_alternative_char == 'A')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,RED);
                            }

                            if(opc_correcta_actual == 'B' || comodin_opc_alternative_char == 'B')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,RED);
                            }

                            if(opc_correcta_actual == 'C' || comodin_opc_alternative_char == 'C')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,RED);
                            }
                            
                            if(opc_correcta_actual == 'D' || comodin_opc_alternative_char == 'D')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,RED);
                            }
                            
                        }
                        else
                        {
                            DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,WHITE);
                        }


                        DrawRectangleRec(opcAbuttonBounds, BLANK);
                        DrawRectangleRec(opcBbuttonBounds, BLANK);
                        DrawRectangleRec(opcCbuttonBounds, BLANK);
                        DrawRectangleRec(opcDbuttonBounds, BLANK);

                        if(opcAButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 15,308, WHITE);
                        }

                        if(opcBButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 280,308, WHITE);
                        }

                        if(opcCButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 18,372, WHITE);
                        }

                        if(opcDButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 284,371, WHITE);
                        }
                    }
                    break;
                    case 14:
                    {
                        derrota = 0;
                        victoria = 0;
                        DrawTexture(gameplayTexture14, (screenWidth - gameplayTexture14.width) / 2, (screenHeight - gameplayTexture14.height) / 2, WHITE);
                        DrawText(preguntas[pregunta_actual-1].pregunta, 60,250,20,WHITE);


                        if(comodin_clicked)
                        {
                            if(opc_correcta_actual == 'A' || comodin_opc_alternative_char == 'A')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,RED);
                            }

                            if(opc_correcta_actual == 'B' || comodin_opc_alternative_char == 'B')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,RED);
                            }

                            if(opc_correcta_actual == 'C' || comodin_opc_alternative_char == 'C')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,RED);
                            }
                            
                            if(opc_correcta_actual == 'D' || comodin_opc_alternative_char == 'D')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,RED);
                            }
                            
                        }
                        else
                        {
                            DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,WHITE);
                        }


                        DrawRectangleRec(opcAbuttonBounds, BLANK);
                        DrawRectangleRec(opcBbuttonBounds, BLANK);
                        DrawRectangleRec(opcCbuttonBounds, BLANK);
                        DrawRectangleRec(opcDbuttonBounds, BLANK);

                        if(opcAButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 15,308, WHITE);
                        }

                        if(opcBButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 280,308, WHITE);
                        }

                        if(opcCButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 18,372, WHITE);
                        }

                        if(opcDButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 284,371, WHITE);
                        }
                    }
                    break;
                    case 15:
                    {
                        derrota = 0;
                        victoria = 0;
                        DrawTexture(gameplayTexture15, (screenWidth - gameplayTexture15.width) / 2, (screenHeight - gameplayTexture15.height) / 2, WHITE);
                        DrawText(preguntas[pregunta_actual-1].pregunta, 60,250,20,WHITE);


                        if(comodin_clicked)
                        {
                            if(opc_correcta_actual == 'A' || comodin_opc_alternative_char == 'A')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,RED);
                            }

                            if(opc_correcta_actual == 'B' || comodin_opc_alternative_char == 'B')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,RED);
                            }

                            if(opc_correcta_actual == 'C' || comodin_opc_alternative_char == 'C')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,RED);
                            }
                            
                            if(opc_correcta_actual == 'D' || comodin_opc_alternative_char == 'D')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,RED);
                            }
                            
                        }
                        else
                        {
                            DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,WHITE);
                        }


                        DrawRectangleRec(opcAbuttonBounds, BLANK);
                        DrawRectangleRec(opcBbuttonBounds, BLANK);
                        DrawRectangleRec(opcCbuttonBounds, BLANK);
                        DrawRectangleRec(opcDbuttonBounds, BLANK);

                        if(opcAButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 15,308, WHITE);
                        }

                        if(opcBButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 280,308, WHITE);
                        }

                        if(opcCButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 18,372, WHITE);
                        }

                        if(opcDButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 284,371, WHITE);
                        }
                    }
                    break;
                    case 16:
                    {
                        derrota = 0;
                        victoria = 0;
                        DrawTexture(gameplayTexture16, (screenWidth - gameplayTexture16.width) / 2, (screenHeight - gameplayTexture16.height) / 2, WHITE);
                        DrawText(preguntas[pregunta_actual-1].pregunta, 60,250,20,WHITE);


                        if(comodin_clicked)
                        {
                            if(opc_correcta_actual == 'A' || comodin_opc_alternative_char == 'A')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,RED);
                            }

                            if(opc_correcta_actual == 'B' || comodin_opc_alternative_char == 'B')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,RED);
                            }

                            if(opc_correcta_actual == 'C' || comodin_opc_alternative_char == 'C')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,RED);
                            }
                            
                            if(opc_correcta_actual == 'D' || comodin_opc_alternative_char == 'D')
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,GREEN);
                            }
                            else
                            {
                                DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,RED);
                            }
                            
                        }
                        else
                        {
                            DrawText(preguntas[pregunta_actual-1].opcionA, 50,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionB, 320,335,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionC, 50,397,20,WHITE);
                            DrawText(preguntas[pregunta_actual-1].opcionD, 320,397,20,WHITE);
                        }


                        DrawRectangleRec(opcAbuttonBounds, BLANK);
                        DrawRectangleRec(opcBbuttonBounds, BLANK);
                        DrawRectangleRec(opcCbuttonBounds, BLANK);
                        DrawRectangleRec(opcDbuttonBounds, BLANK);

                        if(opcAButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 15,308, WHITE);
                        }

                        if(opcBButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 280,308, WHITE);
                        }

                        if(opcCButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 18,372, WHITE);
                        }

                        if(opcDButtonHovered)
                        {
                            DrawTexture(pregunta_hover, 284,371, WHITE);
                        }
                    }
                    break;
                }

                DrawText(name, 335,60,20,WHITE);
                DrawText("Comodines Disponibles:", (screenWidth - MeasureText("Comodines Disponibles:", 18)) / 2, screenHeight / 2 - 120, 18, GRAY);
                if(!comodin_clicked && disponible_comodin)
                {
                    if (comodinButtonHovered)
                    {
                        DrawTexture(comodinButtonHover, comodinButtonBounds.x, comodinButtonBounds.y, WHITE);
                        DrawText("Elimina 2 posibles respuestas", (screenWidth - MeasureText("Elimina 2 posibles respuestas", 18)) / 2 + 10, screenHeight / 2 - 30, 18, GRAY);
                    }
                    else
                    {
                        DrawTexture(comodinButtonNormal, comodinButtonBounds.x, comodinButtonBounds.y, WHITE);
                    }
                }
                else
                {
                    DrawTexture(comodinButtonClicked, comodinButtonBounds.x, comodinButtonBounds.y, WHITE);
                }
            //}

        }
        break;
        case POINTS:
        {
            ClearBackground(RAYWHITE);
            DrawTexture(pointsTexture, (screenWidth - pointsTexture.width) / 2, (screenHeight - pointsTexture.height) / 2, WHITE);

            int y = 150;
            int x1 = 480;
            int x2 = 640;
            int start = numUsuarios - 10 < 0 ? 0 : numUsuarios - 10;

            int maxGanancias = 0;
            int indiceMaxGanancias = -1;

            if(numUsuarios > 0)
            {
                for (int i = start; i < numUsuarios; ++i)
                {
                    DrawText(usuarios[i].usuario, x1, y, 14, WHITE);  // Muestra el nombre del usuario en x1
                    char gananciasStr[10];  // Asume que los puntos no superarán los 9 dígitos
                    sprintf(gananciasStr, "%d", usuarios[i].ganancias);
                    DrawText(gananciasStr, x2, y, 14, WHITE);   // Muestra los puntos del usuario en x2
                    y += 15;
                }

                for (int i = start; i < numUsuarios; ++i)
                {
                    if (usuarios[i].ganancias >= maxGanancias || indiceMaxGanancias == -1) {
                        maxGanancias = usuarios[i].ganancias;
                        indiceMaxGanancias = i;
                    }
                }

                if (indiceMaxGanancias != -1)
                {
                    DrawText(usuarios[indiceMaxGanancias].usuario, 160, 220, 22, WHITE);
                    char gananciasStr[10];

                    sprintf(gananciasStr, "%d", maxGanancias);
                    DrawText(gananciasStr, 290, 220, 22, WHITE);
                }

            }
            else
            {
                DrawText("Sin registros por el momento", x1, y, 14, WHITE);
            }

        }
        break;

        case CREDITS:
        {
            ClearBackground(BLACK);
            DrawTexture(creditsTexture, (screenWidth - creditsTexture.width) / 2, (screenHeight - creditsTexture.height) / 2, WHITE);

            for (int i = 0; i < creditsLines; i++)
            {
                DrawText(creditsText[i], (screenWidth - MeasureText(creditsText[i], 20)) / 2, creditsPositionY + i * 30, 20, RAYWHITE);
            }
        }
        break;

        case ENDING:
        {
            ClearBackground(WHITE);
            
            if(derrota)
            {
                DrawTexture(gameover, (screenWidth - gameover.width) / 2, (screenHeight - gameover.height) / 2, WHITE);
            }

            if(victoria)
            {
                DrawTexture(win, (screenWidth - win.width) / 2, (screenHeight - win.height) / 2, WHITE);
            }
        }
        break;


        default:
            break;
        }

        EndDrawing();
    }

    UnloadTexture(loadingTexture);
    UnloadTexture(tutorialTexture);
    UnloadTexture(usernameTexture);
    UnloadTexture(gameplayTexture1);
    UnloadTexture(gameplayTexture2);
    UnloadTexture(gameplayTexture3);
    UnloadTexture(gameplayTexture4);
    UnloadTexture(gameplayTexture5);
    UnloadTexture(gameplayTexture6);
    UnloadTexture(gameplayTexture7);
    UnloadTexture(gameplayTexture8);
    UnloadTexture(gameplayTexture9);
    UnloadTexture(gameplayTexture10);
    UnloadTexture(gameplayTexture11);
    UnloadTexture(gameplayTexture12);
    UnloadTexture(gameplayTexture13);
    UnloadTexture(gameplayTexture14);
    UnloadTexture(gameplayTexture15);
    UnloadTexture(gameplayTexture16);
    UnloadSound(introSound);
    UnloadSound(startSound);
    UnloadSound(loopSound);
    UnloadSound(endgameSound);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

bool IsAnyKeyPressed()
{
    bool keyPressed = false;
    int key = GetKeyPressed();

    if ((key >= 32) && (key <= 126))
        keyPressed = true;

    return keyPressed;
}