#include "raylib.h"
#include <stdio.h>  // Incluir la biblioteca para printf
#include <string.h> // Incluir la biblioteca para strlen

#define MAX_INPUT_CHARS 9

typedef enum GameScreen
{
    LOGO = 0,
    TITLE,
    USERNAME,
    GAMEPLAY,
    ENDING
} GameScreen;

// Función para verificar si un carácter es válido (letra o número)
bool IsValidChar(int key)
{
    return ((key >= 48 && key <= 57) || // Números 0-9
            (key >= 65 && key <= 90) || // Letras mayúsculas A-Z
            (key >= 97 && key <= 122)); // Letras minúsculas a-z
}

int main(void)
{
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

    // Variables para la escena TITLE
    Texture2D tutorialTexture = LoadTexture("resources/tutorial.png");
    if (tutorialTexture.id == 0)
    {
        printf("Error al cargar la imagen tutorial.png\n");
        return -1; // Salir del programa si no se pudo cargar la imagen
    }

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

    // Definir el área del botón en la imagen TITLE
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
        if (currentScreen == TITLE && !introSoundPlayed)
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
                currentScreen = TITLE;
            }
        }
        break;
        case TITLE:
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
        case GAMEPLAY:
        {

            
        
            // Carga de la textura de la pantalla de juego
            if (gameplayTexture.id == 0)
            {
                printf("Error al cargar la imagen 1.png\n");
                return -1; // Salir del programa si no se pudo cargar la imagen
            }

        }
        break;

        case ENDING:
        {
            // TODO: Update ENDING screen variables here!

            // Press enter to return to TITLE screen
            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
            {
                currentScreen = TITLE;
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

        case TITLE:
        {
            // Dibujar la imagen del tutorial
            DrawTexture(tutorialTexture, (screenWidth - tutorialTexture.width) / 2, (screenHeight - tutorialTexture.height) / 2, WHITE);
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
        case GAMEPLAY:
        {
            DrawTexture(gameplayTexture, (screenWidth - gameplayTexture.width) / 2, (screenHeight - gameplayTexture.height) / 2, WHITE);
        }
        break;
        case ENDING:
        {
            // TODO: Draw ENDING screen here!
            DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
            DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
            DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);
        }
        break;
        default:
            break;
        }

        EndDrawing();
    }

    UnloadTexture(loadingTexture);
    UnloadTexture(tutorialTexture); // Descargar la textura del tutorial
    UnloadTexture(usernameTexture); // Descargar la textura del username
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