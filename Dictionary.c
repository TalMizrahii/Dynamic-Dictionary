/***********************
* Author: Tal Mizrahi
* project: A dynamic dictionary for multiple languages.
***********************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define ONE 1

//saving the data about the dictionaries in the heap
typedef struct Word{
    char** translations;
    struct Word* next;
} Word;

typedef struct {
    char** languages;
    int numOfLanguages;
    Word* wordList;
} Dictionary;

int deleteDictionary(Dictionary *dictionaries, int choice, int numOfDics);
void deleteEverithing(Dictionary *dictionaries, int numOfDictionaries);
void deleteList(Word *word, int numOfLanguages);
void deleteStr(Dictionary dictionary);
int deleteWord(Word *deleteWord, Dictionary *dictionary);
void printDictionary (int numOfDictionaries, Dictionary *dictionaries);
char* makeStr();
Word* findWord(Dictionary dictionary, char *wordCmp);
void newWord(Word **wordList);
void newDictionary(Dictionary *dictionary);

/******************
* Function Name: deleteEverithing
* Input:
* Output:
* Function Operation:
******************/
void deleteEverithing(Dictionary *dictionaries, int numOfDictionaries){

    for(int i = numOfDictionaries - 1 ; i >= 0; i--){
        deleteStr(dictionaries[i]);
        deleteList(dictionaries[i].wordList, dictionaries[i].numOfLanguages);
        free(dictionaries[i].wordList);
        dictionaries[i].wordList = NULL;
    }
    free(dictionaries);
    dictionaries = NULL;

}


/******************
* Function Name: deleteList
* Input:
* Output:
* Function Operation:
******************/
void deleteList(Word *word, int numOfLanguages){

    //if the head is null return
    if( word == NULL){
        return;
    }

    deleteList(word->next, numOfLanguages);

    //freeing each translation
    for(int i = 0; i < numOfLanguages; i++){
        free(word->translations[i]);
        word->translations[i] = NULL;
    }

    free(word->translations);
    word->translations = NULL;
}

/******************
* Function Name: deleteStr
* Input:
* Output:
* Function Operation:
******************/
void deleteStr(Dictionary dictionary){

    //freeing each language
    for(int i = dictionary.numOfLanguages - 1 ; i >= 0; i--){
        free(dictionary.languages[i]);
        dictionary.languages[i] = NULL;
    }

    //freeing the whole languages array
    free(dictionary.languages);
    dictionary.languages = NULL;
}

/******************
* Function Name: deleteDictionary
* Input:
* Output:
* Function Operation:
******************/
int deleteDictionary(Dictionary *dictionaries, int choice, int numOfDics){

    deleteStr(dictionaries[choice - 1]);


    if(dictionaries[choice - 1].wordList != NULL){
        deleteList(dictionaries[choice - 1].wordList, dictionaries[choice -1].numOfLanguages);
        free(dictionaries[choice -1].wordList);
        dictionaries[choice -1].wordList = NULL;
    }
    free(&dictionaries[choice -1]);
    //if there is need to change the positions of the structs in the array
    Dictionary temp = dictionaries[choice -1];
    while(choice < numOfDics){

        dictionaries[choice - 1] = dictionaries[choice];
        choice++;
    }
    dictionaries[numOfDics - 1] = temp;

    Dictionary *tempDic = dictionaries;


    dictionaries = (Dictionary*)realloc(dictionaries, sizeof(Dictionary) * (numOfDics - 1));

    if(dictionaries == NULL){
        dictionaries = tempDic;
        dictionaries[numOfDics - 1] = dictionaries[numOfDics - 2];
    }
    numOfDics--;
    return numOfDics;
}


