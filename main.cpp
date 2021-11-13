#include <iostream>
#include <fstream>
#include <map>

void part_1();
void read_file(std::string fileName, std::string arr_word[]);
bool isCapital(char c);
void set_up_map();
double calculate_time(std::string word);
double calculate_start_time(char firstLetter);
static std::map<char, int> keymap;
char upper_to_lower(char letter);

int main(){
    set_up_map();
    part_1();
    return 0;
}

bool isCapital(char c){
    if(c <= 'Z'){
        return true;
    }
    return false;
}

char upper_to_lower(char letter) {
    // only runs when bool isCapital is true
    if(isCapital(letter)){
        letter += 32; 
    }
    return letter;
}
double calculate_start_time(char firstLetter){
    double total_time =0;
    if(isCapital(firstLetter)){
         total_time += 2;
    }
    int position = keymap[upper_to_lower(firstLetter)] / 10; 
    total_time += 0.25 * (position - 1);
    return total_time;
}
double calculate_time(std::string word){
    char prev_char = word[0];
    double total_time = calculate_start_time(prev_char);
    prev_char = upper_to_lower(prev_char);
    for(int i = 1; word[i] != '\0'; i++){
        char curr_char = upper_to_lower(word[i]);
        if(isCapital(word[i])){
            total_time += 2;
       }
        if(curr_char == prev_char){  // if the current letter of the word is the same as the previous letter of the word
            total_time += 0.5;
            int position = keymap[curr_char] / 10; 
            total_time += 0.25 * (position - 1);
        }else if(keymap[curr_char] % 10 == keymap[prev_char] % 10){ // if the current letter of the word is on the same keypad as the previous letter of the word
            total_time += 0.5;
            int position = keymap[curr_char] / 10; 
            total_time += 0.25 * (position - 1);
        }else{  // if the current letter of the word is not on the same keypad as the previous letter of the word
            total_time += 0.25;
            int position = keymap[curr_char] / 10;
            total_time += 0.25 * (position - 1);
        }
        prev_char = upper_to_lower(word[i]);
    }
    return total_time;
}
void part_1(){
    std::string arr_words[10]{};
    std::string best_words[10]{};
    read_file("Test1.txt",arr_words);
    for(int i = 0; i < 10; i++){
        std::cout << arr_words[i] << ": " <<calculate_time(arr_words[i]) << std::endl;
    }
}

void read_file(std::string fileName, std::string arr_word[]){
    int count = 0;
    std::ifstream fileReader;
    fileReader.open(fileName);
    if(fileReader.is_open()){
        while(!fileReader.eof()){
            fileReader >> arr_word[count];
            count++;
        }
    }
    fileReader.close();
}

void set_up_map(){
    char starting_char = 'a';
    for(int i = 2; i <= 9;i++){
        if(i != 7 && i != 9){
            keymap[starting_char] = i + 10;
            keymap[starting_char + 1] = i + 20;
            keymap[starting_char + 2] = i + 30;
            starting_char += 3;
        }else{ // is 7 or 9
            keymap[starting_char] = i + 10;
            keymap[starting_char + 1] = i + 20;
            keymap[starting_char + 2] = i + 30;
            keymap[starting_char + 3] = i + 40;
            starting_char += 4;
        }
    }
}