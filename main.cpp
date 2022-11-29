#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <conio.h>
#include <windows.h>


using namespace std;

template<typename T>
std::vector<T> create_copy(std::vector<T> const &vec)
{
    std::vector<T> v;
    v.assign(vec.begin(), vec.end());

    return v;
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

        unsigned seed = std::chrono::system_clock::now()
                        .time_since_epoch()
                        .count();
        if(randomize) {
            shuffle(choices.begin(), choices.end(), default_random_engine(seed));
        }
        return choices;
    }

};


struct QuestionHistory : public Question {
    public:
        QuestionHistory(
             Question q,
             int chosenAnswerIndex
        ) : Question(q.question, q.correctAnswer, q.wrongAnswers) {
            this->chosenAnswerIndex = chosenAnswerIndex;
        }


        vector<string>  choices;
        int chosenAnswerIndex;
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

void clear_screen() {
    system("CLS");
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




int initiateQuiz(vector<Question> _questions)
{


    vector<Question> questions = create_copy(_questions);
    int score = 0;
    int questionIndex = 0;
//    history.questions = questions;

    for(Question i : questions) {
        clear_screen();
        int answer;
        vector<string> choices = i.getChoices();
//        history.questionChoices.push_back(choices);
        int questionNumber = questionIndex + 1;
        while(true) {
            cout << questionNumber << ".) " << i.question  << endl << endl;
            int index = 0;
            for(auto choice : choices) {
                cout << prefixes[index] << choice << endl;
                index++;
            }
            cout << endl << "Answer: ";
            cin >> answer;
            if(cin.fail()|| !validateInput(answer))  {
                cout << endl;
                clear_input();
                continue;
            }
            break;
        }

        string chosenAnswer = choices[answer-1];
//        history.chosenAnswers.push_back(answer-1);
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

    cout << "Welcome to Quiz" << endl;
    cout << "Press ANY key to continue " << endl;

    while(true) {
        if(_kbhit() && getch()) {break;}
    }

    clear_screen();

    cout << "Choose the difficulty" << endl;

    cout << "1. Easy"<< endl << "2. Medium" << endl << "3. Hard" << endl;
    int chosenDifficulty;
    vector<Question> questionToAnswer;
    cin >> chosenDifficulty;
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
    int score = initiateQuiz(questionToAnswer);
    int index = 0;

//     for(Question question : history.questions) {
//        cout << index + 1 << ".) " << question.question << endl;
//        int currentChoiceIndex = 0;
//        string correctAnswer = history.questionChoices[index] [history.chosenAnswers[index]];
//        bool gotCorrectAnswer = question.correctAnswer == ;
//        for(string choices : history.questionChoices[index]) {
//            cout << choices;
//            if(!gotCorrectAnswer &&
//               == ) {
//                cout << " (Wrong) ";
//            }
//            cout << endl;
//            currentChoiceIndex++;
//        }
//        index++;
//        cout << endl;
//    }


    cout << "YOU SCORED: " << score << " points";

    return 0;
}
