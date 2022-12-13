#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <conio.h>
#include <windows.h>

using namespace std;

// -------------------------- START - Constants  --------------------------

const int KEY_UP = 72;
const int KEY_DOWN = 80;
const int KEY_ENTER = 13;
const int KEY_SPACE = 32;
const int KEY_W = 119;
const int KEY_S = 115;

const std::string reset("\033[32;40m");
const std::string magenta("\033[0;35m");
// -------------------------- END - Constants  --------------------------

// -------------------------- START - Structs Definitions  --------------------------

template <typename T>
std::vector<T> create_copy(std::vector<T> const &vec)
{
    std::vector<T> v;
    v.assign(vec.begin(), vec.end());
    return v;
}
template <typename T>
void shuffle_vector(vector<T> &vec)
{
    unsigned seed = std::chrono::system_clock::now()
                        .time_since_epoch()
                        .count();
    shuffle(vec.begin(), vec.end(), default_random_engine(seed));
}

vector<string> splitStringToLines(string h)
{
    vector<string> arr;
    string nice = "";
    for (int i = 0; i < h.length(); i++)
    {
        char c = h[i];
        if (c == '\n')
        {
            arr.push_back(nice);
            nice = "";
        }
        nice += c;
    }
    return arr;
}

struct Question
{
public:
    string question;
    string correctAnswer;
    vector<string> wrongAnswers;

    Question(
        string q, string correctAnswer, initializer_list<string> wrongAnswers) : question(q),
                                                                                 correctAnswer(correctAnswer),
                                                                                 wrongAnswers(wrongAnswers){};
    Question(
        string q, string correctAnswer, vector<string> wrongAnswers) : question(q),
                                                                       correctAnswer(correctAnswer),
                                                                       wrongAnswers(wrongAnswers){};
    Question() {}

    vector<string> getChoices(bool randomize = true)
    {
        auto choices = create_copy(wrongAnswers);
        choices.push_back(correctAnswer);
        if (randomize)
        {
            shuffle_vector(choices);
        }
        return choices;
    }
};

struct QuestionHistory : public Question
{
public:
    QuestionHistory(
        Question q,
        int chosenAnswerIndex,
        vector<string> choices) : Question(q.question, q.correctAnswer, q.wrongAnswers)
    {
        this->choices = choices;
        this->chosenAnswerIndex = chosenAnswerIndex;
    }

    vector<string> choices;
    int chosenAnswerIndex;
    bool GotCorrectAnswer()
    {
        return choices[chosenAnswerIndex] == correctAnswer;
    }
    int GetScore()
    {
        if (GotCorrectAnswer())
        {
            return 1;
        }
        return 0;
    }
};

// -------------------------- END - Structs Definitions  --------------------------

// -------------------------- START - General Functions --------------------------
bool validateInput(int input)
{
    if (input <= 0 || input > 4)
    {
        return false;
    }
    return true;
}

void clear_input()
{
    cin.clear();
    while (cin.get() != '\n')
    {
    }
}

void acout(string h, bool skippable = true, int ms = 20)
{
    int index = 0;
    for (char c : h)
    {
        cout << c;
        index++;
        Sleep(ms);
        if (skippable && _kbhit())
        {
            getch();
            string rest = h.substr(index);
            cout << rest;
            break;
        }
    }
}

void clear_screen()
{
    system("CLS");
}

void fakeLoading(string message, int ms = 1500)
{
    const int MS = 100;
    int loops = (ms / MS);
    int dots = 0;
    while (loops > 0)
    {
        clear_screen();
        cout << message;
        for (int i = 0; i < dots; i++)
        {
            cout << ".";
        }
        dots++;
        if (dots == 5)
        {
            dots = 0;
        }
        loops--;
        Sleep(MS);
    }
}

void waitForKey(int key = -1)
{

    while (true)
    {
        if (_kbhit())
        {
            int k = getch();
            if (key < 0 || k == key)
            {
                break;
            }
        }
    }
}

