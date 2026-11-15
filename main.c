#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_songs 50
#define text_size 100

struct song { // one structure holds all the details of a song
    char band[text_size];
    char title[text_size];
    float price;
    int year;
};

// function prototypes = tell compiler each function’s name, what information it takes, and what it returns (so main is read first, and all other functions displayed after -> improving readbility)
void show_menu(void); // prints menu takes no information + returns nothing
void read_line(FILE *source, char text[]); // reads line from file / keyboard input into text + returns nothing
int enter_data(struct song catalogue[], int song_count); // adds song to catalogue + returns updated song count (int)
void save_to_file(struct song catalogue[], int song_count); // aaves  songs in  catalogue to  file + returns nothing
int read_from_file(struct song catalogue[]); // loads songs from  file into  catalogue + returns how many were loaded (int)
void sort_by_year(struct song catalogue[], int song_count); // sorts  catalogue by year, newest first + returns nothing
void print_catalogue(struct song catalogue[], int song_count); // prints  songs in  catalogue + returns nothing

int main(void)
{
    struct song catalogue[max_songs]; // creates the array for up to 50 song, 1 struct = 1 song
    int song_count = 0; // how many songs are stored at the moment
    int choice = 0; // the menu number the user chooses
    char input[text_size]; // temp space to read  user's menu choice as text

    do { // repeats until the user picks exit
        show_menu();
        read_line(stdin, input); // // reads line from keyboard into input
        choice = atoi(input); // turns text -> number

        switch (choice) { // runs case matching menu number
            case 1:
                song_count = enter_data(catalogue, song_count); // updates main's count with  new count returned by enter_data
                break;
            case 2:
                save_to_file(catalogue, song_count);
                break;
            case 3:
                song_count = read_from_file(catalogue);
                break;
            case 4:
                print_catalogue(catalogue, song_count);
                break;
            case 5:
                printf("Exiting program.\n");
                break;
            default: // any other number
                printf("Invalid option, please choose 1 to 5.\n");
        }
    } while (choice != 5); // placed here so menu runs no matter what, and then condition runs

    return 0;
}

void show_menu(void) // 1st void = function returns no value. 2nd value = takes no info when called, no params
{
    printf("\n---------------------------\n");
    printf("Song Catalogue Menu\n");
    printf("---------------------------\n");
    printf("[1] Enter data\n");
    printf("[2] Save data to file\n");
    printf("[3] Read data from file\n");
    printf("[4] Print out the catalogue to screen\n");
    printf("[5] Exit Program\n");
    printf("---------------------------\n");
    printf("Enter choice (1-5): ");
}

void read_line(FILE *source, char text[]) // reads a line from source into text + returns nothing
{
    if (fgets(text, text_size, source) == NULL) { // fgets limits characters read, unlike gets
        text[0] = '\0'; // empty string if nothing was read
        return;
    }
    text[strcspn(text, "\n")] = '\0'; // fgets keeps newline, this removes it
}

int enter_data(struct song catalogue[], int song_count)
{
    char input[text_size]; // temporary space for number input, used to be read before converting

    char *endptr; // points to where strtof() stops reading number

    if (song_count == max_songs) {
        printf("The catalogue is full.\n");
        return song_count;
    }

    printf("Name of the Band or Singer: ");
    read_line(stdin, catalogue[song_count].band); // saved straight into the next free structure

    printf("Name of Song: ");
    read_line(stdin, catalogue[song_count].title);

    do { // repeats until valid price is entered
        printf("Price: £"); // asks user to enter price
        read_line(stdin, input); // reads price as text

        catalogue[song_count].price = strtof(input, &endptr); // converts text to float and records where conversion stops

        if (endptr == input || *endptr != '\0' || catalogue[song_count].price < 0) { // checks for no number, extra characters or negative price
            printf("Invalid price. Please enter a valid number.\n"); // displays error message
        }

    } while (endptr == input || *endptr != '\0' || catalogue[song_count].price < 0); // repeats while price is invalid

    printf("Year of Release: ");
    read_line(stdin, input);
    catalogue[song_count].year = atoi(input);

    return song_count + 1; // one more song in the catalogue
}