/******************
* Function Name: deleteWord
* Input:
* Output:
* Function Operation:
******************/
int deleteWord(Word *deleteWord, Dictionary *dictionary){

    //temp is the head of the list at the beginning
    Word *preNode;

    //if the deletion succeeded or not
    const int GOOD = 1, BAD = 0;

    //if the head is the word we want to delete
    if(dictionary->wordList->translations == deleteWord->translations){

        dictionary->wordList = dictionary->wordList->next;

        //freeing all the translations
        for(int i = dictionary->numOfLanguages - 1; i >= 0; i--){
            free(deleteWord->translations[i]);
            deleteWord->translations[i] = NULL;
        }

        //freeing the array of pointers
        free(deleteWord->translations);
        deleteWord->translations = NULL;

        //freeing the whole word struct
        free(deleteWord);
        deleteWord = NULL;
        return GOOD;
    }

    //it's not the head we want to delete

    Word *current = dictionary->wordList;

    while(current != NULL){

        if(current == deleteWord){
            preNode->next = current->next;

            //freeing all the translations
            for(int i = dictionary->numOfLanguages - 1; i >= 0; i--){
                free(current->translations[i]);
                current->translations[i] = NULL;
            }
            free(current->translations);
            current->translations = NULL;
            free(current);
            current = NULL;
            return GOOD;
        }

        //if the current is not the right word to delete keep going forward
        else{
            preNode = current;
            current = current->next;

        }
    }

    //if no such word was found
    return BAD;

}

/******************
* Function Name: printDictionary
* Input:number of dictionaries
* Output:void
* Function Operation: printing all the dictionaries
******************/
void printDictionary (int numOfDictionaries, Dictionary *dictionaries){
    printf("Choose a dictionary:\n");

    for(int i = 0; i < numOfDictionaries; i++){

        //printing the number of the dictionary
        printf("%d. ", (i + 1));

        //printing the list of the dictionary languages
        for(int j = 0; j < dictionaries[i].numOfLanguages; j++ ){

            //if it's the last word
            if(j == dictionaries[i].numOfLanguages - 1){
                printf("%s", dictionaries[i].languages[j]);
            }

            //if not
            else{
                printf("%s,", dictionaries[i].languages[j]);
            }
        }
        printf("\n");
    }
}

/******************
* Function Name: makeStr
* Input:
* Output:
* Function Operation: creating a new word struct
******************/
char* makeStr(){

    //clear the buffer
    while (getchar() != '\n');

    //the letter we will put to a string each iteration
    char letter = 'A';

    //counter to know how many letter to allocate space
    int countLetter = 0;

    //pointer for the array
    char *newStr;

    //get a new letter
    scanf("%c", &letter);

    while(letter != '\n'){

        countLetter++;

        //if it's the first allocation
        if(countLetter == 1){

            //making space for the array of the word
            newStr = (char*)malloc(sizeof(char));

            //putting the letter in the array
            newStr[0] = letter;
        }

        //if it's not the first allocation
        else{

            //making space for the array of the word
            newStr = (char*) realloc(newStr, sizeof(char)*countLetter);

            //putting the letter in the array
            newStr[countLetter - 1] = letter;
        }

        //get new letter
        scanf("%c", &letter);
    }

    //making one more spot for the \0
    countLetter++;
    newStr = (char*)realloc(newStr, sizeof(char)*countLetter);
    newStr[countLetter -1] = '\0';

    return newStr;
}

/******************
* Function Name: findWord
* Input:
* Output:
* Function Operation: creating a new word struct
******************/
Word* findWord(Dictionary dictionary, char *wordCmp){

    //saving result to be the head of the list
    Word *result = dictionary.wordList;

    //while result isn't NULL keep looking for the right word
    while(result != NULL){

        //return the result if it was the right one
        if((strcmp(result->translations[0], wordCmp)) == 0){
            return result;
        }

        //keep looking
        else{
            result = result->next;
        }
    }

    return result;
}