int interactiveInput(string label, vector<string> choices, string endLabel = "Press ENTER/SPACE to select. Arrow keys to move. \n")
{
    int _index = 0;
    int c = 0;
    bool looping = true;
    int choiceIndex = 0;
    int maxNum = choices.size() - 1;

    clear_screen();
    acout(label + "\n\n");
    Sleep(75);
    for (string h : choices)
    {
        if (_index == choiceIndex)
        {
            cout << magenta;
            cout << "=>";
        }
        else
        {
            cout << "  ";
        }
        cout << h;
        cout << reset;

        cout << "\n";
        choiceIndex++;
        Sleep(75);
    }
    acout("\n" + endLabel);
    while (looping)
    {
        choiceIndex = 0;
        clear_screen();
        cout << label << endl
             << endl;

        for (string h : choices)
        {
            if (_index == choiceIndex)
            {
                cout << magenta;
                cout << "=>";
            }
            else
            {
                cout << "  ";
            }
            cout << h;
            cout << reset;
            cout << endl;
            choiceIndex++;
        }
        cout << endl
             << endLabel;

        switch (c = getch())
        {
        case KEY_UP:
        case KEY_W:
            _index--;
            if (_index < 0)
            {
                _index = maxNum;
            }
            break;
        case KEY_DOWN:
        case KEY_S:
            _index++;
            if (_index > maxNum)
            {
                _index = 0;
            }
            break;
        case KEY_ENTER:
        case KEY_SPACE:
            looping = false;
            break;
        }
    }
    return _index;
}

// -------------------------- END - General Functions --------------------------

// -------------------------- START - Main code --------------------------

vector<Question> easyQuestions = {
    Question(
        "What programming language is C++?",
        "Object-Oriented Language",
        {"Functional Programming Language",
         "Scripting Language",
         "Logic Programming Language"}),

    Question(
        "Who developed C++?",
        "Bjarne Stroustrup",
        {"James Gosling",
         "Guido van Rossum",
         "Tim Berners-Lee"}),

    Question(
        "What input/output methods does C++ use?",
        "cout and cin",
        {"printf and stdin",
         "class and break",
         "del and with"}),

    Question(
        "Which of the following IDEs are used for C++?",
        "CodeBlocks",
        {"Eclipse",
         "BlueJ",
         "PyCharm"}),

    Question(
        "What header file is used in C++?",
        "iostream",
        {"stdio.h",
         "HelloWorld.h",
         "syntax"}),

    Question(
        "What variable is to be used to create user input in C++?",
        "cin",
        {"scanf",
         "cout",
         "printf"}),

    Question(
        "Which of the following operators is to be used to divide values in C++?",
        "/",
        {"*",
         "\\",
         "|"}),
    Question(
        "Which character inserts a new line in C++?",
        "\\n",
        {"\\f",
         "\\d",
         "\\i"}),

    Question(
        "Which of the following operators is to be used to add increment in C++?",
        "++",
        {"==",
         "--",
         "\\/\\/"}),

    Question(
        "Which of the following operators is to be used to multiply values in C++?",
        "*",
        {"x",
         "()",
         "^"})};

vector<Question> mediumQuestions = {
    Question(
        "Define the data type \"char\" in C++.",
        "Stores a single character/letter/number or ASCII values.",
        {"Used for a combination of any characters that appear on a keyboard, such as letters, numbers, and symbols.",
         "Used to display various compiler or linker messages.",
         "Used for whole numbers."}),

    Question(
        "Define the data type \"boolean\" in C++.",
        "Used where data is restricted to True/False or yes/no options.",
        {"Stores a single character/letter/number, or ASCII values.",
         "Used for a combination of any characters that appear on a keyboard, such as letters, numbers, and symbols.",
         "Used for whole numbers."}),

    Question(
        "Define the data type \"float\" in C++.",
        "Used for numbers that contain decimal points, or for fractions. Sufficient for storing 7 decimal digits.",
        {"Stores a single character/letter/number, or ASCII values.",
         "Used where data is restricted to True/False or yes/no options.",
         "Used for whole numbers."}),

    Question(
        "What are strings used for?",
        "Used for storing text. For a combination of any characters that appear on a keyboard, such as letters, numbers, and symbols.",
        {"Used for numbers that contain decimal points, or for fractions. Sufficient for storing 7 decimal digits.",
         "Used where data is restricted to True/False or yes/no options.",
         "Used for whole numbers."}),

    Question(
        "What is a comment in C++ code?",
        "Used to explain C++ code, and to make it more readable.",
        {"Used to teach how to properly use C code.",
         "To declare a code.",
         "Clarifies the content."}),

    Question(
        "Which library allows us to work with files?",
        "fstream",
        {"iostream",
         "include",
         "iosmain"}),

    Question(
        "Which of the following is an access specifier?",
        "Public",
        {"Void",
         "Urban",
         "Rural"}),

    Question(
        "This indicates the end of one logical entity.",
        "Semicolon",
        {"Bracket",
         "Period",
         "None of the above."}),

    Question(
        "Which keyword is used to create a structure?",
        "Struct",
        {"Strctr",
         "Structure",
         "Std"}),

    Question(
        "How do you insert comments in C++ code?",
        "//insert a comment",
        {"*/insert a comment",
         "*insert a comment",
         "*insert a comment*"})};

