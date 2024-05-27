#include "raylib.h"
#include <stdio.h>  // Incluir la biblioteca para printf
#include <string.h> // Incluir la biblioteca para strlen
#include <time.h>
#include <stdlib.h>

#define MAX_INPUT_CHARS 9

typedef enum GameScreen
{
    LOGO = 0,
    MENU,
    USERNAME,
    TUTORIAL,
    GAMEPLAY,
    POINTS,
    CREDITS
} GameScreen;

typedef struct
{
    int id; //aleat 0 a 9999
    char usuario[MAX_INPUT_CHARS+1];
    int ganancias;
}Usuario;


// Función para verificar si un carácter es válido (letra o número)
bool IsValidChar(int key)
{
    return ((key >= 48 && key <= 57) || // Números 0-9
            (key >= 65 && key <= 90) || // Letras mayúsculas A-Z
            (key >= 97 && key <= 122)); // Letras minúsculas a-z
}

int main(void)
{
    srand(time(NULL));
    
    // Inicializar la ventana de Raylib
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "QUIEN QUIERE SER MILLONARIO: JUEGO");

    char name[MAX_INPUT_CHARS + 1] = "\0"; // NOTE: One extra space required for null terminator char '\0'
    int letterCount = 0;

    Rectangle textBox = {screenWidth / 2.0f - 200, 284, 225, 50};
    bool mouseOnText = false;

    // Variables para la primera escena
    Texture2D loadingTexture = LoadTexture("resources/cargando.png");
    const char *loadingText = "Cargando";
    int textSize = MeasureText(loadingText, 20);
    Vector2 textPosition = {(float)(screenWidth - textSize) / 2 + 20, (float)screenHeight / 2 + 50 + 130};
    float timer = 0.0f;
    float interval = 0.5f;
    int dotCount = 0;
    const int maxDots = 3;

    // Variables para la escena MENU
    Rectangle tutorialButtonBounds = {screenWidth / 2.0f - 60, screenHeight / 2.0f + 55, 144, 50};

    Rectangle pointsButtonBounds = {screenWidth / 2.0f - 105, screenHeight / 2.0f + 110, 229, 46};
    Rectangle creditosButtonBounds = {screenWidth / 2.0f -60, screenHeight / 2.0f + 162, 151, 46};

    // Variables para la escena TUTORIAL
    Texture2D tutorialTexture = LoadTexture("resources/tutorial.png");
    if (tutorialTexture.id == 0)
    {
        printf("Error al cargar la imagen tutorial.png\n");
        return -1; // Salir del programa si no se pudo cargar la imagen
    }

    // Variables para la escena MENU
    Texture2D menuTexture = LoadTexture("resources/menu.png");
    if (menuTexture.id == 0)
    {
        printf("Error al cargar la imagen menu.png\n");
        return -1; // Salir del programa si no se pudo cargar la imagen
    }

    Texture2D tutorialButtonNormal = LoadTexture("resources/botones/jugar_normal.png");
    Texture2D tutorialButtonHover = LoadTexture("resources/botones/jugar_hover.png");

    Texture2D pointsButtonNormal = LoadTexture("resources/botones/puntos_normal.png");
    Texture2D pointsButtonHover = LoadTexture("resources/botones/puntos_hover.png");

    Texture2D creditsButtonNormal = LoadTexture("resources/botones/credits_normal.png");
    Texture2D creditsButtonHover = LoadTexture("resources/botones/credits_hover.png");

    bool creditsButtonHovered = false;
    bool pointsButtonHovered = false;
    bool tutorialButtonHovered = false;


    // Variables para la escena USERNAME
    Texture2D usernameTexture = LoadTexture("resources/escribetunombre.png");
    if (usernameTexture.id == 0)
    {
        printf("Error al cargar la imagen escribetunombre.png\n");
        return -1; // Salir del programa si no se pudo cargar la imagen
    }

    // Variables para la escena GAMEPLAY
    Texture2D gameplayTexture = LoadTexture("resources/1.png");
    if (gameplayTexture.id == 0)
    {
        printf("Error al cargar la imagen 1.png\n");
        return -1; // Salir del programa si no se pudo cargar la imagen
    }

    Texture2D comodinButtonNormal = LoadTexture("resources/botones/comodin_normal.png");
    Texture2D comodinButtonHover = LoadTexture("resources/botones/comodin_hover.png");
    Texture2D comodinButtonClicked = LoadTexture("resources/botones/comodin_click.png");
    bool comodin_clicked = false;
    bool comodinButtonHovered = false;

    Rectangle comodinButtonBounds = {screenWidth / 2.0f - 80, screenHeight / 2.0f - 100, 144, 50};

    // Variables para la escena CREDITS

    Texture2D creditsTexture = LoadTexture("resources/creditos.png");
    if (creditsTexture.id == 0)
    {
        printf("Error al cargar la imagen 1.png\n");
        return -1; // Salir del programa si no se pudo cargar la imagen
    }

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



    // Definir el área del botón en la imagen TUTORIAL
    Rectangle buttonBounds = {650, 300, 100, 80}; // (x, y, width, height)

    // Definir el área del botón en la pantalla USERNAME
    Rectangle proceedButtonBounds = {screenWidth / 2.0f - 50 + 180, 270, 110, 80}; // (x, y, width, height)

    GameScreen currentScreen = LOGO;

    int framesCounter = 0; // Useful to count frames

    SetTargetFPS(60); // Set desired framerate (frames-per-second)

    InitAudioDevice(); // Initialize
    Sound introSound = LoadSound("resources/intro.wav");
    Sound startSound = LoadSound("resources/pregunta/start.wav");
    Sound loopSound = LoadSound("resources/pregunta/loop.wav");

    bool introSoundPlayed = false;
    bool startSoundPlayed = false;
    bool loopSoundPlayed = false;

    // Bucle principal del juego
    while (!WindowShouldClose())
    {
        // Actualizar el temporizador y la animación de los puntos
        timer += GetFrameTime();

        if (currentScreen == LOGO && timer >= interval)
        {
            timer = 0.0f;
            dotCount = (dotCount + 1) % (maxDots + 1);
        }

        // Reproducir el sonido de introducción si no se ha reproducido antes
        if (currentScreen == MENU && !introSoundPlayed)
        {
            PlaySound(introSound);
            introSoundPlayed = true; // Marcar como reproducido
        }

        // Reproducir el sonido de la pregunta si no se ha reproducido antes
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
            framesCounter++; // Count frames

            if (framesCounter > 260)
            {
                framesCounter = 0;
                currentScreen = MENU;
            }
        }
        break;
        case MENU:
        {

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

            // Detectar clic en los botones
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 mousePoint = GetMousePosition();
                if (CheckCollisionPointRec(mousePoint, tutorialButtonBounds))
                {
                    currentScreen = TUTORIAL; // Cambiar a la pantalla TUTORIAL
                }
                else if (CheckCollisionPointRec(mousePoint, pointsButtonBounds))
                {
                    currentScreen = POINTS; // Cambiar a la pantalla POINTS
                }
                else if (CheckCollisionPointRec(mousePoint, creditosButtonBounds))
                {
                    currentScreen = CREDITS; // Cambiar a la pantalla CREDITS
                }
            }
        }
        break;
        case USERNAME:
        {
            // Actualizar el estado del cuadro de texto y entrada del usuario
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
                        name[letterCount + 1] = '\0'; // Add null terminator at the end of the string.
                        letterCount++;
                    }

                    key = GetCharPressed(); // Check next character in the queue
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

            // Verificar si se presionó el botón de proceder
            if (strlen(name) > 0 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 mousePoint = GetMousePosition();
                if (CheckCollisionPointRec(mousePoint, proceedButtonBounds))
                {
                    printf("Nombre ingresado: %s\n", name);
                    currentScreen = GAMEPLAY; // Cambiar a la pantalla GAMEPLAY
                }
            }
        }
        break;
        case TUTORIAL:
        {

            // Detectar clic en el área del botón
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 mousePoint = GetMousePosition();
                if (CheckCollisionPointRec(mousePoint, buttonBounds))
                {
                    printf("Botón presionado\n");
                    currentScreen = USERNAME; // Cambiar a la pantalla USERNAME
                }
            }
        }
        break;
        case GAMEPLAY:
        {
            // Carga de la textura de la pantalla de juego
            if (gameplayTexture.id == 0)
            {
                printf("Error al cargar la imagen 1.png\n");
                return -1; // Salir del programa si no se pudo cargar la imagen
            }

            // Detectar el estado del mouse en los botones
            if (CheckCollisionPointRec(GetMousePosition(), comodinButtonBounds))
            {
                comodinButtonHovered = true;
            }
            else
            {
                comodinButtonHovered = false;
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !comodin_clicked)
            {
                Vector2 mousePoint = GetMousePosition();
                if (CheckCollisionPointRec(mousePoint, comodinButtonBounds))
                {
                    comodin_clicked = true;
                }
            }
        }
        break;
        case POINTS:
        {
            // Detectar clic para volver al menú
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                currentScreen = MENU; // Volver al menú
            }
        }
        break;

        case CREDITS:
        {
            ClearBackground(BLACK);

            for (int i = 0; i < creditsLines; i++)
            {
                DrawText(creditsText[i], (screenWidth - MeasureText(creditsText[i], 20)) / 2, creditsPositionY + i * 30, 20, RAYWHITE);
            }

            creditsPositionY -= 0.5f; // Reducir la velocidad de desplazamiento

            // Reiniciar la posición si los créditos se desplazan completamente
            if (creditsPositionY + creditsLines * 30 < 0)
            {
                creditsPositionY = screenHeight;
            }

            // Detectar clic para volver al menú
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                currentScreen = MENU; // Volver al menú
            }
        }
        break;


        default:
            break;
        }

        // Limpiar la ventana actual
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (currentScreen)
        {
        case LOGO:
        {
            DrawTexture(loadingTexture, (screenWidth - loadingTexture.width) / 2, (screenHeight - loadingTexture.height) / 2, WHITE);
            DrawText(TextFormat("%s%s", loadingText, (dotCount == 0 ? "" : (dotCount == 1 ? "." : (dotCount == 2 ? ".." : "...")))), textPosition.x, textPosition.y, 20, PURPLE);
        }
        break;
        case MENU:
        {
            ClearBackground(RAYWHITE);

            DrawTexture(menuTexture, (screenWidth - menuTexture.width) / 2, (screenHeight - menuTexture.height) / 2, WHITE);

            // Dibujar la textura de los botones según el estado del mouse
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

            // Dibujar la imagen del fondo de la pantalla de nombre de usuario
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
                // Draw blinking underscore char
                if (((framesCounter / 20) % 2) == 0)
                    DrawText("_", (int)textBox.x + 8 + MeasureText(name, 40), (int)textBox.y + 12, 40, VIOLET);
            }
            else
            {
                DrawText("Limite alcanzado, presiona RETROCESO para borrar", 130, 350, 20, GRAY);
            }

            // Dibujar el botón de proceder si hay un nombre ingresado
            if (strlen(name) > 0)
            {
                DrawRectangleRec(proceedButtonBounds, BLANK);
            }
        }
        break;
        case TUTORIAL:
        {
            // Dibujar la imagen del tutorial
            DrawTexture(tutorialTexture, (screenWidth - tutorialTexture.width) / 2, (screenHeight - tutorialTexture.height) / 2, WHITE);
        }
        break;
        case GAMEPLAY:
        {
            DrawTexture(gameplayTexture, (screenWidth - gameplayTexture.width) / 2, (screenHeight - gameplayTexture.height) / 2, WHITE);
        
            // Dibujar la textura de los botones según el estado del mouse

            DrawText("Comodines Disponibles:", (screenWidth - MeasureText("Comodines Disponibles:", 18)) / 2, screenHeight / 2 - 120, 18, GRAY);
            if(!comodin_clicked)
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
        }
        break;
        case POINTS:
        {
            ClearBackground(RAYWHITE);
            DrawText("Puntos", (screenWidth - MeasureText("Puntos", 40)) / 2, screenHeight / 2 - 20, 40, BLACK);
        }
        break;

        case CREDITS:
        {
            ClearBackground(BLACK);
            // Dibujar la imagen del fondo de la pantalla 
            DrawTexture(creditsTexture, (screenWidth - creditsTexture.width) / 2, (screenHeight - creditsTexture.height) / 2, WHITE);

            for (int i = 0; i < creditsLines; i++)
            {
                DrawText(creditsText[i], (screenWidth - MeasureText(creditsText[i], 20)) / 2, creditsPositionY + i * 30, 20, RAYWHITE);
            }
        }

        default:
            break;
        }

        EndDrawing();
    }

    UnloadTexture(loadingTexture);
    UnloadTexture(tutorialTexture); // Descargar la textura del tutorial
    UnloadTexture(usernameTexture); // Descargar la textura del username
    UnloadTexture(gameplayTexture); // Descargar la textura del gameplay
    UnloadSound(introSound);
    UnloadSound(startSound);
    UnloadSound(loopSound);
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
