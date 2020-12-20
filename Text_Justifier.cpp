//============================================================================
// Name        : Text_Justifier.cpp
// Author      : Joshua Chu
// Version     :
// Copyright   : © Copyright 2015 Joshua Chu
// Description : Justifies input text
//============================================================================

#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

// A Word owns its word and its spacing
struct Word {
    string word;
    int padding;
    enum WordType {
        REGULAR, END_OF_LINE
    } wordType;

    Word(string w, int p, WordType wt = REGULAR) {
        word = w;
        padding = p;
        wordType = wt;
    }

    void setPadding(int p) {
        if (p >= 0)
            padding = p;
    }
    void incrementPadding() {
        ++padding;
    }

    void setEndOfLine() {
        wordType = END_OF_LINE;
        padding = 0;
    }
};

// A Line owns its words and its length
struct Line {
    vector<Word> text;
    int length;

    Line(vector<Word> t) {
        text = t;
        length = 0;
    }

    Line() {
        text = vector<Word>();
        length = 0;
    }

    void push_back(Word w) {
        text.push_back(w);
        length += w.word.length() + w.padding;
    }

    void pop_back() {
        length -= text[text.size() - 1].word.length() + text[text.size() - 1].padding;
        text.pop_back();
    }

    void setEndOfLine() {
        text[text.size() - 1].padding = 0;
        --length;
    }

    void padToWidth(int pageWidth) {
        int spaces = pageWidth - length;
        if (spaces == 0) return;

        int spacesPerWord = spaces / (text.size() - 1);
        int remainder = spaces % (text.size() - 1);

        for (int i = 0; i < text.size() - 1; ++i) {
            text[i].setPadding(1 + spacesPerWord);
        }
        int idx = 0;
        while (remainder > 0) {
            text[idx++].incrementPadding();
            if (idx == text.size() - 1) idx = 0;
            --remainder;
        }
    }

    string toString() {
        string str;
        for (int i = 0; i < text.size(); ++i) {
            str += (text[i].word + string(text[i].padding, ' '));
        }
        return str;
    }
};

// input functions
string getText() {
    string str;
    cout << "Please enter your text." << endl
         << "Entering an empty line will finish input." << endl;

    string line;
    do {
        cout << "  > ";
        getline(cin, line);
        if (line != "") {
            str += line;
            str += " ";
        }
    } while (line != "");

    cout << endl;

    return str;
};
int getLineWidth() {
    int width = 0;
    cout << "Please enter the desired # of characters per line." << endl;

    string line;
    do {
        cout << "  > ";
        getline(cin, line);
        try {
            width = stoi(line);
        } catch (const exception& e) {
            width = 0;
        }
    } while (width == 0);

    cout << endl;

    return width;
};

// format function
vector<Line> formatText(string text, int width) {
    vector<Line> justifiedText;
    if(text == "") return justifiedText;

    Line line;

    stringstream textStream;
    textStream << text;

    string word;
    while (textStream >> word) {
        line.push_back(Word(word, 1));
        if (line.length > width) {
            Line temp;

            if (line.length - 1 > width) {
                temp.push_back(line.text[line.text.size() - 1]);
                line.pop_back();
            }

            line.setEndOfLine();
            justifiedText.push_back(line);

            line = temp;
        }
    }
    if (line.length > 0) justifiedText.push_back(line);

    Line& lastLine = justifiedText[justifiedText.size() - 1];
    lastLine.setEndOfLine();

    for (int i = 0; i < justifiedText.size() - 1; ++i) {
        justifiedText[i].padToWidth(width);
    }
    // left-justifying the last line (padding its last word with all remaining space)
    lastLine.text[lastLine.text.size() - 1].setPadding(width - lastLine.length);

    return justifiedText;
};

int main() {
    srand(time(0));

    cout << "Welcome to Text Justifier!" << endl
         << endl;

    bool stop = false;
    do {
        string text = getText();
        int width = getLineWidth();
        vector<Line> lines = formatText(text, width);

        cout << "+-" << string(width, '-') << "-+" << endl;
        for (int i = 0; i < lines.size(); ++i) {
            cout << "| " << lines[i].toString() << " |" << endl;
        }
        cout << "+-" << string(width, '-') << "-+" << endl
             << endl;

        cout << "Would you like to justify more text? (y/n)" << endl
             << "  > ";
        char ch; cin >> ch;
        string line; getline(cin, line); //removing newline from stream
        cout << endl;

        if (ch == 'n') stop = true;
    } while (!stop);


    return 0;
}
