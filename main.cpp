#include <fstream>
#include <iostream>
#include <map>

// Function Declarations
double setArrays(std::string arr_words[], std::string best_words[], int capacity);
void read_file(std::string fileName, std::string arr_word[], int &capacity, int &brokenButton);
bool isCapital(char c);
void set_up_map(bool isBroken = false, int brokenButton = 0);
double calculate_time(std::string word);
double calculate_start_time(char firstLetter);
char upper_to_lower(char letter);
void reset_array(std::string arr[], int capacity);
void print_bestWords(std::string arr[], int capacity, double time);

static std::map<char, int> keymap;

int main()
{
    std::cout << "What file would you like to test (Please Append .txt)" << std::endl;
    std::string fileName = "";
    std::cin >> fileName;
    int num_of_words = 10;        // Max number of words
    std::string arr_words[11]{};  // Array to hold words from text file
    std::string best_words[10]{}; // Array to hold the best words
    int brokenButton = 0;         // Broken Button number (If applicable)

    read_file(fileName, arr_words, num_of_words, brokenButton);

    if (brokenButton != 0)
    {
        set_up_map(true, brokenButton);
    }
    else
    {
        set_up_map();
    }
    double bestTime = setArrays(arr_words, best_words, num_of_words);
    print_bestWords(best_words, num_of_words, bestTime);
    return 0;
}

// Read the provided file and save each word int
void read_file(std::string fileName, std::string arr_word[], int &capacity, int &brokenButton)
{
    int count = 0;
    std::ifstream fileReader; // Filereader to read input
    fileReader.open(fileName);
    if (fileReader.is_open())
    {
        while (!fileReader.eof())
        {
            fileReader >> arr_word[count];
            if (arr_word[count][0] < 'A')
            {                                                 // Check if it is a number
                brokenButton = atoi(arr_word[count].c_str()); // Set the brokenButton
                count--;
            }
            count++;
        }
    }
    capacity = count;   // Set capacity to the new capacity
    fileReader.close(); // Close file reader
}
// Map holds a two digit number, where first digit repersents position and 2nd digit repersents which key it is on
// Eg 'a' is on the 1st position of the number 2 key so it will be held as 12
void set_up_map(bool isBroken, int brokenButton)
{                             // Set up hashmap
    char starting_char = 'a'; // Starting character value (Hashmap only sets lowercase letters)
    for (int i = 2; i <= 9; i++)
    {
        if (isBroken && i == brokenButton)
        { // If we are dealing with a broken button, shift everything over to the last digit
            if (i == 9 || i == 7)
            { // 9 and 7 hold 4 instead of 3 so need to seperatly account for that
                keymap[starting_char] = (i - 1) + 40;
                keymap[starting_char + 1] = (i - 1) + 50;
                keymap[starting_char + 2] = (i - 1) + 60;
                keymap[starting_char + 3] = (i - 1) + 70;
                starting_char += 4;
            }
            else if (i == 2)
            { // since 2 is the first digit, we need to shift it into the start of 3 instead
                keymap[starting_char] = (i + 1) + 10;
                keymap[starting_char + 1] = (i + 1) + 20;
                keymap[starting_char + 2] = (i + 1) + 30;
                starting_char += 3;
            }
            else
            { // Every other number gets shifted back
                keymap[starting_char] = (i - 1) + 40;
                keymap[starting_char + 1] = (i - 1) + 50;
                keymap[starting_char + 2] = (i - 1) + 60;
                starting_char += 3;
            }
        }
        else
        {
            if (i == 3 && isBroken && brokenButton == 2)
            { // If we are on 3 and 2 was the broken button, need to shift 3
                keymap[starting_char] = i + 40;
                keymap[starting_char + 1] = i + 50;
                keymap[starting_char + 2] = i + 60;
                starting_char += 3;
            }
            else if (i != 7 && i != 9)
            { // Number is not 7 or 9, just set up normally
                keymap[starting_char] = i + 10;
                keymap[starting_char + 1] = i + 20;
                keymap[starting_char + 2] = i + 30;
                starting_char += 3;
            }
            else
            { // is 7 or 9
                keymap[starting_char] = i + 10;
                keymap[starting_char + 1] = i + 20;
                keymap[starting_char + 2] = i + 30;
                keymap[starting_char + 3] = i + 40;
                starting_char += 4;
            }
        }
    }
}
// Function to set the array of best words
double setArrays(std::string arr_words[], std::string best_words[], int capacity)
{
    double bestTime = UINT_MAX; // By default set the best time to be the largest possible int value
    int count = 0;              // Counter variable for best_words array
    for (int i = 0; i < capacity; i++)
    {
        double currTime = calculate_time(arr_words[i]); // Calculate the time it takes to type the current word
        if (currTime < bestTime)
        { // If we have a new best time
            count = 0;
            reset_array(best_words, 10); // sets all the entries of the passed array to the empty string
            best_words[count] = arr_words[i];
            bestTime = currTime;
            count++;
        }
        else if (currTime == bestTime)
        { // Add on the word to array
            best_words[count] = arr_words[i];
            count++;
        }
    }
    return bestTime;
}
// Function to calculate the time it takes for a word to be typed
double calculate_time(std::string word)
{
    char prev_char = word[0];                            // Get first character
    double total_time = calculate_start_time(prev_char); // Calculate the time it takes for it to be typed
    prev_char = upper_to_lower(prev_char);               // Convert it to lowercase
    for (int i = 1; word[i] != '\0'; i++)
    {
        char curr_char = upper_to_lower(word[i]);
        if (isCapital(word[i]))
        { // If the character is capital it will need an extra 2 seconds
            total_time += 2;
        }
        if (curr_char == prev_char)
        {                      // if the current letter of the word is the same as the previous letter of the word
            total_time += 0.5; // Half a second wait
            int position = keymap[curr_char] / 10;
            total_time += 0.25 * (position - 1); // This calculates how many times the button must be pressed to reach the position
        }
        else if (keymap[curr_char] % 10 == keymap[prev_char] % 10)
        { // if the current letter of the word is on the same keypad as the previous letter of the word
            total_time += 0.5;
            int position = keymap[curr_char] / 10;
            total_time += 0.25 * (position - 1);
        }
        else
        { // if the current letter of the word is not on the same keypad as the previous letter of the word
            total_time += 0.25;
            int position = keymap[curr_char] / 10;
            total_time += 0.25 * (position - 1);
        }
        prev_char = upper_to_lower(word[i]);
    }
    return total_time;
}

void print_bestWords(std::string arr[], int capacity, double time)
{ // Function to print out the contents of bestwords array
    for (int i = 0; i < capacity; i++)
    {
        if (arr[i] == "")
        {
            return;
        }
        std::cout << arr[i] << " = " << time << "s" << std::endl;
    }
}

void reset_array(std::string arr[], int capacity)
{ // Function to reset Array
    for (int i = 0; i < capacity; i++)
    {
        arr[i] = "";
    }
}

bool isCapital(char c)
{ // Function to check if a character is capital
    if (c <= 'Z')
    {
        return true;
    }
    return false;
}

char upper_to_lower(char letter)
{ // Function to convert uppercase to lowercase
    // only runs when bool isCapital is true
    if (isCapital(letter))
    {
        letter += 32;
    }
    return letter;
}

double calculate_start_time(char firstLetter)
{ //Function to calculate start time
    double total_time = 0;
    if (isCapital(firstLetter))
    {
        total_time += 2;
    }
    int position = keymap[upper_to_lower(firstLetter)] / 10;
    total_time += 0.25 * (position - 1);
    return total_time;
}