vector<Question> hardQuestions = {
    Question(
        "Define constructors.",
        "A special method that is automatically called when an object of a class is created.",
        {"A graphical representation of a computer program in relation to its sequence of functions.",
         "A diagram of the sequence of movements or actions of people or things involved in a complex system or activity.",
         "A set of steps or instruction statements to be followed to accomplish specific tasks."}),

    Question(
        "What is ofstream class?",
        "Creates and writes to files. This data type represents the output file stream and is used to create files and to write information to files.",
        {"This data type represents the input file stream and is used to read information from files.",
         "Provides basic input and output services for C++ programs.",
         "A way of including a standard or user-defined file in the program and is mostly written at the beginning of any C/C++ program."}),

    Question(
        "What is a structure?",
        "A way to group several related variables into one place.",
        {"The set of facilities and systems that encompasses the services and facilities necessary for its economy, households, and firms to function.",
         "The arrangement of and relations between the parts or elements of something complex.",
         "Construct or arrange according to a plan; give a pattern or organization to."}),

    Question(
        "What is the difference between C++ and C?",
        "C++ supports classes and objects while C does not.",
        {"C++ is a pre-compiled programming language and doesn't need any interpreter during compilation while C is an interpreted language, and it runs through an interpreter during compilation.",
         "C++ and C have different code structure and compilation.",
         "There is no difference between the two."}),

    Question(
        "What is the similarity between C++ and C?",
        "Both languages have almost the same syntax.",
        {"Function and operator overloading are both supported by C and C++.",
         "They both use printf to display the output.",
         "Data and functions are separated in C++, as well as C."}),

    Question(
        "How do we use \"if...else\" statement in C++?",
        "\"if...else\" statement is used if a condition is true and another code if that condition is false.",
        {"\"if...else\" statement is used if a condition is false and another code if that condition is false.",
         "\"if...else\" statement is used if a condition is true and another code if that condition is true",
         "None of the above."}),

    Question(
        "What is the correct syntax to output \"Hello World\" in C++?",
        "cout<< \"Hello World\";",
        {"print(\"Hello World\");",
         "Console.WriteLine(\"Hello World\");",
         "std::cout<\"Hello Word\";"}),
    Question(
        R"(What is the output of the following program?

    #include<iostream>
    using namespace std;

    int myFunction(int x) {
        return 5 + x;
    }

    int main() {
        cout << myFunction(3);
        return 0;
    }
        )",
        "8",
        {"3",
         "Error",
         "No output."}),
    Question(
        R"(Find the error in the following program:

    #include<iostream>
    using namespace std;

    int main() {
        int i = 0;
        while(i = 0; i < 3; i++) {
            cout << i;
        }
        return 0;
    }
        )",
        "while",
        {"main",
         "return",
         "No error"}),
    Question(
        R"(Find the error in the following program:

    #include<iostream>
    using namespace std;

    void main() {
        char s[] = \"hello\", t[] = \"hello\";
        if(s == t)
            cout << \"Equal strings\";
    }
        )",
        "No output",
        {"Error",
         "Equal strings",
         "hello"})};

vector<QuestionHistory> history = {};

string playerName = "UNDEFINED";

