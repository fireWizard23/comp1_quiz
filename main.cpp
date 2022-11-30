#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <conio.h>
#include <windows.h>


using namespace std;

const bool DEBUG = false;


template<typename T>
std::vector<T> create_copy(std::vector<T> const &vec)
{
    std::vector<T> v;
    v.assign(vec.begin(), vec.end());
    return v;
}
template<typename T>
void shuffle_vector(vector<T> &vec) {
    unsigned seed = std::chrono::system_clock::now()
                        .time_since_epoch()
                        .count();
    shuffle(vec.begin(), vec.end(), default_random_engine(seed));
}

struct Question {
    public:
        string question;
        string correctAnswer;
        vector<string> wrongAnswers;

        Question(
                 string q, string correctAnswer, initializer_list<string> wrongAnswers
        ):
            question(q),
            correctAnswer(correctAnswer),
            wrongAnswers(wrongAnswers){};
        Question(
                 string q, string correctAnswer, vector<string> wrongAnswers
        ):
            question(q),
            correctAnswer(correctAnswer),
            wrongAnswers(wrongAnswers){};
        Question() {}
        string debugString() {
        string wrongs;
        for(auto i : wrongAnswers) {
            wrongs += " " +  i  + " ";
        }
        return "Question: " + question + "\n"
        + "Correct Answer: " + correctAnswer +  "\n" +
         "Wrong Answers: " + wrongs+ "\n";
    }

    vector<string> getChoices(bool randomize=true) {
        auto choices = create_copy(wrongAnswers);
        choices.push_back(correctAnswer);
        if(randomize) {
            shuffle_vector(choices);

        }
        return choices;
    }

};




struct QuestionHistory : public Question {
    public:
        QuestionHistory(
             Question q,
             int chosenAnswerIndex,
             vector<string> choices
        ) : Question(q.question, q.correctAnswer, q.wrongAnswers) {
            this-> choices = choices;
            this->chosenAnswerIndex = chosenAnswerIndex;
        }


        vector<string>  choices;
        int chosenAnswerIndex;
        bool GotCorrectAnswer() {
            return choices[chosenAnswerIndex] == correctAnswer;
        }
        int GetScore() {
            if(GotCorrectAnswer()) {
                return 1;
            }
            return 0;
        }
};



bool validateInput(int input) {
    if(input <= 0 || input > 4) {
        return false;
    }
    return true;
}

void clear_input() {
    cin.clear();
    while(cin.get() != '\n') {}
}

void acout(string h, int ms=20) {
    for(char c : h) {
        cout << c;
        if(!DEBUG) {Sleep(ms);}
    }
}

void clear_screen() {
    system("CLS");
}

void fakeLoading(string message, int ms=1500) {
    const int MS = 100;
    int loops = (ms/MS);
    int dots = 0;
    while(loops > 0) {
        clear_screen();
        cout << message;
        for(int i=0; i < dots; i++) {
            cout << ".";
        }
        dots++;
        if(dots == 5) {
            dots = 0;
        }
        loops--;
        if(!DEBUG) {
            Sleep(MS);
        }
    }
}


vector<Question> easyQuestions = {
    Question("Easy Who invented the first calculator?", "Correct Answer", {"Wrong", "Eminem", "Dwayne Johnson"}),
    Question("Easy Who invented the second calculator?", "Correct Answer", {"Jackie Chad", "Willie Wonka", "Dwayne Johnson"}),
    Question("Easy Who invented the third calculator?", "Correct Answer", {"Luffy Uzumaki", "Blue", "The rock"}),
    Question("Easy Who invented the last calculator?", "Correct Answer", {"Nice Person", "Bruce", "Dwayne Lee"})
};


vector<Question> mediumQuestions = {
    Question("Medium Who invented the first calculator?", "Correct Answer", {"Wrong", "Eminem", "Dwayne Johnson"}),
    Question("Medium Who invented the second calculator?", "Correct Answer", {"Jackie Chad", "Willie Wonka", "Dwayne Johnson"}),
    Question("Medium Who invented the third calculator?", "Correct Answer", {"Luffy Uzumaki", "Blue", "The rock"}),
    Question("Medium Who invented the last calculator?", "Correct Answer", {"Nice Person", "Bruce", "Dwayne Lee"})
};