/******************
* Function Name: newWord
* Input:
* Output:
* Function Operation: creating a new word struct
******************/
void newWord(Word **wordList){

    //clear the buffer
    while (getchar() != '\n');

    //the letter we will put to a string each iteration
    char letter = 'T';

    //counters to check in which place I am it the array
    int countLetter = 0, countTranslation = 0;

    //pointer to the array of strings
    char **translations = (char**)malloc( sizeof(char*));

    //get a new letter
    scanf("%c", &letter);

    while(letter != '\n'){

        //collecting the words to be translated
        while(letter != '\n'){

            //if its the source word, creat a new array
            if(letter != ',' && countLetter == 0){

                //creating the new array
                translations[countTranslation] = (char*)malloc(sizeof(char));

                //inserting the first letter
                translations[countTranslation][countLetter] = letter;

                countLetter++;

            }

            //if the 'letter' is a real letter
            else if (letter != ','){

                //making space for the new letter
                translations[countTranslation] = realloc(translations[countTranslation],sizeof(char)*(countLetter+1));

                //inserting the letter to the array
                translations[countTranslation][countLetter] = letter;

                countLetter++;

            }

            //if letter is coma
            else{

                //making space for the new letter
                translations[countTranslation] = (char*)realloc(translations[countTranslation],sizeof(char)*(countLetter+1));

                //inserting the \0 to the array
                translations[countTranslation][countLetter] = '\0';

                //get a new letter
                scanf("%c", &letter);

                if(letter != '\n'){
                    countLetter = 0;
                    countTranslation++;
                }
                break;
            }

            //get a new letter
            scanf("%c", &letter);

        }//inner while

        if(letter != '\n'){
            //making space for the next dictionary
            translations = (char**)realloc(translations, sizeof(char*) * (countTranslation + 1));
        }
    }//wrap while

    //making space for the last \0
    translations[countTranslation] = realloc(translations[countTranslation], sizeof(char) * (countLetter + 1));

    //inserting the \0 to the array
    translations[countTranslation][countLetter] = '\0';

    //creating space for the new Word struct
    Word *word = (Word*)malloc(sizeof(Word));

    //insert the values to the struct
    word->translations = translations;

    //it will be the lust word - so null
    word->next = NULL;


    Word* move = *wordList;

    //if there are no words
    if(*wordList == NULL){
       *wordList = word;
    }

    //if there are, put the word in the last position
    else{
        while (move->next != NULL){
            move = move->next;
        }
        move->next = word;
    }
}

/******************
* Function Name: newDictionary
* Input:
* Output: struct pointer of type dictionary
* Function Operation: creating a new dictionary
******************/
void newDictionary(Dictionary *dictionary){

    //clear the buffer
    while (getchar() != '\n');

    //asking for new dictionary
    printf("Define a new dictionary:\n");

    //the letter we will put to a string each iteration
    char letter;

    //counters to check in which place I am it the array
    int countLetter = 0, countLeng = 0;

    //pointer to the array of strings
    char **languages = (char**)malloc(sizeof(char*));

    //get a new letter
    scanf("%c", &letter);

    while(letter != '\n'){

        //collecting the languages of the dictionary
        while(letter != '\n' ){

            //if its the first word in the language creat a new array
            if(letter != ',' && countLetter == 0 && letter != '\n'){

                //creating the new array
                languages[countLeng] = (char*)malloc(sizeof(char));

                //inserting the first letter
                languages[countLeng][countLetter] = letter;

                countLetter++;
            }

            //if the 'letter' is a real letter
            else if (letter != ',' && letter != '\n'){

                //making space for the new letter
                languages[countLeng] = (char*)realloc(languages[countLeng], sizeof(char) * (countLetter + 1));

                //inserting the letter to the array
                languages[countLeng][countLetter] = letter;

                countLetter++;
            }

            //if letter is coma
            else{

                    //making space for the new letter
                    languages[countLeng] = (char*)realloc(languages[countLeng], sizeof(char) * (countLetter + 1));

                    //inserting the \0 to the array
                    languages[countLeng][countLetter] = '\0';

                    //get a new letter
                    scanf("%c", &letter);

                    if (letter != '\n'){
                        countLetter = 0;
                        countLeng++;
                    }

                break;
            }

            //get a new letter
            scanf("%c", &letter);

        }//small while

        if(letter != '\n'){
            //making space for the next language
            languages = (char**)realloc(languages, sizeof(char*) * (countLeng + 1));
        }

    }//big while

        //making space for the last \0
        languages[countLeng] = (char*)realloc(languages[countLeng], sizeof(char) * (countLetter + 1));

        //inserting the \0 to the array
        languages[countLeng][countLetter] = '\0';

        //the counter started from 0 so add 1
        countLeng++;

            //insert the values to the struct
            dictionary->languages = languages;
            dictionary->numOfLanguages = countLeng;
            dictionary->wordList = NULL;

            printf("The dictionary has been created successfully!\n");
    }//func end