void save_to_file(struct song catalogue[], int song_count) // saves catalogue songs to file
{
    FILE *file_pointer; // holds connection to open file
    int i; // loop counter for each song

    if (song_count == 0) { // checks if catalogue has no songs
        printf("There is no data to save.\n"); // tells user there is nothing to save
        return; // ends function early
    }

    file_pointer = fopen("songs.txt", "w"); // "w" creates file + replaces old contents
    if (file_pointer == NULL) { // fopen gives NULL if the file could not be opened
        perror("Error opening file"); // print error + reason
        return;
    }

    for (i = 0; i < song_count; i++) { // four lines per song
        fprintf(file_pointer, "%s\n", catalogue[i].band); // like printf but writes to the file
        fprintf(file_pointer, "%s\n", catalogue[i].title);
        fprintf(file_pointer, "%.2f\n", catalogue[i].price);
        fprintf(file_pointer, "%d\n", catalogue[i].year);
    }

    fclose(file_pointer); // closing makes sure everything is saved
    printf("%d songs saved to songs.txt\n", song_count);
}

int read_from_file(struct song catalogue[]) // loads songs into catalogue + returns how many loaded
{
    FILE *file_pointer; // holds the open file
    char input[text_size]; // temporary space for one line of text
    int song_count = 0; // starts the number of loaded songs at zero

    file_pointer = fopen("songs.txt", "r"); // "r" opens  file for reading ONLY
    if (file_pointer == NULL) { // checks whether the file opened
        perror("Error opening file"); // prints an error message + reason
        return 0;
    }

    while (song_count < max_songs && fgets(input, text_size, file_pointer) != NULL) { // reads the band line, stops when the file runs out
        input[strcspn(input, "\n")] = '\0'; // strcspn = finds position of newline character in input + removes it
        strcpy(catalogue[song_count].band, input); // copies the text into  structure's band string

        read_line(file_pointer, catalogue[song_count].title); // reads title into current song

        read_line(file_pointer, input); // reads price as text
        catalogue[song_count].price = atof(input); // converts price text to decimal

        read_line(file_pointer, input);// reads year as text
        catalogue[song_count].year = atoi(input); // converts year text to whole number

        song_count++; // +1 increment of song count
    }

    fclose(file_pointer); // closes file after reading
    printf("%d songs read from songs.txt\n", song_count); // prints how many songs were loaded
    return song_count;
}

void sort_by_year(struct song catalogue[], int song_count) // sorts catalogue by year, newest first
{
    struct song temp; // temporary space for one song during swap
    int i, j; // loop counters

    for (i = 0; i < song_count - 1; i++) { // repeats passes until songs are sorted
        for (j = 0; j < song_count - 1 - i; j++) {  // checks neighbouring songs
            if (catalogue[j].year < catalogue[j + 1].year) { // checks if left song is older
                temp = catalogue[j]; // saves left song
                catalogue[j] = catalogue[j + 1]; // moves newer song left
                catalogue[j + 1] = temp; // moves saved song right
            }
        }
    }
}

    void print_catalogue(struct song catalogue[], int song_count) // prints songs in catalogue
    {
        int i; // loop counter for each song

        if (song_count == 0) { // checks if catalogue has no songs
            printf("The catalogue is empty.\n"); // tells user catalogue is empty
            return; // ends function early
        }

        sort_by_year(catalogue, song_count); // sorts songs newest to oldest

        printf("\n---------------------------\n");
        printf("Song Catalogue\n");
        printf("---------------------------\n");

        for (i = 0; i < song_count; i++) {// repeats once for each song
            printf("Name of Band: %s\n", catalogue[i].band);
            printf("Name of Song: %s\n", catalogue[i].title);
            printf("Price: £%.2f\n", catalogue[i].price);
            printf("Year of Release: %d\n", catalogue[i].year);
            printf("---------------------------\n");
        }
    }