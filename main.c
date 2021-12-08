#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LENTGH_OF_RANDOM_WORD 12 /* Constant size */

char *getRandomWord();                                           /* Creates Array of words and returns a random one */
int getRandomWordLength(char *randomWord);                       /* Returns the randomWord length */
void printFieldStart(int randomWordLength, char *userGuessWord); /* Prints the Field at the start of the game
                                                                    and sets the userGuessWord to '_' */
void userInput(char *randomWord, int randomWordLength, char *userGuessWord, int *userLive, int *tries, int *checkstart,
               char *lookupChar); /* Takes the UserInput and compares it to the randomWord */
                                  /* Prints the current stickmanImage */
                                  /* Prints the  current userLive */
void checkEnding(char *randomWord, char *userGuessWord, int *masterExit, int *masterRestart,
                 int userLive); /* Checks if the randomWord is equal to the userGuessWord */
void resetUserGuessWord(int randomWordLength, char *userGuessWord); /* Resets the userGuessWord */
char *getStickmanImage(int *tries);                                 /* Gets the current state of the Stickman */
int isCharacterInWord(char *randomWord, int randomWordLength,
                      char userInput); /* Counts the number of letters in the word */
int isCharacter(char input);           /* Checks if the Input is a valid character */
void addCharToLookupTable(char *lookupChar, char userInput,
                          int *checkStart); /* Checks if the character has been used already */
void resetData(int *masterRestart, int *userLive, int *tries, int *checkStart,
               char *lookupChar); /* Resets the important arguments */