int initiateQuiz(vector<Question> _questions)
{
    vector<Question> questions = create_copy(_questions);
    int score = 0;
    int questionIndex = 0;
    history.clear();

    for (Question i : questions)
    {
        clear_screen();
        vector<string> choices = i.getChoices();
        int questionNumber = questionIndex + 1;
        int answer =
            interactiveInput(to_string(questionNumber) + ".) " + i.question,
                             choices);

        history.push_back(QuestionHistory(i, answer, choices));
        string chosenAnswer = choices[answer];
        if (chosenAnswer == i.correctAnswer)
        {
            score++;
        }
        questionIndex++;
        cout << endl;
    }

    return score;
}

int main()
{
    system("COLOR 02"); // Change console color

    switch (interactiveInput("Welcome to Quiz", {"Start Game", "Quit Game"}))
    {
    case 0:
        break;
    case 1:
        return 0;
    }

    clear_screen();

    while (true)
    {
        acout("Enter username: ");
        getline(cin, playerName);
        if (
            all_of(playerName.cbegin(), playerName.cend(), [](char c)
                   { return std::isspace(c); }))
        {

            clear_screen();
            continue;
        }
        break;
    }

    bool nameExceededLength = playerName.length() >= 30;
    playerName = playerName.substr(0, 30);
    if (nameExceededLength)
    {
        playerName += "...";
    }

    // Main Loop
    while (true)
    {
        int chosenDifficulty = interactiveInput(playerName + ", please choose the difficulty", vector<string>{"Easy", "Medium", "Hard"});
        vector<Question> questionToAnswer;
        switch (chosenDifficulty)
        {
        case 0:
            questionToAnswer = easyQuestions;
            break;
        case 1:
            questionToAnswer = mediumQuestions;
            break;
        case 2:
            questionToAnswer = hardQuestions;
            break;
        default:
            questionToAnswer = easyQuestions;
            break;
        }
        clear_screen();
        questionToAnswer = create_copy(questionToAnswer);
        shuffle_vector(questionToAnswer);

        fakeLoading("Loading questions", questionToAnswer.size() * 100);
        Sleep(100);

        int score = initiateQuiz(questionToAnswer);
        clear_screen();

        // Render fake checking...
        fakeLoading("Checking results", questionToAnswer.size() * 100);

        Sleep(100);
        clear_screen();

        // Render history
        int index = 0;
        for (QuestionHistory qh : history)
        {
            cout << ++index << ".) " << qh.question << " (" << qh.GetScore() << "/1) point/s\n\n";
            int choiceIndex = 0;
            for (string choice : qh.choices)
            {
                cout << choiceIndex + 1 << ". " << choice << " ";
                if (qh.GotCorrectAnswer() && qh.chosenAnswerIndex == choiceIndex)
                {
                    cout << "(Correctly chosen)";
                }
                else
                {
                    if (qh.choices[choiceIndex] == qh.correctAnswer)
                    {
                        cout << "(Correct)";
                    }
                    else if (qh.chosenAnswerIndex == choiceIndex)
                    {
                        cout << "(Chosen)";
                    }
                }
                cout << endl;
                choiceIndex++;
                Sleep(20);
            }
            cout << endl;
            Sleep(20);
        }
        // Show score
        cout << "----------------------------------------------------------------------------------------------------" << endl;
        Sleep(100);
        acout(playerName + ", you scored " + to_string(score) + "/" + to_string(questionToAnswer.size()) + " points");
        cout << endl
             << "----------------------------------------------------------------------------------------------------" << endl;
        Sleep(500);

        cout << endl
             << "Press ENTER key to continue..." << endl;
        waitForKey(KEY_ENTER);
        const vector<string> _ = {"Play again", "Quit"};
        int shouldQuit = interactiveInput("Play again?", _);
        if (shouldQuit == 0)
        {
            continue;
        }
        break;
    } // End of main loop

    const string THANK_YOU = R"(
******** **  **     **     **   ** **   **    **    ** ******* **   **  **
   **    **  **    *  *    ***  ** **  **      **  **  **   ** **   **  **
   **    ******   ******   ** * ** *****         **    **   ** **   **  **
   **    **  **  **    **  **  *** **  **        **    **   ** **   **
   **    **  ** **      ** **   ** **   **       **    ******* *******  **
   )";

    cout << endl
         << endl;
    auto arr = splitStringToLines(THANK_YOU);
    for (string s : arr)
    {
        cout << s;
        Sleep(100);
    }
    cout << endl
         << endl;

    waitForKey();

    return 0;
}