vector<Question> hardQuestions = {
    Question("Hard Who invented the first calculator?", "Correct Answer", {"Wrong", "Eminem", "Dwayne Johnson"}),
    Question("Hard Who invented the second calculator?", "Correct Answer", {"Jackie Chad", "Willie Wonka", "Dwayne Johnson"}),
    Question("Hard Who invented the third calculator?", "Correct Answer", {"Luffy Uzumaki", "Blue", "The rock"}),
    Question("Hard Who invented the last calculator?", "Correct Answer", {"Nice Person", "Bruce", "Dwayne Lee"})
};


string prefixes[4] = {"1. ", "2. ", "3. ", "4. "};

vector<QuestionHistory> history = {};

string playerName = "UNDEFINED";



int initiateQuiz(vector<Question> _questions)
{
    vector<Question> questions = create_copy(_questions);
    int score = 0;
    int questionIndex = 0;
    history.clear();

    for(Question i : questions) {
        clear_screen();
        int answer;
        vector<string> choices = i.getChoices();
        int questionNumber = questionIndex + 1;
        while(true) {

            acout(to_string(questionNumber) + ".) " + i.question  +"\n\n");
            int index = 0;
            for(auto choice : choices) {
                acout (prefixes[index] + choice + "\n");
                index++;
            }
            acout ("\nAnswer (number only): ");
            cin >> answer;
            if(cin.fail()|| !validateInput(answer))  {
                cout << endl;
                clear_input();
                continue;
            }
            break;
        }

        history.push_back( QuestionHistory(i, answer-1, choices) );
        string chosenAnswer = choices[answer-1];
        if(chosenAnswer == i.correctAnswer) {
            score++;
        }
        questionIndex++;
        cout << endl;
        clear_input();
    }

    return score;

}

int main() {
    system("COLOR 02");
    acout ("Welcome to Quiz\n");
    acout ("Press ANY key to continue ");

    while(true) {
        if(_kbhit() && getch()) {break;}
    }
    clear_screen();


    acout("What is your name? ");
    getline(cin, playerName);


    int chosenDifficulty;
    vector<Question> questionToAnswer;

    while(true) {
        clear_screen();
        acout(playerName + ", please choose the difficulty\n");
        acout("1. Easy\n2. Medium\n3. Hard\n Enter the number: ");
        cin >> chosenDifficulty;
        if(cin.fail() || chosenDifficulty < 0 || chosenDifficulty > 3) {
            clear_input();
            continue;
        }
        clear_input();
        break;
    }
    switch(chosenDifficulty) {
    case 1:
        questionToAnswer = easyQuestions;
        break;
    case 2:
        questionToAnswer = mediumQuestions;
        break;
    case 3:
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

    //Render history
    int index = 0;

    for(QuestionHistory qh : history) {

        cout << ++index << ".) " << qh.question << " (" << qh.GetScore() << "/1) point/s\n\n";

        int choiceIndex=0;
        for(string choice : qh.choices) {
            cout << choiceIndex+1 << ". " << choice << " ";
            if(qh.GotCorrectAnswer() && qh.chosenAnswerIndex == choiceIndex) {
                cout << "(Correctly chosen)";
            } else {
                if(qh.choices[choiceIndex] == qh.correctAnswer ) {
                    cout << "(Correct)";
                } else if(qh.chosenAnswerIndex == choiceIndex) {
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


    acout (playerName + ", you scored " + to_string(score) + "/" + to_string(questionToAnswer.size()) + " points");

    cin.get();

    const char *THANK_YOU = R"(

******** **  **     **     **   ** **   **    **    ** ******* **   **
   **    **  **    *  *    ***  ** **  **      **  **  **   ** **   **
   **    ******   ******   ** * ** *****         **    **   ** **   **
   **    **  **  **    **  **  *** **  **        **    **   ** **   **
   **    **  ** **      ** **   ** **   **       **    ******* *******
   )";

    cout <<endl << endl;
    vector<string> arr;
    string nice = "";
    for(int i =0; i < ((string) THANK_YOU).length(); i++) {
        char c = THANK_YOU[i];
        if(c == '\n') {
            arr.push_back(nice);
            nice = "";
        }
        nice+=c;
    }
    for(string s : arr) {
        cout << s;
        Sleep(100);
    }
    cout << endl << endl;

    return 0;
}