int main()
{
    int run = 1;        /* Run variable */
    int userLive = 7;   /* User Live */
    int tries = 0;      /* Counts the tries */
    int checkStart = 0; /* Variable to check if it is the beginning of the game */
    char lookupChar[MAX_LENTGH_OF_RANDOM_WORD + 7] = {
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'}; /* Lookup Table for already used characters */
    int masterExit = 0;
    int masterRestart = 0;
    char *randomWord = getRandomWord();
    int randomWordLength = getRandomWordLength(randomWord);
    char userGuessWord[MAX_LENTGH_OF_RANDOM_WORD];

    printFieldStart(randomWordLength, userGuessWord);

    while (run) /* Game Loop */
    {
        printf("%s", randomWord);
        userInput(randomWord, randomWordLength, userGuessWord, &userLive, &tries, &checkStart, lookupChar);
        checkEnding(randomWord, userGuessWord, &masterExit, &masterRestart, userLive);

        if (masterExit == 1) /* Exits the game */
        {
            run = 0;
        }
        if (masterRestart == 1) /* Resets all important variables */
        {
            userGuessWord[0] = '\0';                                               /* Clears the userGuessWord */
            randomWord = getRandomWord();                                          /* Get a new randomWord */
            randomWordLength = getRandomWordLength(randomWord);                    /* Get a new randomWordLength */
            resetUserGuessWord(randomWordLength, userGuessWord);                   /* Resets the userGuessWord */
            resetData(&masterRestart, &userLive, &tries, &checkStart, lookupChar); /* Resets the important arguments */
            printf("\n");                                                          /* New line */
        }
    }

    return 0;
}

char *getRandomWord()
{
    int max = 26;
    int min = 0;

    char *randomWord[] = {"car",       "apple",       "dog",      "street",     "train",       "carbecue", "cottle",
                          "record",    "ship",        "table",    "typewriter", "videotape",   "toilet",   "tapestry",
                          "parachute", "thermometer", "water",    "weapon",     "wheelchair",  "pebble",   "pebble",
                          "chisel",    "airforce",    "computer", "ears",       "electricity", "game"};

    srand((unsigned int)time(NULL)); /* Sets the random Seed to the time Seed */

    return randomWord[rand() % (max + 1 - min) + min];
}

int getRandomWordLength(char *randomWord)
{
    int randomWordLength = (int)strlen(randomWord);

    return randomWordLength;
}

void printFieldStart(int randomWordLength, char *userGuessWord)
{
    int i;
    printf("Welcome to Hangman!\n");

    for (i = 0; i < randomWordLength; i++)
    {
        userGuessWord[i] = '_'; /* Sets the userGuessWord to '_' */
    }
    userGuessWord[i] = '\0';
}

void userInput(char *randomWord, int randomWordLength, char *userGuessWord, int *userLive, int *tries, int *checkStart,
               char *lookupChar)
{
    char UserInput;
    char *stickmanImage;
    int i;
    int counter = 0;
    int checkCharacter;
    int isCharDouble = 0;

    if (*checkStart == 0) /* If it is the start of the game */
    {
        printf("\n"); /* New line */
        printf("%s\n", userGuessWord);

        stickmanImage = getStickmanImage(tries); /* Get the current Stickman */
        printf("%s\n", stickmanImage);           /* Print the current Stickman */

        printf("You got %d more trie(s) left.\n", *userLive - 1);
    }


    printf("\n"); /* New line */
start:
    printf("Please enter a letter: ");
    scanf(" %c", &UserInput);

    checkCharacter = isCharacter(UserInput); /* Checks the userInput */
    switch (checkCharacter)
    {
    case 1: /* If the Input is between 'a' and 'z' */

        for (i = 0; i < 27; i++) /* Checks if the userInput is in the lookupTable */
        {
            if (UserInput == lookupChar[i])
            {
                isCharDouble = 1;
            }
        }

        addCharToLookupTable(lookupChar, UserInput, checkStart); /* adds the userInput to the lookupTable */

        if (isCharDouble == 0) /* If the userInput is not in the lookupTable */
        {
            for (i = 0; i < randomWordLength; i++) /* Sets the userInput to the randomWord */
            {
                if (randomWord[i] == UserInput)
                {
                    userGuessWord[i] = UserInput;
                }
            }

            printf("\n"); /* New line */
            printf("%s\n", userGuessWord);

            counter = isCharacterInWord(randomWord, randomWordLength, UserInput);

            if (counter == 0) /* If the userInput is not in the randomWord */
            {
                *userLive -= 1;
                *tries += 1;
            }

            *checkStart += 1;

            stickmanImage = getStickmanImage(tries); /* Get the current Stickman */

            printf("%s\n", stickmanImage); /* Print the current Stickman */
            printf("You got %d more trie(s) left.\n", *userLive - 1);
            break;
        }
        else
        {
            printf("You already tried that character! Try another one!\n");
            break;
        }

    case 2: /* If the Input is between 'A' and 'Z' */
        printf("Please enter a lowercase letter, not a capital letter!\n");
        printf("\n"); /* New line */
        goto start;

    default: /* If the Input is a number */
        printf("Please enter a lowercase letter, not a number!\n");
        printf("\n"); /* New line */
        goto start;
    }
}

void checkEnding(char *randomWord, char *userGuessWord, int *masterExit, int *masterRestart, int userLive)
{
    int userReplay;

    if (userLive == 1) /* If the user doesn't have any life left */
    {
        printf("\n"); /* New line */
        printf("You lost the Game!\n");
        printf("The word was: %s", randomWord);
        *masterExit = 1;
    }

    if (strcmp(userGuessWord, randomWord) == 0) /* If both words are the same */
    {
        printf("\n"); /* New line */
        printf("You Won the game!\n");
        printf("Do you wanna play again?\n");
        printf("1 = Yes / 0 = No\n");
        scanf("%d", &userReplay);

        if (userReplay != 1) /* If UserInput is not equal to 1, then exit, else restart */
        {
            *masterExit = 1;
        }
        else
        {
            *masterRestart = 1;
        }
    }
}

void resetUserGuessWord(int randomWordLength, char *userGuessWord)
{
    int i;

    for (i = 0; i < randomWordLength; i++)
    {
        userGuessWord[i] = '_'; /* Sets the userGuessWord to '_' */
    }
    userGuessWord[i] = '\0';
}

char *getStickmanImage(int *tries)
{
    char *Stickman[] = {
        "  +---+\n  |   |\n      |\n      |\n      |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n      |\n      |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n  |   |\n      |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n /|   |\n      |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n /|\\  |\n      |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n /|\\  |\n /    |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n /|\\  |\n / \\  |\n      |\n=========",
    };

    return Stickman[*tries];
}

int isCharacterInWord(char *randomWord, int randomWordLength, char userInput)
{
    int i;
    int counter = 0;
    for (i = 0; i < randomWordLength; i++)
    {
        if (userInput == randomWord[i])
        {
            counter++;
        }
    }

    return counter;
}

int isCharacter(char input)
{
    if (input >= 'a' && input <= 'z')
    {
        return 1;
    }
    if (input >= 'A' && input <= 'Z')
    {
        return 2;
    }

    return 0;
}

void addCharToLookupTable(char *lookupChar, char userInput, int *checkStart)
{
    lookupChar[*checkStart] = userInput;
}

void resetData(int *masterRestart, int *userLive, int *tries, int *checkStart, char *lookupChar)
{
    int i;
    *masterRestart = 0; /* Resets the masterRestart */
    *userLive = 7;      /* Resets userLive */
    *tries = 0;         /* Resets tries */
    *checkStart = 0;    /* Resets checkStart */

    for (i = 0; i <= 17; i++) /* Resets the Lookup Table */
    {
        lookupChar[i] = ' ';
    }
    lookupChar[i] = '\0';
}