int main(){


    Dictionary* dictionaries;
    int numOfDictionaries = 0;

    //variables to store the choice of function, the dictionary choice, and flag of the printing of the menu
    int choice = 0, dicChoice = 0, menuFlag = 0;

    //constant variables for the choices options
    const int FIRST_CHOICE = 1, LAST_OPTION = 6;

    //the menu loop
    while(ONE){


        if(menuFlag != ONE){
            //asking for a choice in the menu
            printf("Welcome to the dictionaries manager!\nChoose an option:\n"
                   "1. Create a new dictionary.\n"
                   "2. Add a word to a dictionary.\n"
                   "3. Delete a word from a dictionary.\n"
                   "4. Find a word in a dictionary.\n"
                   "5. Delete a dictionary.\n"
                   "6. Exit.\n");
        }

        //redefining the menu flag
        menuFlag = 0;

        //getting the choice of the user
        scanf("%d", &choice);


        //if he enters a wrong option
        if(choice < FIRST_CHOICE || choice > LAST_OPTION){
            printf("Wrong option, try again:\n");
        }

        //if the choice is OK
        else{

            //the list of options
            switch (choice) {
                case 1:

                    //counting the number of dictionaries
                    numOfDictionaries += 1;

                    if(numOfDictionaries == 1){

                        dictionaries = (Dictionary*)malloc(sizeof(Dictionary));

                            //addressing the new dictionary to a spot in the array
                            newDictionary(&dictionaries[0]);
                    }

                    else{

                        //making room for the new dictionaries
                        dictionaries = (Dictionary*)realloc(dictionaries, (sizeof(Dictionary) * (numOfDictionaries)));

                        //addressing the new dictionary to a spot in the array
                         newDictionary(&dictionaries[numOfDictionaries - 1]);
                    }
                         break;
                case 2:

                    //if there are no dictionaries
                    if(numOfDictionaries == 0){
                        printf("This option is not available right now, try again:\n");
                        menuFlag = 1;
                        break;
                    }

                    //printing the dictionaries
                    printDictionary(numOfDictionaries, dictionaries);

                    scanf("%d", &dicChoice);

                    while(dicChoice > numOfDictionaries || dicChoice < ONE){
                        printf("Wrong option, try again:\n");
                        scanf("%d", &dicChoice);
                    }

                    printf("Enter a word in ");

                    for(int i = 0; i < dictionaries[dicChoice - 1].numOfLanguages; i++){
                        if(i == dictionaries[dicChoice - 1].numOfLanguages - 1){
                            printf("%s:", dictionaries[dicChoice - 1].languages[i]);
                        }
                        else{
                            printf("%s,", dictionaries[dicChoice - 1].languages[i] );
                        }
                    }

                    printf("\n");

                    newWord(&dictionaries[dicChoice - 1].wordList);

                    printf("The word has been added successfully!\n");
                    break;


                case 3:

                    //if there are no dictionaries
                    if(numOfDictionaries == 0){
                        printf("This option is not available right now, try again:\n");
                        menuFlag = 1;
                        break;
                    }

                    //printing the dictionaries
                    printDictionary(numOfDictionaries, dictionaries);

                    int deleteChoice;
                    char safetyCheck = 'n';

                    //clear the buffer and get the choice from the user
                    while (getchar() != '\n');
                    scanf("%d", &deleteChoice);

                    //while loop to check if the choice is valid
                    while(deleteChoice > numOfDictionaries || deleteChoice < 1){
                        printf("Wrong option, try again:\n");
                        scanf("%d", &deleteChoice);
                    }

                    printf("Enter a word in %s:\n",dictionaries[deleteChoice -1].languages[0]);

                    //creating a pointer to an array of string and sending it to a function to find
                    char *deleteStr = makeStr();

                    //safety check
                    printf("Are you sure? (y/n)\n");
                    scanf("%c", &safetyCheck);

                    //if the user regretted and doesn't want to delete
                    if(safetyCheck != 'y'){
                        printf("The deletion of the word has been canceled.\n");
                        free(deleteStr);
                        deleteStr = NULL;
                        break;
                    }

                    //if he still does
                    else{
                        int deleteCheck = 0;

                        Word *wordToFree = findWord(dictionaries[deleteChoice -1],deleteStr);

                        //free the string allocation
                        free(deleteStr);
                        deleteStr = NULL;

                        //if no such word exist
                        if(wordToFree == NULL){
                            printf("The deletion of the word has failed!\n");
                        }

                        //if there is such word
                        else{
                            deleteCheck = deleteWord(wordToFree, &dictionaries[deleteChoice -1]);

                            //checking if the word was deleted or not
                            if(deleteCheck == 0){
                                printf("The deletion of the word has failed!\n");
                            }
                            else{
                                printf("The word has been deleted successfully!\n");
                            }
                        }
                    }
                    break;
                case 4:

                    //if there are no dictionaries
                    if(numOfDictionaries == 0){
                        printf("This option is not available right now, try again:\n");
                        menuFlag = 1;
                        break;
                    }

                    //printing the dictionaries
                    printDictionary(numOfDictionaries, dictionaries);

                    int findChoice = 0;

                    //clear the buffer and get the choice from the user
                    while (getchar() != '\n');
                    scanf("%d", &findChoice);

                    //while loop to check if the word is valid
                    while(findChoice > numOfDictionaries || findChoice < 1){
                        printf("Wrong option, try again:\n");
                        scanf("%d", &findChoice);
                    }

                    printf("Enter a word in %s:\n",dictionaries[findChoice -1].languages[0]);

                    //creating a pointer to an array of string and sending it to a function to find
                    char *newStr = makeStr();
                    Word *printWord = findWord(dictionaries[findChoice -1],newStr);


                    //if the translation is not found
                    if(printWord == NULL){
                        printf("There are no translations for \"%s\" in this dictionary.\n", newStr);
                        free(newStr);
                        newStr = NULL;
                        break;
                    }

                    //if the word was found
                    else{
                        free(newStr);
                        newStr = NULL;

                        //printing the translations by format
                        printf("The translations are:\n");
                        for(int i = 1; i < dictionaries[findChoice - 1].numOfLanguages;i++){
                            if(i == dictionaries[findChoice - 1].numOfLanguages - 1){
                                printf("%s: %s",dictionaries[findChoice -1].languages[i],
                                                                                        printWord->translations[i]);
                            }
                            else{
                                printf("%s: %s, ",dictionaries[findChoice -1].languages[i],
                                                                                        printWord->translations[i]);
                            }
                        }
                        printf("\n");
                    }
                    break;
                case 5:

                    //if there are no dictionaries
                    if(numOfDictionaries == 0){
                        printf("This option is not available right now, try again:\n");
                        menuFlag = 1;
                        break;
                    }

                    //printing the dictionaries
                    printDictionary(numOfDictionaries, dictionaries);

                    int deleteDicChoice;
                    char safetyDicCheck = 'n';

                    scanf("%d", &deleteDicChoice);

                    //while loop to check if the choice is valid
                    while(deleteDicChoice > numOfDictionaries || deleteDicChoice < 1){
                        printf("Wrong option, try again:\n");
                        scanf("%d", &deleteDicChoice);
                    }

                    //safety check
                    printf("Are you sure? (y/n)\n");

                    //clear the buffer and get the choice from the user
                    while (getchar() != '\n');
                    scanf("%c", &safetyDicCheck);

                    //if the user regretted and doesn't want to delete
                    if(safetyDicCheck != 'y'){
                        printf("The deletion of the dictionary has been canceled.\n");
                        break;
                    }

                    int successCheck = numOfDictionaries;

                    numOfDictionaries = deleteDictionary(dictionaries, deleteDicChoice, numOfDictionaries);

                    if(successCheck == numOfDictionaries){
                        printf("The deletion of the dictionary has failed!\n");
                        break;
                    }

                    printf("The dictionary has been deleted successfully!\n");
                    break;

                case 6:
                    deleteEverithing(dictionaries, numOfDictionaries);
                    printf("Bye!");
                    return 0;

                default:
                    break;
            }
        }
    }
}